#include "error/error.hpp"
#include "module/module.hpp"

int main(int argc, char **argv)
{
  try
  {
    if (argc < 2) throw Error("Invalid usage.");

    Module module(argv[1]);
    module.visit(module.input);

    module.gen_ll();
    module.gen_obj();
  }
  catch (Error e)
  {
    if (e.row && e.col)
    {
      std::cerr << argv[1] << ":" << e.row << ":" << e.col << ": ";
    }
    else
    {
      std::cerr << argv[0] << ": ";
    }
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
