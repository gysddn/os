/*
 * Error Type Implementation
 * @gysddn <gysddn@gmail.com>
 *
 */

#pragma once

namespace kernel { //Begin namespace kernel

class error {
public:

  error() = default;
  error(char* msg) : _msg(msg) {}

  const char* message() {
    return _msg;
  }

  void setMessage(char* msg) {
    _msg = msg;
  }

private:
  char* _msg;
};

} //End namespace kernel

