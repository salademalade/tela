#ifndef LEAF_AST_NODE_HPP
#define LEAF_AST_NODE_HPP

#include <string>
#include "../ast-node.hpp"

class LeafASTNode: public ASTNode
{
public:
  std::string value;

  LeafASTNode(Type type, std::string value);
  ~LeafASTNode();
};

#endif
