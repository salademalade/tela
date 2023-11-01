#include "ir-visitor.hpp"

IRVisitor::IRVisitor(std::string mod_name)
{
  this->context = std::make_unique<llvm::LLVMContext>();
  this->builder = std::make_unique<llvm::IRBuilder<>>(*this->context);
  this->module = std::make_unique<llvm::Module>(mod_name, *this->context);
}
