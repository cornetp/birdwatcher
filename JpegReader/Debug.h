#ifndef __DEBUG_H
#define __DEBUG_H

extern bool bLogToDebugger;
extern bool bLogToFile;

void DbgPrint(const char *fmt, ...);
void DbgPrint_w(const wchar_t *fmt, ...);

#endif //__DEBUG_H