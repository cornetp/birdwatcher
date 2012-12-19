#include <streams.h>
#include <Wininet.h>
#include <stdio.h>

#include <jpeglib.h>

#include "bwfilter.h"
#include "bwFTPSender.h"
#include "Debug.h"

CBirdwatcherFTPSender::CBirdwatcherFTPSender(CBirdwatcher *parent):
m_pMediaType(NULL),
m_pbmi(NULL)
{
	m_pParent = parent;
	m_connected = FALSE;
	m_hInternet = NULL;
	m_hFTPSession = NULL;
	m_upload_period_sec = 10;	

	m_pImage[0] = NULL;
	m_pImage[1] = NULL;

	m_today = 0;
	GetLocalTime(&m_date);

	// Init interface variables
	m_ftp_enabled = FALSE;
	sprintf_s(m_ftp_server_name,"");
	sprintf_s(m_ftp_user_name,"");
	sprintf_s(m_ftp_password,"");
	sprintf_s(m_ftp_directory,"");
	sprintf_s(m_image_filename, "cam");
	m_image_file_extension = 0;
	sprintf_s(m_image_subdir, "");
	m_image_subdir_extension = 0;
	m_jpeg_quality = 75;
	m_image_size_divider = 1;

	m_overlay_enabled = FALSE;
	m_pOverlay = NULL;
	sprintf_s(m_overlay_chars_file, "c:\\chars.bmp");

	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	m_hStopThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	// Create worker thread
	Create();
}

CBirdwatcherFTPSender::~CBirdwatcherFTPSender()
{
	//CallWorker(0);
	SetEvent(m_hStopThreadEvent);
	Close(); // wait for thread to exit

	if(m_pOverlay)
		delete(m_pOverlay);
	if(m_pImage[0])
	{
		free(m_pImage[0]);
		m_pImage[0] = NULL;
	}
	if(m_pImage[1])
	{
		free(m_pImage[1]);
		m_pImage[1] = NULL;
	}
	if(m_hTimer)
		CloseHandle(m_hTimer);
	if(m_hMutex)
		CloseHandle(m_hMutex);
	if(m_hStopThreadEvent)
		CloseHandle(m_hStopThreadEvent);
}

BOOL CBirdwatcherFTPSender::IsRunning()
{
	return m_pParent->IsActive();
}

HRESULT CBirdwatcherFTPSender::Connect()
{
	m_hInternet = InternetOpen("Birdwatcher_filter", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (m_hInternet == NULL)
	{
		Disconnect();
		return GetLastError();
	}

	DbgPrint("Connect to FTP server %s ...", m_ftp_server_name);
	m_hFTPSession = InternetConnect(
		m_hInternet,
		m_ftp_server_name,
		INTERNET_DEFAULT_FTP_PORT,
		m_ftp_user_name,
		m_ftp_password,
		INTERNET_SERVICE_FTP,
		INTERNET_FLAG_PASSIVE,
		(DWORD_PTR)this);

	if(m_hFTPSession == NULL)
	{
		Disconnect();
		return GetLastError();
	}
	DbgPrint(" ... ok");

	if(!FtpSetCurrentDirectory(m_hFTPSession, m_ftp_directory))
	{
		Disconnect();
		return GetLastError();
	}

	m_connected = TRUE;
	return NOERROR;
}

HRESULT CBirdwatcherFTPSender::Disconnect()
{
	if (m_hFTPSession)
	{
		InternetCloseHandle(m_hFTPSession);
		m_hFTPSession = NULL;
	}

	if (m_hInternet)
	{
		InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
	}

	m_connected = FALSE;
	return NOERROR;
}

HRESULT CBirdwatcherFTPSender::Reconnect()
{
	DbgPrint("Reconnect...");
	Disconnect();
	return Connect();
}

void CBirdwatcherFTPSender::NotifyMediaType(CMediaType *pMediaType)
{
    m_pMediaType = pMediaType;
	m_pbmi = HEADER(m_pMediaType->Format());

	if(m_pImage[0])
	{
		free(m_pImage[0]);
		m_pImage[0] = NULL;
	}
	if(m_pImage[1])
	{
		free(m_pImage[1]);
		m_pImage[1] = NULL;
	}
}

STDMETHODIMP CBirdwatcherFTPSender::Run(REFERENCE_TIME tStart)
{
	DbgPrint("CBirdwatcherFTPSender::Run");
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -10000000LL; // start in 1 sec
	SetWaitableTimer(m_hTimer, &liDueTime, m_upload_period_sec*1000, NULL, NULL, 0); // periodic timer
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::Stop()
{
	CancelWaitableTimer(m_hTimer);
	return NOERROR;
}

void CBirdwatcherFTPSender::Resample(BYTE *bufOut, BYTE *bufIn, BITMAPINFOHEADER *inBMI, int scale)
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

HRESULT CBirdwatcherFTPSender::Deliver(IMediaSample *pSample)
{
	ASSERT(m_pbmi);

    // Copy the image data buffer
	if(m_pImage[0] == NULL)
		m_pImage[0] = (BYTE*)malloc(m_pbmi->biSizeImage);

	BYTE *ptr;
    HRESULT hr = pSample->GetPointer(&ptr);
    if (FAILED(hr)) {
        return hr;
    }

	DWORD dwWaitResult = WaitForSingleObject(m_hMutex, 40); // wait max 40ms
	if(dwWaitResult == WAIT_OBJECT_0)
	{
		//memcpy(m_pImage[0], ptr, m_pbmi->biSizeImage);
		Resample(m_pImage[0], ptr, m_pbmi, m_image_size_divider);
		ReleaseMutex(m_hMutex);
	}
	else if(dwWaitResult == WAIT_TIMEOUT)
		DbgPrint("CBirdwatcherFTPSender::Deliver timeout");

	return NOERROR;
}


DWORD CBirdwatcherFTPSender::ThreadProc()
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
			DbgPrint("Timer was signaled");
			Send();
		}			
	}
	return 1;
}

