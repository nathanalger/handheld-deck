#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * Returns size of string as size_t
 */
size_t strlen(const char *str);

/**
 * Returns size of string represented by a 4 byte value, meaning it has a max length of 4,294,967,295 characters.
 */
uint32_t strlen_32(const char *str);