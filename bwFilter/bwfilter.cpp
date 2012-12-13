#include <streams.h>
#include <initguid.h>

#include "Debug.h"
#include "bwfilter.h"

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_Video,       // Major CLSID
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN psudPins[] =
{
    { L"Input",             // Pin's string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Allowed none
      FALSE,                // Allowed many
      &CLSID_NULL,          // Connects to filter
      L"Output",            // Connects to pin
      1,                    // Number of types
      &sudPinTypes },       // Pin information
    { L"Live feed Output",  // Pin's string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Allowed none
      FALSE,                // Allowed many
      &CLSID_NULL,          // Connects to filter
      L"Input",             // Connects to pin
      1,                    // Number of types
      &sudPinTypes }        // Pin information
};
const AMOVIESETUP_FILTER sudBirdwatcher =
{
    &CLSID_Birdwatcher,     // CLSID of filter
    L"Birdwatcher",         // Filter's name
    MERIT_DO_NOT_USE,       // Filter merit
    2,                      // Number of pins
    psudPins                // Pin information
};

CFactoryTemplate g_Templates[] = {
    {
        L"Birdwatcher",                  // Name.
        &CLSID_Birdwatcher,              // CLSID.
        CBirdwatcher::CreateInstance,    // Creation function.
        NULL,
        &sudBirdwatcher                  // Pointer to filter information.
    }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

//
// CreateInstance
//
// Creator function for the class ID
//
CUnknown * WINAPI CBirdwatcher::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
    return new CBirdwatcher(NAME("Birdwatcher"), pUnk, phr);
}

STDMETHODIMP CBirdwatcher::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IFtpControl)
        return GetInterface((IFtpControl *) this, ppv);
	else if (riid == IID_IStillCaptureControl)
		return GetInterface((IStillCaptureControl *) this, ppv);
	else if (riid == IID_IMotionDetectionControl)
		return GetInterface((IMotionDetectionControl *) this, ppv);
    else if (riid == IID_IPersistStream)
        return GetInterface((IPersistStream *) this, ppv);
	else
		return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

// Constructor
CBirdwatcher::CBirdwatcher(TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr) :
CTransInPlaceFilter (NAME("Birdwatcher filter"), pUnk, CLSID_Birdwatcher, NULL, FALSE),
CPersistStream(pUnk, phr)
{
	m_pftp_sender = new CBirdwatcherFTPSender(this);
	m_send_ftp = FALSE;

	m_pstill_capture = new CBirdwatcherStillCapture(this);
	m_still_capture = FALSE;

	m_pmotion = new CBirdwatcherMotion();
	m_motion = TRUE;
}

CBirdwatcher::~CBirdwatcher()
{
	if(m_pftp_sender)
		delete m_pftp_sender;

	if(m_pstill_capture)
		delete(m_pstill_capture);

	if(m_pmotion)
		delete(m_pmotion);
}

HRESULT CBirdwatcher::CheckInputType(const CMediaType *mtIn)
{
    CheckPointer(mtIn,E_POINTER);

    // check this is a VIDEOINFOHEADER type
    if (*mtIn->FormatType() != FORMAT_VideoInfo)
        return E_INVALIDARG;

    if (*mtIn->Subtype() != MEDIASUBTYPE_RGB24)
		return E_INVALIDARG;

    VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) mtIn->Format();
	if(pvi->bmiHeader.biBitCount != 24)
		return E_INVALIDARG;

	m_media_type = *mtIn;
	m_pftp_sender->NotifyMediaType(&m_media_type);
	m_pstill_capture->NotifyMediaType(&m_media_type);
	m_pmotion->NotifyMediaType(&m_media_type);
	return NOERROR;
}

HRESULT CBirdwatcher::DecideBufferSize(IMemAllocator *pAlloc,ALLOCATOR_PROPERTIES *pProperties)
{
    // Is the input pin connected
    if (m_pInput->IsConnected() == FALSE) {
        return E_UNEXPECTED;
    }

    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);
    HRESULT hr = NOERROR;

    pProperties->cBuffers = 1;
    pProperties->cbBuffer = m_pInput->CurrentMediaType().GetSampleSize();
    ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted
    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if (FAILED(hr)) {
        return hr;
    }

    ASSERT( Actual.cBuffers == 1 );

    if (pProperties->cBuffers > Actual.cBuffers ||
            pProperties->cbBuffer > Actual.cbBuffer) {
                return E_FAIL;
    }
    return NOERROR;
}

