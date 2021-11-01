/*
 * VGA Controller header
 * @gysddn <gysddn@gmail.com>
 *
 */
#pragma once

#include "types.h"

enum {
  VGA_BIOS_COLOR_BLACK         = 0x0,
  VGA_BIOS_COLOR_BLUE          = 0x1,
  VGA_BIOS_COLOR_GREEN         = 0x2,
  VGA_BIOS_COLOR_CYAN          = 0x3,
  VGA_BIOS_COLOR_RED           = 0x4,
  VGA_BIOS_COLOR_MAGENTA       = 0x5,
  VGA_BIOS_COLOR_BROWN         = 0x6,
  VGA_BIOS_COLOR_WHITE         = 0x7,
  VGA_BIOS_COLOR_GRAY          = 0x8,
  VGA_BIOS_COLOR_LIGHT_BLUE    = 0x9,
  VGA_BIOS_COLOR_LIGHT_GREEN   = 0xA,
  VGA_BIOS_COLOR_LIGHT_CYAN    = 0xB,
  VGA_BIOS_COLOR_LIGHT_RED     = 0xC,
  VGA_BIOS_COLOR_LIGHT_MAGENTA = 0xD,
  VGA_BIOS_COLOR_YELLOW        = 0xE,
  VGA_BIOS_COLOR_WHITE_HI      = 0xF,
};


/*
 * Currently supports 80x25 screen with Alphanumeric characters
 */

void vga_init();

void vga_flush();

void vga_putchar(u8 character, u8 attr, u8 position_w, u8 position_h);

u8 attr_make_color(u8 bg, u8 fg);
