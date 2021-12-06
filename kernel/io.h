/*
 * I/O Management Header File
 * @gysddn <gysddn@gmail.com>
 *
 */
#pragma once

#include "types.h"


u8  in8(const u16 port);
u16 in16(const u16 port);
u32 in32(const u16 port);

void out8(const u16 port, const u8 value);
void out16(const u16 port, const u16 value);
void out32(const u16 port, const u32 value);
