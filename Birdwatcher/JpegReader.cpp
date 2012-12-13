#include "JpegReader.h"
#include "FilterGraph.h"
#include "Debug.h"

CJpegReader::CJpegReader()
{
	m_pSrcFilter = NULL;
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGrfBuilder);
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&m_pCaptureGrfBuilder);
	m_pCaptureGrfBuilder->SetFiltergraph(m_pGrfBuilder);

	m_pGrfBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);

	m_pGrfBuilder->QueryInterface(IID_IMediaEventEx, (void**)&m_pMediaEvent);
	if(m_pMediaEvent)
	{
		m_pMediaEvent->GetEventHandle((OAEVENT*)&m_hGraphEvent);
		DbgPrint("Graph event handler = 0x%x", m_pMediaEvent);
	}
	else
		m_hGraphEvent = NULL;


	m_seekCaps = 0;
	m_pSeek = NULL;
	m_pVidWin = NULL;
	m_pJpegReaderControl = NULL;
	m_pMux = NULL;
	m_pFileWriter = NULL;

	
	// Register filter graph
	#ifdef _DEBUG
	DbgPrint("CJpegReader::CJpegReader Add to Rot");
	AddToRot(m_pGrfBuilder, &m_dwRegister);
	#endif
	m_state = STATE_CLOSED;

	m_hStopThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_complete_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	Create(); // start worker thread
}

CJpegReader::~CJpegReader()
{

	SetEvent(m_hStopThreadEvent);
	Close(); // wait for thread to exit

#ifdef _DEBUG
	RemoveFromRot(m_dwRegister);
	#endif

	if(m_pVidWin)
	{
		m_pVidWin->put_Visible(OAFALSE);
		m_pVidWin->put_Owner(NULL);
	}

	if(m_pMediaControl)
		m_pMediaControl->Stop();

	SAFE_RELEASE(m_pSrcFilter);

	SAFE_RELEASE(m_pVidWin);
	SAFE_RELEASE(m_pSeek);
	SAFE_RELEASE(m_pJpegReaderControl);
	SAFE_RELEASE(m_pMediaControl);
	SAFE_RELEASE(m_pMediaEvent);
	SAFE_RELEASE(m_pCaptureGrfBuilder);
	SAFE_RELEASE(m_pGrfBuilder);
	SAFE_RELEASE(m_pMux);
	SAFE_RELEASE(m_pFileWriter);
}

HRESULT CJpegReader::BuildGraph()
{
	DbgPrint("CJpegReader::BuildGraph");
	HRESULT	hr = S_OK;
	if(!m_pSrcFilter)
	{
		hr = CoCreateInstance(CLSID_JpegPushSource, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID *)&m_pSrcFilter);
		if(SUCCEEDED(hr))
			hr = m_pGrfBuilder->AddFilter(m_pSrcFilter, L"JPEG streamer");
		else
			DbgPrint("CJpegReader::BuildGraph(): cannot create JpegPushSource: 0x%x", hr);
	}

	if(SUCCEEDED(hr))
	{
		hr = m_pCaptureGrfBuilder->RenderStream(NULL, &MEDIATYPE_Video, m_pSrcFilter, NULL, NULL);
		if(FAILED(hr))
			DbgPrint("CJpegReader::BuildGraph: failed to connect source filter: 0x%x", hr);

		if(!m_pJpegReaderControl)
		{
			hr = m_pSrcFilter->QueryInterface(IID_IJpegReaderControl, (void**)&m_pJpegReaderControl);
			if(FAILED(hr))
				DbgPrint("CJpegReader::BuildGraph(): cannot query IJpegReaderControl interface from JpegPushSource: 0x%x", hr);
		}
		if(!m_pSeek)
		{
			hr = m_pGrfBuilder->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeek);
			if(SUCCEEDED(hr))
				m_pSeek->GetCapabilities(&m_seekCaps);
		}
	}	

	if (SUCCEEDED(hr))
		m_state = STATE_STOPPED;
	return hr;
}

