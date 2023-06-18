#include <stddef.h>
#include <decl.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

__cdecl bool __STRAPV2_TEST_alloc_routine(int n);
__cdecl bool __STRAPV2_TEST_memcmp(int n);

// VM tests
__cdecl void __STRAPV2_TEST_VM_flow();
__cdecl void __STRAPV2_TEST_VM_types();
__cdecl void __STRAPV2_TEST_VM_count();
