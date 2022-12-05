#include "../boot/multiboot.h"

#include "vga.h"
#include "serial.h"
#include "util.h"
#include "util/optional.h"
#include <string.h>
#include <stdint.h>

namespace kernel {

extern "C" [[noreturn]] void kernel_main(void) {
  unsigned int addr;
  asm volatile("mov %%ebx, %0": "=r" (addr));
  multiboot_info_t *boot_info = (multiboot_info_t*)addr;
  uint32_t width = boot_info->framebuffer_width;
  uint32_t height = boot_info->framebuffer_height;

  uint32_t flags = boot_info->flags;
  uint32_t fr_valid = (flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) >> 12;

  SerialIO io;

  io << "info is valid: " << fr_valid << "\n";
  io << "height is: " << height << "\n";
  io << "width is: " << width << "\n";
  io << "pitch is: " << boot_info->framebuffer_pitch << "\n";
  io << "depth is: " << boot_info->framebuffer_bpp << "\n";

  uint8_t* frame_buffer = (uint8_t*)boot_info->framebuffer_addr;
  io << "buffer addr is: " << (void*)frame_buffer << "\n";

  io << "decimal: " << 10 << ", and hex: " << SerialIO::Base16 << 0x20 << "\n";

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      frame_buffer[i * boot_info->framebuffer_pitch + j * 4 ] = 80;
      frame_buffer[i * boot_info->framebuffer_pitch + j * 4 + 1] = 80;
      frame_buffer[i * boot_info->framebuffer_pitch + j * 4 + 2] = 200;
    }
  }


  while(true);
}

} //End namespace kernel
