/*
 * Optional Type Implementation
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

namespace kernel { //Begin namespace kernel

struct NoneType {};

#define none NoneType()

template<typename T>
class optional {
public:

  optional() {
    _isSet = false;
  }

  optional(T val) {
    _value = val;
    _isSet = true;
  }

  optional(NoneType) {
    _isSet = false;
  }

  void unset() {
    _isSet = false;
  }

  void set(T val) {
    _isSet = true;
    _value = val;
  }

  bool isSet() {
    return _isSet;
  }

  T value() {
    return _value;
  }

  operator bool() {
    return _isSet;
  }

  optional<T>& operator=(T val) {
    _value = val;
    _isSet = true;
    return *this;
  }

  optional<T>& operator=(NoneType) {
    _isSet = false;
    return *this;
  }

private:
  T _value;
  bool _isSet;
};
} //End namespace kernel
