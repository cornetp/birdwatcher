#ifndef __BW_FTPSENDER
#define __BW_FTPSENDER

#include <Wininet.h>

class CBirdwatcher;
class CBirdwatcherFTPSender : public CAMThread
{
	CBirdwatcher *m_pParent;
	BOOL m_connected;
	CMediaType *m_pMediaType;       // Pointer to the current format
	BITMAPINFOHEADER *m_pbmi;

	// sample to process
	BYTE *m_pImage[2]; // two buffers to avoid being blocked in case of long processing

	// Connection handlers
	HINTERNET m_hInternet; 
	HINTERNET m_hFTPSession;

public:
	CBirdwatcherFTPSender(CBirdwatcher *parent);
	~CBirdwatcherFTPSender();

	BOOL IsConnected() { return m_connected; };
	BOOL IsRunning();
	HRESULT Connect();
	HRESULT Disconnect();
	HRESULT Reconnect();

	void NotifyMediaType(CMediaType *pMediaType);
	HRESULT Deliver(IMediaSample *);
	STDMETHODIMP Run(REFERENCE_TIME tStart);
	STDMETHODIMP Stop();

public:
	// IFtpControl interface
	STDMETHODIMP GetEnableUpload(BOOL *pbEnabled);
	STDMETHODIMP SetEnableUpload(BOOL bEnabled);
	STDMETHODIMP GetServerName(CHAR *serverName, size_t size);
	STDMETHODIMP SetServerName(CHAR *serverName);
	STDMETHODIMP GetUserName(CHAR *usrName, size_t size);
	STDMETHODIMP SetUserName(CHAR *usrName);
	STDMETHODIMP GetPassword(CHAR *passwd, size_t size);
	STDMETHODIMP SetPassword(CHAR *passwd);
	STDMETHODIMP GetDirectory(CHAR *dir, size_t size);
	STDMETHODIMP SetDirectory(CHAR *dir);
	STDMETHODIMP GetImageFileName(CHAR *filename, size_t size, int *fileExtension);
	STDMETHODIMP SetImageFileName(CHAR *filename, int fileExtension);
	STDMETHODIMP GetImageSubDir(CHAR *subdir, size_t size, int *subdirExtension);
	STDMETHODIMP SetImageSubDir(CHAR *subdir, int subdirExtension);
	STDMETHODIMP GetUploadPeriod(unsigned long *pUploadPeriod);
	STDMETHODIMP SetUploadPeriod(unsigned long uploadPeriod);
	STDMETHODIMP GetJpegQuality(int *pJpegQuality);
	STDMETHODIMP SetJpegQuality(int jpegQuality);
	STDMETHODIMP GetImageSizeDivider(int *pDivider);
	STDMETHODIMP SetImageSizeDivider(int divider);
	STDMETHODIMP GetOverlayEnable(BOOL *pOverlayEnable);
	STDMETHODIMP SetOverlayEnable(BOOL OverlayEnable);

private:
	CCritSec m_ftpsender_lock; // critical section for the IFtpControl interface

	HANDLE m_hMutex; // critical section for the image buffer
	HANDLE m_hTimer;
	HANDLE m_hStopThreadEvent;
	SYSTEMTIME m_date;
	WORD m_today;


	// ftp settings
	BOOL m_ftp_enabled;
	CHAR m_ftp_server_name[256];
	CHAR m_ftp_user_name[256];
	CHAR m_ftp_password[256];
	CHAR m_ftp_directory[MAX_PATH];
	CHAR m_image_filename[MAX_PATH];
	int m_image_file_extension;
	CHAR m_image_subdir[MAX_PATH];
	int m_image_subdir_extension;

	int m_jpeg_quality;
	int m_image_size_divider;
	unsigned long m_upload_period_sec;

	BOOL m_overlay_enabled;
	CBirdwatcherOverlay *m_pOverlay;
	CHAR m_overlay_chars_file[MAX_PATH];

protected:
	DWORD ThreadProc();
	void Send();
	void Resample(BYTE *bufOut, BYTE *bufIn, BITMAPINFOHEADER *inBMI, int scale);
};

#endif // __BW_FTPSENDER