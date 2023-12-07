#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#include <string.h>
#include <stdlib.h>

DLLEXPORT int length(char *str)
{
  return strlen(str);
}

DLLEXPORT char *concat(char *str1, char *str2)
{
  char *out = malloc(strlen(str1) + strlen(str2) + 1);
  strcat(out, str1);
  strcat(out, str2);
  return out;
}
