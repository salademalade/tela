#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/ADT/Optional.h>
#include <optional>
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

    const char *sfile_name = ".temp.ll";

    std::error_code s_e_code;
    llvm::raw_fd_ostream irdest(sfile_name, s_e_code, llvm::sys::fs::OpenFlags::OF_None);

    if (s_e_code) throw Error("Could not open file: %s", s_e_code.message().c_str());

    ir_visitor.module->print(irdest, nullptr);
    irdest.flush();

    auto target_triple = llvm::sys::getDefaultTargetTriple();

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string err;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple, err);

    if (!target) throw Error("%s", err.c_str());

    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto target_machine = target->createTargetMachine(target_triple, CPU, features, opt, RM);

    ir_visitor.module->setDataLayout(target_machine->createDataLayout());
    ir_visitor.module->setTargetTriple(target_triple);

    char *ofile_name = argv[1];
    strcat(ofile_name, ".o");

    std::error_code o_e_code;
    llvm::raw_fd_ostream dest(ofile_name, o_e_code, llvm::sys::fs::OpenFlags::OF_None);

    if (o_e_code) throw Error("Could not open file: %s", o_e_code.message().c_str());

    llvm::legacy::PassManager pass;
    auto filetype = llvm::CodeGenFileType::CGFT_ObjectFile;

    if (target_machine->addPassesToEmitFile(pass, dest, nullptr, filetype))
    {
      throw Error("Could not emit object file.");
    }

    pass.run(*ir_visitor.module);
    dest.flush();
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
