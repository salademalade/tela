#ifndef FUNC_CALL_AST_NODE_HPP
#define FUNC_CALL_AST_NODE_HPP

#include <vector>
#include "error/error.hpp"
#include "../ast-node.hpp"
#include "../leaf/leaf-ast-node.hpp"

class FuncCallASTNode: public ASTNode
{
public:
  LeafASTNode *name;
  std::vector<ASTNode *> args;

  FuncCallASTNode(ASTNode *name, unsigned int row, unsigned int col);
  ~FuncCallASTNode();

  void add_arg(ASTNode *arg);
};

#endif
