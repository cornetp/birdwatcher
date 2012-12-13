#include <streams.h>
#include <Strsafe.h>
#include <initguid.h>

#include "Debug.h"
#include "JpegReaderFlt.h"

const AMOVIESETUP_MEDIATYPE sudOpPinTypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudOpPin =
{
    L"Output",              // Pin string name
    FALSE,                  // Is it rendered
    TRUE,                   // Is it an output
    FALSE,                  // Can we have none
    FALSE,                  // Can we have many
    &CLSID_NULL,            // Connects to filter
    NULL,                   // Connects to pin
    1,                      // Number of types
    &sudOpPinTypes };       // Pin details

const AMOVIESETUP_FILTER sudJpegStr =
{
    &CLSID_JpegPushSource,    // Filter CLSID
    L"Jpeg Push stream",       // String name
    MERIT_DO_NOT_USE,       // Filter merit
    1,                      // Number pins
    &sudOpPin               // Pin details
};


// COM global table of objects in this dll

CFactoryTemplate g_Templates[] = {
  { L"Jpeg Push stream"
  , &CLSID_JpegPushSource
  , CJpegPushSource::CreateInstance
  , NULL
  , &sudJpegStr }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);



CUnknown * WINAPI CJpegPushSource::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    ASSERT(phr);

    CUnknown *punk = new CJpegPushSource(lpunk, phr);
    if(punk == NULL)
    {
        if(phr)
            *phr = E_OUTOFMEMORY;
    }

	((CJpegPushSource*)punk)->SetStillDirectory("C:\\Documents and Settings\\cornetp\\My Documents\\tmp\\still\\20110404");

	return punk;

} // CreateInstance

CJpegPushSource::CJpegPushSource(LPUNKNOWN lpunk, HRESULT *phr) :
    CSource(NAME("Jpeg push source"), lpunk, CLSID_JpegPushSource)
{
    ASSERT(phr);
    CAutoLock cAutoLock(&m_cStateLock);

    m_paStreams = (CSourceStream **) new CJpegPushStream*[1];
    if(m_paStreams == NULL)
    {
        if(phr)
            *phr = E_OUTOFMEMORY;

        return;
    }

    m_paStreams[0] = new CJpegPushStream(phr, this, L"Jpeg source");
    if(m_paStreams[0] == NULL)
    {
        if(phr)
            *phr = E_OUTOFMEMORY;

        return;
    }

} // (Constructor)

HRESULT STDMETHODCALLTYPE CJpegPushSource::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IJpegReaderControl)
        return GetInterface((IJpegReaderControl *) this, ppv);
	else
		return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

HRESULT CJpegPushSource::SetStillDirectory(CHAR *c)
{
	return ((CJpegPushStream*)m_paStreams[0])->SetStillDirectory(c);
}
HRESULT CJpegPushSource::GetStillDirectory(CHAR *c, size_t s)
{
	return ((CJpegPushStream*)m_paStreams[0])->GetStillDirectory(c, s);
}

HRESULT CJpegPushSource::GetCurrentFileName(CHAR *c, size_t s)
{
	return ((CJpegPushStream*)m_paStreams[0])->GetCurrentFileName(c, s);
}

CJpegPushStream::CJpegPushStream(HRESULT *phr, CJpegPushSource *pParent, LPCWSTR pPinName)
	: CSourceStream(TEXT("CJpegPushStream"),phr,pParent,pPinName)
#pragma warning(push)
#pragma warning(disable: 4355) /// 'this' : used in base member initializer list
	, CSourceSeeking(TEXT("CJpegPushStream"),(IPin*)this,phr,pParent->pStateLock())
#pragma warning(pop)
	, m_tpf(40 * REFTIME_UNITS_PER_MILLISEC) /// change this to change the frame rate
	, m_i(0i64), m_k(0i64), m_gap(true), m_fileix(0)
{
	if(S_OK != *phr) return;

	HRESULT hr = S_OK;

	m_FileCount = 0;
	m_FileList = NULL;

	VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)m_mt.AllocFormatBuffer(sizeof(*vih));
	if(vih == NULL) { hr = E_OUTOFMEMORY; goto exit; } else ZeroMemory(vih,sizeof(*vih));

	vih->bmiHeader.biSize			= sizeof(vih->bmiHeader);
	vih->bmiHeader.biWidth			= 640; /// change this to change the resolution
	vih->bmiHeader.biHeight			= 480; /// change this to change the resolution
	vih->bmiHeader.biPlanes			= 1;
	vih->bmiHeader.biBitCount		= 24;
	vih->bmiHeader.biCompression	= *(DWORD*)"MJPG";
	vih->bmiHeader.biSizeImage		= 64*1024 + sizeof(RGBQUAD) * vih->bmiHeader.biWidth * vih->bmiHeader.biHeight;
	vih->rcSource.right				= vih->bmiHeader.biWidth;
	vih->rcSource.bottom			= vih->bmiHeader.biHeight;
	vih->rcTarget					= vih->rcSource;
	vih->AvgTimePerFrame			= m_tpf;

	m_mt.SetType(&MEDIATYPE_Video);
	m_mt.SetSubtype(&FOURCCMap(vih->bmiHeader.biCompression));
	m_mt.SetVariableSize();
	m_mt.SetTemporalCompression(FALSE);
	m_mt.SetSampleSize(vih->bmiHeader.biSizeImage);
	m_mt.SetFormatType(&FORMAT_VideoInfo);

