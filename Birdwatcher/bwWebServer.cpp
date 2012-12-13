#include "bwWebServer.h"
#include <stdio.h>
#include <string.h>
#include "Debug.h"

using namespace Lacewing;

bool CWebServer::loggedin = false;
CFilterGraph *CWebServer::m_pFilterGraph = NULL;
CWebServer::CWebServer()
{
	m_bEnabled = false;
	m_webserver = NULL;	
}

CWebServer::~CWebServer()
{
	if(m_webserver)
		delete(m_webserver);
}

void CWebServer::Enable()
{
	if(!m_webserver)
	{
		m_webserver = new Webserver();
		m_webserver->RegisterHandlerGet(&CWebServer::HandlerGetCB);
		m_webserver->RegisterHandlerPost(&CWebServer::HandlerPostCB);
		m_webserver->Host(8080);
	}
}

void CWebServer::Disable()
{
	if(m_webserver)
	{
		// Disconnect all IPs
		m_webserver->Unhost();
		delete(m_webserver);
		m_webserver = NULL;
	}
}

bool CWebServer::IsConnected(int IP)
{
	return loggedin;
}

void CWebServer::Connect(int IP)
{
	DbgPrint("Connect client 0x%x", IP);
	loggedin = true;
}

void CWebServer::Disconnect(int IP)
{
	DbgPrint("Disconnect client 0x%x", IP);
	loggedin = false;
}

void CWebServer::AddText(Lacewing::Webserver::Request &Request, char* desc, char* name, char* value, bool passwd)
{
	char tmpstr[512];
	Request << "<tr>";
	Request << "<td>" << desc << "<br/></td>";
	sprintf_s(tmpstr, "<td><input type=%s name='%s' value='%s'></td>", passwd?"password":"text", name, value);
	Request << tmpstr;
	Request << "</tr>";
}

void CWebServer::AddCheckbox(Lacewing::Webserver::Request &Request, char* desc, char* name, BOOL value)
{
	char tmpstr[512];
	Request << "<tr>";
	Request << "<td>" << desc << "<br/></td>";
	sprintf_s(tmpstr, "<td><input type=checkbox name='%s' value='on' %s>Enable/disable</td>"
		, name, value? "checked":"");
	Request << tmpstr;
	Request << "</tr>";
}

void CWebServer::DisplayControlForm(Lacewing::Webserver::Request &Request)
{
	if(!m_pFilterGraph)
	{
		Request << "Internal error: FilterGraph no set !";
		return;
	}

	Request << "Welcome " << Request.StringIP() << "<br/><br/>";

	if(m_pFilterGraph->IsRunning())
	{
		Request << "<font color='green'>The acquisition is active</font> <br/>";
		// display stop button
	}
	else
	{
		Request << "<font color='red'>The acquisition is not active</font> <br/>";
		// Display start button
	}

	Request << "<p align='left'><a href='../capdev_settings'>Capture devices</a></p>";
	Request << "<p align='left'><a href='../ftp_settings'>FTP settings</a></p>";
	Request << "<p align='left'><a href='../still_settings'>Still images settings</a></p>";
	Request << "<p align='left'><a href='../video_settings'>Recording settings</a></p>";
	Request << "<p align='left'><a href='../motion_settings'>Motion detection settings</a></p>";

	Request << "<p align='left'><a href='../logout'>Logout</a></p>";
	//Request.SendFile("../web/control.html");
}

void CWebServer::DisplayCapDevSettings(Lacewing::Webserver::Request &Request)
{
	Request << "<table border=1>";
	Request << "<form method=post action='submit_capdev_settings'>";

	// Video devices
	TCHAR name[256];
	int i = 0;
	int selected = m_pFilterGraph->GetSelectedVideoDevice();

	Request << "<tr><td>Video devices</td>";
	Request << "<td><select name='videodev'>";
	while(m_pFilterGraph->GetVideoDevice(i, name, 256) == NOERROR)
	{
		//this->vidCapDeviceComboBox->Items->Add(gcnew String(name));
		Request << "<option "<< ((selected==i)?"selected ":"") <<"value='" << i << "'>" << name << "</option>";
		i++;
	}
	Request << "</select></td></tr>";

	// Audio devices
	Request << "<tr><td>Audio devices</td>";
	Request << "<td><select name='audiodev'>";

	i = 0;
	selected = m_pFilterGraph->GetSelectedAudioDevice();
	while(m_pFilterGraph->GetAudioDevice(i, name, 256) == NOERROR)
	{
		Request << "<option "<< ((selected==i)?"selected ":"") <<"value='" << i << "'>" << name << "</option>";
		i++;
	}
	Request << "</select></td></tr>";
	
	//
	Request << "<tr><td colspan=2>";
	Request << "<INPUT type='hidden' value='dummy' name='dummy'>";
	Request << "<input type='submit' value='Submit'>";
	Request << "</td></tr>";

	Request << "</form>";
	Request << "</table>";

	Request << "<p align='left'><a href='../birdwatcher'>Back</a></p>";
}

