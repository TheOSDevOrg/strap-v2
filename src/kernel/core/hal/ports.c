#include <hal/ports.h>

uint32_t __STRAPV2_ind(uint16_t address)
{
  uint32_t data;
  __asm__ volatile (
    "inl %%dx, %%eax"
    : "=a" (data)
    : "d"  (address)
  );

  return data;
}
void __STRAPV2_outd(uint16_t address, uint32_t data)
{
  __asm__ volatile (
    "outl %%eax, %%dx"
    :: "a" (data), "d" (address)
  );
}

uint16_t __STRAPV2_inw(uint16_t address)
{
  uint16_t data;
  __asm__ volatile (
    "inw %%dx, %%ax"
    : "=a" (data)
    : "d"  (address)
  );

  return data;
}
void __STRAPV2_outw(uint16_t address, uint16_t data)
{
  __asm__ volatile (
    "outw %%ax, %%dx"
    :: "a" (data), "d" (address)
  );
}

uint8_t __STRAPV2_inb(uint16_t address)
{
  uint8_t data;
  __asm__ volatile (
    "inb %%dx, %%al"
    : "=a" (data)
    : "d"  (address)
  );
  
  return data;
}
void __STRAPV2_outb(uint16_t address, uint8_t data)
{
  __asm__ volatile (
    "outb %%al, %%dx"
    :: "a"(data), "d"(address)
  );
}

int   __STRAPV2_port_read(__STAPV2_port_t port, bool *flag)
{
  if (!port.flags.read) return *flag = false;

  *flag = true;
  switch (port.unit)
  {
    case STRAPV2_PORT_DWORD:
      return __STRAPV2_ind(port.address);
    case STRAPV2_PORT_WORD:
      return __STRAPV2_inw(port.address);
    case STRAPV2_PORT_BYTE:
      return __STRAPV2_inb(port.address);
    case STRAPV2_SLOW_PORT_BYTE:
    default:
      return *flag = false;
  }
}
bool __STRAPV2_port_write(__STAPV2_port_t port, int  data)
{
  if (!port.flags.write) return false;
  switch (port.unit)
  {
    case STRAPV2_PORT_DWORD:
      __STRAPV2_outd(port.address, (uint32_t)data);
      break;
    case STRAPV2_PORT_WORD:
      __STRAPV2_outw(port.address, (uint16_t)data);
      break;
    case STRAPV2_PORT_BYTE:
      __STRAPV2_outb(port.address, (uint8_t)data);
      break;
    case STRAPV2_SLOW_PORT_BYTE:
    default:
      return false;
  }
  return true;
}
__STAPV2_port_t __STRAPV2_port_init(
  uint16_t address,
  __STRAPV2_port_flags_t flags,
  __STRAPV2_port_unit_t unit_type
)
{
  return (__STAPV2_port_t) {
    .address = address,
    .flags = flags,
    .unit = unit_type
  };
}