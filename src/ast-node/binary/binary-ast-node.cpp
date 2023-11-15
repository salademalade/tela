#include "binary-ast-node.hpp"

BinaryASTNode::BinaryASTNode(Type type, ASTNode *left, ASTNode *right, unsigned int row, unsigned int col) : ASTNode(type, row, col)
{
  this->left = left;
  this->right = right;
}

BinaryASTNode::~BinaryASTNode()
{
  delete left;
  delete right;
}
