#include "vga.h"

void kernel_main(void) {
  vga_init();
  u8 color = attr_make_color(VGA_BIOS_COLOR_LIGHT_BLUE, VGA_BIOS_COLOR_WHITE_HI);
  vga_putchar('H', color, 0, 0);
  vga_putchar('e', color, 1, 0);
  vga_putchar('l', color, 2, 0);
  vga_putchar('l', color, 3, 0);
  vga_putchar('o', color, 4, 0);
}
