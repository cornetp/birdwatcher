#pragma once

#include <Streams.h>
#include "../bwFilter/bwinterfaces.h"
#include "bwstream.h"
#include "Scheduler.h"

EXTERN_C const CLSID CLSID_NullRenderer;


// Macros
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#endif

#define WRITEOUT(var)  hr = pStream->Write(&var, sizeof(var), NULL); \
               if (FAILED(hr)) return hr;

#define READIN(var)    hr = pStream->Read(&var, sizeof(var), NULL); \
               if (FAILED(hr)) return hr;

HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
void RemoveFromRot(DWORD pdwRegister);

struct schedulerEntry {
	SYSTEMTIME startTime;
	SYSTEMTIME stopTime;
};

class CWebServer;
class CFilterGraph : public CAMThread
{
public:
	CFilterGraph();
	~CFilterGraph();

	// We must know the web server object to write to the persist stream.
	void SetWebServer(CWebServer *ws) { m_pWebServer = ws; };

	// Interface functions
	HRESULT BuildGraph();
	void SetOwner(HWND handle, LONG off_x, LONG off_y, LONG w, LONG h);
	void EnableRenderer(bool bEnable);

	// Video capture devices
	int m_NbVideoSources;
	HRESULT GetVideoDevice(int ix, TCHAR* pname, size_t size);
	int GetSelectedVideoDevice();
	HRESULT SelectVideoDevice(int ix);
	HRESULT DisplayVideoDevicePropPages();

	// Audio capture devices
	HRESULT GetAudioDevice(int ix, TCHAR* pname, size_t size);
	int GetSelectedAudioDevice();
	HRESULT SelectAudioDevice(int ix);

	// Video compression
	HRESULT GetVideoCompressor(int ix, TCHAR* pname, size_t size);
	int GetSelectedVideoCompressor();
	HRESULT SelectVideoCompressor(int ix);
	HRESULT DisplayVideoCompressorPropPages();

	// FTP Settings
	HRESULT SetEnableFtpUpload(BOOL bEnable);
	HRESULT GetEnableFtpUpload(BOOL *pbEnable);
	HRESULT SetFtpServerInfos(CHAR *serverName, CHAR *usrName, CHAR *passwd, CHAR *dir);
	HRESULT GetFtpServerInfos(CHAR *serverName, CHAR *usrName, CHAR *passwd, CHAR *dir);
	HRESULT GetImageFileName(CHAR *imageFileName, int *fileExtension);
	HRESULT SetImageFileName(CHAR *imageFileName, int fileExtension);
	HRESULT GetImageSubDir(CHAR *imageSubDir, int *subdirExtension);
	HRESULT SetImageSubDir(CHAR *imageSubDir, int subdirExtension);
	HRESULT SetUploadPeriod(unsigned long uploadPeriod);
	HRESULT GetUploadPeriod(unsigned long *pUploadPeriod);
	HRESULT SetJpegQuality(int jpegQuality);
	HRESULT GetJpegQuality(int *pJpegQuality);
	HRESULT SetImageSizeDivider(int divider);
	HRESULT GetImageSizeDivider(int *divider);
	HRESULT SetOverlayEnable(BOOL ovlEnable);
	HRESULT GetOverlayEnable(BOOL *pOvlEnable);

	// Still images capture
	HRESULT SetEnableStillCapture(BOOL bEnable);
	HRESULT GetEnableStillCapture(BOOL *pbEnable);
	HRESULT GetStillDirectory(CHAR *dir);
	HRESULT SetStillDirectory(CHAR *dir);
	HRESULT GetStillImageFileName(CHAR *imageFileName, int *fileExtension);
	HRESULT SetStillImageFileName(CHAR *imageFileName, int fileExtension);
	HRESULT GetStillImageSubDir(CHAR *imageSubDir, int *subdirExtension);
	HRESULT SetStillImageSubDir(CHAR *imageSubDir, int subdirExtension);
	HRESULT SetStillCapturePeriod(unsigned long capturePeriod);
	HRESULT GetStillCapturePeriod(unsigned long *pCapturePeriod);
	HRESULT SetStillImageSizeDivider(int divider);
	HRESULT GetStillImageSizeDivider(int *divider);
	HRESULT SetStillJpegQuality(int jpegQuality);
	HRESULT GetStillJpegQuality(int *pJpegQuality);
	HRESULT SetStillOverlayEnable(BOOL ovlEnable);
	HRESULT GetStillOverlayEnable(BOOL *pOvlEnable);

