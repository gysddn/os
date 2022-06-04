/*
 * Standard C Library result.h header file
 * This code is !NOT! written in accordance with any C Programming Language Standard
 * This header defines a NEW error/result handling implementation for C Language 
 * used in this project.
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

// This project does not use a thred local lvalue to pass the error information, but rather
// a result value that is capable of representing the status of the function. The 0 indicates
// success without additional information, positive values are reserved to represent the success
// cases with additional information, and negative values are error values that are also defined in
// this header.

typedef int32_t Result;

//Success values
enum {
  RESULT_SUCCESS = 0,
};

//Error values
enum {
  ERROR_GENERIC = -1,
}

//This macro is used to pass the result information up to function stack in case of an error.
#define ASSERT_RESULT(R) \
  if (R < 0) return R;

//Generic Result handling
#define RESULT_IS_SUCCESS(R) (R >= 0)
#define RESULT_IS_ERROR(R) (R < 0)
