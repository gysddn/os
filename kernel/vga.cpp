/*
 * VGA Controller
 * @gysddn <gysddn@gmail.com>
 *
 */

#include "vga.h"
#include "io.h"

/*
 * REGISTER MANAGEMENT
 */


namespace kernel { //Namespace kernel begin

uint16_t* alp_num_mem = (uint16_t*)0xB8000;

VGA::VGA() {
  init();
}

void VGA::init() {
  flush_screen();
}

void VGA::flush_screen() {
  for(uint8_t w = 0; w < 80; w++)
    for(uint8_t h = 0; h < 25; h++)
      alp_num_mem[h*80 + w] = 0;
}

void VGA::putchar(uint8_t character, uint8_t attr, uint8_t position_w, uint8_t position_h) {
  if (position_w < 80)
    if (position_h < 25)
      alp_num_mem[position_h*80 + position_w] = (uint16_t)character | (uint16_t) attr << 8;
}

uint8_t VGA::attr_make_color(uint8_t bg, uint8_t fg) {
  return bg << 4 | fg;
}


optional<uint8_t> VGARegister::state() {
  return _state;
}

optional<uint8_t> BasicRegister::read() {
  auto value = in8(_read_port);
  //TODO check for failure
  _state = value;
  return value;
}

void BasicRegister::write(uint8_t value) {
  out8(_write_port, value);
  //TODO check for failure
  //read back again to see if they are equal
}

optional<uint8_t> ExtendedRegister::read() {
  uint8_t value{};
  if (mode() == ExtendedRegister::Mode::Color) {
    value = in8(_read_port_color);
  } else {
    value = in8(_read_port_mono);
  }

  _state = value;
  return value;
}

void ExtendedRegister::write(uint8_t value) {
  if (mode() == ExtendedRegister::Mode::Color) {
    out8(_write_port_color, value);
  } else {
    out8(_write_port_mono, value);
  }
}

optional<uint8_t> IndexedRegister::read() {
  out8(_address_port, _index);
  auto value = in8(_data_port);
  _state = value;
  return value;
}

void IndexedRegister::write(uint8_t value) {
  out8(_address_port, _index);
  out8(_data_port, value);
}

uint8_t IndexedRegister::index() {
  return _index;
}

void IndexedRegister::setIndex(uint8_t index) {
  _index = index;
}

optional<uint8_t> CRTRegister::read() {
 //Read Miscellaneous Output Register and mask with 1 to get
 //I/O Access field, which dictates which pair to use
 auto io_addr_sel = in8(MISC_OUT_REG) & 0x1;
 uint8_t value{};
 if (io_addr_sel == 0) {
   out8(_pair0.addressPort, _index);
   value = in8(_pair0.dataPort);
 } else {
   out8(_pair1.addressPort, _index);
   value =  in8(_pair1.dataPort);
 }
  _state = value;
  return value;
}

void CRTRegister::write(uint8_t value) {
 //Read Miscellaneous Output Register and mask with 1 to get
 //I/O Access field, which dictates which pair to use
 auto io_addr_sel = in8(MISC_OUT_REG) & 0x1;
 if (io_addr_sel == 0) {
   out8(_pair0.addressPort, _index);
   out8(_pair0.dataPort, value);
 } else {
   out8(_pair1.addressPort, _index);
   out8(_pair1.dataPort, value);
 }
}

uint8_t CRTRegister::index() {
  return _index;
}

void CRTRegister::setIndex(uint8_t index) {
  _index = index;
}

optional<uint8_t> AttributeRegister::read() {
  uint8_t value{};
  //read InputStatus #1 to reset flip-flop
  in8(ATTR_FLIP_FLOP);

  out8(_address_port, _index);
  value = in8(_data_port);

  _state = value;
  return value;
}

void AttributeRegister::write(uint8_t value) {

  //read InputStatus #1 to reset flip-flop
  in8(ATTR_FLIP_FLOP);

  //The data port is only available for reading, writing to dataPort is done
  //via address port
  out8(_address_port, _index);
  out8(_address_port, value);
}

}; //Namespace kernel end