exit:
	*phr = hr;
}

CJpegPushStream::~CJpegPushStream()
{
	if(m_FileList)
	{
		for(int i=0; i<m_FileCount; i++)
			free(m_FileList[i]);
		free(m_FileList);
	}
}

/// *** CUnknown *** ///
HRESULT STDMETHODCALLTYPE CJpegPushStream::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{
	if(riid == __uuidof(IMediaSeeking))
		return this->CSourceSeeking::NonDelegatingQueryInterface(riid,ppv);
	return this->CSourceStream::NonDelegatingQueryInterface(riid,ppv);
}

/// *** CSourceStream *** ///
HRESULT CJpegPushStream::GetMediaType(CMediaType* pmt)
{
	return pmt->Set(m_mt);
}
HRESULT CJpegPushStream::DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* lpReq)
{
	ALLOCATOR_PROPERTIES apReq = { 1L, (long)m_mt.GetSampleSize(), 1L, 0L }, apRes = { 0L, 0L, 0L, 0L };

	HRESULT hr = pAlloc->SetProperties(&apReq,&apRes);
	if(S_OK != hr)
		goto exit;

	if(apRes.cBuffers < 1L)
	{
		hr = VFW_E_BUFFER_NOTSET;
		goto exit;
	}
	if(apRes.cbBuffer < (long)m_mt.GetSampleSize())
	{
		hr = VFW_E_BUFFER_OVERFLOW;
		goto exit;
	}
	if(apRes.cbAlign < 1L)
	{
		hr = VFW_E_BADALIGN;
		goto exit;
	}
	if(apRes.cbPrefix != 0L)
	{
		hr = VFW_E_BADALIGN;
		goto exit;
	}

exit:
	return hr;
}
HRESULT CJpegPushStream::FillBuffer(IMediaSample* pms)
{
	HRESULT	hr	= S_OK;
	HANDLE	hf	= INVALID_HANDLE_VALUE;
	TCHAR	fn[MAX_PATH];
	LPBYTE	pb;
	long	cb;
	CAutoLock cAutolock(&m_lock);

	if(S_OK != (hr = pms->GetPointer(&pb)))
		goto exit;
	else
		cb = pms->GetSize();

	/// change this block to fetch the JPEG data in some other way
	{
		/// change this to change the naming scheme of the JPEG files
		//StringCbPrintf(fn,sizeof(fn)/sizeof(*fn),TEXT("c:\\source\\%d.jpg"),m_fileix);
		if(m_fileix >= m_FileCount)
		{
			hr = HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
			goto exit;
		}

		if(m_fileix%25 == 0)
			//DbgPrint_w(L"CJpegPushStream::FillBuffer fn = %s%s%s (ix #%d)", m_StillDir, L"\\", m_FileList[m_fileix], m_fileix);
			DbgPrint_w(L"CJpegPushStream::FillBuffer %s", m_FileList[m_fileix]);
		wcscpy_s(fn, MAX_PATH, m_StillDir);
		wcscat_s(fn, MAX_PATH, L"\\");
		wcscat_s(fn, MAX_PATH, m_FileList[m_fileix]);

		if(INVALID_HANDLE_VALUE == (hf = CreateFile(fn,FILE_READ_DATA,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL)))
		{
			DWORD rc = GetLastError();
			hr = ERROR_FILE_NOT_FOUND == rc ? S_FALSE : HRESULT_FROM_WIN32(rc);
			goto exit;
		}
		if(!ReadFile(hf,pb,(DWORD)cb,(DWORD*)&cb,NULL))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			goto exit;
		}
		if(cb < 1)
		{
			hr = HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
			goto exit;
		}
	}

	if(S_OK != (hr = pms->SetActualDataLength(cb)))
		goto exit;
	if(S_OK != (hr = pms->SetSyncPoint(TRUE)))
		goto exit;
	if(m_gap)
	{
		if(S_OK != (hr = pms->SetDiscontinuity(TRUE)))
			goto exit;
		m_gap = false;
	} /// since we have only keyframes, this is not strictly necessary

	/// timestamps must restart from 0 after a seek, so substract the first frame (m_k) from the current frame (m_i)
	LONGLONG mt1 = m_i - m_k, mt2 = mt1 + 1;
	REFERENCE_TIME rt1 = mt1 * m_tpf, rt2 = rt1 + m_tpf;

	if(S_OK != (hr = pms->SetTime(&rt1,&rt2)))
		goto exit;
	/// this is not necessary, but easy to provide with a frame-based stream
	if(S_OK != (hr = pms->SetMediaTime(&mt1,&mt2)))
		goto exit;

	m_i++;
	m_fileix++;

