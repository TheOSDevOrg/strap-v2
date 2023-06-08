#include <out.h>
#include <stdarg.h>
#include <stddef.h>
#include <int.h>
#include <math.h>

static char __STRAPV2_INTERNAL_digits_uppercase[] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'A', 'B', 'C', 'D', 'E', 'F'
};
static char __STRAPV2_INTERNAL_digits_lowercase[] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'a', 'b', 'c', 'd', 'e', 'f'
};

int __STRAPV2_INTERNAL_print_number(
  int number,
  int base,
  standard_output *std,
  char *digit_set
)
{
  uint8_t sign = number < 0 ? 1 : 0;
  size_t nsz;
  uint32_t factor;

  if (sign) { __STRAPV2_putc_std(std, '-'); number = -number; }

  nsz = numbersz(number, base);
  if (nsz == 0) return 0;
  factor = (int)pow(base, nsz - 1);

  for (; ; factor /= base)
  {
    int digit = 0;
    while ((uint32_t)number >= factor)
    {
      digit++;
      number -= factor;
    }
    __STRAPV2_putc_std(std, digit_set[digit]);
    if (factor == 1) break;
  }

  return nsz + sign;
}
int __STRAPV2_INTERNAL_print_unsigned_number(
  uint32_t number,
  int base,
  standard_output *std,
  char *digit_set
)
{
  size_t nsz;
  uint32_t factor;

  nsz = numbersz(number, base);
  if (nsz == 0) return 0;
  factor = pow(base, nsz - 1);
  
  for (; ; factor /= base)
  {
    int digit = 0;
    while ((uint32_t)number >= factor)
    {
      digit++;
      number -= factor;
    }
    __STRAPV2_putc_std(std, digit_set[digit]);
    if (factor == 1) break;
  }

  return nsz;
}

int vprintf_std(va_list lst, standard_output *std, char *fmt)
{
  /**
    * @brief this will be used to loop through
    *        the format string
  */
  int c;
  /**
    * @brief this will be used to count the total
    *        number of chars written to the string
  */
  int tot;
  
  for (c = 0; fmt[c] != '\0'; c++)
  {
    if (fmt[c] != '%')
    { __STRAPV2_putc(fmt[c]); tot++; continue; }

    int num;

    switch (fmt[++c])
    {
    case '%':
      __STRAPV2_putc_std(std, '%');
      tot++;
      break;
    case 'd':
      num = va_arg(lst, int);
      tot += __STRAPV2_INTERNAL_print_number(
        num,
        10,
        std,
        __STRAPV2_INTERNAL_digits_uppercase
      );
      break;
    case 'x':
      num = va_arg(lst, int);
      tot += __STRAPV2_INTERNAL_print_number(
        num, 
        16,
        std,
        __STRAPV2_INTERNAL_digits_lowercase
      );
      break;
    case 'X':
      num = va_arg(lst, int);
      tot += __STRAPV2_INTERNAL_print_number(
        num, 
        16,
        std,
        __STRAPV2_INTERNAL_digits_uppercase
      );
      break;
    case 'b':
      num = va_arg(lst, int);
      tot += __STRAPV2_INTERNAL_print_number(
        num,
        2,
        std,
        __STRAPV2_INTERNAL_digits_uppercase
      );
    break;
    case 's':
      tot += vprintf_std(lst, std, va_arg(lst, char *));
      break;
    case 'u':
      num = va_arg(lst, int);
      if (fmt[++c] == 'd')
        tot += __STRAPV2_INTERNAL_print_unsigned_number(
          num,
          10,
          std,
          __STRAPV2_INTERNAL_digits_uppercase
        );
      else if (fmt[c] == 'X')
        tot += __STRAPV2_INTERNAL_print_unsigned_number(
          num,
          16,
          std,
          __STRAPV2_INTERNAL_digits_uppercase
        );
      else if (fmt[c] == 'x')
        tot += __STRAPV2_INTERNAL_print_unsigned_number(
          num,
          16,
          std,
          __STRAPV2_INTERNAL_digits_lowercase
        );
      else if (fmt[c] == 'b')
        tot += __STRAPV2_INTERNAL_print_unsigned_number(
          num,
          2,
          std,
          __STRAPV2_INTERNAL_digits_uppercase
        );
    break;
    default:
      break;
    }
  }

  return tot;
}

int printf_std(standard_output *std, char *fmt, ...)
{
  va_list lst;
  va_start(lst, fmt);

  int result = vprintf_std(lst, std, fmt);
  
  va_end(lst);
  return result;
}
int printf(char *fmt, ...)
{
  va_list lst;
  va_start(lst, fmt);

  int result = vprintf_std(lst, current_std, fmt);
  
  va_end(lst);
  return result;
}
int vprintf(char *fmt, va_list lst)
{
  return vprintf_std(lst, current_std, fmt);
}