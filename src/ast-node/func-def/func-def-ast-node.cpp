#include "func-def-ast-node.hpp"

FuncDefASTNode::FuncDefASTNode(ASTNode *name): ASTNode(NodeType::N_FUNC_DEF)
{
  if (name->type != NodeType::N_ID) throw Error("Invalid function name.");
  this->name = static_cast<LeafASTNode *>(name);
}

FuncDefASTNode::~FuncDefASTNode()
{
  delete name;
  delete body;
}

void FuncDefASTNode::add_arg(std::string id, std::string type)
{
  args.push_back(std::pair<std::string, LeafASTNode *>(id, new LeafASTNode(NodeType::N_TYPE, type)));
}
