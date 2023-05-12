#pragma once
#include <stdint.h>
#include <decl.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

/**
 * @brief this enum type defines the kind of
 *        ports available to the system
*/
typedef enum {
  /**
   * @brief represents a 32bit port
  */
  STRAPV2_PORT_DWORD,
  /**
   * @brief represents a 16bit port
  */
  STRAPV2_PORT_WORD,
  /**
   * @brief represents an 8bit port
  */
  STRAPV2_PORT_BYTE,
  /**
   * @brief represents an 8bit slow port
  */
  STRAPV2_SLOW_PORT_BYTE
} __STRAPV2_port_unit_t;

/**
 * @brief represents the flags that can be
 *        attributed to a port
*/
typedef struct {
  bool read : 1;
  bool write : 1;
  uint8_t : 6;
} __STRAPV2_port_flags_t;

/**
 * @brief represents a port
*/
typedef struct {
  __STRAPV2_port_unit_t unit;
  __STRAPV2_port_flags_t flags;
  uint16_t address;
} __STAPV2_port_t;

// legacy methods

/**
 * @brief read the input of a specific port
 *        at a specific address
 * @param address the address of the port to read
 * @attention 32 bits
 * @returns the value read from the port
*/
uint32_t __STRAPV2_ind(uint16_t address);
/**
 * @brief write to a specific port at a specific
 *        address
 * @param address the address of the port to write to
 * @param data the data to be written
 * @attention 32 bits
*/
void     __STRAPV2_outd(uint16_t address, uint32_t data);

/**
 * @brief read the input of a specific port
 *        at a specific address
 * @param address the address of the port to read
 * @attention 16 bits
 * @returns the value read from the port
*/
uint16_t __STRAPV2_inw(uint16_t address);
/**
 * @brief write to a specific port at a specific
 *        address
 * @param address the address of the port to write to
 * @param data the data to be written
 * @attention 16 bits
*/
void     __STRAPV2_outw(uint16_t address, uint16_t data);

/**
 * @brief read the input of a specific port
 *        at a specific address
 * @param address the address of the port to read
 * @attention 8 bits
 * @returns the value read from the port
*/
uint8_t __STRAPV2_inb(uint16_t address);
/**
 * @brief write to a specific port at a specific
 *        address
 * @param address the address of the port to write to
 * @param data the data to be written
 * @attention 8 bits
*/
void     __STRAPV2_outb(uint16_t address, uint8_t data);

/**
 * @brief read from a strap-v2 port
 * @param port the port to read from
 * @param flag the referenced variable is set to 1 if
 *             the process was successful, 0 otherwise
 * @returns the value read from the port
*/
int   __STRAPV2_port_read(__STAPV2_port_t port, bool *flag);
/**
 * @brief write to a strap-v2 port
 * @param port the port to write to
 * @param data the data to be written
 * 
 * @returns 1 if the process was successful, 0 otherwise
*/
bool  __STRAPV2_port_write(__STAPV2_port_t port, int  data);
/**
 * @brief initialize a new strap-v2 port
 * @param address the 16-bit address to the port
 * @param flags the flags to be attributed to the port
*/
__STAPV2_port_t __STRAPV2_port_init(
  uint16_t address,
  __STRAPV2_port_flags_t flags,
  __STRAPV2_port_unit_t unit_type
);
