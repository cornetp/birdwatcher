#ifndef __BWFILTER_H
#define __BWFILTER_H

#include "bwOverlay.h"
#include "bwFTPSender.h"
#include "bwStillCapture.h"
#include "bwMotion.h"
#include "bwinterfaces.h"

class CBirdwatcher: public CCritSec,
					public CTransInPlaceFilter,
					public CPersistStream,
					public IFtpControl,
					public IStillCaptureControl,
					public IMotionDetectionControl
{
	CMediaType m_media_type;
	CBirdwatcherFTPSender *m_pftp_sender; // FTP connection object
	CBirdwatcherStillCapture *m_pstill_capture; // Still images recorder
	CBirdwatcherMotion *m_pmotion;

public:
	DECLARE_IUNKNOWN;
    CBirdwatcher(TCHAR *pName,LPUNKNOWN pUnk,HRESULT *hr);
	~CBirdwatcher();

	HRESULT CheckInputType(const CMediaType *mtIn);

    // Function needed for the class factory
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	HRESULT Transform(IMediaSample *pSample);
	HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties);
	HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

	STDMETHODIMP Run(REFERENCE_TIME tStart);
	STDMETHODIMP Stop();

	// IPersistStream
    HRESULT WriteToStream(IStream *pStream);
    HRESULT ReadFromStream(IStream *pStream);
	STDMETHODIMP GetClassID(CLSID *pClsid);

	// IFtpControl interface
	STDMETHODIMP GetEnableUpload(BOOL *pval) { return m_pftp_sender->GetEnableUpload(pval); };
	STDMETHODIMP SetEnableUpload(BOOL val)
	{
		HRESULT hr = m_pftp_sender->SetEnableUpload(val);
		if(hr == NOERROR)
			m_send_ftp = val;
		return hr; 
	};
	STDMETHODIMP GetServerName(CHAR *str, size_t s) { return m_pftp_sender->GetServerName(str, s); };
	STDMETHODIMP SetServerName(CHAR *str){ return m_pftp_sender->SetServerName(str); };
	STDMETHODIMP GetUserName(CHAR *str, size_t s) { return m_pftp_sender->GetUserName(str, s); };
	STDMETHODIMP SetUserName(CHAR *str){ return m_pftp_sender->SetUserName(str); };
	STDMETHODIMP GetPassword(CHAR *str, size_t s) { return m_pftp_sender->GetPassword(str, s); };
	STDMETHODIMP SetPassword(CHAR *str){ return m_pftp_sender->SetPassword(str); };
	STDMETHODIMP GetDirectory(CHAR *str, size_t s) { return m_pftp_sender->GetDirectory(str, s); };
	STDMETHODIMP SetDirectory(CHAR *str) { return m_pftp_sender->SetDirectory(str); };
	STDMETHODIMP GetImageFileName(CHAR *str, size_t s, int *i) { return m_pftp_sender->GetImageFileName(str, s, i); };
	STDMETHODIMP SetImageFileName(CHAR *str, int i) { return m_pftp_sender->SetImageFileName(str, i); };
	STDMETHODIMP GetImageSubDir(CHAR *str, size_t s, int *i) { return m_pftp_sender->GetImageSubDir(str, s, i); };
	STDMETHODIMP SetImageSubDir(CHAR *str, int i) { return m_pftp_sender->SetImageSubDir(str, i); };
	STDMETHODIMP GetUploadPeriod(unsigned long *pul) { return m_pftp_sender->GetUploadPeriod(pul); };
	STDMETHODIMP SetUploadPeriod(unsigned long ul) { return m_pftp_sender->SetUploadPeriod(ul); };
	STDMETHODIMP GetJpegQuality(int *i) { return m_pftp_sender->GetJpegQuality(i); };
	STDMETHODIMP SetJpegQuality(int i) { return m_pftp_sender->SetJpegQuality(i); };
	STDMETHODIMP GetImageSizeDivider(int *i) { return m_pftp_sender->GetImageSizeDivider(i); };
	STDMETHODIMP SetImageSizeDivider(int i) { return m_pftp_sender->SetImageSizeDivider(i); };
	STDMETHODIMP GetOverlayEnable(BOOL *b) { return m_pftp_sender->GetOverlayEnable(b); };
	STDMETHODIMP SetOverlayEnable(BOOL b) { return m_pftp_sender->SetOverlayEnable(b); };

	// IStillCaptureControl interface
	STDMETHODIMP GetEnableStillCapture(BOOL *b) { return m_pstill_capture->GetEnableStillCapture(b); };
	STDMETHODIMP SetEnableStillCapture(BOOL b)
	{
		HRESULT hr = m_pstill_capture->SetEnableStillCapture(b);
		if(hr == NOERROR)
			m_still_capture = b;
		return hr; 
	};
	STDMETHODIMP GetStillDirectory(CHAR *str, size_t s) { return m_pstill_capture->GetStillDirectory(str, s); };
	STDMETHODIMP SetStillDirectory(CHAR *str) { return m_pstill_capture->SetStillDirectory(str); };
	STDMETHODIMP GetStillImageFileName(CHAR *str, size_t s, int *i) { return m_pstill_capture->GetStillImageFileName(str, s, i); };
	STDMETHODIMP SetStillImageFileName(CHAR *str, int i) { return m_pstill_capture->SetStillImageFileName(str, i); };
	STDMETHODIMP GetStillImageSubDir(CHAR *str, size_t s, int *i) { return m_pstill_capture->GetStillImageSubDir(str, s, i); };
	STDMETHODIMP SetStillImageSubDir(CHAR *str, int i) { return m_pstill_capture->SetStillImageSubDir(str, i); };	
	STDMETHODIMP GetStillCapturePeriod(unsigned long *pul) { return m_pstill_capture->GetStillCapturePeriod(pul); };
	STDMETHODIMP SetStillCapturePeriod(unsigned long ul) { return m_pstill_capture->SetStillCapturePeriod(ul); };
	STDMETHODIMP GetStillImageSizeDivider(int *i) { return m_pstill_capture->GetStillImageSizeDivider(i); };
	STDMETHODIMP SetStillImageSizeDivider(int i) { return m_pstill_capture->SetStillImageSizeDivider(i); };
	STDMETHODIMP GetStillJpegQuality(int *i) { return m_pstill_capture->GetStillJpegQuality(i); };
	STDMETHODIMP SetStillJpegQuality(int i) { return m_pstill_capture->SetStillJpegQuality(i); };
	STDMETHODIMP GetStillOverlayEnable(BOOL *b) { return m_pstill_capture->GetOverlayEnable(b); };
	STDMETHODIMP SetStillOverlayEnable(BOOL b) { return m_pstill_capture->SetOverlayEnable(b); };	
	
	// IMotionDetectionControl interface
	STDMETHODIMP GetEnableMotionDetection(BOOL *b) { return m_pmotion->GetEnableMotionDetection(b); };
	STDMETHODIMP SetEnableMotionDetection(BOOL b)
	{
		HRESULT hr = m_pmotion->SetEnableMotionDetection(b);
		if(hr == NOERROR)
			m_motion= b;
		return hr; 
	};
private:
	BOOL m_send_ftp;
	BOOL m_still_capture;
	BOOL m_motion;
};


#endif //__BWFILTER_H