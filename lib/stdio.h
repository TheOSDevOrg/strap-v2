#include <out.h>
#include <stdarg.h>

int printf(char *fmt, ...);
int printf_std(standard_output *std, char *fmt, ...);
int vprintf(char *fmt, va_list lst);