void CWebServer::DisplayFTPSettings(Lacewing::Webserver::Request &Request)
{
	// FTP settings
	Request << "<table border=1>";
	Request << "<form method=post action='submit_ftp_settings'>";

	// Enable/disable
	BOOL bFTPEnabled = FALSE;
	m_pFilterGraph->GetEnableFtpUpload(&bFTPEnabled);
	AddCheckbox(Request, "Unable FTP upload", "ftp_enable_checkbox", bFTPEnabled);

	char serverName[256];
	char userName[256];
	char passwd[256];
	char directory[256];
	m_pFilterGraph->GetFtpServerInfos(serverName, userName, passwd, directory);

	AddText(Request, "Server name", "ftp_server_name", serverName);
	AddText(Request, "User name", "ftp_user_name", userName);
	AddText(Request, "Password", "ftp_password", passwd, true);
	AddText(Request, "Directory", "ftp_directory", directory);

	// Enable timestamp overlay
	BOOL bOverlayEnabled = FALSE;
	m_pFilterGraph->GetOverlayEnable(&bOverlayEnabled);
	AddCheckbox(Request, "Unable timestamp overlay", "ftp_ovl_checkbox", bOverlayEnabled);

	//
	Request << "<tr><td colspan=2>";
	Request << "<INPUT type='hidden' value='dummy' name='dummy'>";
	Request << "<input type='submit' value='Submit FTP settings'>";
	Request << "</td></tr>";

	Request << "</form>";
	Request << "</table>";

	Request << "<p align='left'><a href='../birdwatcher'>Back</a></p>";
}

void CWebServer::DisplayStillSettings(Lacewing::Webserver::Request &Request)
{
	// Still capture settings
	Request << "<table border=1>";
	Request << "<form method=post action='submit_still_settings'>";

	// Enable/disable
	BOOL bStillCaptureEnabled = FALSE;
	m_pFilterGraph->GetEnableStillCapture(&bStillCaptureEnabled);
	AddCheckbox(Request, "Unable still capture", "still_enable_checkbox", bStillCaptureEnabled);

	//
	Request << "<tr><td colspan=2>";
	Request << "<INPUT type='hidden' value='dummy' name='dummy'>";
	Request << "<input type='submit' value='Submit still capture settings'>";
	Request << "</td></tr>";

	Request << "</form>";
	Request << "</table>";

	Request << "<p align='left'><a href='../birdwatcher'>Back</a></p>";
}

void CWebServer::DisplayVideoSettings(Lacewing::Webserver::Request &Request)
{
	// Video capture settings
	Request << "<table border=1>";
	Request << "<form method=post action='submit_video_settings'>";

	// Enable/disable
	BOOL bVideoCaptureEnabled = m_pFilterGraph->GetEnableRecording();
	AddCheckbox(Request, "Unable video capture", "video_enable_checkbox", bVideoCaptureEnabled);

	//
	Request << "<tr><td colspan=2>";
	Request << "<INPUT type='hidden' value='dummy' name='dummy'>";
	Request << "<input type='submit' value='Submit video capture settings'>";
	Request << "</td></tr>";

	Request << "</form>";
	Request << "</table>";

	Request << "<p align='left'><a href='../birdwatcher'>Back</a></p>";
}

void CWebServer::DisplayMotionSettings(Lacewing::Webserver::Request &Request)
{
	// Motion detection settings
	Request << "<table border=1>";
	Request << "<form method=post action='submit_motion_settings'>";

	// Enable/disable
	BOOL bMotionDetectionEnabled = FALSE;
	m_pFilterGraph->GetEnableMotionDetection(&bMotionDetectionEnabled);
	AddCheckbox(Request, "Unable motion detection", "motion_enable_checkbox", bMotionDetectionEnabled);

	//
	Request << "<tr><td colspan=2>";
	Request << "<INPUT type='hidden' value='dummy' name='dummy'>";
	Request << "<input type='submit' value='Submit motion detection settings'>";
	Request << "</td></tr>";

	Request << "</form>";
	Request << "</table>";

	Request << "<p align='left'><a href='../birdwatcher'>Back</a></p>";
}

void CWebServer::HandlerGetCB(Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request)
{
	Request.StripGet();
	
	if(!IsConnected(Request.IP()))
		Request.SendFile("../web/login.html");
	else
	{
		if(!strcmp(Request.URL, "birdwatcher"))
			DisplayControlForm(Request);
		else if(!strcmp(Request.URL, "capdev_settings"))
			DisplayCapDevSettings(Request);
		else if(!strcmp(Request.URL, "ftp_settings"))
			DisplayFTPSettings(Request);
		else if(!strcmp(Request.URL, "still_settings"))
			DisplayStillSettings(Request);
		else if(!strcmp(Request.URL, "video_settings"))
			DisplayVideoSettings(Request);
		else if(!strcmp(Request.URL, "motion_settings"))
			DisplayMotionSettings(Request);
		else if(!strcmp(Request.URL, "logout"))
		{
			Disconnect(Request.IP());
			Request.SetRedirect("birdwatcher");
		}
		else
			Request << "Page not found";
	}
}

