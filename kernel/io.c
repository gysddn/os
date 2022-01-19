/*
 * I/O Management Source File
 * @gysddn <gysddn@gmail.com>
 *
 */

#include "io.h"

u8 in8(const u16 port) {
  u8 value;
  __asm__("mov %1, %%dx\n"
          "in %%dx, %%al\n"
          "movb %%al, %0\n"
          : "=r" (value)
          : "r" (port)
          : "%eax", "edx"
          );
  return value;
}

u16 in16(const u16 port) {
  u16 value;
  __asm__("mov %1, %%dx\n"
          "in %%dx, %%ax\n"
          "movw %%ax, %0\n"
          : "=r" (value)
          : "r" (port)
          : "%eax", "edx"
          );
  return value;
}

u32 in32(const u16 port) {
  u32 value;
  __asm__("mov %1, %%dx\n"
          "in %%dx, %%eax\n"
          "movl %%eax, %0\n"
          : "=r" (value)
          : "r" (port)
          : "%eax", "edx"
          );
  return value;
}


void out8(const u16 port, const u8 value) {
  __asm__("mov %1, %%dx\n"
          "movb %0, %%al\n"
          "out %%al, %%dx\n"
          :
          :"r" (value),  "r" (port)
          : "%eax", "edx"
          );
}

void out16(const u16 port, const u16 value) {
  __asm__("mov %1, %%dx\n"
          "movw %0, %%ax\n"
          "out %%ax, %%dx\n"
          :
          :"r" (value),  "r" (port)
          : "%eax", "edx"
          );
}

void out32(const u16 port, const u32 value) {
  __asm__("mov %1, %%dx\n"
          "movl %0, %%eax\n"
          "out %%eax, %%dx\n"
          :
          :"r" (value),  "r" (port)
          : "%eax", "edx"
          );
}
