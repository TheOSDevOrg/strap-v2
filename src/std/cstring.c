#include <memory.h>
#include <cstring.h>

size_t strlen(char str[])
{
  size_t sz = 0;
  while (str[sz++]) ;
  return sz-1;
}
int strcmp(char first[], char second[])
{
  size_t fsz = strlen(first),
         ssz = strlen(second);
  if (fsz != ssz) return fsz - ssz;
  return memcmp(first, second, fsz);
}