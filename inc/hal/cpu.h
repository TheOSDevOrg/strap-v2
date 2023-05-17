#pragma once

typedef enum __STRAPV2_cpu_dpl {
  STRAPV2_CPU_DPL_KERNEL = 0,
  STRAPV2_CPU_DPL_MODULE = 1,
  STRAPV2_CPU_DPL_DRIVER = 2,
  STRAPV2_CPU_DPL_USER   = 3,
} __STRAPV2_cpu_dpl_t;