HRESULT CBirdwatcher::GetMediaType(int iPosition, CMediaType *pMediaType)
{
    // Is the input pin connected
    if (m_pInput->IsConnected() == FALSE) {
        return E_UNEXPECTED;
    }

    // This should never happen
    if (iPosition < 0) {
        return E_INVALIDARG;
    }

    // Do we have more items to offer
    if (iPosition > 0) {
        return VFW_S_NO_MORE_ITEMS;
    }

    CheckPointer(pMediaType,E_POINTER);
    *pMediaType = m_pInput->CurrentMediaType();

    return NOERROR;
}

STDMETHODIMP CBirdwatcher::Run(REFERENCE_TIME tStart)
{
    CAutoLock cObjectLock(m_pLock);
    HRESULT hr = CBaseFilter::Run(tStart);

	if(m_send_ftp)
	{
		m_pftp_sender->Connect();
		m_pftp_sender->Run(tStart);
	}
	if(m_still_capture)
		m_pstill_capture->Run();

	if(m_motion)
		m_pmotion->Run();

    return hr;
}

STDMETHODIMP CBirdwatcher::Stop()
{
    CBaseFilter::Stop();
    m_State = State_Stopped;

	if(m_send_ftp)
	{
		m_pftp_sender->Stop();
		m_pftp_sender->Disconnect();
	}
	if(m_still_capture)
		m_pstill_capture->Stop();

	if(m_motion)
		m_pmotion->Stop();

    return NOERROR;
}

HRESULT CBirdwatcher::Transform(IMediaSample *pSample)
{
	if(m_send_ftp)
		m_pftp_sender->Deliver(pSample);

	if(m_still_capture)
		m_pstill_capture->Deliver(pSample);

	if(m_motion)
		m_pmotion->Deliver(pSample);

    return NOERROR;
}

// IPersistStream
#define WRITEOUT(var)  hr = pStream->Write(&var, sizeof(var), NULL); \
               if (FAILED(hr)) return hr;

#define READIN(var)    hr = pStream->Read(&var, sizeof(var), NULL); \
               if (FAILED(hr)) return hr;

STDMETHODIMP CBirdwatcher::GetClassID(CLSID *pClsid)
{
    return CBaseFilter::GetClassID(pClsid);
}

HRESULT CBirdwatcher::WriteToStream(IStream *pStream)
{
	HRESULT hr;

	int streamVersion = 1;

	BOOL bEnabled;
	CHAR serverName[256];
	CHAR userName[256];
	CHAR password[256];
	CHAR directory[MAX_PATH];
	CHAR imageFileName[MAX_PATH];
	int imageFileExtension;
	CHAR imageSubdir[MAX_PATH];
	int imageSubdirExtension;
	int jpegQuality;
	int imageSizeDivider;
	unsigned long uploadPeriod;
	BOOL bOverlayEnabled;

	GetEnableUpload(&bEnabled);
	GetServerName(serverName, 256);
	GetUserName(userName, 256);
	GetPassword(password, 256);
	GetDirectory(directory, MAX_PATH);
	GetImageFileName(imageFileName, MAX_PATH, &imageFileExtension);
	GetImageSubDir(imageSubdir, MAX_PATH, &imageSubdirExtension);
	GetJpegQuality(&jpegQuality);
	GetImageSizeDivider(&imageSizeDivider);
	GetUploadPeriod(&uploadPeriod);
	GetOverlayEnable(&bOverlayEnabled);

	WRITEOUT(streamVersion);
	WRITEOUT(bEnabled);
	WRITEOUT(serverName);
	WRITEOUT(userName);
	WRITEOUT(password);
	WRITEOUT(directory);
	WRITEOUT(imageFileName);
	WRITEOUT(imageFileExtension);
	WRITEOUT(imageSubdir);
	WRITEOUT(imageSubdirExtension);
	WRITEOUT(jpegQuality);
	WRITEOUT(imageSizeDivider);
	WRITEOUT(uploadPeriod);
	WRITEOUT(bOverlayEnabled);

	// Still images capture
	BOOL bStillEnabled;
	CHAR stillDirectory[MAX_PATH];
	CHAR stillImageFileName[MAX_PATH];
	int stillImageFileExtension;
	CHAR stillImageSubdir[MAX_PATH];
	int stillImageSubdirExtension;
	int stillJpegQuality;
	int stillImageSizeDivider;
	unsigned long stillCapturePeriod;
	BOOL bStillOverlayEnabled;

	GetEnableStillCapture(&bStillEnabled);
	GetStillDirectory(stillDirectory, MAX_PATH);
	GetStillImageFileName(stillImageFileName, MAX_PATH, &stillImageFileExtension);
	GetStillImageSubDir(stillImageSubdir, MAX_PATH, &stillImageSubdirExtension);
	GetStillJpegQuality(&stillJpegQuality);
	GetStillImageSizeDivider(&stillImageSizeDivider);
	GetStillCapturePeriod(&stillCapturePeriod);
	GetStillOverlayEnable(&bStillOverlayEnabled);

	WRITEOUT(bStillEnabled);
	WRITEOUT(stillDirectory);
	WRITEOUT(stillImageFileName);
	WRITEOUT(stillImageFileExtension);
	WRITEOUT(stillImageSubdir);
	WRITEOUT(stillImageSubdirExtension);
	WRITEOUT(stillJpegQuality);
	WRITEOUT(stillImageSizeDivider);
	WRITEOUT(stillCapturePeriod);
	WRITEOUT(bStillOverlayEnabled);

	return NOERROR;
}

