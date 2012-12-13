#include <streams.h>
#include "Scheduler.h"
#include "Debug.h"


struct enumDayOfWeek DayOfWeek[] = {
	{-1, "Daily"},
	{ 0, "Sunday"},
	{ 1, "Monday"},
	{ 2, "Tuesday"},
	{ 3, "Wednesday"},
	{ 4, "Thursday"},
	{ 5, "Friday"},
	{ 6, "Saturday"},
};

CBWScheduler::CBWScheduler()
{
	m_eventCount = 0;
	for(int i=0; i<MAX_EVENT_COUNT; i++)
		m_eventList[i].hEvent = NULL;

	m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	m_hStopThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Start thread
	Create();

	// Start Timer
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -1LL;
	SetWaitableTimer(m_hTimer, &liDueTime, 1000, NULL, NULL, 0); // periodic timer of 1 sec

}

CBWScheduler::~CBWScheduler()
{
	SetEvent(m_hStopThreadEvent);
	Close(); // wait for thread to exit

	if(m_hTimer)
		CloseHandle(m_hTimer);
	if(m_hStopThreadEvent)
		CloseHandle(m_hStopThreadEvent);
}

HRESULT CBWScheduler::AddEvent(struct scheduledEvent *pEvent)
{
	if(m_eventCount<MAX_EVENT_COUNT)
	{
		memcpy(&m_eventList[m_eventCount++], pEvent, sizeof(struct scheduledEvent));
		return S_OK;
	}
	return S_FALSE;
}

DWORD CBWScheduler::ThreadProc()
{
    BOOL bShutDown = FALSE;
    while (!bShutDown)
	{
		HANDLE objs[2] = {m_hTimer, m_hStopThreadEvent};
		DWORD dwWaitResult = WaitForMultipleObjects(2, objs, FALSE, INFINITE);
		if(dwWaitResult ==  WAIT_OBJECT_0 + 1)
		{
			bShutDown = TRUE;
		}
		else 
		{
			// check conditions
			SYSTEMTIME time;
			GetLocalTime(&time);
			WORD today = time.wDayOfWeek;
			
			for(int i=0; i<m_eventCount; i++)
			{
				/*DbgPrint("cond1: %s", m_eventList[i].hEvent?"true":"false");
				DbgPrint("cond2: %s", (m_eventList[i].wSecond    == -1 || m_eventList[i].wSecond == time.wSecond      )?"true":"false");
				DbgPrint("cond3: %s", (m_eventList[i].wMinute    == -1 || m_eventList[i].wMinute == time.wMinute      )?"true":"false");
				DbgPrint("cond4: %s", (m_eventList[i].wHour      == -1 || m_eventList[i].wHour == time.wHour          )?"true":"false");
				DbgPrint("cond5: %s", (m_eventList[i].wDayOfWeek == -1 || m_eventList[i].wDayOfWeek == time.wDayOfWeek)?"true":"false");*/

				/*
				if(  m_eventList[i].hEvent &&
					(m_eventList[i].wSecond    == -1 || m_eventList[i].wSecond == time.wSecond      ) &&
					(m_eventList[i].wMinute    == -1 || m_eventList[i].wMinute == time.wMinute      ) &&
					(m_eventList[i].wHour      == -1 || m_eventList[i].wHour == time.wHour          ) &&
					(m_eventList[i].wDayOfWeek == -1 || m_eventList[i].wDayOfWeek == time.wDayOfWeek))*/

				if(  m_eventList[i].hEvent &&
					(/*m_eventList[i].wDayOfWeek&(1<<7) || */m_eventList[i].wDayOfWeek&(1<<today)) &&
					(m_eventList[i].wSecond[today] == -1 || m_eventList[i].wSecond[today] == time.wSecond ) &&
					(m_eventList[i].wMinute[today] == -1 || m_eventList[i].wMinute[today] == time.wMinute ) &&
					(m_eventList[i].wHour[today]   == -1 || m_eventList[i].wHour[today] == time.wHour     ))
				{
					SetEvent(m_eventList[i].hEvent);
				}
			}
		}
	}
	return 1;
}

