/*
 * Serial Interface Header File
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

#include <stdint.h>
#include "util/optional.h"

namespace kernel {

struct SerialIO {
public:
  //Use enum so that operator can differentiate between a base input
  //and a number input
  using Base = enum {
    Base8   =  8,
    Base10  = 10,
    Base16  = 16
  };

  SerialIO() : _base(Base10), _msg() {};

  void write(const char *str);
  void write(const uint32_t num);
  void write(const void *ptr);

  Base base();
  void setBase(Base);

  optional<const char*> msg();
  void setMsg(const char* message);

  //TODO make out flush
  /* static void endl(); */

private:
  static constexpr uint32_t COM1 = 0x3F8;

  Base _base;
  optional<const char*> _msg;

  //Helper function
  void prntnum(unsigned long num, int base, char *outbuf);

};

SerialIO& operator<<(SerialIO& io, SerialIO::Base base);

template<typename T>
SerialIO& operator<<(SerialIO& io, T arg) {
  io.write(arg);
  return io;
}

} //End namespace kernel