void CBirdwatcherFTPSender::Send()
{
	if(!m_pImage[0])
	{
		DbgPrint("No image yet");
		return;
	}

	ASSERT(m_pbmi);
	SYSTEMTIME time;
	char fileName[MAX_PATH];
	char subDir[MAX_PATH];

	GetLocalTime(&time);

	if(m_image_file_extension == 1)
		sprintf_s(fileName, "%s%02d%02d%02d.jpg", m_image_filename, time.wHour, time.wMinute, time.wSecond);
	else
		sprintf_s(fileName, "%s.jpg", m_image_filename);

	if(m_image_subdir_extension == 1)
		sprintf_s(subDir, "%s%04d%02d%02d/", m_image_subdir, time.wYear, time.wMonth, time.wDay);
	else
	{
		if(strlen(m_image_subdir)==0)
			sprintf_s(subDir, "%s", "");
		else
			sprintf_s(subDir, "%s/", m_image_subdir);
	}

	if(strlen(subDir) && m_today != time.wDay)
	{
		// Create remote directory
		DbgPrint("Create remote directory %s", subDir);
		if (IsConnected())
		{
			if(!FtpCreateDirectory(m_hFTPSession, subDir))
			{
				DWORD err = GetLastError();
				//DbgPrint("FtpCreateDirectory failed with error %d (0x%x)", err, err);
				if(err == ERROR_INTERNET_EXTENDED_ERROR)
				{
					DWORD error;
					char text[512];
					DWORD size = 512;
					InternetGetLastResponseInfo(&error, text, &size);
					DbgPrint("FtpCreateDirectory Error: %s", text);
				}
			}
		}
		m_today = time.wDay;
	}


	char fullFileName[MAX_PATH];
	sprintf_s(fullFileName, "%s%s", subDir, fileName);

	FILE* outfile = NULL;
	if(fopen_s(&outfile, fileName, "wb"))
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
		cinfo.image_height     = abs(m_pbmi->biHeight/m_image_size_divider);
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
				m_pOverlay->PrintString(m_pImage[0],  m_pbmi->biWidth/m_image_size_divider, abs(m_pbmi->biHeight)/m_image_size_divider, 10, 10, ovl);
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
						scanline[x*3] = m_pImage[0][(x+y*cinfo.image_width)*3+2];
						scanline[x*3+1] = m_pImage[0][(x+y*cinfo.image_width)*3+1];
						scanline[x*3+2] = m_pImage[0][(x+y*cinfo.image_width)*3];
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

	// Send file
	if (IsConnected())
	{
		DbgPrint("FTP Send file %s", fileName);
		if(!FtpPutFile(m_hFTPSession, fileName, fullFileName, FTP_TRANSFER_TYPE_BINARY, NULL))
		{
			DWORD err = GetLastError();
			DbgPrint("Ftp send error: 0x%x (%d)", err, err);
			// 12002 - ERROR_INTERNET_TIMEOUT
			// 12003 - ERROR_INTERNET_EXTENDED_ERROR  (call InternetGetLastResponseInfo)
			// 12031 - ERROR_INTERNET_CONNECTION_RESET

			if(err == ERROR_INTERNET_EXTENDED_ERROR)
			{
				DWORD error;
				char text[512];
				DWORD size = 512;
				InternetGetLastResponseInfo(&error, text, &size);
				DbgPrint("FtpPutFile Error: 0x%x -> %s", error, text);
			}

			if(err == ERROR_INTERNET_CONNECTION_RESET)
				Reconnect();
		}
	}
	else
		DbgPrint("FTP Not connected");

	_unlink(fileName);
}

