#ifndef __BW_OUTPUTPIN
#define __BW_OUTPUTPIN

class CBirdwatcher;
class CBirdwatcherLiveOutputPin : public CBaseOutputPin
{
	friend class CBirdwatcherInputPin;
	friend class CBirdwatcher;

	CBirdwatcher *m_pParentFilter; // Main filter object

public:
	CBirdwatcherLiveOutputPin(TCHAR *pObjName, CBirdwatcher *pBirdwatcher, HRESULT *phr, LPCWSTR pPinName);
	~CBirdwatcherLiveOutputPin(){};

	HRESULT CheckMediaType(const CMediaType *pmt);
	STDMETHODIMP EnumMediaTypes(IEnumMediaTypes **ppEnum);

	//HRESULT DecideAllocator(IMemInputPin *pPin, IMemAllocator **ppAlloc);
	HRESULT DecideBufferSize(IMemAllocator *pMemAllocator, ALLOCATOR_PROPERTIES * ppropInputRequest);


	STDMETHODIMP Notify(IBaseFilter *pSender, Quality q);
};



#endif //__BW_OUTPUTPIN
