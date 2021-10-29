/*
 * VGA Controller
 * @gysddn <gysddn@gmail.com>
 *
 */

#include "vga.h"
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
 * REGISTER MANAGEMENT
 */

typedef struct {
  u16 readPort;
  u16 writePort;
} BasicRegister;

typedef struct {
  u16 readPortMono;
  u16 readPortColor;
  u16 writePortMono;
  u16 writePortColor;
} ExtendedRegister;

typedef struct {
  u16 addressPort;
  u16 dataPort;
  u8 index;
} IndexedRegister;

typedef struct {
  struct {
    u16 addressPort;
    u16 dataPort;
  } pair0;
  struct {
    u16 addressPort;
    u16 dataPort;
  } pair1;
  u8 index;
} CRTRegister;

/*
 * AttributeRegister has the same fields as IndexedRegister
 * but AttributeRegister has a more confusing way of using them
 * so they will be processed differently.
 */
typedef IndexedRegister AttributeRegister;


static inline u8 _read_vga_port(const u16 port) {
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

/*
 * This function *just* writes to an I/O port, there can be no say as to whether
 * the operation was successful or not. Therefore it should not be used on its own
 * but in a wrapper where this sort of mechanism takes place.
 */
static void _write_vga_port(const u16 port, const u8 value) {
  __asm__("mov %1, %%dx\n"
          "movb %0, %%al\n"
          "out %%al, %%dx\n"
          :
          :"r" (value),  "r" (port)
          : "%eax", "edx"
          );
}

static u8 _write_vga_port_safe(const u16 port, const u8 value) {
  _write_vga_port(port, value);

  if (_read_vga_port(port) == value) {
    return 0;
  } else {
    return 1;
  }
}

inline static u8 _read_reg_basic(const BasicRegister reg) {
  return _read_vga_port(reg.readPort);
}

inline static u8 _write_reg_basic(const BasicRegister reg, const u8 value) {
  return _write_vga_port_safe(reg.readPort, value);
}

inline static u8 _read_reg_ext_mono(const ExtendedRegister reg) {
  return _read_vga_port(reg.readPortMono);
}

inline static u8 _read_reg_ext_color(const ExtendedRegister reg) {
  return _read_vga_port(reg.readPortColor);
}

inline static u8 _write_reg_ext_mono(const ExtendedRegister reg, const u8 value) {
  return _write_vga_port_safe(reg.writePortMono, value);
}

inline static u8 _write_reg_ext_color(const ExtendedRegister reg, const u8 value) {
  return _write_vga_port_safe(reg.writePortColor, value);
}

inline static u8 _read_reg_idx(const IndexedRegister reg) {
  _write_vga_port_safe(reg.addressPort, reg.index);
  return _read_vga_port(reg.dataPort);
}

inline static u8 _write_reg_idx(const IndexedRegister reg, const u8 value) {
  _write_vga_port_safe(reg.addressPort, reg.index);
  return _write_vga_port_safe(reg.dataPort, value);
}

//TODO Disable "CRTC Registers Protect Enable" field because the kernel will not allow
//any access to VGA other than the interface provided by vga.h
static u8 _read_reg_crt(const CRTRegister reg) {
  //Read Miscellaneous Output Register and mask with 1 to get
  //I/O Access field, which dictates which pair to use
  u8 io_addr_sel = _read_vga_port(0x3CC) & 0x1;
  if (io_addr_sel == 0) {
    _write_vga_port_safe(reg.pair0.addressPort, reg.index);
    return _read_vga_port(reg.pair0.dataPort);
  } else {
    _write_vga_port_safe(reg.pair1.addressPort, reg.index);
    return _read_vga_port(reg.pair1.dataPort);
  }
}

static u8 _write_reg_crt(const CRTRegister reg, const u8 value) {
  //Read Miscellaneous Output Register and mask with 1 to get
  //I/O Access field, which dictates which pair to use
  u8 io_addr_sel = _read_vga_port(0x3CC) & 0x1;
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
static u8 _read_reg_atr(const AttributeRegister reg) {
  //read InputStatus #1 to reset flip-flop
  _read_vga_port(0x3BA);

  _write_vga_port_safe(reg.addressPort, reg.index);
  return _read_vga_port(reg.dataPort);
}

static u8 _write_reg_atr(const AttributeRegister reg, const u8 value) {
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
static u8 _write_ext_vga_port(const u16 write_port, const u16 read_port, const u8 value) {
  _write_vga_port(write_port, value);

  if (_read_vga_port(read_port) == value) {
    return 0;
  } else {
    return 1;
  }
}

static inline u8 _read_ext_vga_feature(const u16 port, const u8 offset, const u8 length) {
  /*
   * @offset is 0-7 inclusive: start position of first bit
   */
  return _read_vga_port(port) >> offset & ((1 << length)-1);
}

static inline u8 _write_ext_vga_feature(const u16 write_port, const u16 read_port, const u8 offset, const u8 length, u8 value) {
  /*
   * @offset is 0-7 inclusive: start position of first bit
   * @value should be alligned to the lowest order bit. Bits extending length will be ignored.
   */
  u8 mask = (1 << length)-1;
  value = (value & mask) << offset;
  u8 reg_value = (_read_vga_port(read_port) & ( ~(mask << offset) )) | value;
  return _write_ext_vga_port(write_port, read_port, reg_value);
}


void vga_main(void) {
}
