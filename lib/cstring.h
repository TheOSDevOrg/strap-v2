#pragma once
#include <stdint.h>
#include <stddef.h>

/// @brief calculate the length of a string
/// @param str the string to calculate the length of
/// @return the length of the string
size_t strlen(char str[]);

/// @brief compare two strings
/// @param first the first string
/// @param second the second string
/// @return 0 if the strings are equal
int strcmp(char first[], char second[]);