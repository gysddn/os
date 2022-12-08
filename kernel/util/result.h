/*
 * Result Type Implementation
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

#include "error.h"
#include "optional.h"

namespace kernel { //Begin namespace kernel

template<typename T>
class result {
public:

  result() = delete;

  result(T res) {
    _value = res;
    _error = none;
  }

  result(error err) {
    _value = none;
    _error = err;
  }

  optional<T> value() {
    return _value;
  }

  void setValue(T val) {
    _value = val;
  }

  optional<error> err() {
    return _error;
  }

  void setErr(error err) {
    _error = err;
  }

  operator bool() {
    return _error;
  }

private:
  optional<T> _value;
  optional<error> _error;
};

} //End namespace kernel
