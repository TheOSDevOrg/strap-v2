#include <stddef.h>
#include <stdint.h>

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
  int i;
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
  int i;
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