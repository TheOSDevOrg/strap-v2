#include <main.h>
#include <out.h>
#include <stdio.h>
#include <hal/gdt.h>
#include <memory.h>
#include <loader.h>
#include <tests.h>
#include <TabosVM.h>

console_char_t _videobuffer[80*25];
standard_output _stdout = {
  .framebuffer = _videobuffer,
  .max_x = 80,
  .max_y = 25,
  .x = 0,
  .y = 0,
  .bg = STRAPV2_CONSOLECOLOR_black,
  .fg = STRAPV2_CONSOLECOLOR_white,
};

void __STRAPV2_kernel_prerun(multiboot_hdr *)
{
  __STRAPV2_init_output_system(&_stdout);
  __STRAPV2_memorymgr_init();
  __STRAPV2_clear();
  __STRAPV2_print("info:  output system ready!\n");
  __STRAPV2_print("info:  multiboot data ready!\n");
  TVM_init();
  __STRAPV2_print("info:  Tabos Virtual Machine initialized!\n");
}
int __STRAPV2_kernel_run()
{
  printf("Welcome to " KERN_NAME " " KERN_VER "\n");
  printf("From commit " KERN_COMMIT_ID "\n");

  //__STRAPV2_TEST_alloc_routine(0);
  //__STRAPV2_TEST_memcmp(0);
  //__STRAPV2_TEST_memcmp(1);
  //__STRAPV2_TEST_memcmp(2);

  __STRAPV2_TEST_VM_flow();
  __STRAPV2_heap_print_alloc_stack(&__STRAPV2_heap);
  __STRAPV2_TEST_VM_types();
  __STRAPV2_render();

  while(1) ;
  return 0;
}