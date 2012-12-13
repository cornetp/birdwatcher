#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#define MAX_EVENT_COUNT 10
struct scheduledEvent {
	HANDLE hEvent;
	short wDayOfWeek; // bit field, bit 0=Sunday, 1=Monday, ... 6=Saterday, 7=Daily
	short wHour[8];
	short wMinute[8];
	short wSecond[8];
};

struct enumDayOfWeek
{
	short val;
	char *str;
};

class CBWScheduler : public CAMThread
{
public:
	CBWScheduler::CBWScheduler();
	CBWScheduler::~CBWScheduler();

	HRESULT AddEvent(struct scheduledEvent *pEvent);

	DWORD ThreadProc();

private:
	HANDLE m_hTimer;
	HANDLE m_hStopThreadEvent;
	int m_eventCount;

	struct scheduledEvent m_eventList[MAX_EVENT_COUNT];

};


#endif //__SCHEDULER_H
