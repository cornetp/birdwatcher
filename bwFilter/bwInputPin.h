#ifndef __BW_INPUTPIN
#define __BW_INPUTPIN

class CBirdwatcher;
class CBirdwatcherInputPin : public CBaseInputPin
{
	friend class CBirdwatcherLiveOutputPin;

	CBirdwatcher *m_pParentFilter; // Main filter object

	CMediaType m_media_type;

public:
    CBirdwatcherInputPin(TCHAR *pObjName, CBirdwatcher *pBirdwatcher, HRESULT *phr, LPCWSTR pPinName);
	~CBirdwatcherInputPin();

	HRESULT CheckMediaType(const CMediaType *pmt);
	HRESULT SetMediaType(const CMediaType *pmt);
	HRESULT BreakConnect();

	//STDMETHODIMP NotifyAllocator(IMemAllocator *pAllocator, BOOL bReadOnly);


	STDMETHODIMP Receive(IMediaSample *pSample);
};

#endif // __BW_INPUTPIN