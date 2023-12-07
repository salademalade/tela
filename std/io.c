#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#include <stdio.h>

DLLEXPORT void print(char *str)
{
  fprintf(stdout, "%s", str);
}
