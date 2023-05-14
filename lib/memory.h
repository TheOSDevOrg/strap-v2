#pragma once

#include <stddef.h>
#include <stdint.h>

void memcpyd(uint32_t *dst, uint32_t *src, size_t sz);
void memsetd(uint32_t *dst, uint32_t data, size_t sz);

void memcpyw(uint16_t *dst, uint16_t *src, size_t sz);
void memsetw(uint16_t *dst, uint16_t data, size_t sz);

void memcpyb(uint8_t *dst, uint8_t *src, size_t sz);
void memsetb(uint8_t *dst, uint8_t data, size_t sz);

void memcpy(void *dst, void *src, size_t sz);
void memset(void *dst, int data, size_t sz);