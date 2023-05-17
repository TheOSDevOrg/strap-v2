#pragma once
#include <stddef.h>
#include <stdint.h>
#include <hal/cpu.h>
#include <decl.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#define STRAPV2_MAX_GDT32_ENTRIES 0x2000
#define STRAPV2_MAX_LIMIT_VALUE   524287
#define STRAPV2_PAGE_GRAN_SIZE    4096


typedef enum __STRAPV2_gdt32_segment_type {
  STRAPV2_GDT32_SYSTEM_SEGMENT = 0,
  STRAPV2_GDT32_DATA_SEGMENT = (1 << 1),
  STRAPV2_GDT32_CODE_SEGMENT = (1 << 1) | 1
} __STRAPV2_gdt32_segment_type_t;
typedef enum __STRAPV2_gdt32_granularity_type {
  STRAPV2_GDT32_BYTE_GRANULARITY,
  STRAPV2_GDT32_PAGE_GRANULARITY
} __STRAPV2_gdt32_granularity_type_t;

/**
 * @brief this type represents an access byte for
 *        a segment descriptor, refer to
 *        https://wiki.osdev.org/Global_Descriptor_Table#Segment_Descriptor
*/
typedef struct __STRAPV2_gdt32_access_byte {
  /**
   * @brief this bit is set by the processor
   *        when the memory segment is first
   *        accessed, better initialized to 0
  */
  bool accessed : 1;
  /**
   * @brief this bit defines whether the memory is
   *        fully accessible or not.
   * @attention for code segments it defines if the
   *            segment can be read (readable bit).
   *            For data segments it defines if the
   *            segment can be written on (writable bit)
  */
  bool full_access : 1;
  /**
   * @brief this is the direction/conforming bit which
   *        defines different ways of accessing the
   *        memory segment
   * @attention for code segments it is the conforming
   *            bit, which defines whether lower DPLs
   *            can far jump into this mode.
   *            For data segments it defines if the
   *            segment grows down (stacks) or not
  */
  bool access_type : 1;
  /**
   * @brief this field comes from the union of two
   *        bits (the descriptor type bit and the
   *        executable bit). It specifies the type of
   *        the segment (code, data or system)
  */
  __STRAPV2_gdt32_segment_type_t type : 2;
  /**
   * @brief this field specifies the CPL of the
   *        segment
  */
  __STRAPV2_cpu_dpl_t dpl : 2;
  /**
   * @brief this field needs to be set to true for
   *        every valid gdt entry. Segments descriptors
   *        accessed with this field set to false will
   *        cause general protection faults
  */
  bool present : 1;
} packed__ __STRAPV2_gdt32_access_byte_t;
typedef struct __STRAPV2_gdt32_entry {
  uint16_t limit_low, base_low;
  uint8_t  base_mid;
  __STRAPV2_gdt32_access_byte_t access_byte;
  uint8_t  limit_high : 4;
  
  // flags
  /**
   * @brief if set, this flag defines a managed
   *        region of memory that must be
   *        supervised at runtime by some code
   *        interpreter
  */
  bool managed : 1;
  /**
   * @brief if set, this flag defines a 64bit
   *        descriptor.
   * @attention when set, the protected_mode flag
   *            should always be clear.
  */
  bool long_mode : 1;
  /**
   * @brief if set, this flag defines a 32bit
   *        descriptor. A table can have both 32 and
   *        16 bit segments at the same time.
  */
  bool protected_mode : 1;
  /**
   * @brief this flag represents the granularity of the
   *        limit. If set to byte-granularity the limit
   *        is considered to be in byte blocks,
   *        otherwise it is considered to be in page
   *        blocks (or blocks of 4kib each)
  */
  __STRAPV2_gdt32_granularity_type_t granularity : 1; 

  uint8_t base_high;
} packed__ __STRAPV2_gdt32_entry_t;

typedef struct __STRAPV2_gdt32_ptr {
  uint16_t  size;
  uintptr_t table;
} packed__ __STRAPV2_gdt32_ptr_t;

typedef struct __STRAPV2_entry_request {
  uintptr_t base;
  uintptr_t limit;
  __STRAPV2_gdt32_access_byte_t access_byte;
  struct __STRAPV2_gdt32_flags {
    /**
     * @brief if set, this flag defines a managed
     *        region of memory that must be
     *        supervised at runtime by some code
     *        interpreter
    */
    bool managed : 1;
    /**
     * @brief if set, this flag defines a 64bit
     *        descriptor.
     * @attention when set, the protected_mode flag
     *            should always be clear.
    */
    bool long_mode : 1;
    /**
     * @brief if set, this flag defines a 32bit
     *        descriptor. A table can have both 32 and
     *        16 bit segments at the same time.
    */
    bool protected_mode : 1;
    /**
     * @brief this flag represents the granularity of the
     *        limit. If set to byte-granularity the limit
     *        is considered to be in byte blocks,
     *        otherwise it is considered to be in page
     *        blocks (or blocks of 4kib each)
    */
    __STRAPV2_gdt32_granularity_type_t granularity : 1;
  } flags;
} __STRAPV2_entry_request_t;
typedef struct __STRAPV2_gdt32_segment {
  uint16_t index;
  __STRAPV2_cpu_dpl_t CPL;
} __STRAPV2_gdt32_segment_t;

/**
 * @brief create a new gdt entry
 * @param request the requested entry to create
 * @returns the created entry
*/
__STRAPV2_gdt32_entry_t __STRAPV2_gdt32_create_entry(
  __STRAPV2_entry_request_t request
);
/**
 * @brief initialize a new gdt table
 * @param table the pointer to the table
*/
void __STRAPV2_gdt32_init_table(
  __STRAPV2_gdt32_entry_t *table
);
/**
 * @brief create a new gdt pointer
 * @param table the pointer to the table
 * @param entries the number of max entries
*/
__STRAPV2_gdt32_ptr_t __STRAPV2_gdt32_create_gdtr(
  __STRAPV2_gdt32_entry_t *table,
  uint16_t entries
);
/**
 * @brief load the gdt pointer to the gdt register
 * @param table_ptr the table pointer
*/
void __STRAPV2_gdt32_load_gdtr(
  __STRAPV2_gdt32_ptr_t table_ptr,
  uintptr_t dest,
  __STRAPV2_gdt32_segment_t code_segment,
  __STRAPV2_gdt32_segment_t data_segment
);