#include <main.h>
#include <out.h>
#include <stdio.h>
#include <hal/gdt.h>

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

void __STRAPV2_kernel_prerun(multiboot_hdr *multiboot)
{
  __STRAPV2_init_output_system();
  __STRAPV2_switch_std(&_stdout);
  __STRAPV2_clear();
  __STRAPV2_print("info:  output system ready!\n");
  __STRAPV2_mboot_init(multiboot);
  __STRAPV2_print("info:  multiboot data ready!\n");
  __STRAPV2_kernel_run();
}
int __STRAPV2_kernel_run()
{
  __STRAPV2_print("Welcome to " KERN_NAME " " KERN_VER "\n");
  __STRAPV2_print("From commit " KERN_COMMIT_ID "\n");
  //__STRAPV2_print("sample_prompt strap ? \n");

  printf("%d\n", 10);
  printf("%d\n", -10);
  printf("%x\n", 0x0a);
  printf("%x\n", -0x0a);
  printf("0x%uX\n", 0xdeadbeef);
  printf("0x%%test%%\n");

  __STRAPV2_render();
  while(1) ;
  return 0;
}