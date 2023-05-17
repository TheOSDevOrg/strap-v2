#include <hal/gdt.h>
#include <loader.h>

__STRAPV2_gdt32_entry_t __STRAPV2_gdt32_create_entry(
  __STRAPV2_entry_request_t request
)
{
  if (request.flags.granularity == STRAPV2_GDT32_PAGE_GRANULARITY)
    request.limit /= STRAPV2_PAGE_GRAN_SIZE;

  uint16_t bh16 = request.base >> 16;
  return (__STRAPV2_gdt32_entry_t) {
    .base_low = request.base & 0xFFFF,
    .base_mid = bh16 & 0xFF,
    .base_high = bh16 >> 8,

    .limit_low = (request.limit & 0xFFFF),
    .limit_high = (request.limit >> 16) & 0xF,

    .access_byte.access_type = request.access_byte.access_type,
    .access_byte.dpl = request.access_byte.dpl,
    .access_byte.full_access = request.access_byte.full_access,
    .access_byte.type = request.access_byte.type,
    .access_byte.present = true,
    .access_byte.accessed = false,

    .managed = request.flags.managed,
    .long_mode = request.flags.long_mode,
    .protected_mode = request.flags.protected_mode,
    .granularity = request.flags.granularity,
  };
}
void __STRAPV2_gdt32_init_table(
  __STRAPV2_gdt32_entry_t *table
)
{
  table[0] = (__STRAPV2_gdt32_entry_t){0};
}
__STRAPV2_gdt32_ptr_t __STRAPV2_gdt32_create_gdtr(
  __STRAPV2_gdt32_entry_t *table,
  uint16_t entries
)
{
  return (__STRAPV2_gdt32_ptr_t) {
    .table = table,
    .size = (entries * 8) - 1
  };
}
void __STRAPV2_gdt32_load_gdtr(
  __STRAPV2_gdt32_ptr_t table_ptr,
  uintptr_t dest,
  __STRAPV2_gdt32_segment_t code_segment,
  __STRAPV2_gdt32_segment_t data_segment
)
{
  uint16_t cs = (code_segment.index * 8) | code_segment.CPL;
  uint16_t ds = (data_segment.index * 8) | data_segment.CPL;

  __asm__ volatile ("lgdt %0" : : "m"(table_ptr));
  __asm__ volatile ("movw %0, %%ds" : : "m"(ds));
  __asm__ volatile ("movw %0, %%es" : : "m"(ds));
  __asm__ volatile ("movw %0, %%gs" : : "m"(ds));
  __asm__ volatile ("movw %0, %%ss" : : "m"(ds));
  __asm__ volatile ("movw %0, %%fs" : : "m"(ds));
  __asm__ volatile ("pushl %0" : : "m"(cs));
  __asm__ volatile ("pushl %0" : : "m"(dest));
  __asm__ volatile ("iret");
  return;
}