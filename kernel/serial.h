/*
 * Serial Interface Header File
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

#include "types.h"

#define COM1 0x3F8

void print_debug(char * str);

int kprintf(const char * restrict format);
