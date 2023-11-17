#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdio>
#include <cstdarg>

class Error
{
  char msg[1024];
public:
  unsigned int row = 0;
  unsigned int col = 0;

  Error(const char *format, ...);
  Error(unsigned int row, unsigned int col, const char *format, ...);

  const char *what() const;
};

#endif
