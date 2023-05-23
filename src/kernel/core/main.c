#include <main.h>
#include <out.h>
#include <stdio.h>
#include <hal/gdt.h>
#include <memory.h>

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
  __STRAPV2_print("info:  multiboot data ready!\n");
}
int __STRAPV2_kernel_run()
{
  uint32_t installed = __STRAPV2_get_installed_mem_size() / 1024 / 1024;
  uint32_t usable = __STRAPV2_get_usable_mem_size() / 1024 / 1024;

  __STRAPV2_print("Welcome to " KERN_NAME " " KERN_VER "\n");
  __STRAPV2_print("From commit " KERN_COMMIT_ID "\n");
  printf(
    "Installed memory: %ud MB (usable %ud MB)\n",
    installed,
    usable
  );
  //__STRAPV2_print("sample_prompt strap ? \n");

  __STRAPV2_render();
  while(1) ;
  return 0;
}