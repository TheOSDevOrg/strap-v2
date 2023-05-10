#pragma once
#define section__(x) __attribute__((section(#x)))
#define packed__ __attribute__((packed))

// c++ only components
#ifdef __cplusplus
#define __cdecl extern "C"
#endif