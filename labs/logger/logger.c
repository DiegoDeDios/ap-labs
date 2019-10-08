#include <stdarg.h>
#include "colors.h"
int infof(const char *format, ...){
    va_list arg;

    va_start(arg, format);
    textcolor(BRIGHT, BLUE, BLACK);
    while (format) {
        printf("%s\n",format);
        format = va_arg(arg, const char *);
    }
    textcolor(RESET, WHITE, BLACK);	
    va_end(arg);
}
int warnf(const char *format, ...){
    va_list arg;

    va_start(arg, format);
    textcolor(BRIGHT, YELLOW, BLACK);

    while (format) {
        printf("%s\n",format);
        format = va_arg(arg, const char *);
    }
    textcolor(RESET, WHITE, BLACK);	
    va_end(arg);
}
int errorf(const char *format, ...){
    va_list arg;

    va_start(arg, format);
    textcolor(BRIGHT, RED, BLACK);

    while (format) {
        printf("%s\n",format);
        format = va_arg(arg, const char *);
    }
    textcolor(RESET, WHITE, BLACK);	
    va_end(arg);
}
int panicf(const char *format, ...){
    va_list arg;

    va_start(arg, format);
    textcolor(BRIGHT, RED, BLACK);

    while (format) {	
        printf("%s\n",format);
        format = va_arg(arg, const char *);
    }
    textcolor(RESET, WHITE, BLACK);	
    va_end(arg);
}


