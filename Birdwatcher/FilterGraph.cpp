#include <stdio.h>
#include <string.h>
#include "FilterGraph.h"
#include <Strsafe.h>
#include "Debug.h"

HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
    IMoniker * pMoniker = NULL;
    IRunningObjectTable *pROT = NULL;

    if (FAILED(GetRunningObjectTable(0, &pROT))) 
    {
        return E_FAIL;
    }
    
    const size_t STRING_LENGTH = 256;

    WCHAR wsz[STRING_LENGTH];
 
   StringCchPrintfW(
        wsz, STRING_LENGTH, 
        L"FilterGraph %08x pid %08x", 
        (DWORD_PTR)pUnkGraph, 
        GetCurrentProcessId()
        );
    
    HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if (SUCCEEDED(hr)) 
    {
        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
            pMoniker, pdwRegister);
        pMoniker->Release();
    }
    pROT->Release();
    
    return hr;
}
void RemoveFromRot(DWORD pdwRegister)
{
    IRunningObjectTable *pROT;
    if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
        pROT->Revoke(pdwRegister);
        pROT->Release();
    }
}

CFilterGraph::CFilterGraph()
{
	//DbgPrint("CFilterGraph::CFilterGraph: Create the filter graph object");
	bIsRunning = false;
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraphBuilder);
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&m_pCaptureGraphBuilder);
	m_pCaptureGraphBuilder->SetFiltergraph(m_pGraphBuilder);
	
	m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
	m_pGraphBuilder->QueryInterface(IID_IFilterGraph, (void**)&m_pFilterGraph);

	ZeroMemory(m_VideoCaptureDevices, sizeof(m_VideoCaptureDevices));
	ZeroMemory(m_VideoCaptureDevicesName, sizeof(m_VideoCaptureDevicesName));
	ZeroMemory(m_AudioCaptureDevices, sizeof(m_AudioCaptureDevices));
	ZeroMemory(m_AudioCaptureDevicesName, sizeof(m_AudioCaptureDevicesName));
	ZeroMemory(m_VideoCompressors, sizeof(m_VideoCompressors));
	ZeroMemory(m_VideoCompressorsName, sizeof(m_VideoCompressorsName));
	m_VideoDeviceIndex = 0;
	m_AudioDeviceIndex = 0;
	m_VideoCompressorIndex = 0;
	m_bMustReconnect = false;

	m_bFirstGraphBuild = true;
	m_bRecordingEnabled = false;
	m_bPreviewEnabled = false;

	m_pVidWin = NULL;
	m_pVideoSource = NULL;
	m_pAudioSource = NULL;
	m_pBirdwatcher = NULL;
	m_pXbar = NULL;
	m_pInfTee = NULL;
	m_pNullRenderer = NULL;
	m_pVideoCompressor = NULL;
	m_pMux = NULL;
	m_pFileWriter = NULL;

	m_pFtpControl = NULL;
	m_pStillCaptureControl = NULL;
	m_pMotionDetectionControl = NULL;

	m_NbVideoSources = 0;

	// Enum Video&Audio sources
	if(FAILED(EnumCaptureDevices()))
		DbgPrint("CFilterGraph::BuildGraph(): error while enumerating capture devices");

	// Compression filters
	if(FAILED(EnumCompressionFilters()))
		DbgPrint("CFilterGraph::BuildGraph(): error while enumerating compression filters");

	swprintf_s(m_pVideoFileName, MAX_PATH, L"video");
	m_VideoFileNameExtension = 1;
	swprintf_s(m_pVideoDirectory, MAX_PATH, L"");
	swprintf_s(m_pVideoSubdir, MAX_PATH, L"");
	m_VideoSubdirExtension = 1;
	

	// Register filter graph
	#ifdef _DEBUG
	AddToRot(m_pGraphBuilder, &m_dwRegister);
	#endif

	// Video file time slicing
	m_bLimitVideoDuration = true;
	m_VideoFileDuration_sec = 3600;
	m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	m_hStopThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_pScheduler = new CBWScheduler();
	m_startRecordingEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_stopRecordingEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// scheduling
	m_bRecordingScheduleEnabled = false;
	struct scheduledEvent startEv;
	struct scheduledEvent stopEv;
	startEv.hEvent = m_startRecordingEvent;
	startEv.wDayOfWeek = 0x7f;
	stopEv.hEvent = m_stopRecordingEvent;
	stopEv.wDayOfWeek = 0x7f;
	for(int i=0; i<7; i++)
	{
		m_VideoRecordingSchedule[i].startTime.wHour = 25;
		m_VideoRecordingSchedule[i].startTime.wMinute = 0;
		m_VideoRecordingSchedule[i].startTime.wSecond = 0;
		m_VideoRecordingSchedule[i].stopTime.wHour = 0;
		m_VideoRecordingSchedule[i].stopTime.wMinute = 0;
		m_VideoRecordingSchedule[i].stopTime.wSecond = 0;

		startEv.wHour[i] = m_VideoRecordingSchedule[i].startTime.wHour;
		startEv.wMinute[i] = m_VideoRecordingSchedule[i].startTime.wMinute;
		startEv.wSecond[i] = m_VideoRecordingSchedule[i].startTime.wSecond;
		stopEv.wHour[i] = m_VideoRecordingSchedule[i].stopTime.wHour;
		stopEv.wMinute[i] = m_VideoRecordingSchedule[i].stopTime.wMinute;
		stopEv.wSecond[i] = m_VideoRecordingSchedule[i].stopTime.wSecond;
	}
	m_pScheduler->AddEvent(&startEv);
	m_pScheduler->AddEvent(&stopEv);

	// Create worker thread
	Create();
}

