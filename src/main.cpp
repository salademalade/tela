#include <CLI/CLI.hpp>
#include <vector>
#include <string>
#include "error/error.hpp"
#include "module/module.hpp"

int main(int argc, char **argv)
{
  try
  {
    CLI::App app;

    std::string source;
    bool gen_ir = false;
    app.add_option("Source", source, "Source file to compile to compile")->required();
    app.add_flag("-l,--gen-ir", gen_ir, "Generate LLVM IR");

    CLI11_PARSE(app, argc, argv);

    Module module(source);
    module.visit(module.input);
    if (gen_ir) module.gen_ll();
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
