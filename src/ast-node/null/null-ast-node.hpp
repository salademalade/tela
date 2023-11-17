#ifndef NULL_AST_NODE_HPP
#define NULL_AST_NODE_HPP

#include "../ast-node.hpp"

class NullASTNode: public ASTNode
{
public:
  NullASTNode(unsigned int row, unsigned int col);
  ~NullASTNode();
};

#endif