CFilterGraph::~CFilterGraph()
{
	//DbgPrint("CFilterGraph::~CFilterGraph: Delete the filter graph object");
	#ifdef _DEBUG
	RemoveFromRot(m_dwRegister);
	#endif

	if(m_pScheduler)
		delete m_pScheduler;

	if(m_startRecordingEvent)
		CloseHandle(m_startRecordingEvent);
	if(m_stopRecordingEvent)
		CloseHandle(m_stopRecordingEvent);

	// Exit thread
	SetEvent(m_hStopThreadEvent);
	Close();


	SaveConfig();

	if(m_pVidWin)
	{
		m_pVidWin->put_Visible(OAFALSE);
		m_pVidWin->put_Owner(NULL);
	}
	
	if(m_pMediaControl)
		m_pMediaControl->Stop();

	// delete filters
	SAFE_RELEASE(m_pVideoSource);
	SAFE_RELEASE(m_pAudioSource);
	SAFE_RELEASE(m_pBirdwatcher);
	SAFE_RELEASE(m_pXbar);
	SAFE_RELEASE(m_pInfTee);
	SAFE_RELEASE(m_pNullRenderer);
	SAFE_RELEASE(m_pVideoCompressor);
	SAFE_RELEASE(m_pMux);
	SAFE_RELEASE(m_pFileWriter);

	// Release interfaces
	SAFE_RELEASE(m_pFtpControl);
	SAFE_RELEASE(m_pStillCaptureControl);
	SAFE_RELEASE(m_pMotionDetectionControl);
	SAFE_RELEASE(m_pVidWin);
	SAFE_RELEASE(m_pMediaControl);
	SAFE_RELEASE(m_pFilterGraph);
	SAFE_RELEASE(m_pCaptureGraphBuilder);
	SAFE_RELEASE(m_pGraphBuilder);

    for(int i = 0; i < NUMELMS(m_VideoCaptureDevices); i++)
        SAFE_RELEASE(m_VideoCaptureDevices[i]);
    for(int i = 0; i < NUMELMS(m_AudioCaptureDevices); i++)
        SAFE_RELEASE(m_AudioCaptureDevices[i]);
    for(int i = 0; i < NUMELMS(m_VideoCompressors); i++)
        SAFE_RELEASE(m_VideoCompressors[i]);


	if(m_hTimer)
		CloseHandle(m_hTimer);
	if(m_hStopThreadEvent)
		CloseHandle(m_hStopThreadEvent);

	m_NbVideoSources = 0;

	_unlink("tmp.bwc");
}

HRESULT CFilterGraph::WriteToStream(IStream *pStream)
{
	HRESULT hr;
	WRITEOUT(m_VideoDeviceIndex);
	WRITEOUT(m_AudioDeviceIndex);
	WRITEOUT(m_VideoCompressorIndex);
	WRITEOUT(m_bRecordingEnabled);
	WRITEOUT(m_pVideoDirectory);
	WRITEOUT(m_pVideoSubdir);
	WRITEOUT(m_VideoSubdirExtension);
	WRITEOUT(m_pVideoFileName);
	WRITEOUT(m_VideoFileNameExtension);
	WRITEOUT(m_bLimitVideoDuration);
	WRITEOUT(m_VideoFileDuration_sec);
	WRITEOUT(m_bRecordingScheduleEnabled);	
	for(int i=0; i<7; i++)
	{
		WRITEOUT(m_VideoRecordingSchedule[i].startTime.wHour);
		WRITEOUT(m_VideoRecordingSchedule[i].startTime.wMinute);
		WRITEOUT(m_VideoRecordingSchedule[i].startTime.wSecond);
		WRITEOUT(m_VideoRecordingSchedule[i].stopTime.wHour);
		WRITEOUT(m_VideoRecordingSchedule[i].stopTime.wMinute);
		WRITEOUT(m_VideoRecordingSchedule[i].stopTime.wSecond);
	}

	return hr;
}

HRESULT CFilterGraph::ReadFromStream(IStream *pStream)
{
	HRESULT hr;
	READIN(m_VideoDeviceIndex);
	READIN(m_AudioDeviceIndex);
	READIN(m_VideoCompressorIndex);
	READIN(m_bRecordingEnabled);
	READIN(m_pVideoDirectory);
	READIN(m_pVideoSubdir);
	READIN(m_VideoSubdirExtension);
	READIN(m_pVideoFileName);
	READIN(m_VideoFileNameExtension);
	READIN(m_bLimitVideoDuration);
	READIN(m_VideoFileDuration_sec);
	READIN(m_bRecordingScheduleEnabled);
	for(int i=0; i<7; i++)
	{
		READIN(m_VideoRecordingSchedule[i].startTime.wHour);
		READIN(m_VideoRecordingSchedule[i].startTime.wMinute);
		READIN(m_VideoRecordingSchedule[i].startTime.wSecond);
		READIN(m_VideoRecordingSchedule[i].stopTime.wHour);
		READIN(m_VideoRecordingSchedule[i].stopTime.wMinute);
		READIN(m_VideoRecordingSchedule[i].stopTime.wSecond);
	}

	return hr;
}

HRESULT CFilterGraph::SaveBWFilterConfig()
{
	HRESULT hr = S_OK;
	IStorage *pStorage = NULL;
	IStream *pStream = NULL;
	IPersistStream *pPersist = NULL;
	hr = StgCreateDocfile(L"tmp.bwc", STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, &pStorage);
	hr = pStorage->CreateStream(L"bwFilter", STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream);
	if(m_pBirdwatcher)
	{
		hr = m_pBirdwatcher->QueryInterface(IID_IPersistStream, (void**)&pPersist);
		hr = pPersist->Save(pStream, TRUE);
	}
    SAFE_RELEASE(pStream);
    SAFE_RELEASE(pPersist);
	hr = pStorage->Commit(STGC_DEFAULT);
    SAFE_RELEASE(pStorage);
    return hr;
}

HRESULT CFilterGraph::SaveConfig()
{
	HRESULT hr = S_OK;
	IStorage *pStorage = NULL;
	IStream *pStream = NULL;
	IPersistStream *pPersist = NULL;

	hr = StgCreateDocfile(L"config.bwc", STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, &pStorage);

	hr = pStorage->CreateStream(L"bwFilter", STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream);
	if(m_pBirdwatcher)
	{
		hr = m_pBirdwatcher->QueryInterface(IID_IPersistStream, (void**)&pPersist);
		hr = pPersist->Save(pStream, TRUE);
	}
    SAFE_RELEASE(pStream);
    SAFE_RELEASE(pPersist);

	hr = pStorage->CreateStream(L"application", STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream);
	WriteToStream(pStream);
	SAFE_RELEASE(pStream);

	hr = pStorage->Commit(STGC_DEFAULT);
    SAFE_RELEASE(pStorage);

    return hr;
}

HRESULT CFilterGraph::LoadBWFilterConfig()
{
    if (S_OK != StgIsStorageFile(L"tmp.bwc"))
    {
        return E_FAIL;
    }
	HRESULT hr = S_OK;
	IStorage *pStorage = NULL;
	IPersistStream *pPersistStream = NULL;
	IStream *pStream = NULL;

	hr = StgOpenStorage(L"tmp.bwc", 0, STGM_TRANSACTED | STGM_READ | STGM_SHARE_DENY_WRITE, 0, 0, &pStorage);
	if(FAILED(hr))
		//STG_E_FILENOTFOUND
		return hr;

	if(m_pBirdwatcher)
	{
		hr = m_pBirdwatcher->QueryInterface(IID_IPersistStream, (void**)&pPersistStream);
		hr = pStorage->OpenStream(L"bwFilter", 0, STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStream);
		hr = pPersistStream->Load(pStream);
	}
	SAFE_RELEASE(pPersistStream);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pStorage);
	return hr;
}

