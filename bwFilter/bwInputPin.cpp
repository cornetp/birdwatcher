#include <streams.h>
#include <initguid.h>

#include "bwfilter.h"
#include "bwInputPin.h"
#include "bwFTPSender.h"

// Input pin
CBirdwatcherInputPin::CBirdwatcherInputPin(TCHAR *pName, CBirdwatcher *pBirdwatcher, HRESULT *phr, LPCWSTR pPinName) :
CBaseInputPin(pName, pBirdwatcher, pBirdwatcher, phr, pPinName),
m_pParentFilter(pBirdwatcher)
{
    ASSERT(pBirdwatcher);
}
CBirdwatcherInputPin::~CBirdwatcherInputPin()
{
	ASSERT(m_pParentFilter->m_pAllocator == NULL);
}

HRESULT CBirdwatcherInputPin::CheckMediaType(const CMediaType *pmt)
{
	CAutoLock lock_it(m_pLock);

	if (m_pParentFilter->m_live_output_pin.m_Connected != NULL)
	{
		if(NOERROR != m_pParentFilter->m_live_output_pin.m_Connected->QueryAccept(pmt))
			return VFW_E_TYPE_NOT_ACCEPTED;
	}

	BITMAPINFOHEADER *pbmi = HEADER(pmt->Format());
	if(pbmi && pbmi->biWidth==640 && pbmi->biHeight==480)
		return NOERROR;

	return S_FALSE;
}

HRESULT CBirdwatcherInputPin::SetMediaType(const CMediaType *pmt)
{
    CAutoLock lock_it(m_pLock);
    HRESULT hr = NOERROR;

    // Make sure that the base class likes it
    hr = CBaseInputPin::SetMediaType(pmt);
    if (FAILED(hr))
        return hr;

    ASSERT(m_Connected != NULL);

	m_media_type = *pmt;
	m_pParentFilter->m_ftp_sender.NotifyMediaType(&m_media_type);

    return NOERROR;

}

HRESULT CBirdwatcherInputPin::BreakConnect()
{
    // Release any allocator that we are holding
    if (m_pParentFilter->m_pAllocator)
    {
        m_pParentFilter->m_pAllocator->Release();
        m_pParentFilter->m_pAllocator = NULL;
    }

    return NOERROR;
}
/*
//
// NotifyAllocator
//
STDMETHODIMP CBirdwatcherInputPin::NotifyAllocator(IMemAllocator *pAllocator, BOOL bReadOnly)
{
    CheckPointer(pAllocator,E_FAIL);
    CAutoLock lock_it(m_pLock);
	
    // Free the old allocator if any
    if (m_pParentFilter->m_pAllocator)
        m_pParentFilter->m_pAllocator->Release();

    // Store away the new allocator
    pAllocator->AddRef();
    m_pParentFilter->m_pAllocator = pAllocator;
	
    // Notify the base class about the allocator
    return CBaseInputPin::NotifyAllocator(pAllocator,bReadOnly);
}
*/


HRESULT CBirdwatcherInputPin::Receive(IMediaSample *pSample)
{
    ASSERT(pSample);
    CAutoLock lock_it(m_pLock);

    // Check that all is well with the base class
    HRESULT hr = NOERROR;
    hr = CBaseInputPin::Receive(pSample);
    if(hr != NOERROR)
        return hr;

	// Fwd to output pin
    CBirdwatcherLiveOutputPin *pOutputPin = &m_pParentFilter->m_live_output_pin;
    if(pOutputPin)
    {
        hr = pOutputPin->Deliver(pSample);
        if(hr != NOERROR)
            return hr;
    }

	// Fwd to FTP sender
	/*
	CBirdwatcherFTPSender *pFtpSender = &m_pParentFilter->m_ftp_sender;
	if(pFtpSender)
	{
		hr = pFtpSender->Deliver(pSample);
		if(hr != NOERROR)
			return hr;
	}*/

    return NOERROR;
} 