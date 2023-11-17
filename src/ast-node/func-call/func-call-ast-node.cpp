#include "func-call-ast-node.hpp"

FuncCallASTNode::FuncCallASTNode(ASTNode *name): ASTNode(NodeType::N_FUNC_CALL, name->row, name->col)
{
  if (name->type != NodeType::N_ID) throw Error("Invalid function name.");
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