HRESULT CFilterGraph::LoadConfig()
{
    if (S_OK != StgIsStorageFile(L"config.bwc"))
    {
        return E_FAIL;
    }
	HRESULT hr = S_OK;
	IStorage *pStorage = NULL;
	IPersistStream *pPersistStream = NULL;
	IStream *pStream = NULL;

	hr = StgOpenStorage(L"config.bwc", 0, STGM_TRANSACTED | STGM_READ | STGM_SHARE_DENY_WRITE, 0, 0, &pStorage);
	if(m_pBirdwatcher)
	{
		hr = m_pBirdwatcher->QueryInterface(IID_IPersistStream, (void**)&pPersistStream);
		hr = pStorage->OpenStream(L"bwFilter", 0, STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStream);
		hr = pPersistStream->Load(pStream);
	}
	SAFE_RELEASE(pPersistStream);
	SAFE_RELEASE(pStream);

	hr = pStorage->OpenStream(L"application", 0, STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStream);
	ReadFromStream(pStream);
	SAFE_RELEASE(pStream);

	SAFE_RELEASE(pStorage);
	return hr;
}

HRESULT CFilterGraph::EnumCaptureDevices()
{
	ICreateDevEnum *pCreateDevEnum=0;
	IEnumMoniker *pEm = 0;
	HRESULT hr = NOERROR;
	UINT uIndex = 0;

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if(hr != NOERROR)
	{
		DbgPrint("CFilterGraph::EnumCaptureDevices(): Error Creating Device Enumerator: 0x%x", hr);
		return hr;
	}

	// Enum video devices
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if(hr != NOERROR)
	{
		if(hr = S_FALSE)
			DbgPrint("CFilterGraph::EnumCaptureDevices(): No Video Input Device found");
		else
			DbgPrint("CFilterGraph::EnumCaptureDevices(): Error Creating Video Class Enumerator: 0x%x", hr);

		goto EnumAudio;
	}

	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;

	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
	{
		IPropertyBag *pBag = NULL;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if(hr == NOERROR)
			{
				size_t i;
				wcstombs_s(&i, m_VideoCaptureDevicesName[uIndex], 256, var.bstrVal, 256); 
				VariantClear(&var); 
                ASSERT(m_VideoCaptureDevices[uIndex] == 0);
                m_VideoCaptureDevices[uIndex] = pM;
                pM->AddRef();
			}			
			pBag->Release();
		}

		pM->Release();
		uIndex++;
	}
	pEm->Release();
	m_NbVideoSources = uIndex;

	// Enum audio devices
EnumAudio:
	ASSERT(pCreateDevEnum != NULL);
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEm, 0);
	pCreateDevEnum->Release();
	if(hr != NOERROR)
	{
		if(hr = S_FALSE)
			DbgPrint("CFilterGraph::EnumCaptureDevices(): No Audio Input Device found", hr);
		else
			DbgPrint("CFilterGraph::EnumCaptureDevices(): Error Creating Audio Class Enumerator: 0x%x", hr);

		return hr;
	}
	pEm->Reset();
	uIndex = 0;

	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
	{
		IPropertyBag *pBag = NULL;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if(hr == NOERROR)
			{
				size_t i;
				wcstombs_s(&i, m_AudioCaptureDevicesName[uIndex], 256, var.bstrVal, 256); 
				VariantClear(&var); 
                ASSERT(m_AudioCaptureDevices[uIndex] == 0);
                m_AudioCaptureDevices[uIndex] = pM;
                pM->AddRef();
			}			
			pBag->Release();
		}
		pM->Release();
		uIndex++;
	}

	pEm->Release();
	
	return hr;
}

HRESULT CFilterGraph::EnumCompressionFilters()
{
	ICreateDevEnum *pCreateDevEnum=0;
	IEnumMoniker *pEm = 0;
	HRESULT hr = NOERROR;
	UINT uIndex = 0;

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if(hr != NOERROR)
	{
		DbgPrint("CFilterGraph::EnumCompressionFilters(): Error Creating Device Enumerator: 0x%x", hr);
		return hr;
	}

	// Enum video compression filters
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoCompressorCategory, &pEm, 0);
	pCreateDevEnum->Release();
	if(hr != NOERROR)
	{
		if(hr = S_FALSE)
			DbgPrint("CFilterGraph::EnumCompressionFilters(): No video compressor found", hr);
		else
			DbgPrint("CFilterGraph::EnumCompressionFilters(): Error creating video compressor enumerator: 0x%x", hr);

		pCreateDevEnum->Release();
		return hr;
	}

	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;

	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
	{
		IPropertyBag *pBag = NULL;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if(hr == NOERROR)
			{
				size_t i;
				wcstombs_s(&i, m_VideoCompressorsName[uIndex], 256, var.bstrVal, 256); 
				VariantClear(&var); 
                ASSERT(m_VideoCompressors[uIndex] == 0);
                m_VideoCompressors[uIndex] = pM;
                pM->AddRef();
			}			
			pBag->Release();
		}

		pM->Release();
		uIndex++;
	}
	pEm->Release();

	return hr;
}

