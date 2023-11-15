#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
#include "error/error.hpp"
#include "token/token.hpp"
#include "lexer/lexer.hpp"
#include "ast-node/ast-node.hpp"
#include "parser/parser.hpp"
#include "ir/ir-visitor.hpp"

int main(int argc, char **argv)
{
  try
  {
    if (argc < 2) throw Error("Invalid usage.");

    std::string source;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
      std::stringstream stream;

      file.open(argv[1]);
      stream << file.rdbuf();
      file.close();

      source = stream.str();
    }
    catch(std::ifstream::failure e)
    {
      throw Error("Error while reading file %s: %s", argv[1], e.what());
    }

    Lexer lexer(source);
    std::vector<Token> toks = lexer.tokenize();

    Parser parser(toks);
    ASTNode *node = parser.parse();

    IRVisitor ir_visitor(argv[1]);
    ir_visitor.visit(node);

    ir_visitor.module->print(llvm::errs(), nullptr);
  }
  catch (Error e)
  {
    if (e.row && e.col)
    {
      std::cerr << argv[1] << ":" << e.row << ":" << e.col << ":";
    }
    else
    {
      std::cerr << argv[0] << ":";
    }
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
