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
    return llvm::ConstantInt::get(*context, llvm::APInt(32, std::stoi(static_cast<LeafASTNode *>(node)->value)));
  case NodeType::N_FLOAT:
    return llvm::ConstantFP::get(*context, llvm::APFloat(std::stof(static_cast<LeafASTNode *>(node)->value)));
  case NodeType::N_ID:
    break;
  case NodeType::N_ADD:
  case NodeType::N_SUB:
  case NodeType::N_MUL:
  case NodeType::N_DIV:
    return visit_binary(static_cast<BinaryASTNode *>(node));
  case NodeType::N_DECL:
  case NodeType::N_DECL_CONST:
  case NodeType::N_TYPE:
  case NodeType::N_TYPE_DECL:
  case NodeType::N_ASSIGN:
    break;
  case NodeType::N_FUNC_DEF:
    return visit_fdef(static_cast<FuncDefASTNode *>(node));
  case NodeType::N_FUNC_CALL:
    return visit_fcall(static_cast<FuncCallASTNode *>(node));
  case NodeType::N_RET:
  case NodeType::N_STMT_SEQ:
  case NodeType::N_NULL:
  default:
    return nullptr;
  }
}

llvm::Value *IRVisitor::visit_binary(BinaryASTNode *node)
{
  llvm::Value *left = visit(node->left);
  llvm::Value *right = visit(node->right);

  if (!left || !right) return nullptr;

  bool int_op = false;
  if (left->getType()->isIntegerTy() && right->getType()->isIntegerTy()) int_op = true;

  switch (node->type)
  {
  case NodeType::N_ADD:
    if (int_op) return builder->CreateAdd(left, right, "addtmp");
    else return builder->CreateFAdd(left, right, "faddtmp");
  case NodeType::N_SUB:
    if (int_op) return builder->CreateSub(left, right, "subtmp");
    else return builder->CreateFSub(left, right, "fsubtmp");
  case NodeType::N_MUL:
    if (int_op) return builder->CreateMul(left, right, "multmp");
    else return builder->CreateFMul(left, right, "fmultmp");
  case NodeType::N_DIV:
    return builder->CreateFDiv(left, right, "fdivtmp");
  default:
    return nullptr;
  }
}

llvm::Value *IRVisitor::visit_fcall(FuncCallASTNode *node)
{
  std::string f_name = static_cast<LeafASTNode *>(node->name)->value;
  llvm::Function *callee = module->getFunction(f_name);
  if (!callee) throw Error("Undefined reference to function: %s.", f_name);

  if (callee->arg_size() != node->args.size()) throw Error("Invalid arguments for function: %s", f_name);

  std::vector<llvm::Value *> args;
  for (auto i : node->args)
  {
    args.push_back(visit(i));
    if (args.back()) return nullptr;
  }

  return builder->CreateCall(callee, args, "calltmp");
}