HRESULT CFilterGraph::BuildGraph()
{
	DbgPrint("build graph");
	// Add bwFilter
	HRESULT hr = CoCreateInstance(CLSID_Birdwatcher, NULL, CLSCTX_INPROC,
                                    IID_IBaseFilter, (LPVOID *)&m_pBirdwatcher);

	if(SUCCEEDED(hr))
	{
		hr = m_pGraphBuilder->AddFilter(m_pBirdwatcher, L"Bird Watcher");
		if(SUCCEEDED(hr))
		{
			hr = m_pBirdwatcher->QueryInterface(IID_IFtpControl, (void**)&m_pFtpControl);
			if(FAILED(hr))
				DbgPrint("CFilterGraph::BuildGraph(): cannot query FtpControl interface from bwFilter: 0x%x", hr);

			hr = m_pBirdwatcher->QueryInterface(IID_IStillCaptureControl, (void**)&m_pStillCaptureControl);
			if(FAILED(hr))
				DbgPrint("CFilterGraph::BuildGraph(): cannot query StillCaptureControl interface from bwFilter: 0x%x", hr);

			hr = m_pBirdwatcher->QueryInterface(IID_IMotionDetectionControl, (void**)&m_pMotionDetectionControl);
			if(FAILED(hr))
				DbgPrint("CFilterGraph::BuildGraph(): cannot query MotionDetectionControl interface from bwFilter: 0x%x", hr);
			
		}
		else
			DbgPrint("CFilterGraph::BuildGraph(): cannot add bwFilter to graph: 0x%x", hr);
	}	
	else
		DbgPrint("CFilterGraph::BuildGraph(): cannot create bwFilter: 0x%x", hr);

	if(m_bFirstGraphBuild)
	{
		LoadConfig();
		m_bFirstGraphBuild = false;
	}

	// Add Video Source filter
	DbgPrint("m_VideoDeviceIndex = %d, m_VideoCaptureDevices[m_VideoDeviceIndex] = 0x%x", m_VideoDeviceIndex, m_VideoCaptureDevices[m_VideoDeviceIndex]);
	if(m_VideoCaptureDevices[m_VideoDeviceIndex])
	{
		hr = m_VideoCaptureDevices[m_VideoDeviceIndex]->BindToObject(0, 0, IID_IBaseFilter, (void **)&m_pVideoSource);
		if(SUCCEEDED(hr))
		{
			hr = m_pGraphBuilder->AddFilter(m_pVideoSource, L"Video Capture");
			if(FAILED(hr))
				DbgPrint("CFilterGraph::EnumCaptureDevices: cannot add video capture device to graph: 0x%x", hr);
			// Add crossbar
			hr = m_pCaptureGraphBuilder->FindInterface(&LOOK_UPSTREAM_ONLY, &MEDIATYPE_Video, m_pVideoSource, IID_IAMCrossbar, (void **)&m_pXbar);

			//if(FAILED(hr))
				//DbgPrint("CFilterGraph::EnumCaptureDevices: ccrossbar interface not found: 0x%x", hr);
		}
		else
			DbgPrint("CFilterGraph::EnumCaptureDevices: cannot instanciate device #%d: 0x%x", m_VideoDeviceIndex, hr);
	}

	// Add Audio Source Filter
	if(m_AudioCaptureDevices[m_AudioDeviceIndex])
	{
		hr = m_AudioCaptureDevices[m_AudioDeviceIndex]->BindToObject(0, 0, IID_IBaseFilter, (void **)&m_pAudioSource);
		if(SUCCEEDED(hr))
		{
			hr = m_pGraphBuilder->AddFilter(m_pAudioSource, L"Audio Capture");
			if(FAILED(hr))
				DbgPrint("CFilterGraph::EnumCaptureDevices: cannot add audio capture device to graph: 0x%x", hr);

		}
		else
			DbgPrint("CFilterGraph::EnumCaptureDevices: cannot instanciate audio device #%d: 0x%x", m_AudioDeviceIndex, hr);
		

		IAMAudioInputMixer *pAudioMixer;
		hr = m_pCaptureGraphBuilder->FindInterface(&LOOK_UPSTREAM_ONLY, NULL, m_pAudioSource, IID_IAMAudioInputMixer, (void **)&pAudioMixer);
		SAFE_RELEASE(pAudioMixer);
	}

	// Add Video compressor if needed
	if(!bMustRecord())
	{
		// Add NULL renderer
		hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_pNullRenderer);
		if(SUCCEEDED(hr))
		{
			hr = m_pGraphBuilder->AddFilter(m_pNullRenderer, L"Null Renderer");
			if(FAILED(hr))
				DbgPrint("CFilterGraph::BuildGraph(): cannot add Null renderer to graph: 0x%x", hr);
		}
		else
			DbgPrint("CFilterGraph::BuildGraph(): cannot create the NULL renderer", hr);
	}
	// Connect filters together
	ConnectFilters();

	return S_OK;
}

HRESULT CFilterGraph::ConnectFilters()
{
	DbgPrint("connect filters");
	HRESULT hr = S_OK;

	if(bMustRecord())
	{
		hr = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoSource, NULL, m_pBirdwatcher);
		if(FAILED(hr))
			DbgPrint("CFilterGraph::ConnectFilters: failed to connect BirdWatcher filter: 0x%x", hr);
		//else
		{
			// Add compression filter now
			if(m_VideoCompressors[m_VideoCompressorIndex])
			{
				hr = m_VideoCompressors[m_VideoCompressorIndex]->BindToObject(0, 0, IID_IBaseFilter, (void **)&m_pVideoCompressor);
				if(SUCCEEDED(hr))
				{
					hr = m_pGraphBuilder->AddFilter(m_pVideoCompressor, L"Compressor");
					if(FAILED(hr))
						DbgPrint("CFilterGraph::EnumCaptureDevices: cannot add compression filter to graph: 0x%x", hr);
				}
			}
			// Add mux and file writer
			WCHAR fullName[MAX_PATH];
			if(wcscmp(m_pVideoDirectory, L"") == 0)
				swprintf_s(fullName, L"%s", m_pVideoFileName);
			else
				swprintf_s(fullName, L"%s\\%s", m_pVideoDirectory, m_pVideoFileName);
			
			hr = m_pCaptureGraphBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi, fullName, &m_pMux, &m_pFileWriter);
			if(FAILED(hr))
				DbgPrint("CFilterGraph::EnumCaptureDevices: cannot create mux and file writer: 0x%x", hr);


			// connect pins
			IPin *pOutPin = NULL;
			hr = m_pCaptureGraphBuilder->FindPin(m_pBirdwatcher, PINDIR_OUTPUT, NULL, NULL, TRUE, 0, &pOutPin);
			IPin *pInPin = NULL;
			hr = m_pCaptureGraphBuilder->FindPin(m_pVideoCompressor, PINDIR_INPUT, NULL, NULL, TRUE, 0, &pInPin);

			if(pOutPin && pInPin)
				hr = m_pGraphBuilder->ConnectDirect(pOutPin, pInPin, NULL);

			SAFE_RELEASE(pOutPin);
			SAFE_RELEASE(pInPin);

			hr = m_pCaptureGraphBuilder->RenderStream(NULL, NULL, m_pVideoCompressor, NULL, m_pMux);
			if(FAILED(hr))
				DbgPrint("CFilterGraph::ConnectFilters: failed to connect file writer: 0x%x", hr);
		}
	}
	else
	{
		hr = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoSource, m_pBirdwatcher, m_pNullRenderer);
		if(FAILED(hr))
			DbgPrint("CFilterGraph::ConnectFilters: failed to connect null renderer: 0x%x", hr);
	}

	if(m_bPreviewEnabled)
	{
		hr = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, m_pVideoSource, NULL, NULL);
		if(FAILED(hr))
			DbgPrint("CFilterGraph::ConnectFilters: failed to render preview stream: 0x%x", hr);
	}

	// Audio
	/*
	if(m_pXbar)
	{
		long inPinCount = 0;
		long outPinCount = 0;
		hr = m_pXbar->get_PinCounts(&outPinCount, &inPinCount);
		if(SUCCEEDED(hr))
		{
			for(long i=0; i<outPinCount; i++)
			{
				long PinIndexRelated = 0;
				long PhysicalType = 0;
				hr = m_pXbar->get_CrossbarPinInfo(FALSE, i, &PinIndexRelated, &PhysicalType);
				if(SUCCEEDED(hr))
				{
					if(PhysicalType == PhysConn_Audio_AudioDecoder)
					{
						// try to connect it to audio source
					}
				}
			}
		}
	}*/
	
	if(bMustRecord())
	{	
		hr = CoCreateInstance(CLSID_InfTee, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_pInfTee);
		if(SUCCEEDED(hr))
		{
			hr = m_pGraphBuilder->AddFilter(m_pInfTee, L"Infinite Pin Tee");
			if(FAILED(hr))
				DbgPrint("CFilterGraph::BuildGraph(): cannot add Infinite Pin Tee to graph: 0x%x", hr);
		}
		else
			DbgPrint("CFilterGraph::BuildGraph(): cannot create the Infinite Pin Tee filter", hr);

		hr = m_pCaptureGraphBuilder->RenderStream(NULL, &MEDIATYPE_Audio, m_pAudioSource, m_pInfTee, m_pMux);
		if(FAILED(hr))
			DbgPrint("CFilterGraph::ConnectFilters: failed to connect audio stream to avi mux: 0x%x", hr);

		// Make the audio stream the master stream
		IConfigAviMux *pConfigMux = NULL;
		hr = m_pMux->QueryInterface(IID_IConfigAviMux, (void**)&pConfigMux);
		if (SUCCEEDED(hr))
		{
			pConfigMux->SetMasterStream(1);
			pConfigMux->Release();
		}
	}
	
	if(m_bPreviewEnabled)
	{
		hr = m_pCaptureGraphBuilder->RenderStream(NULL, &MEDIATYPE_Audio, m_pAudioSource, NULL, NULL);
		if(FAILED(hr))
			DbgPrint("CFilterGraph::ConnectFilters: failed to render audio stream: 0x%x", hr);
	}
	return hr;
}

