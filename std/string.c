#include <string.h>
#include <stdlib.h>

int length(char *str)
{
  return strlen(str);
}

char *concat(char *str1, char *str2)
{
  char *out = calloc(strlen(str1) + strlen(str2) + 1, sizeof(char));
  strcat(out, str1);
  strcat(out, str2);
  return out;
}
