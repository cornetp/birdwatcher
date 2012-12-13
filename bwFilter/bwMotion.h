#ifndef __BW_MOTION
#define __BW_MOTION

class CBirdwatcherMotion
{
public:
	CBirdwatcherMotion();
	~CBirdwatcherMotion();

	void NotifyMediaType(CMediaType *pMediaType);
	HRESULT Deliver(IMediaSample *);
	STDMETHODIMP Run();
	STDMETHODIMP Stop();

private:
	CCritSec m_motion_lock;
	BOOL m_motion_enabled;

	int m_fr_id; // frame counter
	float m_sensitivity;

	CMediaType *m_pMediaType;
	BITMAPINFOHEADER *m_pbmi;

	BYTE *m_pPrevImage;
	SYSTEMTIME m_start;

public:
	// IMotionDetectionControl interface
	STDMETHODIMP GetEnableMotionDetection(BOOL *bEnable);
	STDMETHODIMP SetEnableMotionDetection(BOOL bEnable);

};

#endif //__BW_MOTION