HRESULT CFilterGraph::Reconnect()
{
	if(m_bMustReconnect)
	{
		DbgPrint("Rebuild filter graph");

		SaveBWFilterConfig();
		HRESULT hr = DeleteFilters();
		if(hr != NO_ERROR)
			DbgPrint("Errors while delete filters of the graph: 0x%x", hr);

		hr = BuildGraph();
		if(hr != NO_ERROR)
			DbgPrint("Errors while building the graph: 0x%x", hr);
		LoadBWFilterConfig();
	}

	m_bMustReconnect = false;
	return S_OK;
}

HRESULT CFilterGraph::DeleteFilters()
{
	DbgPrint("delete filters of the graph");
	// Enumerate the filters in the graph.
	IEnumFilters *pEnum = NULL;
	HRESULT hr = m_pGraphBuilder->EnumFilters(&pEnum);
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		while (S_OK == pEnum->Next(1, &pFilter, NULL))
		 {
			 // Remove the filter.
			 m_pGraphBuilder->RemoveFilter(pFilter);
			 // Reset the enumerator.
			 pEnum->Reset();
			 pFilter->Release();
		}
		pEnum->Release();
	}

	SAFE_RELEASE(m_pFtpControl);
	SAFE_RELEASE(m_pStillCaptureControl);
	SAFE_RELEASE(m_pMotionDetectionControl);
	SAFE_RELEASE(m_pVideoSource);
	SAFE_RELEASE(m_pAudioSource);
	SAFE_RELEASE(m_pBirdwatcher);
	SAFE_RELEASE(m_pXbar);
	SAFE_RELEASE(m_pInfTee);
	SAFE_RELEASE(m_pNullRenderer);
	SAFE_RELEASE(m_pVideoCompressor);
	SAFE_RELEASE(m_pMux);
	SAFE_RELEASE(m_pFileWriter);

	return hr;
}

bool CFilterGraph::bMustRecord()
{
	if(!m_bRecordingEnabled)
		return false;

	if(!m_bRecordingScheduleEnabled)
		return true;

	SYSTEMTIME time;
	GetLocalTime(&time);
	if( time.wHour >= m_VideoRecordingSchedule[time.wDay].startTime.wHour &&
		time.wHour < m_VideoRecordingSchedule[time.wDay].stopTime.wHour)
		return true;

	return false;
}

// link renderer to a windows form
void CFilterGraph::SetOwner(HWND handle, LONG off_x, LONG off_y, LONG w, LONG h)
{
	if(m_pVidWin)
	{
		m_pVidWin->put_Visible(OAFALSE);
		m_pVidWin->put_Owner(NULL);
		SAFE_RELEASE(m_pVidWin);
	}

	HRESULT hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&m_pVidWin);
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

// Put renderer visible or not
void CFilterGraph::EnableRenderer(bool bEnable)
{
	if(m_pVidWin)
		m_pVidWin->put_Visible(bEnable? OATRUE : OAFALSE);
}


