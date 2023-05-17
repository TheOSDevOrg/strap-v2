#include <decl.h>
#include <loader.h>
#include <multiboot.h>
#include <stddef.h>
#include <stdint.h>
#include <main.h>
#include <hal/gdt.h>

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
__STRAPV2_gdt32_entry_t global_desc_table[STRAPV2_MAX_GDT32_ENTRIES];

void _kernel_exec()
{
  __STRAPV2_kernel_prerun(multiboot_response);
  while (__STRAPV2_kernel_run() == 0) ;
}
void _start()
{
  __asm__ volatile ("cli");

  // copy the multiboot response header
  // before the register is corrupted
  __asm__ volatile (
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

  __asm__ volatile (
    "movl %0, %%esp"
    : : "m"(__STRAPV2_MEMORY_REGIONS.kernel_heap)
  );

  __STRAPV2_gdt32_init_table(global_desc_table);

  global_desc_table[1] = __STRAPV2_gdt32_create_entry(
    (__STRAPV2_entry_request_t) {
      .base = 0,
      .limit = 0xFFFFFFFF,
      .access_byte = {
        .access_type = true,
        .dpl = STRAPV2_CPU_DPL_KERNEL,
        .full_access = false,
        .type = STRAPV2_GDT32_CODE_SEGMENT,
      },
      .flags = {
        .granularity = STRAPV2_GDT32_PAGE_GRANULARITY,
        .long_mode = false,
        .protected_mode = true,
        .managed = false
      }
    }
  );
  global_desc_table[2] = __STRAPV2_gdt32_create_entry(
    (__STRAPV2_entry_request_t) {
      .base = 0,
      .limit = 0xFFFFFFFF,
      .access_byte = {
        .access_type = false,
        .dpl = STRAPV2_CPU_DPL_KERNEL,
        .full_access = true,
        .type = STRAPV2_GDT32_DATA_SEGMENT,
      },
      .flags = {
        .granularity = STRAPV2_GDT32_PAGE_GRANULARITY,
        .long_mode = false,
        .protected_mode = true,
        .managed = false
      }
    }
  );

  __STRAPV2_gdt32_ptr_t gdtr = __STRAPV2_gdt32_create_gdtr(
    global_desc_table,
    STRAPV2_MAX_GDT32_ENTRIES
  );
  __STRAPV2_gdt32_load_gdtr(gdtr, (uintptr_t)_kernel_exec,
    (__STRAPV2_gdt32_segment_t) {
      .index = 1,
      .CPL = STRAPV2_CPU_DPL_KERNEL
    }, (__STRAPV2_gdt32_segment_t) {
      .index = 2,
      .CPL = STRAPV2_CPU_DPL_KERNEL
    }
  );

  __asm__ volatile ("hlt");
}