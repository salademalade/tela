#include <stdio.h>

void tela_print(char *str)
{
  fprintf(stdout, "%s", str);
}

void tela_pint(int i)
{
  fprintf(stdout, "%i", i);
}

void tela_pfloat(float f)
{
  fprintf(stdout, "%f", f);
}

void tela_pchar(char c)
{
  fprintf(stdout, "%c", c);
}
