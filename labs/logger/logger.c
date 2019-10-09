#include <stdarg.h>
#include "colors.h"
void infof(const char *format, ...){
    va_list arg;
    va_start(arg, format);
    textcolor(BRIGHT, BLUE, BLACK);
    vfprintf (stdout, format, arg);
    textcolor(RESET, WHITE, BLACK);	
    va_end(arg);
}
void warnf(const char *format, ...){
    va_list arg;
    va_start(arg, format);
    textcolor(BRIGHT, YELLOW, BLACK);
    vfprintf (stdout, format, arg);
    textcolor(RESET, WHITE, BLACK);	
    va_end(arg);
}
void errorf(const char *format, ...){
    va_list arg;
    va_start(arg, format);
    textcolor(BRIGHT, RED, BLACK);
    vfprintf (stdout, format, arg);
    textcolor(RESET, WHITE, BLACK);	
    va_end(arg);
}
void panicf(const char *format, ...){
    va_list arg;
    va_start(arg, format);
    textcolor(BRIGHT, RED, BLACK);
	vfprintf (stdout, format, arg);
    textcolor(RESET, WHITE, BLACK);	
    va_end(arg);
}


