#pragma once

#include <stddef.h>
#include <stdint.h>
#include <heap.h>
#include <decl.h>

__cdecl void memcpyd(uint32_t *dst, uint32_t *src, size_t sz);
__cdecl void memsetd(uint32_t *dst, uint32_t data, size_t sz);

__cdecl void memcpyw(uint16_t *dst, uint16_t *src, size_t sz);
__cdecl void memsetw(uint16_t *dst, uint16_t data, size_t sz);

__cdecl void memcpyb(uint8_t *dst, uint8_t *src, size_t sz);
__cdecl void memsetb(uint8_t *dst, uint8_t data, size_t sz);

__cdecl void memcpy(void *dst, void *src, size_t sz);
__cdecl void memset(void *dst, int data, size_t sz);

extern heap_t __STRAPV2_heap;

void __STRAPV2_memorymgr_init();

__cdecl void *malloc(size_t sz);
__cdecl void free(void *ptr);
__cdecl size_t countof(void *ptr);

__cdecl uint64_t __STRAPV2_get_installed_mem_size();
__cdecl uint32_t __STRAPV2_get_usable_mem_size();