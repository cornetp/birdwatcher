#pragma once

#include <streams.h>
#include "../JpegReader/JpegReaderFlt.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#endif

enum PlaybackState
{
	STATE_RUNNING,
	STATE_PAUSED,
	STATE_STOPPED,
	STATE_CLOSED
};

class CJpegReader : public CAMThread
{
private:
	IGraphBuilder* m_pGrfBuilder;
	ICaptureGraphBuilder2 *m_pCaptureGrfBuilder;
	IBaseFilter* m_pSrcFilter;
	IMediaControl *m_pMediaControl;
	IVideoWindow *m_pVidWin;
	IMediaSeeking *m_pSeek;
	IMediaEvent *m_pMediaEvent;
	HANDLE m_hGraphEvent;

	IBaseFilter *m_pMux;
	IFileSinkFilter *m_pFileWriter;


	HANDLE m_complete_event;

	DWORD m_seekCaps;

	IJpegReaderControl *m_pJpegReaderControl;

	PlaybackState	m_state;

	#ifdef _DEBUG
	DWORD m_dwRegister;
	#endif

	HANDLE m_hStopThreadEvent;
	DWORD ThreadProc();

public:
	CJpegReader();
	~CJpegReader();

	HRESULT SetStillDirectory(CHAR *StillDir);
	HRESULT GetCurrentFileName(CHAR *fileName, size_t size);	

	bool CanSeek();
	HRESULT SetPosition(REFERENCE_TIME *pos);
	HRESULT GetCurrentPosition(LONGLONG *pTimeNow);
	HRESULT GetDuration(LONGLONG *pDuration);

	void SetOwner(HWND handle, LONG off_x, LONG off_y, LONG w, LONG h);
	HRESULT Run();
	HRESULT Stop();
	HRESULT Pause();

	HRESULT BuildGraph();
	HRESULT BuildRecordingGraph(WCHAR *fileName);
	HRESULT DeleteFilters();
	HRESULT Disconnect();

	HRESULT GetCompleteEvent(HANDLE *h);
};