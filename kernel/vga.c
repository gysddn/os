/*
 * VGA Controller
 * @gysddn <gysddn@gmail.com>
 *
 */

#include "vga.h"

/*
 * REGISTER MANAGEMENT
 */

typedef struct {
  uint16_t readPort;
  uint16_t writePort;
} BasicRegister;

typedef struct {
  uint16_t readPortMono;
  uint16_t readPortColor;
  uint16_t writePortMono;
  uint16_t writePortColor;
} ExtendedRegister;

typedef struct {
  uint16_t addressPort;
  uint16_t dataPort;
  uint8_t index;
} IndexedRegister;

typedef struct {
  struct {
    uint16_t addressPort;
    uint16_t dataPort;
  } pair0;
  struct {
    uint16_t addressPort;
    uint16_t dataPort;
  } pair1;
  uint8_t index;
} CRTRegister;

/*
 * AttributeRegister has the same fields as IndexedRegister
 * but AttributeRegister has a more confusing way of using them
 * so they will be processed differently.
 */
typedef IndexedRegister AttributeRegister;


static inline uint8_t _read_vga_port(const uint16_t port) {
  uint8_t value;
  __asm__("mov %1, %%dx\n"
          "in %%dx, %%al\n"
          "movb %%al, %0\n"
          : "=r" (value)
          : "r" (port)
          : "%eax", "edx"
          );
  return value;
}

/*
 * This function *just* writes to an I/O port, there can be no say as to whether
 * the operation was successful or not. Therefore it should not be used on its own
 * but in a wrapper where this sort of mechanism takes place.
 */
static void _write_vga_port(const uint16_t port, const uint8_t value) {
  __asm__("mov %1, %%dx\n"
          "movb %0, %%al\n"
          "out %%al, %%dx\n"
          :
          :"r" (value),  "r" (port)
          : "%eax", "edx"
          );
}

static uint8_t _write_vga_port_safe(const uint16_t port, const uint8_t value) {
  _write_vga_port(port, value);

  if (_read_vga_port(port) == value) {
    return 0;
  } else {
    return 1;
  }
}

inline static uint8_t _read_reg_basic(const BasicRegister reg) {
  return _read_vga_port(reg.readPort);
}

inline static uint8_t _write_reg_basic(const BasicRegister reg, const uint8_t value) {
  return _write_vga_port_safe(reg.readPort, value);
}

inline static uint8_t _read_reg_ext_mono(const ExtendedRegister reg) {
  return _read_vga_port(reg.readPortMono);
}

inline static uint8_t _read_reg_ext_color(const ExtendedRegister reg) {
  return _read_vga_port(reg.readPortColor);
}

inline static uint8_t _write_reg_ext_mono(const ExtendedRegister reg, const uint8_t value) {
  return _write_vga_port_safe(reg.writePortMono, value);
}

inline static uint8_t _write_reg_ext_color(const ExtendedRegister reg, const uint8_t value) {
  return _write_vga_port_safe(reg.writePortColor, value);
}

inline static uint8_t _read_reg_idx(const IndexedRegister reg) {
  _write_vga_port_safe(reg.addressPort, reg.index);
  return _read_vga_port(reg.dataPort);
}

inline static uint8_t _write_reg_idx(const IndexedRegister reg, const uint8_t value) {
  _write_vga_port_safe(reg.addressPort, reg.index);
  return _write_vga_port_safe(reg.dataPort, value);
}

//TODO Disable "CRTC Registers Protect Enable" field because the kernel will not allow
//any access to VGA other than the interface provided by vga.h
static uint8_t _read_reg_crt(const CRTRegister reg) {
  //Read Miscellaneous Output Register and mask with 1 to get
  //I/O Access field, which dictates which pair to use
  uint8_t io_addr_sel = _read_vga_port(0x3CC) & 0x1;
  if (io_addr_sel == 0) {
    _write_vga_port_safe(reg.pair0.addressPort, reg.index);
    return _read_vga_port(reg.pair0.dataPort);
  } else {
    _write_vga_port_safe(reg.pair1.addressPort, reg.index);
    return _read_vga_port(reg.pair1.dataPort);
  }
}

static uint8_t _write_reg_crt(const CRTRegister reg, const uint8_t value) {
  //Read Miscellaneous Output Register and mask with 1 to get
  //I/O Access field, which dictates which pair to use
  uint8_t io_addr_sel = _read_vga_port(0x3CC) & 0x1;
  if (io_addr_sel == 0) {
    _write_vga_port_safe(reg.pair0.addressPort, reg.index);
    return _write_vga_port_safe(reg.pair0.dataPort, value);
  } else {
    _write_vga_port_safe(reg.pair1.addressPort, reg.index);
    return _write_vga_port_safe(reg.pair1.dataPort, value);
  }
}

//AttributeRegister uses the same port for writes to address and data ports, with a flip-flop
//inside the card holding the state. There is no standard way to access it but it is reset 
//by reading the external register InputStatus #1. TODO handle interrupts here.
static uint8_t _read_reg_atr(const AttributeRegister reg) {
  //read InputStatus #1 to reset flip-flop
  _read_vga_port(0x3BA);

  _write_vga_port_safe(reg.addressPort, reg.index);
  return _read_vga_port(reg.dataPort);
}

static uint8_t _write_reg_atr(const AttributeRegister reg, const uint8_t value) {
  //read InputStatus #1 to reset flip-flop
  _read_vga_port(0x3BA);

  _write_vga_port_safe(reg.addressPort, reg.index);
  //The .dataPort is only available for reading, writing to dataPort is done
  //via .addressPort
  return _write_vga_port_safe(reg.addressPort, value);
}
/*
 * The external registers have different ports to read from and write to, for that reason
 * this separate safe function is added.
 */
static uint8_t _write_ext_vga_port(const uint16_t write_port, const uint16_t read_port, const uint8_t value) {
  _write_vga_port(write_port, value);

  if (_read_vga_port(read_port) == value) {
    return 0;
  } else {
    return 1;
  }
}

static inline uint8_t _read_ext_vga_feature(const uint16_t port, const uint8_t offset, const uint8_t length) {
  /*
   * @offset is 0-7 inclusive: start position of first bit
   */
  return _read_vga_port(port) >> offset & ((1 << length)-1);
}

static inline uint8_t _write_ext_vga_feature(const uint16_t write_port, const uint16_t read_port, const uint8_t offset, const uint8_t length, uint8_t value) {
  /*
   * @offset is 0-7 inclusive: start position of first bit
   * @value should be alligned to the lowest order bit. Bits extending length will be ignored.
   */
  uint8_t mask = (1 << length)-1;
  value = (value & mask) << offset;
  uint8_t reg_value = (_read_vga_port(read_port) & ( ~(mask << offset) )) | value;
  return _write_ext_vga_port(write_port, read_port, reg_value);
}

uint16_t* alp_num_mem = (uint16_t*)0xB8000;

void vga_init(void) {
  vga_flush();
}

void vga_flush(void) {
  for(uint8_t w = 0; w < 80; w++)
    for(uint8_t h = 0; h < 25; h++)
      alp_num_mem[h*80 + w] = 0;
}

void vga_putchar(uint8_t character, uint8_t attr, uint8_t position_w, uint8_t position_h) {
  if (position_w < 80)
    if (position_h < 25)
      alp_num_mem[position_h*80 + position_w] = (uint16_t)character | (uint16_t) attr << 8;
}

inline uint8_t attr_make_color(uint8_t bg, uint8_t fg) {
  return bg << 4 | fg;
}
