#include "leaf-ast-node.hpp"

LeafASTNode::LeafASTNode(Type type, std::string value, unsigned int row, unsigned int col) : ASTNode(type, row, col)
{
  this->value = value;
}

LeafASTNode::~LeafASTNode() {}