HRESULT CFilterGraph::UpdateVideoFileName()
{
	HRESULT hr = S_OK;
	if(!m_pFileWriter || !m_pMediaControl)
		return hr;

	bool bMustRestart = false;
	OAFilterState state;
	hr = m_pMediaControl->GetState(INFINITE, &state);

	if(state != State_Stopped)
	{
		m_pMediaControl->Stop();
		bMustRestart = true;
	}
	
	SYSTEMTIME time;
	GetLocalTime(&time);

	WCHAR fullName[MAX_PATH];

	WCHAR subdir[MAX_PATH];
	if(m_VideoSubdirExtension == 1)
		swprintf_s(subdir, L"%s%04d%02d%02d\\", m_pVideoSubdir, time.wYear, time.wMonth, time.wDay);
	else
	{
		if(wcscmp(m_pVideoSubdir, L"") == 0)
			swprintf_s(subdir, L"%s", "");
		else
			swprintf_s(subdir, L"%s\\", m_pVideoSubdir);
	}

	WCHAR fulldir[MAX_PATH];
	if(wcscmp(m_pVideoDirectory, L"") == 0)
		swprintf_s(fulldir, L"%s", subdir);
	else
		swprintf_s(fulldir, L"%s\\%s", m_pVideoDirectory, subdir);

	TCHAR tcfulldir[MAX_PATH];
	size_t i;
	wcstombs_s(&i, tcfulldir, MAX_PATH, fulldir, MAX_PATH); 


	if(m_VideoFileNameExtension == 1)
		swprintf_s(fullName, L"%s%s%02d%02d%02d.avi", fulldir, m_pVideoFileName, time.wHour, time.wMinute, time.wSecond);
	else
		swprintf_s(fullName, L"%s%s.avi", fulldir, m_pVideoFileName);
		
	if(m_bRecordingEnabled)
		CreateDirectory(tcfulldir, NULL);

	hr = m_pFileWriter->SetFileName(fullName, NULL);
	if (FAILED(hr))
		DbgPrint("Rename filewriter faied with error 0x%x (%d)", hr, hr);

	DbgPrint_w(L"Video name = %s", fullName);

	if(bMustRestart)
		m_pMediaControl->Run();

	return hr;
}

// Graph control
void CFilterGraph::Run()
{
	UpdateVideoFileName();

	if(bMustRecord() && m_bLimitVideoDuration)
	{
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = -(m_VideoFileDuration_sec*10000000LL);
		SetWaitableTimer(m_hTimer, &liDueTime, m_VideoFileDuration_sec*1000, NULL, NULL, 0); // periodic timer
	}
	if(m_pMediaControl)
		m_pMediaControl->Run();

	bIsRunning = true;
}

void CFilterGraph::Stop()
{
	CancelWaitableTimer(m_hTimer);
	if(m_pMediaControl)
		m_pMediaControl->Stop();

	bIsRunning = false;
}

HRESULT CFilterGraph::GetVideoSize(LONG *pWidth, LONG *pHeight)
{
	if(!m_pCaptureGraphBuilder)
		return S_FALSE;

	IAMStreamConfig *pStreamCfg = NULL;
	HRESULT hr = m_pCaptureGraphBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
													   &MEDIATYPE_Video, 
													   m_pVideoSource, 
													   IID_IAMStreamConfig, 
													   (void **)&pStreamCfg);
	if(hr == NOERROR)
	{
		AM_MEDIA_TYPE *pmt;
		hr = pStreamCfg->GetFormat(&pmt);
		if(hr == NOERROR)
		{
			*pWidth = HEADER(pmt->pbFormat)->biWidth;
			*pHeight = HEADER(pmt->pbFormat)->biHeight;
			DeleteMediaType(pmt);
		}
		else
			DbgPrint("CFilterGraph::GetVideoSize error: cannot get video format: 0x%x", hr);
	}
	else
		DbgPrint("CFilterGraph::GetVideoSize error: failed to find StreamConfig interface: 0x%x", hr);

	SAFE_RELEASE(pStreamCfg);
	return hr;
}

// Capture devices
HRESULT CFilterGraph::GetVideoDevice(int ix, TCHAR* pname, size_t size)
{
	if(m_VideoCaptureDevices[ix])
	{
		StringCbPrintf(pname, size, "%s", m_VideoCaptureDevicesName[ix]);
		return NOERROR;
	}
	return S_FALSE;
}

int CFilterGraph::GetSelectedVideoDevice()
{
	return m_VideoDeviceIndex;
}

int CFilterGraph::GetSelectedAudioDevice()
{
	return m_AudioDeviceIndex;
}

int CFilterGraph::GetSelectedVideoCompressor()
{
	return m_VideoCompressorIndex;
}

HRESULT CFilterGraph::SelectVideoDevice(int ix)
{
	if(m_VideoDeviceIndex == ix)
		return S_OK;
	if(m_VideoCaptureDevices[ix])
	{
		m_VideoDeviceIndex = ix;
		m_bMustReconnect = true;
		return S_OK;
	}
	return S_OK;
}

HRESULT CFilterGraph::DisplayVideoDevicePropPages()
{
	HRESULT hr = S_OK;
	if(m_pVideoSource)
	{
		ISpecifyPropertyPages *pProp;
		HRESULT hr = m_pVideoSource->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pProp);
		if (SUCCEEDED(hr)) 
		{
			// Get the filter's name and IUnknown pointer.
			FILTER_INFO FilterInfo;
			hr = m_pVideoSource->QueryFilterInfo(&FilterInfo); 
			IUnknown *pFilterUnk;
			m_pVideoSource->QueryInterface(IID_IUnknown, (void **)&pFilterUnk);

			// Show the page. 
			CAUUID caGUID;
			pProp->GetPages(&caGUID);
			pProp->Release();
			OleCreatePropertyFrame(
				NULL,                   // Parent window
				0, 0,                   // Reserved
				FilterInfo.achName,     // Caption for the dialog box
				1,                      // Number of objects (just the filter)
				&pFilterUnk,            // Array of object pointers. 
				caGUID.cElems,          // Number of property pages
				caGUID.pElems,          // Array of property page CLSIDs
				0,                      // Locale identifier
				0, NULL                 // Reserved
			);

			// Clean up.
			pFilterUnk->Release();
			FilterInfo.pGraph->Release(); 
			CoTaskMemFree(caGUID.pElems);
		}
	}
	return hr;
}

HRESULT CFilterGraph::GetAudioDevice(int ix, TCHAR* pname, size_t size)
{
	if(m_AudioCaptureDevices[ix])
	{
		StringCbPrintf(pname, size, "%s", m_AudioCaptureDevicesName[ix]);
		return NOERROR;
	}
	return S_FALSE;
}

HRESULT CFilterGraph::SelectAudioDevice(int ix)
{
	if(m_AudioDeviceIndex == ix)
		return S_OK;
	if(m_AudioCaptureDevices[ix])
	{
		m_AudioDeviceIndex = ix;
		m_bMustReconnect = true;
		return S_OK;
	}
	return S_OK;
}

// Video compressors
HRESULT CFilterGraph::GetVideoCompressor(int ix, TCHAR* pname, size_t size)
{
	if(m_VideoCompressors[ix])
	{
		StringCbPrintf(pname, size, "%s", m_VideoCompressorsName[ix]);
		return NOERROR;
	}
	return S_FALSE;
}

