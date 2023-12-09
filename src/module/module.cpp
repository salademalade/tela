#include "module.hpp"

Module::Symbol::Symbol(llvm::Value *value, bool is_const, bool is_global)
{
  this->value = value;
  this->is_const = is_const;
  this->is_global = is_global;
}

Module::Module(std::string filename)
{
  this->filename = filename;
  std::string source;
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    std::stringstream stream;

    file.open(this->filename);
    stream << file.rdbuf();
    file.close();

    source = stream.str();
  }
  catch(std::ifstream::failure e)
  {
    throw Error("Error while reading file %s: %s", this->filename.c_str(), e.what());
  }

  Lexer lexer(source);
  std::vector<Token> toks = lexer.tokenize();

  Parser parser(toks);
  this->input = parser.parse();

  this->context = new llvm::LLVMContext();
  this->builder = std::make_unique<llvm::IRBuilder<>>(*this->context);
  this->llvm_module = std::make_unique<llvm::Module>(this->filename, *this->context);
}

Module::Module(llvm::LLVMContext *context, std::string filename)
{
  this->filename = filename;
  std::string source;
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    std::stringstream stream;

    file.open(this->filename);
    stream << file.rdbuf();
    file.close();

    source = stream.str();
  }
  catch(std::ifstream::failure e)
  {
    throw Error("Error while reading file %s: %s", this->filename.c_str(), e.what());
  }

  Lexer lexer(source);
  std::vector<Token> toks = lexer.tokenize();

  Parser parser(toks);
  this->input = parser.parse();

  this->context = context;
  this->builder = std::make_unique<llvm::IRBuilder<>>(*this->context);
  this->llvm_module = std::make_unique<llvm::Module>(this->filename, *this->context);
}

llvm::Value *Module::visit(ASTNode *node)
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
  case NodeType::N_IMPORT:
    return visit_import(static_cast<UnaryASTNode *>(node));
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

llvm::Value *Module::visit_identifier(LeafASTNode *node)
{
  Symbol var = sym_table[node->value];
  if (!var.value) throw Error(node->row, node->col, "Undefined reference to variable: %s.", node->value.c_str());

  if (var.is_global)
  {
    llvm::GlobalVariable *global = static_cast<llvm::GlobalVariable *>(var.value);
    return builder->CreateLoad(global->getType(), global, node->value.c_str());
  }
  else
  {
    llvm::AllocaInst *alloca = static_cast<llvm::AllocaInst *>(var.value);
    return builder->CreateLoad(alloca->getAllocatedType(), alloca, node->value.c_str());
  }
}

llvm::Value *Module::visit_binary(BinaryASTNode *node)
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

llvm::Value *Module::visit_unary(UnaryASTNode *node)
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

llvm::Value *Module::visit_decl(UnaryASTNode *node)
{
  std::string name;
  llvm::Value *right;
  llvm::Type *type;
  if (node->child->type == NodeType::N_ASSIGN)
  {
    if (static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_ID)
    {
      name = static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value;
      right = visit(static_cast<BinaryASTNode *>(node->child)->right);
      type = right->getType();
    }
    else if (static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_TYPE_DECL)
    {
      name = static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->left)->value;
      right = visit(static_cast<BinaryASTNode *>(node->child)->right);
      type = get_type(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->right));
    }

    bool is_global = fdef_stack.empty();
    bool is_const = node->type == NodeType::N_DECL_CONST;

    Symbol symbol(nullptr, is_const, is_global);

    if (is_global)
    {
      llvm_module->getOrInsertGlobal(name, type);
      llvm::GlobalVariable *global = llvm_module->getNamedGlobal(name);
      llvm::Constant *right_c = static_cast<llvm::Constant *>(right);
      global->setInitializer(right_c);
      symbol.value = global;
    }
    else
    {
      symbol.value = builder->CreateAlloca(type, nullptr, name);
      builder->CreateStore(right, symbol.value);
    }

    sym_table[name] = symbol;
    return sym_table[name].value;
  }
  else if (node->child->type == NodeType::N_TYPE_DECL)
  {
    name = static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value;
    type = get_type(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->right));

    if (fdef_stack.empty()) throw Error(node->row, node->col, "Cannot define global without value.");
    if (node->type == NodeType::N_DECL_CONST) throw Error(node->row, node->col, "Cannot define constant without value.");

    Symbol symbol(builder->CreateAlloca(type, nullptr, name));

    sym_table[name] = symbol;
    return sym_table[name].value;
  }
  else return nullptr;
}

llvm::Value *Module::visit_assignment(BinaryASTNode *node)
{
  std::string name = static_cast<LeafASTNode *>(node->left)->value;
  llvm::Value *right = visit(node->right);
  if (node->left->type != NodeType::N_ID) throw Error(node->row, node->col, "Cannot assign value to expression.");
  if (sym_table.find(name) == sym_table.end()) throw Error(node->row, node->col, "Undefined reference to variable: %s.", name.c_str());

  Symbol symbol = sym_table[name];
  if (symbol.is_const) throw Error(node->row, node->col, "Cannot redefine constant.");
  if (symbol.is_global) return builder->CreateStore(right, static_cast<llvm::GlobalVariable *>(symbol.value));
  else return builder->CreateStore(right, static_cast<llvm::AllocaInst *>(symbol.value));
}

