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
  case NodeType::N_CHAR:
    return llvm::ConstantInt::get(*context, llvm::APInt(8, (int)static_cast<LeafASTNode *>(node)->value[0]));
  case NodeType::N_STRING:
    return builder->CreateGlobalStringPtr(llvm::StringRef(static_cast<LeafASTNode *>(node)->value));
  case NodeType::N_ID:
    return visit_identifier(static_cast<LeafASTNode *>(node));
  case NodeType::N_ADD:
  case NodeType::N_SUB:
  case NodeType::N_MUL:
  case NodeType::N_DIV:
    return visit_binary(static_cast<BinaryASTNode *>(node));
  case NodeType::N_POS:
  case NodeType::N_NEG:
    return visit_unary(static_cast<UnaryASTNode *>(node));
  case NodeType::N_DECL:
  case NodeType::N_DECL_CONST:
    return visit_decl(static_cast<UnaryASTNode *>(node));
  case NodeType::N_ASSIGN:
    return visit_assignment(static_cast<BinaryASTNode *>(node));
  case NodeType::N_FUNC_DEF:
    visit_fdef(static_cast<FuncDefASTNode *>(node));
    return nullptr;
  case NodeType::N_FUNC_CALL:
    return visit_fcall(static_cast<FuncCallASTNode *>(node));
  case NodeType::N_RET:
    return visit_ret(static_cast<UnaryASTNode *>(node));
  case NodeType::N_STMT_SEQ:
    visit_seq(static_cast<StmtSeqASTNode *>(node));
    return nullptr;
  case NodeType::N_TYPE:
  case NodeType::N_TYPE_DECL:
  case NodeType::N_NULL:
  default:
    return nullptr;
  }
}

llvm::Value *IRVisitor::visit_identifier(LeafASTNode *node)
{
  llvm::AllocaInst *alloca = sym_table[node->value];
  if (!alloca) throw Error(node->row, node->col, "Undefined reference to variable: %s.", node->value.c_str());

  return builder->CreateLoad(alloca->getAllocatedType(), alloca, node->value.c_str());
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

llvm::Value *IRVisitor::visit_unary(UnaryASTNode *node)
{
  llvm::Value *zero = llvm::ConstantInt::get(*context, llvm::APInt(32, 0));
  llvm::Value *child = visit(node->child);

  if (!child) return nullptr;

  bool int_op = false;
  if (child->getType()->isIntegerTy()) int_op = true;

  switch (node->type)
  {
  case NodeType::N_POS:
    if (int_op) return builder->CreateAdd(zero, child, "postmp");
    else return builder->CreateFAdd(zero, child, "fpostmp");
  case NodeType::N_NEG:
    if (int_op) return builder->CreateSub(zero, child, "negtmp");
    else return builder->CreateFSub(zero, child, "fnegtmp");
  default:
    return nullptr;
  }
}

llvm::Value *IRVisitor::visit_decl(UnaryASTNode *node)
{
  std::string name;
  llvm::Type *type;
  if (node->child->type == NodeType::N_ASSIGN)
  {
    if (static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_ID)
    {
      name = static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value;
    }
    else if (static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_TYPE_DECL)
    {
      name = static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->left)->value;
    }

    sym_table[name] = nullptr;
    return visit(node->child);
  }
  else if (node->child->type == NodeType::N_TYPE_DECL)
  {
    BinaryASTNode *child = static_cast<BinaryASTNode *>(node->child);
    name = static_cast<LeafASTNode *>(child->left)->value;
    type = get_type(static_cast<LeafASTNode *>(child->right));

    sym_table[name] = builder->CreateAlloca(type, nullptr, name);
    return sym_table[name];
  }
  else throw Error(node->row, node->col, "Cannot declare untyped variable.");
}

