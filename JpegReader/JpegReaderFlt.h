#ifndef __JPEGREADERFLT_H
#define __JPEGREADERFLT_H

#include "JpegReaderItf.h"

#define REFTIME_UNITS_PER_MILLISEC	10000i64

class CJpegPushSource : 
	public CSource,
	public IJpegReaderControl
{
public:
	DECLARE_IUNKNOWN;
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:
	CJpegPushSource(LPUNKNOWN lpunk, HRESULT *phr);

/// *** CUnknown *** ///
protected:
	HRESULT STDMETHODCALLTYPE NonDelegatingQueryInterface(REFIID riid, void** ppv);

protected:
/// *** IJpegReaderControl *** ///
	STDMETHODIMP SetStillDirectory(CHAR *c);
	STDMETHODIMP GetStillDirectory(CHAR *c, size_t s);
	STDMETHODIMP GetCurrentFileName(CHAR *c, size_t s);
};


class CJpegPushStream :
	public CSourceStream,
	public CSourceSeeking
{
friend class CJpegPushSource;

private:
	REFERENCE_TIME	m_tpf;
	CMediaType		m_mt;
	LONGLONG		m_i;
	LONGLONG		m_k;
	bool			m_gap;

	LONGLONG		m_fileix;

	TCHAR m_StillDir[MAX_PATH];
	int m_FileCount;
	TCHAR **m_FileList;

	CCritSec m_lock;

	CJpegPushStream(HRESULT *phr, CJpegPushSource *pParent, LPCWSTR pPinName);
	~CJpegPushStream();

/// *** CUnknown *** ///
protected:
	HRESULT STDMETHODCALLTYPE NonDelegatingQueryInterface(REFIID riid, void** ppv);

/// *** CSourceStream *** ///
protected:
	HRESULT GetMediaType(CMediaType* pmt);
	HRESULT DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* lpReq);
	HRESULT FillBuffer(IMediaSample* pms);
	HRESULT OnThreadStartPlay(void);

/// *** CSourceSeeking *** ///	
private:
	HRESULT STDMETHODCALLTYPE OnSeek(bool start);
protected:
	HRESULT ChangeStart(void);
	HRESULT ChangeStop(void);
	HRESULT ChangeRate(void);

protected:
/// *** IJpegReaderControl *** ///
	STDMETHODIMP SetStillDirectory(CHAR *dir);
	STDMETHODIMP GetStillDirectory(CHAR *dir, size_t size);
	STDMETHODIMP GetCurrentFileName(CHAR *c, size_t s);
};

#endif //__JPEGREADERFLT_H