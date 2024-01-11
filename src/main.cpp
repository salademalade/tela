#include <cstdio>
#include "error/error.hpp"

int main(int argc, char **argv)
{
  try
  {
    fprintf(stdout, "Hello world!\n");
  }
  catch (Error& error)
  {
    if (error.filename != nullptr)
    {
      fprintf(stderr, "%s:%d:%d: %s\n", error.filename, error.row, error.col, error.what());
    }
    else
    {
      fprintf(stderr, "%s: %s\n", argv[0], error.what());
    }
  }

  return 0;
}
