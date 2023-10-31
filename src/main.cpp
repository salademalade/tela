#include <cstdio>
#include <cstdlib>
#include "error/error.hpp"

int main(int argc, char **argv)
{
  try
  {
    fprintf(stdout, "Hello world!\n");
  }
  catch (Error e)
  {
    fprintf(stderr, "%s", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
