// Birdwatcher.cpp : main project file.

#include <initguid.h>

#include "Debug.h"
#include "Birdwatcher.h"
#include "FilterGraph.h"
#include "Form1.h"

#include "bwWebServer.h"

using namespace Birdwatcher;

static bool loggedin = false;

[STAThreadAttribute]

int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create web server object
	CWebServer web_server;

	// Create the filter graph
	CFilterGraph graph;

	web_server.SetFilterGraph(&graph);
	// Create the main window and run it
	Form1 ^mainWindow = gcnew Form1(&graph, &web_server);
	mainWindow->Init();
	Application::Run(mainWindow);

	return 0;
}
