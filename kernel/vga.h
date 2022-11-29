/*
 * VGA Controller header
 * @gysddn <gysddn@gmail.com>
 *
 */
#pragma once

#include "util/optional.h"

#include <stdint.h>

#define MISC_OUT_REG   0x3CC
#define ATTR_FLIP_FLOP 0x3BA


namespace kernel { //Namespace kernel begin

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

class VGA {
  uint16_t* alp_num_mem = (uint16_t*)0xB8000;

public:
  VGA();

  void init();

  void flush_screen();

  void putchar(uint8_t character, uint8_t attr, uint8_t position_w, uint8_t position_h);

  static uint8_t attr_make_color(uint8_t bg, uint8_t fg);

};


class VGARegister {
public:

  optional<uint8_t> state();

  /*
   * These functions do not exist and for documentation only.
   * They are meant to be virtual but since there is not memory management yet,
   * it is not possible.
   */

  /* optional<uint8_t> read(); */

  //TODO error state, a Result type implementation??
  /* void write(uint8_t); */

  /* optional<uint8_t> state(); */

protected:
  //Can't use virtual functions, so disable the instantiation of this class
  //out of child classes
  VGARegister() = default;

  //read() will update this value as well
  optional<uint8_t> _state;
};

class BasicRegister : public VGARegister {
public:
  BasicRegister() = delete;
  BasicRegister(uint16_t read_port, uint16_t write_port):
    _read_port(read_port), _write_port(write_port) {}

  optional<uint8_t> read();
  void write(uint8_t);


private:
    uint16_t _read_port;
    uint16_t _write_port;
};

class ExtendedRegister : public VGARegister {
public:
  using Mode = enum {
    Color,
    Mono
  };

  ExtendedRegister() = delete;
  ExtendedRegister(uint16_t read_mono_port, uint16_t read_color_port, 
      uint16_t write_mono_port, uint16_t write_color_port):
    _read_port_mono(read_mono_port), _read_port_color(read_color_port), 
    _write_port_mono(write_mono_port), _write_port_color(write_color_port) {}

  optional<uint8_t> read();
  void write(uint8_t);

  void setMode(Mode mode) {
    _mode = mode;
  }

  Mode mode() {
    return _mode;
  }

private:
    uint16_t _read_port_mono;
    uint16_t _read_port_color;
    uint16_t _write_port_mono;
    uint16_t _write_port_color;
    Mode _mode;
};

class IndexedRegister : public VGARegister {
public:
  IndexedRegister() = delete;
  IndexedRegister(uint16_t address_port, uint16_t data_port, uint8_t index):
    _address_port(address_port), _data_port(data_port), _index(index) {}

  optional<uint8_t> read();
  void write(uint8_t);

protected:
    uint16_t _address_port;
    uint16_t _data_port;
    uint8_t  _index;
};

class CRTRegister : public VGARegister {
  using Pair = struct {
    uint16_t addressPort;
    uint16_t dataPort;
  };

  CRTRegister() = delete;
  CRTRegister(uint16_t pair0_address, uint16_t pair0_data, 
      uint16_t pair1_address, uint16_t pair1_data, uint8_t index):
    _pair0({.addressPort = pair0_address, .dataPort = pair0_data}), 
    _pair1({.addressPort = pair1_address, .dataPort = pair1_data}),
    _index(index) {}

  optional<uint8_t> read();
  void write(uint8_t);

private:
    Pair _pair0;
    Pair _pair1;
    uint8_t _index;
};

/*
 * AttributeRegister has the same fields as IndexedRegister
 * but AttributeRegister has a more confusing way of using them
 * so they will be processed differently.
 */
class AttributeRegister : public IndexedRegister {
public:
  AttributeRegister() = delete;
  AttributeRegister(uint16_t address_port, uint16_t data_port, uint8_t index):
    IndexedRegister(address_port, data_port, index) {}

  optional<uint8_t> read();
  void write(uint8_t);
};

}; //Namespace kernel end
