#include "error.hpp"

Error::Error(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(msg, format, args);
  va_end(args);
}

const char *Error::what() const
{
  return msg;
}
