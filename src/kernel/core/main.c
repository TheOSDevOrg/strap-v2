#include <main.h>

void __STRAPV2_kernel_prerun(multiboot_hdr *multiboot)
{

}
int __STRAPV2_kernel_run()
{
  *((uint16_t *)0xb8000) = 0x0f21;
  return 0;
}