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

/*
 * External Registers
 */


#define EXT_MISCELLANEOUS_OUTPUT_REGISTER_READ     0x3CC
#define EXT_MISCELLANEOUS_OUTPUT_REGISTER_WRITE    0x3C2

#define EXT_FEATURE_CONTROL_REGISTER_READ          0x3CA
#define EXT_FEATURE_CONTROL_REGISTER_WRITE_MONO    0x3BA
#define EXT_FEATURE_CONTROL_REGISTER_WRITE_COLOR   0x3DA

/* READ ONLY */
#define EXT_INPUT_STATUS_ZERO                      0x3C2
#define EXT_INPUT_STATUS_ONE_MONO                  0x3BA
#define EXT_INPUT_STATUS_ONE_COLOR                 0x3DA

#define VSYNCP_OFFSET               7
#define VSYNCP_LENGTH               1
#define HSYNCP_OFFSET               6
#define HSYNCP_LENGTH               1
#define OEPAGE_OFFSET               5
#define OEPAGE_LENGTH               1
#define CLOCK_SEL_OFFSET            2
#define CLOCK_SEL_LENGTH            2
#define RAM_EN_OFFSET               1
#define RAM_EN_LENGTH               1
#define IOEN_OFFSET                 0
#define IOEN_LENGTH                 1

#define FC1_OFFSET                  1
#define FC1_LENGTH                  1
#define FC0_OFFSET                  0
#define FC0_LENGTH                  1

#define SS_OFFSET                   4
#define SS_LENGTH                   1

#define VRETRACE_OFFSET             3
#define VRETRACE_LENGTH             1
#define DD_OFFSET                   0
#define DD_LENGTH                   1

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