HRESULT CJpegReader::BuildRecordingGraph(WCHAR *fileName)
{
	HRESULT	hr = S_OK;
	if(!m_pSrcFilter)
	{
		hr = CoCreateInstance(CLSID_JpegPushSource, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID *)&m_pSrcFilter);
		if(SUCCEEDED(hr))
			hr = m_pGrfBuilder->AddFilter(m_pSrcFilter, L"JPEG streamer");
		else
			DbgPrint("CJpegReader::BuildRecordingGraph(): cannot create JpegPushSource: 0x%x", hr);
	}

	if(!m_pJpegReaderControl)
	{
		hr = m_pSrcFilter->QueryInterface(IID_IJpegReaderControl, (void**)&m_pJpegReaderControl);
		if(FAILED(hr))
			DbgPrint("CJpegReader::BuildGraph(): cannot query IJpegReaderControl interface from JpegPushSource: 0x%x", hr);
	}

	hr = m_pCaptureGrfBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi, fileName, &m_pMux, &m_pFileWriter);
	if(FAILED(hr))
		DbgPrint("CFilterGraph::EnumCaptureDevices: cannot create mux and file writer: 0x%x", hr);

	hr = m_pCaptureGrfBuilder->RenderStream(NULL, NULL, m_pSrcFilter, NULL, m_pMux);
	if(FAILED(hr))
		DbgPrint("CFilterGraph::ConnectFilters: failed to connect file writer: 0x%x", hr);

	return hr;
}

HRESULT CJpegReader::DeleteFilters()
{
	DbgPrint("delete filters of the graph");
	// Enumerate the filters in the graph.
	IEnumFilters *pEnum = NULL;
	HRESULT hr = m_pGrfBuilder->EnumFilters(&pEnum);
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		while (S_OK == pEnum->Next(1, &pFilter, NULL))
		 {
			 // Remove the filter.
			 m_pGrfBuilder->RemoveFilter(pFilter);
			 // Reset the enumerator.
			 pEnum->Reset();
			 pFilter->Release();
		}
		pEnum->Release();
	}

	SAFE_RELEASE(m_pSrcFilter);
	SAFE_RELEASE(m_pJpegReaderControl);
	SAFE_RELEASE(m_pSeek);
	SAFE_RELEASE(m_pMux);
	SAFE_RELEASE(m_pFileWriter);
	return hr;
}

HRESULT CJpegReader::Disconnect()
{
	DbgPrint("CJpegReader::Disconnect");
	IEnumFilters *pEnumFilters = NULL;
	HRESULT hr = m_pGrfBuilder->EnumFilters(&pEnumFilters);
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		while(S_OK == pEnumFilters->Next(1, &pFilter, NULL))
		{
			IEnumPins *pEnumPins = NULL;
			hr = pFilter->EnumPins(&pEnumPins);
			if(SUCCEEDED(hr))
			{
				IPin* pIPin = NULL;
				while(S_OK == pEnumPins->Next(1, &pIPin, NULL))
				{
					IPin* pIPinConnection = NULL;
					if (S_OK == pIPin->ConnectedTo(&pIPinConnection))
					{
						// pins are connected, to disconnect filters, both pins must be disconnected
						hr = m_pGrfBuilder->Disconnect(pIPin);
						hr = m_pGrfBuilder->Disconnect(pIPinConnection);
						SAFE_RELEASE(pIPinConnection);
					}
					SAFE_RELEASE(pIPin);
				}
				SAFE_RELEASE(pEnumPins);
			}
			SAFE_RELEASE(pFilter);
		}
		SAFE_RELEASE(pEnumFilters);
	}
	return S_OK;
}

HRESULT CJpegReader::Run()
{
	ResetEvent(m_complete_event);
	if (m_state != STATE_PAUSED && m_state != STATE_STOPPED)
		return VFW_E_WRONG_STATE;

	if(m_pMediaControl)
	{
		HRESULT hr = m_pMediaControl->Run();

		if (SUCCEEDED(hr))
			m_state = STATE_RUNNING;

		return hr;
	}
	return E_FAIL;
}

HRESULT CJpegReader::Stop()
{
	if (m_state != STATE_RUNNING && m_state != STATE_PAUSED)
		return VFW_E_WRONG_STATE;

	if(m_pMediaControl)
	{
		HRESULT hr = m_pMediaControl->Stop();
		//HRESULT hr = m_pMediaControl->StopWhenReady();
		if (SUCCEEDED(hr))
			m_state = STATE_STOPPED;

		return hr;
	}
	return E_FAIL;
}

HRESULT CJpegReader::Pause()
{
	if (m_state != STATE_RUNNING)
		return VFW_E_WRONG_STATE;

	if(m_pMediaControl)
	{
		HRESULT hr = m_pMediaControl->Pause();
		if (SUCCEEDED(hr))
			m_state = STATE_PAUSED;

		return hr;
	}
	return E_FAIL;
}

