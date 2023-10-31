#include "unary-ast-node.hpp"

UnaryASTNode::UnaryASTNode(Type type, ASTNode *child) : ASTNode(type)
{
  this->child = child;
}

UnaryASTNode::~UnaryASTNode()
{
  delete child;
}
