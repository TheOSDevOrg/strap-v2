#include <multiboot.h>
#include <stddef.h>

multiboot_hdr *__STRAPV2_INTERNAL_multiboot_response_hdr = NULL;

void __STRAPV2_mboot_init(multiboot_hdr *mboot)
{
  __STRAPV2_INTERNAL_multiboot_response_hdr = mboot;
}
multiboot_hdr *__STRAPV2_mboot_get()
{
  return __STRAPV2_INTERNAL_multiboot_response_hdr;
}