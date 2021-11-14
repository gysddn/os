/*
 * Utility Functions Source File
 * @gysddn <gysddn@gmail.com>
 *
 */
#include "util.h"

void int_to_string(char* buf, int32_t num) {
  if (num < 0) {
    *(buf++) = '-';
    num = -1 * num;
  };

  int32_t temp = num;
  while(temp > 10) {
    buf++;
    temp /= 10;
  }

  while (num != 0) {
    *(buf--) = 0x30 + (num % 10);
    num = num / 10;
  }
}
