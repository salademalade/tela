#ifndef MODULE_HPP
#define MODULE_HPP

#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <memory>
#include <string>
#include <stack>
#include <map>
#include "error/error.hpp"
#include "token/token.hpp"
#include "lexer/lexer.hpp"
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf/leaf-ast-node.hpp"
#include "ast-node/binary/binary-ast-node.hpp"
#include "ast-node/unary/unary-ast-node.hpp"
#include "ast-node/func-def/func-def-ast-node.hpp"
#include "ast-node/func-call/func-call-ast-node.hpp"
#include "ast-node/stmt-seq/stmt-seq-ast-node.hpp"
#include "ast-node/null/null-ast-node.hpp"
#include "parser/parser.hpp"

class Module
{
public:
  struct Symbol
  {
    llvm::Value *value;
    bool is_const;
    bool is_global;

    Symbol(llvm::Value *value = nullptr, bool is_const = false, bool is_global = false);
  };

  ASTNode *input;
  std::string filename;

  llvm::LLVMContext *context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> llvm_module;

  std::map<std::string, Symbol> sym_table;
  std::map<std::string, llvm::FunctionType *> func_table;

  std::stack<llvm::Function *> fdef_stack;

  Module(std::string mod_name);
  Module(llvm::LLVMContext *context, std::string mod_name);

  void gen_ir();

  void gen_ll();
  void gen_obj();
private:
  llvm::Value *visit(ASTNode *node);
  llvm::Value *visit_identifier(LeafASTNode *node);
  llvm::Value *visit_binary(BinaryASTNode *node);
  llvm::Value *visit_unary(UnaryASTNode *node);
  llvm::Value *visit_decl(UnaryASTNode *node);
  llvm::Value *visit_assignment(BinaryASTNode *node);
  llvm::Value *visit_fdef(FuncDefASTNode *node);
  llvm::Value *visit_fcall(FuncCallASTNode *node);
  llvm::Value *visit_ret(UnaryASTNode *node);
  llvm::Value *visit_import(UnaryASTNode *node);
  llvm::Value *visit_seq(StmtSeqASTNode *node);

  llvm::Function *create_fproto(FuncDefASTNode *node);
  llvm::Type *get_type(LeafASTNode *node);
};

#endif
