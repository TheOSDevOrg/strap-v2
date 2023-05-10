/*
Loader by: Nicola Leone Ciardi and Kevin Meerts
Please leave credits if using this code
*/
#pragma once
#include <decl.h>
#include <stddef.h>
#include <stdint.h>
#include <options.h>

// C only requirements
#ifndef __cplusplus
#include <stdbool.h>
#endif

#define STRAPV2_LOADER_MAGIC 0x1BADB002

struct __STRAPV2_multiboot_header_loader
{
  uint32_t magic;
  struct
  {
    bool align : 1;
    bool mmap : 1;
    bool video : 1;
    uint32_t : 29;
  } flags;
  uint32_t checksum;
  uint8_t reserved[20];
  struct
  {
    uint32_t video_mode;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
  } vbe_info;
};
struct __STRAPV2_memory_regions
{
  uint8_t *kernel_stack;
  uint8_t *kernel_heap;
  uint8_t *common_heap;
  uint8_t *lf_heap;
  uintptr_t end;
};

#ifndef __cplusplus
typedef struct __STRAPV2_multiboot_header_loader __STRAPV2_multiboot_header_loader_t;
typedef struct __STRAPV2_memory_regions __STRAPV2_memory_regions_t;
extern __STRAPV2_memory_regions_t __STRAPV2_MEMORY_REGIONS;
#else
__cdecl __STRAPV2_memory_regions __STRAPV2_MEMORY_REGIONS;
#endif