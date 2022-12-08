#include "../boot/multiboot.h"

#include "vga.h"
#include "serial.h"
#include "util.h"
#include "util/optional.h"
#include "gui/window.h"

#include <string.h>
#include <stdint.h>

namespace kernel {

extern "C" [[noreturn]] void kernel_main(void) {
  unsigned int addr;
  asm volatile("mov %%ebx, %0": "=r" (addr));
  multiboot_info_t *boot_info = (multiboot_info_t*)addr;

  SerialIO io;

  uint32_t flags = boot_info->flags;
  uint32_t fr_valid = (flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) >> 12;

  if (fr_valid)
    io << "[INFO] Framebuffer is valid. Initializing display...\n";

  Window window{
    (uint8_t*)boot_info->framebuffer_addr,
    boot_info->framebuffer_pitch,
    boot_info->framebuffer_width,
    boot_info->framebuffer_height,
    boot_info->framebuffer_bpp
  };


  while(true);
}

} //End namespace kernel
