#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdio>
#include <cstdarg>

class Error
{
  char msg[1024];
public:
  Error(const char *format, ...);

  const char *what();
};

#endif
