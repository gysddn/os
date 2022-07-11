/*
 * Serial Interface Header File
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

#include <stdint.h>

namespace kernel {

struct SerialIO {
  static const uint32_t COM1 = 0x3F8;

  static void write(const char *str);
  static void write(const uint32_t num, int base);
  static void write(const void *ptr);
  static void endl();

  private:
  static void prntnum(unsigned long num, int base, char *outbuf);
   
};

} //End namespace kernel
