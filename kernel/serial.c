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

//Debug
void* ptr = (void*)0x1000000;

void print_debug(char * str) {
  while (*str != 0x00) {
    out8(COM1, (u8)*str++);
  }
}

struct conv_flags {
  bool left_just;
  bool sign;
  bool space_pre;
  bool alternate;
  bool lead_zeros;
};

enum length_modifier {
  LEN_MOD_CHAR,
  LEN_MOD_SHORT_INT,
  LEN_MOD_LONG_INT,
  LEN_MOD_LONG_LONG,
  LEN_MOD_INT_MAX,
  LEN_MOD_SIZE_T,
  LEN_MOD_PTR_DIFF,
  LEN_MOD_LONG_DOUBLE,
};

enum conv_specifier {
  CONV_SPE_SIGN_DEC,
  CONV_SPE_USIGN_DEC,
  CONV_SPE_USIGN_HEX_L,
  CONV_SPE_USIGN_HEX_U,
  CONV_SPE_PTR,
  CONV_SPE_SPEC_CHAR,
};

int _str2int(const char* str){
    int num = 0;
    int i = 0;
    bool isNegetive = false;
    if(str[i] == '-'){
        isNegetive = true;
        i++;
    }
    while (str[i] && (str[i] >= '0' && str[i] <= '9')){
        num = num * 10 + (str[i] - '0');
        i++;
    }
    if(isNegetive) num = -1 * num;
    return num;
}



static const char *_parse_conv_flags(const char * conv_ptr, struct conv_flags *flags)
{
  //Check for flags
  switch (*conv_ptr)
  {
    case '\0':
      return conv_ptr;
    case '-':
      flags->left_just = true;
      break;
    case '+':
      flags->sign = true;
      break;
    case ' ':
      flags->space_pre = true;
      break;
    case '#':
      flags->alternate = true;
      break;
    case '0':
      flags->lead_zeros = true;
      break;
    default:
      return conv_ptr;
  }
  return ++conv_ptr;
}


static const char *_parse_conv_field(const char * conv_ptr, uint32_t *field_width)
{
  while (*conv_ptr != 0x00 && *conv_ptr >= '0' && *conv_ptr <= '9') {
    *field_width = *field_width * 10 + (uint32_t)(*conv_ptr++ - '0');
    /* *(char*)ptr++ =  *field_width; */
  }
  return conv_ptr;
}

static const char *_parse_conv_precision(const char * conv_ptr, uint32_t *precision)
{
  if (*conv_ptr && *conv_ptr != '.') {
    return conv_ptr;
  }

  //Ignore the minus
  if (*++conv_ptr == '-') {
    conv_ptr++;
  }

  while (*conv_ptr != 0x00 && *conv_ptr >= '0' && *conv_ptr <= '9') {
    *precision = *precision * 10 + (*conv_ptr++ - '0');
  }

  return conv_ptr;
}

static const char *_parse_conv_len_mod(const char * conv_ptr, enum length_modifier *len_modifier)
{
  if (*conv_ptr) {
    switch (*conv_ptr) {
      case 'j':
        *len_modifier = LEN_MOD_INT_MAX;
        conv_ptr++;
        break;
      case 'z':
        *len_modifier = LEN_MOD_SIZE_T;
        conv_ptr++;
        break;
      case 't':
        *len_modifier = LEN_MOD_PTR_DIFF;;
        conv_ptr++;
        break;
      case 'L':
        *len_modifier = LEN_MOD_LONG_DOUBLE;;
        conv_ptr++;
        break;
      case 'h':
        if ( *(conv_ptr+1) ) {
          if ( *(conv_ptr+1) == 'h') {
            *len_modifier = LEN_MOD_SHORT_INT;
            conv_ptr += 2;
          } else {
            *len_modifier = LEN_MOD_CHAR;;
            conv_ptr++;
          }
        }
        break;
      case 'l':
        if ( *(conv_ptr+1) ) {
          if ( *(conv_ptr+1) == 'l') {
            *len_modifier = LEN_MOD_LONG_LONG;
            conv_ptr += 2;
          } else {
            *len_modifier = LEN_MOD_LONG_INT;;
            conv_ptr++;
          }
        }
        break;
    }
  }
  return conv_ptr;
}

static const char *_parse_conv_conv_sp(const char * conv_ptr, enum conv_specifier *conv_sp)
{
  if (*conv_ptr) {
    switch (*conv_ptr) {
      case 'd':
        *conv_sp = CONV_SPE_SIGN_DEC;
        conv_ptr++;
        break;
      case 'u':
        *conv_sp = CONV_SPE_USIGN_DEC;
        conv_ptr++;
        break;
      case 'x':
        *conv_sp = CONV_SPE_USIGN_HEX_L;
        conv_ptr++;
        break;
      case 'X':
        *conv_sp = CONV_SPE_USIGN_HEX_U;
        conv_ptr++;
      case 'p':
        *conv_sp = CONV_SPE_PTR;
        conv_ptr++;
        break;
      case '%':
        *conv_sp = CONV_SPE_SPEC_CHAR;
        conv_ptr++;
        break;
    }
  }
  return conv_ptr;

}

static const char *_parse_printf_conv(const char * conv)
{
  struct conv_flags flags = {};
  uint32_t field_width = 0, precision = 0;
  enum length_modifier len_modifier;
  enum conv_specifier conv_sp;

  conv = _parse_conv_flags(conv, &flags);
  conv = _parse_conv_field(conv, &field_width);
  conv = _parse_conv_precision(conv, &precision);
  conv = _parse_conv_len_mod(conv, &len_modifier);
  conv = _parse_conv_conv_sp(conv, &conv_sp);

  //Debug
  /* *(uint32_t*)(ptr) = field_width; */
  /* *(uint32_t*)(ptr+sizeof(uint32_t)) = precision; */
  /* if (len_modifier  == LEN_MOD_LONG_LONG) { */
  /* *(uint32_t*)(ptr+2*sizeof(uint32_t)) = 0x10; */
  /* } */
  /* if (conv_sp  == CONV_SPE_PTR) { */
  /* *(uint32_t*)(ptr+3*sizeof(uint32_t)) = 0xff; */
  /* } */

  return conv;
}

int kprintf(const char * format)
{
  while (*format != 0x00) {
    if (*format == '%') {
      format = _parse_printf_conv(++format);
    } else
      out8(COM1, (uint8_t)*format++);
  }

  return 0;
}