llvm::Value *Module::visit_fdef(FuncDefASTNode *node)
{
  llvm::Function *func = llvm_module->getFunction(static_cast<LeafASTNode *>(node->name)->value);

  if (!func) func = create_fproto(node);

  if (node->body->type != NodeType::N_NULL)
  {
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", func);
    builder->SetInsertPoint(block);

    for (auto sym : sym_table)
    {
      if (!sym.second.is_global) sym_table.erase(sym.first);
    }
    for (auto &arg : func->args())
    {
      llvm::AllocaInst *alloca = builder->CreateAlloca(arg.getType(), nullptr, arg.getName());
      builder->CreateStore(&arg, alloca);
      sym_table[std::string(arg.getName())] = alloca;
    }

    fdef_stack.push(func);
    visit(node->body);
    fdef_stack.pop();
  }

  llvm::verifyFunction(*func);

  return func;
}

llvm::Value *Module::visit_fcall(FuncCallASTNode *node)
{
  std::string f_name = static_cast<LeafASTNode *>(node->name)->value;
  llvm::Function *callee = llvm_module->getFunction(f_name);
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

llvm::Value *Module::visit_ret(UnaryASTNode *node)
{
  llvm::Value *child = visit(node->child);
  return builder->CreateRet(child);
}

llvm::Value *Module::visit_import(UnaryASTNode *node)
{
  std::string filename = static_cast<LeafASTNode *>(static_cast<UnaryASTNode *>(node)->child)->value;
  Module module(filename);
  module.visit(module.input);

  for (auto i = module.func_table.begin(); i != module.func_table.end(); i++)
  {
    llvm::Function::Create(i->second, llvm::Function::ExternalLinkage, i->first, *llvm_module);
    func_table[i->first] = i->second;
  }

  for (auto i = module.sym_table.begin(); i != module.sym_table.end(); i++)
  {
    llvm_module->getOrInsertGlobal(i->first, i->second.value->getType());
    llvm::GlobalVariable *global = llvm_module->getNamedGlobal(i->first);
    global->isExternallyInitialized();
    sym_table[i->first] = Symbol(global, true, true);
  }

  return nullptr;
}

void Module::visit_seq(StmtSeqASTNode *node)
{
  for (auto i : node->statements)
  {
    visit(i);
  }
}

void Module::gen_ll()
{
  std::string out_file = filename + ".ll";

  std::error_code ecode;
  llvm::raw_fd_ostream dest(out_file, ecode, llvm::sys::fs::OpenFlags::OF_None);

  if (ecode) throw Error("Could not open file: %s", ecode.message().c_str());

  llvm_module->print(dest, nullptr);
  dest.flush();
}

void Module::gen_obj()
{
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

  llvm_module->setDataLayout(target_machine->createDataLayout());
  llvm_module->setTargetTriple(target_triple);

  std::string out_file = filename + ".o";

  std::error_code ecode;
  llvm::raw_fd_ostream dest(out_file, ecode, llvm::sys::fs::OpenFlags::OF_None);

  if (ecode) throw Error("Could not open file: %s", ecode.message().c_str());

  llvm::legacy::PassManager pass;
  auto filetype = llvm::CodeGenFileType::CGFT_ObjectFile;

  if (target_machine->addPassesToEmitFile(pass, dest, nullptr, filetype))
  {
    throw Error("Could not emit object file.");
  }

  pass.run(*llvm_module);
  dest.flush();
}

llvm::Function *Module::create_fproto(FuncDefASTNode *node)
{
  std::string f_name = static_cast<LeafASTNode *>(node->name)->value;

  std::vector<llvm::Type *> arg_types;
  for (auto i : node->args)
  {
    arg_types.push_back(get_type(static_cast<LeafASTNode *>(i->right)));
  }

  llvm::Type *f_ret_type = get_type(static_cast<LeafASTNode *>(node->ret_type));

  llvm::FunctionType *f_type = llvm::FunctionType::get(f_ret_type, arg_types, false);

  llvm::Function *func = llvm::Function::Create(f_type, llvm::Function::ExternalLinkage, f_name, llvm_module.get());
  func_table[f_name] = f_type;

  unsigned int i = 0;
  for (auto &arg : func->args())
  {
    arg.setName(static_cast<LeafASTNode *>(node->args[i++]->left)->value);
  }

  return func;
}

llvm::Type *Module::get_type(LeafASTNode *node)
{
  if (node->value == "int") return llvm::Type::getInt32Ty(*context);
  else if (node->value == "float") return llvm::Type::getFloatTy(*context);
  else if (node->value == "char") return llvm::Type::getInt8Ty(*context);
  else if (node->value == "string") return llvm::Type::getInt8PtrTy(*context);
  else if (node->value == "void") return llvm::Type::getVoidTy(*context);
  else return nullptr;
}
