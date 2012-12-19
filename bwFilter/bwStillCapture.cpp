#include <streams.h>
#include <stdio.h>
#include <jpeglib.h>

#include "bwOverlay.h"
#include "Debug.h"
#include "bwfilter.h"
#include "bwStillCapture.h"

CBirdwatcherStillCapture::CBirdwatcherStillCapture(CBirdwatcher *parent) :
m_pMediaType(NULL),
m_pbmi(NULL)
{
	m_pParent = parent;

	m_stillcapture_enabled = FALSE;
	sprintf_s(m_still_directory,"c:\\");
	sprintf_s(m_still_image_filename, "cam_");
	m_still_image_file_extension = 1;
	sprintf_s(m_still_image_subdir, "");
	m_still_image_subdir_extension = 1;
	m_capture_period_sec = 3;
	m_image_size_divider = 1;
	m_jpeg_quality = 80;

	m_today = 0;
	m_pImage = NULL;

	m_overlay_enabled = FALSE;
	m_pOverlay = NULL;
	sprintf_s(m_overlay_chars_file, "c:\\chars.bmp");

	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	m_hStopThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	Create(); // start worker thread
}

CBirdwatcherStillCapture::~CBirdwatcherStillCapture()
{
	SetEvent(m_hStopThreadEvent);
	Close(); // wait for thread to exit

	if(m_pOverlay)
		delete(m_pOverlay);
	if(m_hTimer)
		CloseHandle(m_hTimer);
	if(m_hMutex)
		CloseHandle(m_hMutex);
	if(m_hStopThreadEvent)
		CloseHandle(m_hStopThreadEvent);
}

void CBirdwatcherStillCapture::NotifyMediaType(CMediaType *pMediaType)
{
    m_pMediaType = pMediaType;
	m_pbmi = HEADER(m_pMediaType->Format());

	if(m_pImage)
	{
		free(m_pImage);
		m_pImage[0] = NULL;
	}
}