HRESULT CBirdwatcher::ReadFromStream(IStream *pStream)
{
	HRESULT hr;
	int streamVersion;
	BOOL bEnabled;
	CHAR serverName[256];
	CHAR userName[256];
	CHAR password[256];
	CHAR directory[MAX_PATH];
	CHAR imageFileName[MAX_PATH];
	int imageFileExtension;
	CHAR imageSubdir[MAX_PATH];
	int imageSubdirExtension;
	int jpegQuality;
	int imageSizeDivider;
	unsigned long uploadPeriod;
	BOOL bOverlayEnabled;

	READIN(streamVersion);
	READIN(bEnabled);
	READIN(serverName);
	READIN(userName);
	READIN(password);
	READIN(directory);
	READIN(imageFileName);
	READIN(imageFileExtension);
	READIN(imageSubdir);
	READIN(imageSubdirExtension);
	READIN(jpegQuality);
	READIN(imageSizeDivider);
	READIN(uploadPeriod);
	READIN(bOverlayEnabled);

	SetEnableUpload(bEnabled);
	SetServerName(serverName);
	SetUserName(userName);
	SetPassword(password);
	SetDirectory(directory);
	SetImageFileName(imageFileName, imageFileExtension);
	SetImageSubDir(imageSubdir, imageSubdirExtension);
	SetJpegQuality(jpegQuality);
	SetImageSizeDivider(imageSizeDivider);
	SetUploadPeriod(uploadPeriod);
	SetOverlayEnable(bOverlayEnabled);

	// Still images capture
	BOOL bStillEnabled;
	CHAR stillDirectory[MAX_PATH];
	CHAR stillImageFileName[MAX_PATH];
	int stillImageFileExtension;
	CHAR stillImageSubdir[MAX_PATH];
	int stillImageSubdirExtension;
	int stillJpegQuality;
	int stillImageSizeDivider;
	unsigned long stillCapturePeriod;
	BOOL bStillOverlayEnabled;

	READIN(bStillEnabled);
	READIN(stillDirectory);
	READIN(stillImageFileName);
	READIN(stillImageFileExtension);
	READIN(stillImageSubdir);
	READIN(stillImageSubdirExtension);
	READIN(stillJpegQuality);
	READIN(stillImageSizeDivider);
	READIN(stillCapturePeriod);
	READIN(bStillOverlayEnabled);

	SetEnableStillCapture(bStillEnabled);
	SetStillDirectory(stillDirectory);
	SetStillImageFileName(stillImageFileName, stillImageFileExtension);
	SetStillImageSubDir(stillImageSubdir, stillImageSubdirExtension);
	SetStillJpegQuality(stillJpegQuality);
	SetStillImageSizeDivider(stillImageSizeDivider);
	SetStillCapturePeriod(stillCapturePeriod);
	SetStillOverlayEnable(bStillOverlayEnabled);

	return NOERROR;
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