// Post handlers
void CWebServer::HandlerPostFtpSettings(Lacewing::Webserver::Request &Request)
{
	DbgPrint("checkbox value = %s",Request.GetPostItem("ftp_enable_checkbox"));
	if(!strcmp(Request.GetPostItem("ftp_enable_checkbox"), "on"))
		m_pFilterGraph->SetEnableFtpUpload(true);
	else
		m_pFilterGraph->SetEnableFtpUpload(false);

	char serverName[256];
	char userName[256];
	char passwd[256];
	char directory[256];
	sprintf_s(serverName, "%s", Request.GetPostItem("ftp_server_name"));
	sprintf_s(userName, "%s", Request.GetPostItem("ftp_user_name"));
	sprintf_s(passwd, "%s", Request.GetPostItem("ftp_password"));
	sprintf_s(directory, "%s", Request.GetPostItem("ftp_directory"));

	DbgPrint("servername = %s - userName = %s - passwd = %s - dir = %s", serverName, userName, passwd, directory);
	m_pFilterGraph->SetFtpServerInfos(serverName, userName, passwd, directory);

	if(!strcmp(Request.GetPostItem("ftp_ovl_checkbox"), "on"))
		m_pFilterGraph->SetOverlayEnable(true);
	else
		m_pFilterGraph->SetOverlayEnable(false);

	Request.SetRedirect("ftp_settings");
}

void CWebServer::HandlerPostStillSettings(Lacewing::Webserver::Request &Request)
{
	if(!strcmp(Request.GetPostItem("still_enable_checkbox"), "on"))
		m_pFilterGraph->SetEnableStillCapture(true);
	else
		m_pFilterGraph->SetEnableStillCapture(false);

	Request.SetRedirect("still_settings");
}

void CWebServer::HandlerPostVideoSettings(Lacewing::Webserver::Request &Request)
{
	if(!strcmp(Request.GetPostItem("video_enable_checkbox"), "on"))
		m_pFilterGraph->SetEnableRecording(true);
	else
		m_pFilterGraph->SetEnableRecording(false);

	Request.SetRedirect("video_settings");
}

void CWebServer::HandlerPostMotionSettings(Lacewing::Webserver::Request &Request)
{
	if(!strcmp(Request.GetPostItem("motion_enable_checkbox"), "on"))
		m_pFilterGraph->SetEnableMotionDetection(true);
	else
		m_pFilterGraph->SetEnableMotionDetection(false);

	Request.SetRedirect("motion_settings");
}

void CWebServer::HandlerPostCapdevSettings(Lacewing::Webserver::Request &Request)
{
	DbgPrint("selected video device = %s - audio device = %s", Request.GetPostItem("videodev"), Request.GetPostItem("audiodev"));
	if(strlen(Request.GetPostItem("videodev"))>0)
	{
		DbgPrint("selected video device = #%d", atoi(Request.GetPostItem("videodev")));
		m_pFilterGraph->SelectVideoDevice(atoi(Request.GetPostItem("videodev")));
	}

	if(strlen(Request.GetPostItem("audiodev"))>0)
	{
		DbgPrint("selected audio device = #%d", atoi(Request.GetPostItem("audiodev")));
		m_pFilterGraph->SelectAudioDevice(atoi(Request.GetPostItem("audiodev")));
	}

	Request.SetRedirect("capdev_settings");
}

void CWebServer::HandlerPostCB(Lacewing::Webserver &Webserver, Lacewing::Webserver::Request &Request)
{
	DbgPrint("CWebServer::HandlerPostCB before StripGet");
	Request.StripGet();
	DbgPrint("CWebServer::HandlerPostCB after StripGet");
	
	if(!strcmp(Request.URL, "submit_passwd"))
	{
		//if(!strcmp(Request.GetPostItem("passwd"), "toto123"))
			Connect(Request.IP());

		Request.SetRedirect("birdwatcher");
		return;
	}
	if(!strcmp(Request.URL, "submit_ftp_settings"))
		return HandlerPostFtpSettings(Request);
	if(!strcmp(Request.URL, "submit_capdev_settings"))
		return HandlerPostCapdevSettings(Request);
	if(!strcmp(Request.URL, "submit_still_settings"))
		return HandlerPostStillSettings(Request);
	if(!strcmp(Request.URL, "submit_video_settings"))
		return HandlerPostVideoSettings(Request);
	if(!strcmp(Request.URL, "submit_motion_settings"))
		return HandlerPostMotionSettings(Request);

	
	Request << "Page not found";
}
