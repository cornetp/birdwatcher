#ifndef __BWINTERFACES_H
#define __BWINTERFACES_H


// {C3632CD3-A9FF-471B-AD17-C12B1689312C}
DEFINE_GUID(CLSID_Birdwatcher, 
0xc3632cd3, 0xa9ff, 0x471b, 0xad, 0x17, 0xc1, 0x2b, 0x16, 0x89, 0x31, 0x2c);

// {DB04E9D3-059C-4c12-8120-9100FBF98AF8}
DEFINE_GUID(IID_IFtpControl, 
0xdb04e9d3, 0x59c, 0x4c12, 0x81, 0x20, 0x91, 0x0, 0xfb, 0xf9, 0x8a, 0xf8);

// {48CC43CB-7FDF-44E7-BE6B-1446FCEB5472}
DEFINE_GUID(IID_IStillCaptureControl, 
0x48cc43cb, 0x7fdf, 0x44e7, 0xbe, 0x6b, 0x14, 0x46, 0xfc, 0xeb, 0x54, 0x72);

// {C06F9841-C99A-4950-96E9-3092E2F0016A}
DEFINE_GUID(IID_IMotionDetectionControl, 
0xc06f9841, 0xc99a, 0x4950, 0x96, 0xe9, 0x30, 0x92, 0xe2, 0xf0, 0x1, 0x6a);


// fileExtension:
// 0: no extension
// 1: timestamp (hhmmss)

// subdirExtension:
// 0 : no extension
// 1 : date (yyyymmdd)
interface IFtpControl : public IUnknown
{
    STDMETHOD(GetEnableUpload)(BOOL *pbEnabled) = 0;
    STDMETHOD(SetEnableUpload)(BOOL bEnabled) = 0;
	STDMETHOD(GetServerName)(CHAR *serverName, size_t str_size) = 0;
	STDMETHOD(SetServerName)(CHAR *serverName) = 0;
	STDMETHOD(GetUserName)(CHAR *usrName, size_t size) = 0;
	STDMETHOD(SetUserName)(CHAR *usrName) = 0;
	STDMETHOD(GetPassword)(CHAR *passwd, size_t size) = 0;
	STDMETHOD(SetPassword)(CHAR *passwd) = 0;
	STDMETHOD(GetDirectory)(CHAR *dir, size_t size) = 0;
	STDMETHOD(SetDirectory)(CHAR *dir) = 0;
	STDMETHOD(GetImageFileName)(CHAR *filename, size_t size, int *fileExtension) = 0;
	STDMETHOD(SetImageFileName)(CHAR *filename, int fileExtension) = 0;
	STDMETHOD(GetImageSubDir)(CHAR *subdir, size_t size, int *subdirExtension) = 0;
	STDMETHOD(SetImageSubDir)(CHAR *subdir, int subdirExtension) = 0;
	STDMETHOD(GetUploadPeriod)(unsigned long *pUploadPeriod_sec) = 0;
	STDMETHOD(SetUploadPeriod)(unsigned long uploadPeriod_sec) = 0;
	STDMETHOD(GetJpegQuality)(int *pJpegQuality) = 0;
	STDMETHOD(SetJpegQuality)(int jpegQuality) = 0;
	STDMETHOD(GetImageSizeDivider)(int *pDivider) = 0;
	STDMETHOD(SetImageSizeDivider)(int divider) = 0;
    STDMETHOD(GetOverlayEnable)(BOOL *pbEnabled) = 0;
    STDMETHOD(SetOverlayEnable)(BOOL bEnabled) = 0;
};

interface IStillCaptureControl : public IUnknown
{
    STDMETHOD(GetEnableStillCapture)(BOOL *pbEnabled) = 0;
    STDMETHOD(SetEnableStillCapture)(BOOL bEnabled) = 0;
	STDMETHOD(GetStillDirectory)(CHAR *dir, size_t size) = 0;
	STDMETHOD(SetStillDirectory)(CHAR *dir) = 0;
	STDMETHOD(GetStillImageFileName)(CHAR *filename, size_t size, int *fileExtension) = 0;
	STDMETHOD(SetStillImageFileName)(CHAR *filename, int fileExtension) = 0;
	STDMETHOD(GetStillImageSubDir)(CHAR *subdir, size_t size, int *subdirExtension) = 0;
	STDMETHOD(SetStillImageSubDir)(CHAR *subdir, int subdirExtension) = 0;
	STDMETHOD(GetStillCapturePeriod)(unsigned long *pCapturePeriod_sec) = 0;
	STDMETHOD(SetStillCapturePeriod)(unsigned long capturePeriod_sec) = 0;
	STDMETHOD(GetStillImageSizeDivider)(int *pDivider) = 0;
	STDMETHOD(SetStillImageSizeDivider)(int divider) = 0;
	STDMETHOD(GetStillJpegQuality)(int *pJpegQuality) = 0;
	STDMETHOD(SetStillJpegQuality)(int jpegQuality) = 0;
    STDMETHOD(GetStillOverlayEnable)(BOOL *pbEnabled) = 0;
    STDMETHOD(SetStillOverlayEnable)(BOOL bEnabled) = 0;
};

interface IMotionDetectionControl : public IUnknown
{
    STDMETHOD(GetEnableMotionDetection)(BOOL *pbEnabled) = 0;
    STDMETHOD(SetEnableMotionDetection)(BOOL bEnabled) = 0;
};

#endif // __BWINTERFACES_H