	// Recording
	bool GetEnableRecording();
	void SetEnableRecording(bool bEnable);
	void GetVideoFileName(WCHAR *name, int *fileNameExtension, WCHAR *dir);
	void SetVideoFileName(WCHAR *name, int fileNameExtension, WCHAR *dir);
	void GetVideoSubdir(WCHAR *subdir, int *subdirExtension);
	void SetVideoSubdir(WCHAR *subdir, int subdirExtension);
	void GetVideoFileDuration(bool *bLimitEnabled, unsigned long *duration_sec);
	void SetVideoFileDuration(bool bLimitEnabled, unsigned long duration_sec);

	bool GetEnableRecordingSchedule();
	void SetEnableRecordingSchedule(bool bEnable);
	void GetVideoRecordingSchedule(short day, SYSTEMTIME *startTime, SYSTEMTIME *stopTime);
	void SetVideoRecordingSchedule(short day, SYSTEMTIME *startTime, SYSTEMTIME *stopTime);

	// Motion detection
	HRESULT SetEnableMotionDetection(BOOL bEnable);
	HRESULT GetEnableMotionDetection(BOOL *pbEnable);


	// Preview
	bool GetEnablePreview();
	void SetEnablePreview(bool bEnable);

	// Graph control
	HRESULT GetVideoSize(LONG *pWidth, LONG *pHeight);
	HRESULT Reconnect();
	void Run();
	void Stop();
private:
	bool bIsRunning;
public:
	bool IsRunning() { return bIsRunning; };

	// Persitence
	HRESULT WriteToStream(IStream *pStream);
	HRESULT ReadFromStream(IStream *pStream);

	HRESULT SaveConfig();
	HRESULT LoadConfig();
	HRESULT SaveBWFilterConfig();
	HRESULT LoadBWFilterConfig();

private:

	bool m_bFirstGraphBuild;

	// Available capture and conpression filters
	IMoniker *m_VideoCaptureDevices[10];
	TCHAR m_VideoCaptureDevicesName[10][256];
	IMoniker *m_AudioCaptureDevices[10];
	TCHAR m_AudioCaptureDevicesName[10][256];
	IMoniker *m_VideoCompressors[50];
	TCHAR m_VideoCompressorsName[50][256];

	// Graph components
	int m_VideoDeviceIndex;
	int m_AudioDeviceIndex;
	int m_VideoCompressorIndex;
	bool m_bMustReconnect;

	// Filters
	IBaseFilter *m_pBirdwatcher;
	IBaseFilter *m_pVideoSource;
	IBaseFilter *m_pAudioSource;
	IAMCrossbar *m_pXbar;
	IBaseFilter *m_pInfTee;
	IBaseFilter *m_pNullRenderer;
	IBaseFilter *m_pVideoCompressor;
	IBaseFilter *m_pMux;
	IFileSinkFilter *m_pFileWriter;

	// interfaces
	IGraphBuilder *m_pGraphBuilder;
	ICaptureGraphBuilder2 *m_pCaptureGraphBuilder;
	IMediaControl *m_pMediaControl;
	IFilterGraph  *m_pFilterGraph;
	IVideoWindow *m_pVidWin;
	IFtpControl *m_pFtpControl;
	IStillCaptureControl *m_pStillCaptureControl;
	IMotionDetectionControl *m_pMotionDetectionControl;

	// Video recording
	bool m_bRecordingEnabled;
	WCHAR m_pVideoDirectory[MAX_PATH];
	WCHAR m_pVideoSubdir[MAX_PATH];
	int m_VideoSubdirExtension; // 0: no extension - 1: yyyymmdd
	WCHAR m_pVideoFileName[MAX_PATH];
	int m_VideoFileNameExtension; // 0: no extension - 1: hhmmss
	bool m_bLimitVideoDuration;
	unsigned long m_VideoFileDuration_sec;

	// Scheduling video recording
	bool m_bRecordingScheduleEnabled;
	schedulerEntry m_VideoRecordingSchedule[7];

	// Preview
	bool m_bPreviewEnabled;

	// Scheduler
	CBWScheduler *m_pScheduler;
	HANDLE m_startRecordingEvent;
	HANDLE m_stopRecordingEvent;

	#ifdef _DEBUG
	DWORD m_dwRegister;
	#endif

	// private methods
	HRESULT EnumCaptureDevices();
	HRESULT EnumCompressionFilters();
	HRESULT ConnectFilters();
	HRESULT DeleteFilters();
	HRESULT UpdateVideoFileName();
	bool bMustRecord();

	HANDLE m_hTimer;
	HANDLE m_hStopThreadEvent;
	DWORD ThreadProc();

	CWebServer *m_pWebServer;
};
