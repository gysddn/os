/*
 * Serial Interface Source File
 * @gysddn <gysddn@gmail.com>
 *
 */

#include "serial.h"
#include "io.h"

void print_debug(char * str) {
  while (*str != 0x00) {
    out8(COM1, (u8)*str++);
  }
}
