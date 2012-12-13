#include <streams.h>

#include "bwMotion.h"
#include "Debug.h"

CBirdwatcherMotion::CBirdwatcherMotion()
{
	m_fr_id = 0;
	m_pMediaType = NULL;
	m_pbmi = NULL;
	m_pPrevImage = NULL;

	m_motion_enabled = FALSE;
	m_sensitivity = 10.0;
}

CBirdwatcherMotion::~CBirdwatcherMotion()
{
	if(m_pPrevImage)
		free(m_pPrevImage);
}

void CBirdwatcherMotion::NotifyMediaType(CMediaType *pMediaType)
{
    m_pMediaType = pMediaType;
	m_pbmi = HEADER(m_pMediaType->Format());

	if(m_pPrevImage)
	{
		free(m_pPrevImage);
		m_pPrevImage = NULL;
	}
}

HRESULT CBirdwatcherMotion::Deliver(IMediaSample *pSample)
{
	BYTE *ptr;
    HRESULT hr = pSample->GetPointer(&ptr);

	// Do it every 2 frames
	if(m_fr_id++%2)
		return S_OK;

	if(m_pPrevImage)
	{
		int diff = 0;
		for(unsigned int i=0; i<m_pbmi->biSizeImage; i++)
			diff+=abs(ptr[i]-m_pPrevImage[i]);

		if(float(diff)/m_pbmi->biSizeImage > m_sensitivity)
		{
			//SYSTEMTIME time;
			//GetLocalTime(&time);
			//WORD h = time.wHour>=m_start.wHour?time.wHour-m_start.wHour : 24+time.wHour-m_start.wHour;
			//WORD m = time.wMinute>=m_start.wMinute?time.wMinute-m_start.wMinute : 60+time.wMinute-m_start.wMinute;
			//WORD s = time.wSecond>=m_start.wSecond?time.wSecond-m_start.wSecond : 60+time.wSecond-m_start.wSecond;
			//DbgPrint("%02d:%02d:%02d - Motion detected", h, m, s);
			DbgPrint("Motion detected");
		}

		memcpy(m_pPrevImage, ptr, m_pbmi->biSizeImage);
	}
	else
	{
		m_pPrevImage = (BYTE*)malloc(m_pbmi->biSizeImage);
		memcpy(m_pPrevImage, ptr, m_pbmi->biSizeImage);
	}

	return S_OK;
}

STDMETHODIMP CBirdwatcherMotion::Run()
{
	m_fr_id = 0;
	GetLocalTime(&m_start);
	return S_OK;
}

STDMETHODIMP CBirdwatcherMotion::Stop()
{
	return S_OK;
}

STDMETHODIMP CBirdwatcherMotion::GetEnableMotionDetection(BOOL *pbEnabled)
{
	CAutoLock cAutolock(&m_motion_lock);
	*pbEnabled = m_motion_enabled;
	return NOERROR;
}

STDMETHODIMP CBirdwatcherMotion::SetEnableMotionDetection(BOOL bEnabled)
{
	CAutoLock cAutolock(&m_motion_lock);
	m_motion_enabled = bEnabled;
	return NOERROR;
}