#ifndef __DEBUG_H
#define __DEBUG_H

extern bool bLogToDebugger;
extern bool bLogToFile;

void DbgPrint(const char *fmt, ...);
void DbgPrint2(const char *fmt, ...);

#endif //__DEBUG_H