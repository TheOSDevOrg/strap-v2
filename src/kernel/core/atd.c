#include <atd/io.h>
#include <atd/mem.h>
#include <stdarg.h>
#include <stdio.h>
#include <memory.h>
#include <cstring.h>

// IO
int ATD_printf(const char fmt[], ...)
{
  va_list lst;

  va_start(lst, fmt);
  int r = vprintf(fmt, lst);
  va_end(lst);

  return r;
}

// MemoryMGR
void *ATD_malloc(size_t sz) { return malloc(sz); }
void ATD_free(void *data) { free(data); }

// Utilities
int ATD_memcmp(const void *left, const void *right, size_t sz)
{ return memcmp(left, right, sz); }
int ATD_strcmp(const char *left, const char *right)
{ return strcmp(left, right); }
size_t ATD_strlen(const char *str)
{ return strlen(str); }