// IFtpControl interface
STDMETHODIMP CBirdwatcherFTPSender::GetEnableUpload(BOOL *pbEnabled)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	*pbEnabled = m_ftp_enabled;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetEnableUpload(BOOL bEnabled)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	m_ftp_enabled = bEnabled;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetServerName(CHAR *serverName, size_t size)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(serverName, size, m_ftp_server_name);
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetServerName(CHAR *serverName)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(m_ftp_server_name, sizeof(m_ftp_server_name), serverName);
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetUserName(CHAR *usrName, size_t size)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(usrName, size, m_ftp_user_name);
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetUserName(CHAR *usrName)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(m_ftp_user_name, sizeof(m_ftp_user_name), usrName);
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetPassword(CHAR *passwd, size_t size)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(passwd, size, m_ftp_password);
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetPassword(CHAR *passwd)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(m_ftp_password, sizeof(m_ftp_password), passwd);
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetDirectory(CHAR *dir, size_t size)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(dir, size, m_ftp_directory);
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetDirectory(CHAR *dir)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(m_ftp_directory, sizeof(m_ftp_directory), dir);
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetImageFileName(CHAR *filename, size_t size, int *fileExtension)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(filename, size, m_image_filename);
	*fileExtension = m_image_file_extension ;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetImageFileName(CHAR *filename, int fileExtension)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(m_image_filename, sizeof(m_image_filename), filename);
	m_image_file_extension = fileExtension;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetImageSubDir(CHAR *subdir, size_t size, int *subdirExtension)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(subdir, size, m_image_subdir);
	*subdirExtension = m_image_subdir_extension ;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetImageSubDir(CHAR *subdir, int subdirExtension)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	sprintf_s(m_image_subdir, sizeof(m_image_filename), subdir);
	m_image_subdir_extension = subdirExtension;
	m_today = 0;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetUploadPeriod(unsigned long *pUploadPeriod)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	*pUploadPeriod = m_upload_period_sec;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetUploadPeriod(unsigned long uploadPeriod)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	m_upload_period_sec = uploadPeriod;

	// retart the timer
	if(IsRunning())
	{
		CancelWaitableTimer(m_hTimer);
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = -10000000LL; // start in 1 sec
		SetWaitableTimer(m_hTimer, &liDueTime, m_upload_period_sec*1000, NULL, NULL, 0);
	}
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetJpegQuality(int *pJpegQuality)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	*pJpegQuality = m_jpeg_quality;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetJpegQuality(int jpegQuality)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	m_jpeg_quality = jpegQuality;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetImageSizeDivider(int *pDivider)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	*pDivider = m_image_size_divider;
	return NOERROR;
}
STDMETHODIMP CBirdwatcherFTPSender::SetImageSizeDivider(int divider)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	m_image_size_divider = divider;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::GetOverlayEnable(BOOL *pEnableOvl)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
	*pEnableOvl = m_overlay_enabled;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherFTPSender::SetOverlayEnable(BOOL enableOvl)
{
	CAutoLock cAutolock(&m_ftpsender_lock);
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