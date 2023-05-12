#include <main.h>
#include <out.h>

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
  __STRAPV2_switch_std(&_stdout);
}
int __STRAPV2_kernel_run()
{
  __STRAPV2_print("a\bb");
  __STRAPV2_render();
  while(1) ;
  return 0;
}