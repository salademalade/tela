#include "func-def-ast-node.hpp"

FuncDefASTNode::FuncDefASTNode(ASTNode *name, unsigned int row, unsigned int col): ASTNode(NodeType::N_FUNC_DEF, row, col)
{
  if (name->type != NodeType::N_ID) throw Error("Invalid function name.");
  this->name = static_cast<LeafASTNode *>(name);
}

FuncDefASTNode::~FuncDefASTNode()
{
  delete name;
  delete body;
}

void FuncDefASTNode::add_arg(BinaryASTNode *node)
{
  args.push_back(node);
}
