/*
 * Standard C Library stddef.h header file
 * This code is written in accordance with the C Programming Language
 * Standard defined in ISO/IEC 9899:2017.
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

typedef __PTRDIFF_TYPE__ ptrdiff_t;

typedef __SIZE_TYPE__ size_t;

typedef long double max_align_t;

#define NULL (void*)0

#define offsetof(t, m) __builtin_offsetof(t,m)
