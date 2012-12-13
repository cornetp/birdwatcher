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

		wchar_t wbuffer[512];
		size_t ret;
		mbstowcs_s(&ret, wbuffer, 512, buffer, 512);
        OutputDebugString(wbuffer);
    }
}

void DbgPrint_w(const wchar_t *fmt, ...)
{
	if(!(bLogToDebugger||bLogToFile))
		return;

	if(bLogToDebugger)
    {
        wchar_t buffer[512];

        va_list argptr;
        va_start(argptr, fmt);
        _vsnwprintf_s(buffer, 512, _TRUNCATE, fmt, argptr);
        va_end(argptr);
        wcscat_s(buffer, 512, L"\r\n");

        OutputDebugString(buffer);
    }
}
