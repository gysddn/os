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

  SerialIO::write("info is valid: ");
  SerialIO::write(fr_valid, 10);
  SerialIO::endl();

  SerialIO::write("height is: ");
  SerialIO::write(height, 10);
  SerialIO::endl();

  SerialIO::write("width is: ");
  SerialIO::write(width, 10);
  SerialIO::endl();

  SerialIO::write("pitch is: ");
  SerialIO::write(boot_info->framebuffer_pitch, 10);
  SerialIO::endl();

  SerialIO::write("depth is: ");
  SerialIO::write(boot_info->framebuffer_bpp, 10);
  SerialIO::endl();

  uint8_t* frame_buffer = (uint8_t*)boot_info->framebuffer_addr;
  SerialIO::write("buffer addr is: ");
  SerialIO::write(frame_buffer);
  SerialIO::endl();

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
