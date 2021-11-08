/*
 * I/O Management Header File
 * @gysddn <gysddn@gmail.com>
 *
 */
#pragma once

#include "types.h"


u8  read_port_byte(const u16 port);
u16 read_port_word(const u16 port);
u32 read_port_dword(const u16 port);

void write_port_byte(const u16 port, const u8 value);
void write_port_word(const u16 port, const u16 value);
void write_port_dword(const u16 port, const u32 value);
