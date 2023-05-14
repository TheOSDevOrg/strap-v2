#include <int.h>

size_t numbersz(uint32_t number, int base)
{
  /**
   * @brief This variable will be used to count
   *        the number of digits of the given
   *        integer
  */
  size_t sz;

  if (number == 0) return 1;

  for (sz = 0; number > 0; sz++) number /= base;
  return sz;
}