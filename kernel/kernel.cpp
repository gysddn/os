#include "vga.h"
#include "serial.h"
#include "util.h"
#include <string.h>
#include <stdint.h>

extern "C" void kernel_main(void) {
  vga_init();
  uint8_t color = attr_make_color(VGA_BIOS_COLOR_LIGHT_BLUE, VGA_BIOS_COLOR_WHITE_HI);
  vga_putchar('H', color, 0, 0);
  vga_putchar('e', color, 1, 0);
  vga_putchar('l', color, 2, 0);
  vga_putchar('l', color, 3, 0);
  vga_putchar('o', color, 4, 0);
  const char * message = "Hello\n";
  print_debug(message);

  char buf[10];
  int_to_string(buf, -1234567);
  print_debug(buf);
}