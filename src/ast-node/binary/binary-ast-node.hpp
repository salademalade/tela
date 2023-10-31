#ifndef BINARY_AST_NODE_HPP
#define BINARY_AST_NODE_HPP

#include "../ast-node.hpp"

class BinaryASTNode: public ASTNode
{
public:
  ASTNode *left;
  ASTNode *right;

  BinaryASTNode(Type type, ASTNode *left, ASTNode *right);
  ~BinaryASTNode();
};

#endif
