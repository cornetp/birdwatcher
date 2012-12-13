#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "Debug.h"

#pragma warning( disable : 4793)


bool bLogToDebugger = true;
bool bLogToFile = false;

void DbgPrint(const char *fmt, ...)
{
	if(!(bLogToDebugger||bLogToFile))
		return;

	if(bLogToDebugger)
    {
        char buffer[512];

        va_list argptr;
        va_start(argptr, fmt);
        _vsnprintf_s(buffer, 512, _TRUNCATE, fmt, argptr);
        va_end(argptr);
        strcat_s(buffer, 512, "\r\n");

        OutputDebugString(buffer);
    }

}

void DbgPrint2(const char *fmt, ...)
{
	if(!(bLogToDebugger||bLogToFile))
		return;

	if(bLogToDebugger)
    {
        char buffer[512];

        va_list argptr;
        va_start(argptr, fmt);
        _vsnprintf_s(buffer, 512, _TRUNCATE, fmt, argptr);
        va_end(argptr);
        //strcat_s(buffer, 512, "");

        OutputDebugString(buffer);
    }

}

