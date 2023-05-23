#pragma once
#include <multiboot.h>
#include <stdint.h>
#include <stddef.h>

extern uintptr_t __STRAPV2_kernel_start;
extern uintptr_t __STRAPV2_kernel_end;
extern size_t __STRAPV2_kernel_size;

void __STRAPV2_kernel_prerun(multiboot_hdr *multiboot);
int __STRAPV2_kernel_run();