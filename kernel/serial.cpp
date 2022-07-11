/*
 * Serial Interface Source File
 * @gysddn <gysddn@gmail.com>
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "serial.h"
#include "io.h"

namespace kernel {

//Debug
void* ptr = (void*)0x1000000;

void SerialIO::write(const char *str) {
  while (*str != 0x00) {
    out8(SerialIO::COM1, (uint8_t)*str++);
  }
}

void SerialIO::write(uint32_t num, int base) {
  char buf[13];
  prntnum(num, base, (char*)&buf);
  write((char*)&buf);
}

void SerialIO::write(const void *ptr) {
  uint32_t int_ptr = (uint32_t)ptr;

  char buf[11];
  buf[0] = '0';
  buf[1] = 'x';
  buf[10] = 0;
  int j=2, i=9;
  for (int j = 2; j < 10; j++)
    buf[j] = '0';

  do{
      buf[i] = "0123456789ABCDEF"[int_ptr % 16];
      i--;
      int_ptr = int_ptr/16;
  }while( int_ptr > 0);

  write((char*)&buf);
}

void SerialIO::endl() {
  write("\n");
}

void SerialIO::prntnum(unsigned long num, int base, char *outbuf)
{

    int i = 12;
    int j = 0;

    do{
        outbuf[i] = "0123456789ABCDEF"[num % base];
        i--;
        num = num/base;
    }while( num > 0);

    if(num < 0){
        outbuf[0] = '-';
        ++j;
    }

    while( ++i < 13){
       outbuf[j++] = outbuf[i];
    }

    //Terminate it with null
    outbuf[j] = 0;
}


} //End namespace kernel
