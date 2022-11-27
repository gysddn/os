#include "vga.h"
#include "serial.h"
#include "util.h"
#include "util/optional.h"
#include <string.h>
#include <stdint.h>

namespace kernel {

 extern "C" [[noreturn]] void kernel_main(void) {
  vga_init();
  uint8_t color = attr_make_color(VGA_BIOS_COLOR_LIGHT_BLUE, VGA_BIOS_COLOR_WHITE_HI);
  vga_putchar('H', color, 0, 0);
  vga_putchar('e', color, 1, 0);
  vga_putchar('l', color, 2, 0);
  vga_putchar('l', color, 3, 0);
  vga_putchar('o', color, 4, 0);

  SerialIO::write("This is a string");
  SerialIO::endl();
  SerialIO::write((uint32_t)0xFFFFFFFF, 16);
  SerialIO::endl();
  int test_int = 0xed;
  SerialIO::write((void*)&test_int);
  SerialIO::endl();

  optional<uint32_t> val{32};
  if (val) {
    SerialIO::write("optional value is: ");
    SerialIO::write(val.value(), 10);
    SerialIO::endl();
  }

  optional<uint32_t> val2{};
  val2 = 5;
  if (val2) {
    SerialIO::write("optional value 2 is: ");
    SerialIO::write(val2.value(), 10);
    SerialIO::endl();
  }

  while(true);
}

} //End namespace kernel
