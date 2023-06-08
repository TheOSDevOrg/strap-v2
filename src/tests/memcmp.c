#include <tests.h>
#include <memory.h>
#include <out.h>
#include <stdio.h>

bool __STRAPV2_TEST_INTERNAL_memcmp_0()
{
  char buffer1[4] = { 1, 2, 3, 4 };
  char buffer2[4] = { 1, 2, 3, 4 };

  int r = memcmp(buffer1, buffer2, 4);
  printf("%d\n", r);

  return r == 0;
}
bool __STRAPV2_TEST_INTERNAL_memcmp_1()
{
  char buffer1[4] = { 1, 2, 3, 4 };
  char buffer2[4] = { 1, 2, 3, 5 };

  int r = memcmp(buffer1, buffer2, 4);
  printf("%d\n", r);

  return r != 0;
}
bool __STRAPV2_TEST_INTERNAL_memcmp_2()
{
  char buffer1[] = { 1, 2, 3, 4, 1, 2, 1 };
  char buffer2[] = { 1, 2, 3, 4, 1, 2, 1 };

  int r = memcmp(buffer1, buffer2, 7);
  printf("%d\n", r);

  return r == 0;
}
bool __STRAPV2_TEST_memcmp(int n)
{
  bool result;

  console_color_t old_fg = __STRAPV2_get_fg();
  __STRAPV2_set_fg(STRAPV2_CONSOLECOLOR_light_green);
  printf("[START TEST] ");
  __STRAPV2_set_fg(STRAPV2_CONSOLECOLOR_light_cyan);

  switch (n)
  {
    case 0:
      printf("memcmp 0\n");
      __STRAPV2_set_fg(STRAPV2_CONSOLECOLOR_white);
      result = __STRAPV2_TEST_INTERNAL_memcmp_0();
      break;
    case 1:
      printf("memcmp 1\n");
      __STRAPV2_set_fg(STRAPV2_CONSOLECOLOR_white);
      result = __STRAPV2_TEST_INTERNAL_memcmp_1();
      break;
    case 2:
      printf("memcmp 2\n");
      __STRAPV2_set_fg(STRAPV2_CONSOLECOLOR_white);
      result = __STRAPV2_TEST_INTERNAL_memcmp_2();
      break;
    default:
      result = false;
      break;
  }
  
  __STRAPV2_set_fg(STRAPV2_CONSOLECOLOR_yellow);
  printf("[END TEST]   ");
  __STRAPV2_set_fg(
    result ? STRAPV2_CONSOLECOLOR_green : STRAPV2_CONSOLECOLOR_red
  );
  printf("%s\n", result ? "PASSED" : "FAILED");

  __STRAPV2_set_fg(old_fg);
  return result;
}
