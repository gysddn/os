/*
 * GUI Window Header File
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

#include <stdint.h>

namespace kernel {

class Window {

public:
  Window(uint8_t* _addr, uint32_t pitch,
      uint32_t width, uint32_t height, uint8_t bpp);

private:
  uint8_t*  _fb_addr;
  uint32_t  _fb_pitch;
  uint32_t  _fb_width;
  uint32_t  _fb_height;
  uint8_t   _fb_bpp;
};

} //End namespace kernel
