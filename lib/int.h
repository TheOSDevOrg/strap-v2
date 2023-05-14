#pragma once
#include <stddef.h>
#include <stdint.h>

/**
 * @brief count the number of digits in a given
 *        number
 * @param number the number
 * @param base   the base of the number (8-10-16)
*/
size_t numbersz(uint32_t number, int base);