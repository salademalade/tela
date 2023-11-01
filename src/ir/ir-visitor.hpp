#ifndef IR_VISITOR_HPP
#define IR_VISITOR_HPP

#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <memory>
#include <string>
#include <map>
#include "error/error.hpp"
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf/leaf-ast-node.hpp"
#include "ast-node/binary/binary-ast-node.hpp"
#include "ast-node/unary/unary-ast-node.hpp"
#include "ast-node/func-def/func-def-ast-node.hpp"
#include "ast-node/func-call/func-call-ast-node.hpp"
#include "ast-node/stmt-seq/stmt-seq-ast-node.hpp"
#include "ast-node/null/null-ast-node.hpp"

class IRVisitor
{
public:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::map<std::string, llvm::Value *> sym_table;

  IRVisitor(std::string mod_name);

  llvm::Value *visit(ASTNode *node);
private:
  llvm::Value *visit_binary(BinaryASTNode *node);
  llvm::Value *visit_fdef(FuncDefASTNode *node);
  llvm::Value *visit_fcall(FuncCallASTNode *node);
};

#endif
