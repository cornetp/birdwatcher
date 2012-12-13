#ifndef __BW_WEBSERVER_H
#define __BW_WEBSERVER_H

#include "Lacewing.h"
#include "FilterGraph.h"

class CWebServer
{
public:
	CWebServer();
	~CWebServer();
	void Enable();
	void Disable();
	bool IsEnabled() { return m_webserver?true:false; };
	static void SetFilterGraph(CFilterGraph *pGraph) {m_pFilterGraph = pGraph; };

private:
	Lacewing::Webserver *m_webserver;
	static CFilterGraph *m_pFilterGraph;

	static void AddCheckbox(Lacewing::Webserver::Request &Request, char* desc, char* name, BOOL value);
	static void AddText(Lacewing::Webserver::Request &Request, char* desc, char* name, char* value, bool passwd=false);

	static void HandlerGetCB(Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request);
	static void HandlerPostCB(Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request);

	static void HandlerPostFtpSettings(Lacewing::Webserver::Request &Request);
	static void HandlerPostCapdevSettings(Lacewing::Webserver::Request &Request);
	static void HandlerPostStillSettings(Lacewing::Webserver::Request &Request);
	static void HandlerPostVideoSettings(Lacewing::Webserver::Request &Request);
	static void HandlerPostMotionSettings(Lacewing::Webserver::Request &Request);

	static void DisplayControlForm(Lacewing::Webserver::Request &Request);
	static void DisplayCapDevSettings(Lacewing::Webserver::Request &Request);
	static void DisplayFTPSettings(Lacewing::Webserver::Request &Request);
	static void DisplayStillSettings(Lacewing::Webserver::Request &Request);
	static void DisplayVideoSettings(Lacewing::Webserver::Request &Request);
	static void DisplayMotionSettings(Lacewing::Webserver::Request &Request);
	

	static bool IsConnected(int IP);
	static void Connect(int IP);
	static void Disconnect(int IP);
	static bool loggedin;

	bool m_bEnabled;
};

#endif //__BW_WEBSERVER_H