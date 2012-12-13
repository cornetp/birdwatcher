#include <streams.h>
#include <initguid.h>

#include "bwfilter.h"
#include "bwOutputPin.h"

// Live output pin
CBirdwatcherLiveOutputPin::CBirdwatcherLiveOutputPin(TCHAR *pName, CBirdwatcher *pBirdwatcher, HRESULT *phr, LPCWSTR pPinName) :
CBaseOutputPin(pName, pBirdwatcher, pBirdwatcher, phr, pPinName),
m_pParentFilter(pBirdwatcher)
{
}

HRESULT CBirdwatcherLiveOutputPin::CheckMediaType(const CMediaType *pmt)
{
	CAutoLock lock_it(m_pLock);

	// The input needs to have been conneced first
	if(m_pParentFilter->m_input_pin.m_Connected == NULL)
	{
		return VFW_E_NOT_CONNECTED;
	}
	
	// Make sure that our input pin peer is happy with this
    HRESULT hr = m_pParentFilter->m_input_pin.m_Connected->QueryAccept(pmt);
    if(hr != NOERROR)
    {
        return VFW_E_TYPE_NOT_ACCEPTED;
    }

	BITMAPINFOHEADER *pbmi = HEADER(pmt->Format());
	return NOERROR;
}

STDMETHODIMP CBirdwatcherLiveOutputPin::EnumMediaTypes(IEnumMediaTypes **ppEnum)
{
    CAutoLock lock_it(m_pLock);
    ASSERT(ppEnum);

    // Make sure that we are connected
    if(m_pParentFilter->m_input_pin.m_Connected == NULL)
        return VFW_E_NOT_CONNECTED;

    // We will simply return the enumerator of our input pin's peer
    return m_pParentFilter->m_input_pin.m_Connected->EnumMediaTypes(ppEnum);

}
/*
HRESULT CBirdwatcherLiveOutputPin::DecideAllocator(IMemInputPin *pPin, IMemAllocator **ppAlloc)
{
    CheckPointer(pPin,E_POINTER);
    CheckPointer(ppAlloc,E_POINTER);
    ASSERT(m_pParentFilter->m_pAllocator != NULL);
	
    *ppAlloc = NULL;

    // Tell the pin about our allocator, set by the input pin.
    HRESULT hr = NOERROR;
    hr = pPin->NotifyAllocator(m_pParentFilter->m_pAllocator,TRUE);
    if(FAILED(hr))
        return hr;

    // Return the allocator
    *ppAlloc = m_pParentFilter->m_pAllocator;
    m_pParentFilter->m_pAllocator->AddRef();
    return NOERROR;
}*/

HRESULT CBirdwatcherLiveOutputPin::DecideBufferSize(IMemAllocator *pMemAllocator, ALLOCATOR_PROPERTIES * ppropInputRequest)
{
	return NOERROR;
}



STDMETHODIMP CBirdwatcherLiveOutputPin::Notify(IBaseFilter *pSender, Quality q)
{
    // We pass the message on, which means that we find the quality sink
    // for our input pin and send it there

    if(m_pParentFilter->m_input_pin.m_pQSink!=NULL)
    {
        return m_pParentFilter->m_input_pin.m_pQSink->Notify(m_pParentFilter, q);
    }
    else
    {
        // No sink set, so pass it upstream
        HRESULT hr;
        IQualityControl * pIQC;

        hr = VFW_E_NOT_FOUND;
        if(m_pParentFilter->m_input_pin.m_Connected)
        {
            m_pParentFilter->m_input_pin.m_Connected->QueryInterface(IID_IQualityControl,(void**)&pIQC);

            if(pIQC!=NULL)
            {
                hr = pIQC->Notify(m_pParentFilter, q);
                pIQC->Release();
            }
        }
        return hr;
    }


    // Quality management is too hard to do
    return NOERROR;

} 