#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cstdio>

class Error {
  const char *msg;

  static const char *msg_sprintf(const char *format, std::va_list args);

  public:
  const char *filename = nullptr;
  unsigned int row = 0;
  unsigned int col = 0;

  Error(const char *format, ...);
  Error(const char *filename, unsigned int row, unsigned int col, const char *format, ...);
  ~Error();

  const char *what();
};

#endif
