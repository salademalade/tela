#include "func-call-ast-node.hpp"

FuncCallASTNode::FuncCallASTNode(ASTNode *name): ASTNode(ASTNode::Type::N_FUNC_CALL)
{
  if (name->type != ASTNode::Type::N_ID) throw Error("Invalid function name.");
  this->name = static_cast<LeafASTNode *>(name);
}

FuncCallASTNode::~FuncCallASTNode()
{
  delete name;
  for (auto i : args) delete i;
}

void FuncCallASTNode::add_arg(ASTNode *arg)
{
  args.push_back(arg);
}
