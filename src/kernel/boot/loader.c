#include <decl.h>
#include <loader.h>
#include <multiboot.h>
#include <stddef.h>
#include <stdint.h>
#include <main.h>

__STRAPV2_multiboot_header_loader_t multiboot section__(.multiboot) =
  (__STRAPV2_multiboot_header_loader_t) {
    .magic = STRAPV2_LOADER_MAGIC,
    .flags = {
        .align = true,
        .mmap = true,
        #ifdef VBE_MODE
        .video = true,
        #else
        .video = false,
        #endif
    },
    #ifdef VBE_MODE
    .checksum = -(STRAPV2_LOADER_MAGIC + ((1 << 0) | (1 << 1) | (1 << 2))),
    #else
    .checksum = -(STRAPV2_LOADER_MAGIC + ((1 << 0) | (1 << 1))),
    #endif
    .reserved = {0},
    .vbe_info = {
        .video_mode = 0,
        .width = 800,
        .height = 600,
        .depth = 32
    }
  };

extern uintptr_t kernel_end;
extern uintptr_t kernel_start;

multiboot_hdr *multiboot_response;
__STRAPV2_memory_regions_t __STRAPV2_MEMORY_REGIONS;

void _start()
{
  asm volatile ("cli");

  // copy the multiboot response header
  // before the register is corrupted
  asm volatile (
    "mov %%ebx, %0"
    : "=m"(multiboot_response)
  );
  
  // setup the memory regions
  __STRAPV2_MEMORY_REGIONS.kernel_stack =
    (uint8_t *)kernel_end + STRAPV2_BUFFER_REGION_SZ;
  
  __STRAPV2_MEMORY_REGIONS.kernel_heap =
    (uint8_t *)__STRAPV2_MEMORY_REGIONS.kernel_stack +
               STRAPV2_KERNEL_STACK_SZ;
  __STRAPV2_MEMORY_REGIONS.lf_heap =
    (uint8_t *)__STRAPV2_MEMORY_REGIONS.kernel_heap +
               STRAPV2_KERNEL_HEAP_SZ;
  __STRAPV2_MEMORY_REGIONS.common_heap =
    (uint8_t *)__STRAPV2_MEMORY_REGIONS.lf_heap +
               STRAPV2_LF_HEAP_SZ;
  __STRAPV2_MEMORY_REGIONS.end =
  (uint8_t *)__STRAPV2_MEMORY_REGIONS.common_heap +
               STRAPV2_COMMON_HEAP_SZ;
  
  int response = 0;

  __STRAPV2_kernel_prerun(multiboot_response);
  while (response == 0) response = __STRAPV2_kernel_run();

  asm volatile ("hlt");
}