STDMETHODIMP CBirdwatcherStillCapture::Run()
{
	DbgPrint("CBirdwatcherStillCapture::Run");
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -10000000LL; // start in 1 sec
	SetWaitableTimer(m_hTimer, &liDueTime, m_capture_period_sec*1000, NULL, NULL, 0); // periodic timer
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::Stop()
{
	CancelWaitableTimer(m_hTimer);
	return NOERROR;
}

BOOL CBirdwatcherStillCapture::IsRunning()
{
	return m_pParent->IsActive();
}

void CBirdwatcherStillCapture::Resample(BYTE *bufOut, BYTE *bufIn, BITMAPINFOHEADER *inBMI, int scale)
{
	for(int cy = 0; cy < inBMI->biHeight/scale; cy++)
	{
		for(int cx = 0; cx < inBMI->biWidth/scale; cx++)
		{
			int pixel = (cy*(inBMI->biWidth/scale) + cx) * 3;
			//int nearestMatch =  (((int)(cy / scale) * inBMI->biWidth) + ((int)(cx / scale) *3) );
			int nearestMatch =  (cy*scale * inBMI->biWidth + cx*scale) * 3;

			bufOut[pixel    ] =  bufIn[nearestMatch    ];
			bufOut[pixel + 1] =  bufIn[nearestMatch + 1];
			bufOut[pixel + 2] =  bufIn[nearestMatch + 2];
		}
	}
}

HRESULT CBirdwatcherStillCapture::Deliver(IMediaSample *pSample)
{
	ASSERT(m_pbmi);

    // Copy the image data buffer
	if(m_pImage == NULL)
		m_pImage = (BYTE*)malloc(m_pbmi->biSizeImage);

	BYTE *ptr;
    HRESULT hr = pSample->GetPointer(&ptr);
    if (FAILED(hr)) {
        return hr;
    }

	DWORD dwWaitResult = WaitForSingleObject(m_hMutex, 40); // wait max 40ms
	if(dwWaitResult == WAIT_OBJECT_0)
	{
		//memcpy(m_pImage, ptr, m_pbmi->biSizeImage);
		Resample(m_pImage, ptr, m_pbmi, m_image_size_divider);
		ReleaseMutex(m_hMutex);
	}
	else if(dwWaitResult == WAIT_TIMEOUT)
		DbgPrint("CBirdwatcherStillCapture::Deliver timeout");

	return NOERROR;

}

DWORD CBirdwatcherStillCapture::ThreadProc()
{
    BOOL bShutDown = FALSE;
    while (!bShutDown)
	{
		HANDLE objs[2] = {m_hTimer, m_hStopThreadEvent};
		DWORD dwWaitResult = WaitForMultipleObjects(2, objs, FALSE, INFINITE);
		if(dwWaitResult ==  WAIT_OBJECT_0 + 1)
		{
			bShutDown = TRUE;
		}
		else 
		{
			DbgPrint("CBirdwatcherStillCapture::ThreadProc: Timer was signaled");
			Process();
		}			
	}
	DbgPrint("CBirdwatcherStillCapture Exit thread");
	return 1;
}

void CBirdwatcherStillCapture::Process()
{
	if(!m_pImage)
	{
		DbgPrint("CBirdwatcherStillCapture::Process No image yet");
		return;
	}

	ASSERT(m_pbmi);
	SYSTEMTIME time;
	char fileName[MAX_PATH];
	char subDir[MAX_PATH];

	GetLocalTime(&time);

	if(m_still_image_file_extension == 1)
		sprintf_s(fileName, "%s%02d%02d%02d.jpg", m_still_image_filename, time.wHour, time.wMinute, time.wSecond);
	else
		sprintf_s(fileName, "%s.jpg", m_still_image_filename);

	if(m_still_image_subdir_extension == 1)
		sprintf_s(subDir, "%s%04d%02d%02d\\", m_still_image_subdir, time.wYear, time.wMonth, time.wDay);
	else
	{
		if(strlen(m_still_image_subdir)==0)
			sprintf_s(subDir, "%s", "");
		else
			sprintf_s(subDir, "%s\\", m_still_image_subdir);
	}

	char absolutePath[MAX_PATH];
	if(strlen(m_still_directory))
		sprintf_s(absolutePath, "%s\\%s", m_still_directory, subDir);
	else
		sprintf_s(absolutePath, "%s%s", subDir, fileName);

	if(strlen(subDir) && m_today != time.wDay)
	{
		// Create local directory

		if(!CreateDirectory(absolutePath, NULL))
		{
			DWORD err = GetLastError();
			DbgPrint("Error: cannot create directory %s: 0x%x", absolutePath, err);
		}
		else
			m_today = time.wDay;
	}

	char fullFileName[MAX_PATH];
	sprintf_s(fullFileName, "%s%s", absolutePath, fileName);

	DbgPrint("Still image filename: %s", fullFileName);

	FILE* outfile = NULL;
	if(fopen_s(&outfile, fullFileName, "wb"))
	{
		DbgPrint("errror: cannot open file %s: %d", fullFileName, errno);
		return;
	}

	// Compress
	{
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
 
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);
		jpeg_stdio_dest(&cinfo, outfile);
 
		cinfo.image_width      = m_pbmi->biWidth/m_image_size_divider;
		cinfo.image_height     = abs(m_pbmi->biHeight)/m_image_size_divider;
		cinfo.input_components = 3;
		cinfo.in_color_space   = JCS_RGB;

		jpeg_set_defaults(&cinfo);
		/*set the quality [0..100]  */
		jpeg_set_quality (&cinfo, m_jpeg_quality, true);
		jpeg_start_compress(&cinfo, true);

		int row_stride;
		row_stride = m_pbmi->biWidth * 3;

		// take the mutex to access m_pImage
		DWORD dwWaitResult = WaitForSingleObject(m_hMutex, INFINITE);
		if(dwWaitResult == WAIT_OBJECT_0)
		{
			// Add text overlay
			if(m_overlay_enabled)
			{
				char ovl[32];
				char *day[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
				//sprintf_s(ovl, "%s %02d/%02d/%04d - %02d:%02d:%02d", day[time.wDayOfWeek], time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);
				sprintf_s(ovl, "%02d/%02d/%04d - %02d:%02d:%02d", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);
				m_pOverlay->PrintString(m_pImage,  m_pbmi->biWidth/m_image_size_divider, abs(m_pbmi->biHeight)/m_image_size_divider, 10, 10, ovl);
			}

			JSAMPLE * scanline = NULL;
			scanline = new JSAMPLE[cinfo.image_width*3]; 
			if(scanline == NULL)
			{
				DbgPrint("Fatal error: cannot allocate mem to create jpeg");
			}
			else
			{
				for(int y=cinfo.image_height-1; y >= 0; y--)
				{
					for(unsigned int x=0; x<cinfo.image_width; x++)
					{
						scanline[x*3] = m_pImage[(x+y*cinfo.image_width)*3+2];
						scanline[x*3+1] = m_pImage[(x+y*cinfo.image_width)*3+1];
						scanline[x*3+2] = m_pImage[(x+y*cinfo.image_width)*3];
					}
					jpeg_write_scanlines(&cinfo, &scanline, 1);
				}
				delete scanline;
			}
			ReleaseMutex(m_hMutex);
		}

		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
	}

	fclose(outfile);

}

STDMETHODIMP CBirdwatcherStillCapture::GetEnableStillCapture(BOOL *pbEnabled)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	*pbEnabled = m_stillcapture_enabled;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherStillCapture::SetEnableStillCapture(BOOL bEnabled)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	m_stillcapture_enabled = bEnabled;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::GetStillDirectory(CHAR *dir, size_t size)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	sprintf_s(dir, size, m_still_directory);
	return NOERROR;
}
STDMETHODIMP CBirdwatcherStillCapture::SetStillDirectory(CHAR *dir)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	sprintf_s(m_still_directory, sizeof(m_still_directory), dir);
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::GetStillImageFileName(CHAR *filename, size_t size, int *fileExtension)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	sprintf_s(filename, size, m_still_image_filename);
	*fileExtension = m_still_image_file_extension ;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherStillCapture::SetStillImageFileName(CHAR *filename, int fileExtension)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	sprintf_s(m_still_image_filename, sizeof(m_still_image_filename), filename);
	m_still_image_file_extension = fileExtension;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::GetStillImageSubDir(CHAR *subdir, size_t size, int *subdirExtension)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	sprintf_s(subdir, size, m_still_image_subdir);
	*subdirExtension = m_still_image_subdir_extension ;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherStillCapture::SetStillImageSubDir(CHAR *subdir, int subdirExtension)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	sprintf_s(m_still_image_subdir, sizeof(m_still_image_filename), subdir);
	m_still_image_subdir_extension = subdirExtension;
	m_today = 0;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::GetStillImageSizeDivider(int *pDivider)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	*pDivider = m_image_size_divider;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherStillCapture::SetStillImageSizeDivider(int divider)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	m_image_size_divider = divider;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::GetStillCapturePeriod(unsigned long *pCapturePeriod)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	*pCapturePeriod = m_capture_period_sec;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherStillCapture::SetStillCapturePeriod(unsigned long capturePeriod)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	m_capture_period_sec = capturePeriod;

	// retart the timer
	if(IsRunning())
	{
		CancelWaitableTimer(m_hTimer);
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = -10000000LL; // start in 1 sec
		SetWaitableTimer(m_hTimer, &liDueTime, m_capture_period_sec*1000, NULL, NULL, 0);
	}
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::GetStillJpegQuality(int *pJpegQuality)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	*pJpegQuality = m_jpeg_quality;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::SetStillJpegQuality(int jpegQuality)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	m_jpeg_quality = jpegQuality;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::GetOverlayEnable(BOOL *pEnableOvl)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	*pEnableOvl = m_overlay_enabled;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherStillCapture::SetOverlayEnable(BOOL enableOvl)
{
	CAutoLock cAutolock(&m_stillcapture_lock);
	m_overlay_enabled = enableOvl;
	if(m_overlay_enabled)
	{
		if(!m_pOverlay)
			m_pOverlay = new CBirdwatcherOverlay(m_overlay_chars_file);
	}
	else
	{
		if(m_pOverlay)
		{
			delete(m_pOverlay);
			m_pOverlay = NULL;
		}
	}
	return NOERROR;
}

