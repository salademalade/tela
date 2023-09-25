#include "binary-ast-node.hpp"

BinaryASTNode::BinaryASTNode(Type type, ASTNode *left, ASTNode *right) : ASTNode(type), left(left), right(right) {}

BinaryASTNode::~BinaryASTNode()
{
  delete left;
  delete right;
}
