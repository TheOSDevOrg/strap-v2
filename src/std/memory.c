#include <stddef.h>
#include <stdint.h>
#include <multiboot.h>

void memcpyd(uint32_t *dst, uint32_t *src, size_t sz)
{
  for (size_t i = 0; i < sz; i++)
    dst[i] = src[i];
}
void memsetd(uint32_t *dst, uint32_t data, size_t sz)
{
  for (size_t i = 0; i < sz; i++)
    dst[i] = data;
}

void memcpyw(uint16_t *dst, uint16_t *src, size_t sz)
{
  for (size_t i = 0; i < sz; i++)
    dst[i] = src[i];
}
void memsetw(uint16_t *dst, uint16_t data, size_t sz)
{
  for (size_t i = 0; i < sz; i++)
    dst[i] = data;
}

void memcpyb(uint8_t *dst, uint8_t *src, size_t sz)
{
  for (size_t i = 0; i < sz; i++)
    dst[i] = src[i];
}
void memsetb(uint8_t *dst, uint8_t data, size_t sz)
{
  for (size_t i = 0; i < sz; i++)
    dst[i] = data;
}

void memcpy(void *dst, void *src, size_t sz)
{
  int s, k;

  s = sz / 4;
  sz -= (k = s * 4);
  memcpyd((uint32_t *)dst, (uint32_t *)src, s);
  dst = (void *)((uintptr_t)dst + k);
  src = (void *)((uintptr_t)src + k);

  s = sz / 2;
  sz -= (k = s * 2);
  memcpyw((uint16_t *)dst, (uint16_t *)src, s);
  dst = (void *)((uintptr_t)dst + k);
  src = (void *)((uintptr_t)src + k);

  memcpyb((uint8_t *)dst, (uint8_t *)src, sz);
}
void memset(void *dst, int data, size_t sz)
{
  int s, k;

  s = sz / 4;
  sz -= (k = s * 4);
  memsetd((uint32_t *)dst, (uint32_t)data, s);
  dst = (void *)((uintptr_t)dst + k);

  s = sz / 2;
  sz -= (k = s * 2);
  memcpyw((uint16_t *)dst, (uint16_t *)data, s);
  dst = (void *)((uintptr_t)dst + k);

  memcpyb((uint8_t *)dst, (uint8_t *)data, sz);
}

uint64_t __STRAPV2_INTERNAL_mem_size_cache = 0;
uint64_t __STRAPV2_get_installed_mem_size()
{
  if (__STRAPV2_INTERNAL_mem_size_cache)
    return __STRAPV2_INTERNAL_mem_size_cache;

  multiboot_hdr *mbi = __STRAPV2_mboot_get();
  if (!mbi)
    return __STRAPV2_INTERNAL_mem_size_cache = -1;
  
  size_t mmap_slots = mbi->mmap_len / sizeof(mmap_entry_t);
  mmap_entry_t *mmap_entries = (mmap_entry_t *)mbi->mmap_addr;

  for (size_t i = 0; i < mmap_slots; i++)
    if (mmap_entries[i].type == MULTIBOOT_MEMORY_AVAILABLE)
      __STRAPV2_INTERNAL_mem_size_cache += mmap_entries[i].length;

  return __STRAPV2_INTERNAL_mem_size_cache;
}

uint32_t __STRAPV2_get_usable_mem_size()
{
  uint64_t total_msz = __STRAPV2_get_installed_mem_size();
  
  if (total_msz >= UINT32_MAX) return UINT32_MAX-1;
  return (uint32_t)total_msz;
}