HRESULT CFilterGraph::SelectVideoCompressor(int ix)
{
	if(m_VideoCompressorIndex == ix)
		return S_OK;
	if(m_VideoCompressors[ix])
	{
		m_VideoCompressorIndex = ix;
		m_bMustReconnect = true;
		return S_OK;
	}
	return S_OK;
}

HRESULT CFilterGraph::DisplayVideoCompressorPropPages()
{
	HRESULT hr = S_OK;
	if(m_pVideoCompressor)
	{
		IAMVfwCompressDialogs *pProp;
		HRESULT hr = m_pVideoCompressor->QueryInterface(IID_IAMVfwCompressDialogs, (void **)&pProp);
		if (SUCCEEDED(hr)) 
		{
			// Show the page. 
			pProp->ShowDialog(VfwCompressDialog_Config, NULL);
			pProp->Release();
		}
	}
	return hr;
}

// FTP setting -> bwFilter configuration
HRESULT CFilterGraph::SetEnableFtpUpload(BOOL bEnable)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->SetEnableUpload(bEnable);
}

HRESULT CFilterGraph::GetEnableFtpUpload(BOOL *pbEnable)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->GetEnableUpload(pbEnable);
}

HRESULT CFilterGraph::SetFtpServerInfos(CHAR *serverName, CHAR *usrName, CHAR *passwd, CHAR *dir)
{
	if(!m_pFtpControl)
		return S_FALSE;

	m_pFtpControl->SetServerName(serverName);
	m_pFtpControl->SetUserName(usrName);
	m_pFtpControl->SetPassword(passwd);
	m_pFtpControl->SetDirectory(dir);
	return NOERROR;
}

HRESULT CFilterGraph::GetFtpServerInfos(CHAR *serverName, CHAR *usrName, CHAR *passwd, CHAR *dir)
{
	if(!m_pFtpControl)
		return S_FALSE;
	
	if(serverName)
		m_pFtpControl->GetServerName(serverName, 256);
	if(usrName)
		m_pFtpControl->GetUserName(usrName, 256);
	if(passwd)
		m_pFtpControl->GetPassword(passwd, 256);
	if(dir)
		m_pFtpControl->GetDirectory(dir, 256);

	return NOERROR;
}

HRESULT CFilterGraph::GetImageFileName(CHAR *imageFileName, int *fileExtension)
{
	if(m_pFtpControl)
		return m_pFtpControl->GetImageFileName(imageFileName, MAX_PATH, fileExtension);

	return S_FALSE;
}

HRESULT CFilterGraph::SetImageFileName(CHAR *imageFileName, int fileExtension)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->SetImageFileName(imageFileName, fileExtension);
}

HRESULT CFilterGraph::GetImageSubDir(CHAR *imageSubDir, int *subdirExtension)
{
	if(m_pFtpControl)
		return m_pFtpControl->GetImageSubDir(imageSubDir, MAX_PATH, subdirExtension);

	return S_FALSE;
}

HRESULT CFilterGraph::SetImageSubDir(CHAR *imageSubDir, int subdirExtension)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->SetImageSubDir(imageSubDir, subdirExtension);
}

HRESULT CFilterGraph::SetUploadPeriod(unsigned long uploadPeriod)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->SetUploadPeriod(uploadPeriod);
}

HRESULT CFilterGraph::GetUploadPeriod(unsigned long *pUploadPeriod)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->GetUploadPeriod(pUploadPeriod);
}


HRESULT CFilterGraph::SetJpegQuality(int jpegQuality)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->SetJpegQuality(jpegQuality);
}
HRESULT CFilterGraph::GetJpegQuality(int *pJpegQuality)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->GetJpegQuality(pJpegQuality);
}

HRESULT CFilterGraph::SetImageSizeDivider(int divider)
{
	if(!m_pFtpControl)
		return S_FALSE;
	return m_pFtpControl->SetImageSizeDivider(divider);
}
HRESULT CFilterGraph::GetImageSizeDivider(int *divider)
{
	if(!m_pFtpControl)
		return S_FALSE;
	return m_pFtpControl->GetImageSizeDivider(divider);
}

HRESULT CFilterGraph::SetOverlayEnable(BOOL ovlEnable)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->SetOverlayEnable(ovlEnable);
}

HRESULT CFilterGraph::GetOverlayEnable(BOOL *pOvlEnable)
{
	if(!m_pFtpControl)
		return S_FALSE;

	return m_pFtpControl->GetOverlayEnable(pOvlEnable);
}


// Video recording
bool CFilterGraph::GetEnableRecording()
{
	return m_bRecordingEnabled;
}

void CFilterGraph::SetEnableRecording(bool bEnable)
{
	if(bEnable == m_bRecordingEnabled)
		return;

	m_bMustReconnect = true;
	m_bRecordingEnabled = bEnable;
}

void CFilterGraph::GetVideoFileName(WCHAR *fileName, int *fileNameExtension, WCHAR *dir)
{
	swprintf_s(fileName, MAX_PATH, L"%s", m_pVideoFileName);
	*fileNameExtension = m_VideoFileNameExtension;
	swprintf_s(dir, MAX_PATH, L"%s", m_pVideoDirectory);
}

void CFilterGraph::SetVideoFileName(WCHAR *fileName, int fileNameExtension, WCHAR *dir)
{
	if(wcscmp(m_pVideoFileName, fileName) == 0 && wcscmp(m_pVideoDirectory, dir) == 0 && fileNameExtension == m_VideoFileNameExtension)
		return;

	swprintf_s(m_pVideoFileName, MAX_PATH, L"%s", fileName);
	m_VideoFileNameExtension = fileNameExtension;
	swprintf_s(m_pVideoDirectory, MAX_PATH, L"%s", dir);

	UpdateVideoFileName();
}

void CFilterGraph::GetVideoSubdir(WCHAR *subdir, int *subdirExtension)
{
	swprintf_s(subdir, MAX_PATH, L"%s", m_pVideoSubdir);
	*subdirExtension = m_VideoSubdirExtension;
}

void CFilterGraph::SetVideoSubdir(WCHAR *subdir, int subdirExtension)
{
	swprintf_s(m_pVideoSubdir, MAX_PATH, L"%s", subdir);
	m_VideoSubdirExtension = subdirExtension;

	UpdateVideoFileName();
}

void CFilterGraph::GetVideoFileDuration(bool *bLimitEnabled, unsigned long *duration_sec)
{
	*bLimitEnabled = m_bLimitVideoDuration;
	*duration_sec = m_VideoFileDuration_sec;
}

void CFilterGraph::SetVideoFileDuration(bool bLimitEnabled, unsigned long duration_sec)
{
	m_bLimitVideoDuration = bLimitEnabled;
	m_VideoFileDuration_sec = duration_sec;
}

