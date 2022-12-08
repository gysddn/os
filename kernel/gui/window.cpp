/*
 * GUI Window Source File
 * @gysddn <gysddn@gmail.com>
 *
 */

#include "window.h"

namespace kernel {

Window::Window(uint8_t* addr, uint32_t pitch,
    uint32_t width, uint32_t height, uint8_t bpp) {
  _fb_addr = addr;
  _fb_pitch = pitch;
  _fb_width = width;
  _fb_height = height;
  _fb_bpp = bpp;

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      _fb_addr[i * _fb_pitch + j * 4 ] = 80;
      _fb_addr[i * _fb_pitch + j * 4 + 1] = 80;
      _fb_addr[i * _fb_pitch + j * 4 + 2] = 200;
    }
  }

}


} //End namespace kernel

