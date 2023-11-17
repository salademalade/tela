#include "unary-ast-node.hpp"

UnaryASTNode::UnaryASTNode(Type type, ASTNode *child, unsigned int row, unsigned int col) : ASTNode(type, row, col)
{
  this->child = child;
}

UnaryASTNode::~UnaryASTNode()
{
  delete child;
}
