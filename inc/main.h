#pragma once
#include <multiboot.h>
#include <stdint.h>

void __STRAPV2_kernel_prerun(multiboot_hdr *multiboot);
int __STRAPV2_kernel_run();