#ifndef __JPEG_READER_ITF_H
#define __JPEG_READER_ITF_H

// Filter GUID
// {91C303D1-B9B5-4c9e-960E-9EC905CD91A5}
DEFINE_GUID(CLSID_JpegPushSource, 
0x91c303d1, 0xb9b5, 0x4c9e, 0x96, 0xe, 0x9e, 0xc9, 0x5, 0xcd, 0x91, 0xa5);

// Interface GUID
// {26484A23-63F1-4FB1-BF07-316599BF9119}
DEFINE_GUID(IID_IJpegReaderControl, 
0x26484a23, 0x63f1, 0x4fb1, 0xbf, 0x7, 0x31, 0x65, 0x99, 0xbf, 0x91, 0x19);



interface IJpegReaderControl : public IUnknown
{
	STDMETHOD(SetStillDirectory)(CHAR *dir) = 0;
	STDMETHOD(GetStillDirectory)(CHAR *dir, size_t size) = 0;
	STDMETHOD(GetCurrentFileName)(CHAR *c, size_t s) = 0;
};

#endif //__JPEG_READER_ITF_H