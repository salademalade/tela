#ifndef UNARY_AST_NODE_HPP
#define UNARY_AST_NODE_HPP

#include "ast-node/ast-node.hpp"

class UnaryASTNode: public ASTNode
{
public:
  ASTNode *child;

  UnaryASTNode(Type type, ASTNode *child);
  ~UnaryASTNode();
};

#endif
