#include "func-def-ast-node.hpp"

FuncDefASTNode::FuncDefASTNode(ASTNode *name): ASTNode(ASTNode::Type::N_FUNC_DEF)
{
  if (name->type != ASTNode::Type::N_ID) throw Error("Invalid function name.");
  this->name = static_cast<LeafASTNode *>(name);
}

FuncDefASTNode::~FuncDefASTNode()
{
  delete name;
  delete body;
}

void FuncDefASTNode::add_arg(std::string id, ReturnType type)
{
  args[id] = type;
}
