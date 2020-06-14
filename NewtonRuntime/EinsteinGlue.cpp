#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

extern "C" void EinsteinLog(const char* str);
extern "C" void EinsteinLogF(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buf[512];
    (void) vsprintf(buf, fmt, ap);
    va_end(ap);
    EinsteinLog(buf);
}
