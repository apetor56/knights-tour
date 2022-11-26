#include "Exception.hpp"

Exception::Exception(int line, const char *file) noexcept
: _line(line),
  _file(file) {}

const std::string& Exception::getFile() const noexcept {
    return _file;
}

const int& Exception::getLine() const noexcept {
    return _line;
}