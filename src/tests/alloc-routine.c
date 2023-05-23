#include <tests.h>
#include <memory.h>
#include <out.h>
#include <stdio.h>

bool __STRAPV2_TEST_INTERNAL_alloc_routine_0()
{
  void *test1 = malloc(15);
  void *test2 = malloc(15);
  void *test3 = malloc(15);

  free(test2);

  void *test4 = malloc(15);
  return test4 == test2;
}
bool __STRAPV2_TEST_alloc_routine(int n)
{
  bool result;

  console_color_t old_fg = __STRAPV2_get_fg();
  __STRAPV2_set_fg(STRAPV2_CONSOLECOLOR_light_green);
  printf("[START TEST] ");
  __STRAPV2_set_fg(STRAPV2_CONSOLECOLOR_light_cyan);

  switch (n)
  {
    case 0:
      printf("alloc routine 0\n");
      result = __STRAPV2_TEST_INTERNAL_alloc_routine_0();
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