llvm::Value *IRVisitor::visit_assignment(BinaryASTNode *node)
{
  std::string name;
  llvm::Type *type;
  llvm::Value *right = visit(node->right);
  llvm::AllocaInst *alloca;

  if (node->left->type == NodeType::N_ID)
  {
    name = static_cast<LeafASTNode *>(node->left)->value;
    type = right->getType();
  }
  else if (node->left->type == NodeType::N_TYPE_DECL)
  {
    name = static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value;
    type = get_type(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right));
  }

  if (sym_table.find(name) == sym_table.end()) throw Error(node->row, node->col, "Undefined reference to variable: %s.", name.c_str());

  alloca = sym_table[name];
  if (alloca == nullptr)
  {
    alloca = builder->CreateAlloca(type, nullptr, name);
    sym_table[name] = alloca;
  }

  builder->CreateStore(right, alloca);

  return alloca;
}

llvm::Value *IRVisitor::visit_fdef(FuncDefASTNode *node)
{
  llvm::Function *func = module->getFunction(static_cast<LeafASTNode *>(node->name)->value);

  if (!func) func = create_fproto(node);

  if (node->body->type != NodeType::N_NULL)
  {
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", func);
    builder->SetInsertPoint(block);

    sym_table.clear();
    for (auto &arg : func->args())
    {
      llvm::AllocaInst *alloca = builder->CreateAlloca(arg.getType(), nullptr, arg.getName());
      builder->CreateStore(&arg, alloca);
      sym_table[std::string(arg.getName())] = alloca;
    }

    visit(node->body);
  }

  llvm::verifyFunction(*func);

  return func;
}

llvm::Value *IRVisitor::visit_fcall(FuncCallASTNode *node)
{
  std::string f_name = static_cast<LeafASTNode *>(node->name)->value;
  llvm::Function *callee = module->getFunction(f_name);
  if (!callee) throw Error(node->row, node->col, "Undefined reference to function: %s.", f_name.c_str());

  if (callee->arg_size() != node->args.size()) throw Error(node->row, node->col, "Invalid arguments for function: %s", f_name.c_str());

  std::vector<llvm::Value *> args;
  for (auto i : node->args)
  {
    args.push_back(visit(i));
    if (!args.back()) return nullptr;
  }

  return builder->CreateCall(callee, args, "calltmp");
}

llvm::Value *IRVisitor::visit_ret(UnaryASTNode *node)
{
  llvm::Value *child = visit(node->child);
  return builder->CreateRet(child);
}

void IRVisitor::visit_seq(StmtSeqASTNode *node)
{
  for (auto i : node->statements)
  {
    visit(i);
  }
}

llvm::Function *IRVisitor::create_fproto(FuncDefASTNode *node)
{
  std::string f_name = static_cast<LeafASTNode *>(node->name)->value;

  std::vector<llvm::Type *> arg_types;
  for (auto i : node->args)
  {
    arg_types.push_back(get_type(static_cast<LeafASTNode *>(i->right)));
  }

  llvm::Type *f_ret_type = get_type(static_cast<LeafASTNode *>(node->ret_type));

  llvm::FunctionType *f_type = llvm::FunctionType::get(f_ret_type, arg_types, false);

  llvm::Function *func = llvm::Function::Create(f_type, llvm::Function::ExternalLinkage, f_name, module.get());

  unsigned int i = 0;
  for (auto &arg : func->args())
  {
    arg.setName(static_cast<LeafASTNode *>(node->args[i++]->left)->value);
  }

  return func;
}

llvm::Type *IRVisitor::get_type(LeafASTNode *node)
{
  if (node->value == "int") return llvm::Type::getInt32Ty(*context);
  else if (node->value == "float") return llvm::Type::getFloatTy(*context);
  else if (node->value == "char") return llvm::Type::getInt8Ty(*context);
  else if (node->value == "string") return llvm::Type::getInt8PtrTy(*context);
  else if (node->value == "void") return llvm::Type::getVoidTy(*context);
  else return nullptr;
}
