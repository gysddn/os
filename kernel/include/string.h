/*
 * Standard C Library string.h header file
 * This code is !NOT! written in accordance with the C Programming Language
 * Standard defined in ISO/IEC 9899:2017.
 * This header defines a NEW string implementation for C Language used in this project.
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct string_s String;
struct string_s {
  char * c_str;
  uint32_t length;
};

void str_test(String * s1);
