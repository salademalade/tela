#include "ast-node.hpp"

ASTNode::ASTNode(Type type, unsigned int row, unsigned int col)
{
  this->type = type;
  this->row = row;
  this->col = col;
}

ASTNode::~ASTNode() {}
