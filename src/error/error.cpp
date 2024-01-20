#include "error.hpp"

const char *Error::msg_sprintf(const char *format, std::va_list args)
{
  char *out = nullptr;
  unsigned int length = 0;
  const char *fmt_copy = format;
  std::va_list copy;
  va_copy(copy, args);

  do {
    if (*fmt_copy == '%') {
      fmt_copy++;

      if (*fmt_copy == 'c') {
        va_arg(args, int);
        length++;
      } else if (*fmt_copy == 's') {
        char *str = va_arg(args, char *);

        length += strlen(str);
      }
    } else {
      length++;
    }

    fmt_copy++;
  } while (*fmt_copy != '\0');

  out = (char *)std::calloc(length, sizeof(char));
  std::vsprintf(out, format, copy);

  return out;
}

Error::Error(const char *format, ...)
{
  std::va_list args;
  va_start(args, format);

  this->msg = this->msg_sprintf(format, args);

  va_end(args);
}

Error::Error(const char *filename, unsigned int row, unsigned int col, const char *format, ...)
{
  this->filename = filename;
  this->row = row;
  this->col = col;

  std::va_list args;
  va_start(args, format);

  this->msg = this->msg_sprintf(format, args);

  va_end(args);
}

Error::~Error() { delete this->msg; }

const char *Error::what() { return this->msg; }