exit:
	if(hf != INVALID_HANDLE_VALUE)
		(void)CloseHandle(hf);
	return hr;
}
HRESULT CJpegPushStream::OnThreadStartPlay(void)
{
	m_gap = true;
	return DeliverNewSegment(m_rtStart,m_rtStop,m_dRateSeeking);
}

/// *** CSourceSeeking *** ///
HRESULT STDMETHODCALLTYPE CJpegPushStream::OnSeek(bool start)
{
	//CAutoLock cAutolock(&m_lock);
	HRESULT	hr	= S_OK;
	bool	z	= TRUE == ThreadExists();
	if(z)
	{
		if(S_OK != (hr = DeliverBeginFlush()))
			goto exit;
		if(S_OK != (hr = Stop()))
			goto exit;
		if(S_OK != (hr = DeliverEndFlush()))
			goto exit;
	}
	/// timestamps must restart from 0 after a seek, so keep track of first frame in m_k
	if(start)
	{
		m_k = m_i = m_rtStart / m_tpf;
		///m_fileix = 0;
		m_fileix = m_i;
	}
	if(z) 
	{
		if(S_OK != (hr = Run()))
			goto exit;
	}
exit:
	return hr;
}
HRESULT CJpegPushStream::ChangeStart(void)
{
	return OnSeek(true);
}
HRESULT CJpegPushStream::ChangeStop(void)
{
	return OnSeek(false);
}
HRESULT CJpegPushStream::ChangeRate(void)
{
	return OnSeek(false);
}

/// *** IJpegReaderControl *** ///
HRESULT CJpegPushStream::SetStillDirectory(CHAR *dir)
{
	DbgPrint("CJpegPushStream::SetStillDirectory: set dir to %s", dir);
	CAutoLock cAutolock(&m_lock);
	size_t ret;
	mbstowcs_s(&ret, m_StillDir, MAX_PATH, dir, _TRUNCATE);

	if(m_FileList)
	{
		if(m_FileList)
		{
			for(int i=0; i<m_FileCount; i++)
				free(m_FileList[i]);
			free(m_FileList);
		}
		m_FileList = NULL;
	}
	m_FileCount = 0;

	// parse directory
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	mbstowcs_s(&ret, szDir, MAX_PATH, dir, _TRUNCATE);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*.jpg"));
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) 
	{
		dwError = GetLastError();
		DbgPrint("FindFirstFile Error %d", dwError);
		return dwError;
	}
	// List all the files in the directory with some info about them.
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			m_FileCount++;
	} while (FindNextFile(hFind, &ffd) != 0);
 
	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		dwError = GetLastError();
		DbgPrint("FindNextFile Error %d", dwError);
		return dwError;
	}

	FindClose(hFind);

	DbgPrint("Number of files in dir: %d", m_FileCount);
	if(m_FileCount>0)
	{
		m_FileList = (TCHAR**)malloc(m_FileCount*sizeof(TCHAR*));

		int i = 0;
		hFind = FindFirstFile(szDir, &ffd);
		do
		{
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				m_FileList[i] = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
				wcscpy_s(m_FileList[i], MAX_PATH, ffd.cFileName);
				i++;
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);
	}

	m_rtStop = m_FileCount * m_tpf;
	m_rtDuration = m_rtStop;

	return dwError;
}
HRESULT CJpegPushStream::GetStillDirectory(CHAR *dir, size_t size)
{
	CAutoLock cAutolock(&m_lock);
	return S_OK;
}

HRESULT CJpegPushStream::GetCurrentFileName(CHAR *fileName, size_t size)
{
	CAutoLock cAutolock(&m_lock);
	if(m_FileCount == 0)
	{
		sprintf_s(fileName, size, "No data at this date");
		return S_FALSE;
	}
	int ix = m_fileix;
	if(ix >= m_FileCount)
		ix = m_FileCount - 1;
	size_t ret;
	wcstombs_s(&ret, fileName, size, m_FileList[ix], _TRUNCATE);
	return S_OK;
}


////////////////////////////////////////////////////////////////////////
//
// Exported entry points for registration and unregistration 
// (in this case they only call through to default implementations).
//
////////////////////////////////////////////////////////////////////////

//
// DllRegisterServer
//
STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2(TRUE);
}


//
// DllUnregisterServer
//
STDAPI
DllUnregisterServer()
{
    return AMovieDllRegisterServer2(FALSE);
}


//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
    return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}