// Preview
bool CFilterGraph::GetEnablePreview()
{
	return m_bPreviewEnabled;
}

void CFilterGraph::SetEnablePreview(bool bEnable)
{
	if(bEnable == m_bPreviewEnabled)
		return;

	m_bMustReconnect = true;
	m_bPreviewEnabled = bEnable;
}


bool CFilterGraph::GetEnableRecordingSchedule()
{
	return m_bRecordingScheduleEnabled;
}

void CFilterGraph::SetEnableRecordingSchedule(bool bEnable)
{
	m_bRecordingScheduleEnabled = bEnable;
}

void CFilterGraph::GetVideoRecordingSchedule(short day, SYSTEMTIME *startTime, SYSTEMTIME *stopTime)
{
	memcpy(startTime, &m_VideoRecordingSchedule[day].startTime, sizeof(SYSTEMTIME));
	memcpy(stopTime, &m_VideoRecordingSchedule[day].stopTime, sizeof(SYSTEMTIME));
}

void CFilterGraph::SetVideoRecordingSchedule(short day, SYSTEMTIME *startTime, SYSTEMTIME *stopTime)
{
	memcpy(&m_VideoRecordingSchedule[day].startTime, startTime, sizeof(SYSTEMTIME));
	memcpy(&m_VideoRecordingSchedule[day].stopTime, stopTime, sizeof(SYSTEMTIME));
}

// Still images capture
HRESULT CFilterGraph::SetEnableStillCapture(BOOL bEnable)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->SetEnableStillCapture(bEnable);
}

HRESULT CFilterGraph::GetEnableStillCapture(BOOL *pbEnable)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->GetEnableStillCapture(pbEnable);
}

HRESULT CFilterGraph::GetStillDirectory(CHAR *dir)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	m_pStillCaptureControl->GetStillDirectory(dir, MAX_PATH);

	return NOERROR;
}

HRESULT CFilterGraph::SetStillDirectory(CHAR *dir)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	m_pStillCaptureControl->SetStillDirectory(dir);

	return NOERROR;
}

HRESULT CFilterGraph::GetStillImageFileName(CHAR *imageFileName, int *fileExtension)
{
	if(m_pStillCaptureControl)
		return m_pStillCaptureControl->GetStillImageFileName(imageFileName, MAX_PATH, fileExtension);

	return S_FALSE;
}

HRESULT CFilterGraph::SetStillImageFileName(CHAR *imageFileName, int fileExtension)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->SetStillImageFileName(imageFileName, fileExtension);
}

HRESULT CFilterGraph::GetStillImageSubDir(CHAR *imageSubDir, int *subdirExtension)
{
	if(m_pStillCaptureControl)
		return m_pStillCaptureControl->GetStillImageSubDir(imageSubDir, MAX_PATH, subdirExtension);

	return S_FALSE;
}

HRESULT CFilterGraph::SetStillImageSubDir(CHAR *imageSubDir, int subdirExtension)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->SetStillImageSubDir(imageSubDir, subdirExtension);
}

HRESULT CFilterGraph::SetStillCapturePeriod(unsigned long capturePeriod)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->SetStillCapturePeriod(capturePeriod);
}

HRESULT CFilterGraph::GetStillCapturePeriod(unsigned long *pCapturePeriod)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->GetStillCapturePeriod(pCapturePeriod);
}

HRESULT CFilterGraph::SetStillImageSizeDivider(int divider)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;
	return m_pStillCaptureControl->SetStillImageSizeDivider(divider);
}
HRESULT CFilterGraph::GetStillImageSizeDivider(int *divider)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;
	return m_pStillCaptureControl->GetStillImageSizeDivider(divider);
}

HRESULT CFilterGraph::SetStillJpegQuality(int jpegQuality)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->SetStillJpegQuality(jpegQuality);
}
HRESULT CFilterGraph::GetStillJpegQuality(int *pJpegQuality)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->GetStillJpegQuality(pJpegQuality);
}

HRESULT CFilterGraph::SetStillOverlayEnable(BOOL ovlEnable)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->SetStillOverlayEnable(ovlEnable);
}

HRESULT CFilterGraph::GetStillOverlayEnable(BOOL *pOvlEnable)
{
	if(!m_pStillCaptureControl)
		return S_FALSE;

	return m_pStillCaptureControl->GetStillOverlayEnable(pOvlEnable);
}

// Motion detection
HRESULT CFilterGraph::SetEnableMotionDetection(BOOL bEnable)
{
	if(!m_pMotionDetectionControl)
		return S_FALSE;

	return m_pMotionDetectionControl->SetEnableMotionDetection(bEnable);
}

HRESULT CFilterGraph::GetEnableMotionDetection(BOOL *pbEnable)
{
	if(!m_pMotionDetectionControl)
		return S_FALSE;

	return m_pMotionDetectionControl->GetEnableMotionDetection(pbEnable);
}

// Thread for scheduled events
DWORD CFilterGraph::ThreadProc()
{
    BOOL bShutDown = FALSE;
    while (!bShutDown)
	{
		HANDLE objs[4] = {m_hStopThreadEvent, m_startRecordingEvent, m_stopRecordingEvent, m_hTimer};
		DWORD dwWaitResult = WaitForMultipleObjects(4, objs, FALSE, INFINITE);
		if(dwWaitResult ==  WAIT_OBJECT_0)
		{
			bShutDown = TRUE;
		}
		else if(dwWaitResult ==  WAIT_OBJECT_0 + 1)
		{
			DbgPrint(" ********* Start video recording *********");
			bool bMustRestart = false;
			OAFilterState state;
			m_pMediaControl->GetState(INFINITE, &state);

			if(state != State_Stopped)
			{
				Stop();
				bMustRestart = true;
			}
			m_bMustReconnect = true;
			Reconnect();

			if(bMustRestart)
				Run();

		}
		else if(dwWaitResult ==  WAIT_OBJECT_0 + 2)
		{
			DbgPrint(" ********* Stop video recording *********");
			bool bMustRestart = false; // same code as for start recording (will depend on the time)
			OAFilterState state;
			m_pMediaControl->GetState(INFINITE, &state);

			if(state != State_Stopped)
			{
				Stop();
				bMustRestart = true;
			}
			m_bMustReconnect = true;
			Reconnect();

			if(bMustRestart)
				Run();
		}
		else
		{
			DbgPrint("Update video filename");
			UpdateVideoFileName();
		}
	}
	DbgPrint("Exit thread");
	return 1;
}