#include <math.h>

uint32_t pow(uint32_t base, uint32_t exponent)
{
  int result;
  
  for (result = 1; exponent > 0; exponent--)
    result *= base;

  return result;
}

int abs(int n) { return (int)(n < 0 ? -n : n); }