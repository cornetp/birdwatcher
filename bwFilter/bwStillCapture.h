#ifndef __BW_STILLCAPTURE
#define __BW_STILLCAPTURE

class CBirdwatcherStillCapture : public CAMThread
{
public:
	CBirdwatcherStillCapture(CBirdwatcher *parent);
	~CBirdwatcherStillCapture();

	void NotifyMediaType(CMediaType *pMediaType);
	HRESULT Deliver(IMediaSample *);
	STDMETHODIMP Run();
	STDMETHODIMP Stop();
	BOOL IsRunning();

private:
	CBirdwatcher *m_pParent;

	CCritSec m_stillcapture_lock;
	HANDLE m_hMutex;
	HANDLE m_hTimer;
	HANDLE m_hStopThreadEvent;
	WORD m_today;

	CMediaType *m_pMediaType;
	BITMAPINFOHEADER *m_pbmi;
	BYTE *m_pImage;

	BOOL m_stillcapture_enabled;
	CHAR m_still_directory[MAX_PATH];
	CHAR m_still_image_filename[MAX_PATH];
	int m_still_image_file_extension;
	CHAR m_still_image_subdir[MAX_PATH];
	int m_still_image_subdir_extension;

	int m_image_size_divider;
	int m_jpeg_quality;
	unsigned long m_capture_period_sec;

	BOOL m_overlay_enabled;
	CBirdwatcherOverlay *m_pOverlay;
	CHAR m_overlay_chars_file[MAX_PATH];

protected:
	DWORD ThreadProc();
	void Process();
	void Resample(BYTE *bufOut, BYTE *bufIn, BITMAPINFOHEADER *inBMI, int scale);
	
public:
	// IFtpControl interface
	STDMETHODIMP GetEnableStillCapture(BOOL *bEnable);
	STDMETHODIMP SetEnableStillCapture(BOOL bEnable);
	STDMETHODIMP GetStillDirectory(CHAR *dir, size_t size);
	STDMETHODIMP SetStillDirectory(CHAR *dir);
	STDMETHODIMP GetStillImageFileName(CHAR *filename, size_t size, int *fileExtension);
	STDMETHODIMP SetStillImageFileName(CHAR *filename, int fileExtension);
	STDMETHODIMP GetStillImageSubDir(CHAR *subdir, size_t size, int *subdirExtension);
	STDMETHODIMP SetStillImageSubDir(CHAR *subdir, int subdirExtension);
	STDMETHODIMP GetStillCapturePeriod(unsigned long *pCapturePeriod);
	STDMETHODIMP SetStillCapturePeriod(unsigned long capturePeriod);
	STDMETHODIMP GetStillImageSizeDivider(int *pDivider);
	STDMETHODIMP SetStillImageSizeDivider(int divider);
	STDMETHODIMP GetStillJpegQuality(int *pJpegQuality);
	STDMETHODIMP SetStillJpegQuality(int jpegQuality);
	STDMETHODIMP GetOverlayEnable(BOOL *pOverlayEnable);
	STDMETHODIMP SetOverlayEnable(BOOL OverlayEnable);
};

#endif // __BW_STILLCAPTURE

