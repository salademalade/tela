#include "ir-visitor.hpp"

IRVisitor::IRVisitor(std::string mod_name)
{
  this->context = std::make_unique<llvm::LLVMContext>();
  this->builder = std::make_unique<llvm::IRBuilder<>>(*this->context);
  this->module = std::make_unique<llvm::Module>(mod_name, *this->context);
}

llvm::Value *IRVisitor::visit(ASTNode *node)
{
  switch (node->type)
  {
    case NodeType::N_INT:
    case NodeType::N_FLOAT:
    case NodeType::N_ID:
    case NodeType::N_ADD:
    case NodeType::N_SUB:
    case NodeType::N_MUL:
    case NodeType::N_DIV:
    case NodeType::N_ASSIGN:
    case NodeType::N_FUNC_DEF:
    case NodeType::N_FUNC_CALL:
    case NodeType::N_RET:
    case NodeType::N_STMT_SEQ:
    case NodeType::N_NULL:
    default:
      return nullptr;
  }
}