bool CJpegReader::CanSeek()
{
	const DWORD caps = AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration;
	return ((m_seekCaps & caps) == caps);
}

HRESULT CJpegReader::SetPosition(REFERENCE_TIME *pos)
{
	if(m_pMediaControl == NULL || m_pSeek == NULL)
		return E_UNEXPECTED;

	HRESULT hr = S_OK;

	hr = m_pSeek->SetPositions(pos, AM_SEEKING_AbsolutePositioning,
		NULL, AM_SEEKING_NoPositioning);

	if (SUCCEEDED(hr))
	{
		// If playback is stopped, we need to put the graph into the paused
		// state to update the video renderer with the new frame, and then stop 
		// the graph again. The IMediaControl::StopWhenReady does this.
		if (m_state == STATE_STOPPED)
		{
			hr = m_pMediaControl->StopWhenReady();
		}
	}

	return S_OK;
}

HRESULT CJpegReader::GetCurrentPosition(LONGLONG *pTimeNow)
{
	if (m_pSeek == NULL)
	{
		return E_UNEXPECTED;
	}

	return m_pSeek->GetCurrentPosition(pTimeNow);
}

HRESULT CJpegReader::GetDuration(LONGLONG *pDuration)
{
	if (m_pSeek == NULL)
		return E_UNEXPECTED;

	return m_pSeek->GetDuration(pDuration);
}

HRESULT CJpegReader::GetCurrentFileName(CHAR *fileName, size_t size)
{
	if (m_pJpegReaderControl == NULL)
	{
		return E_UNEXPECTED;
	}

	return m_pJpegReaderControl->GetCurrentFileName(fileName, size);
}

void CJpegReader::SetOwner(HWND handle, LONG off_x, LONG off_y, LONG w, LONG h)
{
	if(m_pVidWin)
	{
		m_pVidWin->put_Visible(OAFALSE);
		m_pVidWin->put_Owner(NULL);
		SAFE_RELEASE(m_pVidWin);
	}

	HRESULT hr = m_pGrfBuilder->QueryInterface(IID_IVideoWindow, (void **)&m_pVidWin);
	if(!m_pVidWin)
	{
		DbgPrint("Error: failed to retrieve VideoWindow interface: 0x%x", hr);
		return;
	}

	m_pVidWin->put_Owner((OAHWND)handle);
	m_pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

	RECT grc;
	GetClientRect(handle, &grc);
	m_pVidWin->SetWindowPosition(off_x, off_y, w, h);
}

HRESULT CJpegReader::SetStillDirectory(CHAR *StillDir)
{
	if(!m_pJpegReaderControl)
		return S_FALSE;

	return m_pJpegReaderControl->SetStillDirectory(StillDir);
}

HRESULT CJpegReader::GetCompleteEvent(HANDLE *h)
{
	*h = m_complete_event;
	return S_OK;
}

DWORD CJpegReader::ThreadProc()
{
    BOOL bShutDown = FALSE;
    while (!bShutDown)
	{
		HANDLE objs[2] = {m_hStopThreadEvent, m_hGraphEvent};
		DWORD dwWaitResult = WaitForMultipleObjects(2, objs, FALSE, INFINITE);
		if(dwWaitResult ==  WAIT_OBJECT_0)
		{
			bShutDown = TRUE;
		}
		else
		{
			long lEventCode;
	        LONG_PTR lpParam1, lpParam2;
			if(SUCCEEDED(m_pMediaEvent->GetEvent(&lEventCode, &lpParam1, &lpParam2, 1000)))
				m_pMediaEvent->FreeEventParams(lEventCode, lpParam1, lpParam2);
			switch(lEventCode)
			{		
			case EC_ERRORABORT:
				DbgPrint("CJpegReader::ThreadProc(): EC_ERRORABORT");
				break;
			case EC_COMPLETE:
				DbgPrint("CJpegReader::ThreadProc(): EC_COMPLETE");
				Stop();
				if(m_complete_event)
					SetEvent(m_complete_event);
				break;
			default:
				DbgPrint("CJpegReader::ThreadProc(): Filter Graph event 0x%x", lEventCode);
				break;
			}
			
		}
	}

	DbgPrint("CJpegReader::ThreadProc Exit thread");
	return 1;
}