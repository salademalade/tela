#ifndef FUNC_DEF_AST_NODE_HPP
#define FUNC_DEF_AST_NODE_HPP

#include <utility>
#include <vector>
#include <map>
#include "error/error.hpp"
#include "../ast-node.hpp"
#include "../leaf/leaf-ast-node.hpp"
#include "../binary/binary-ast-node.hpp"

class FuncDefASTNode: public ASTNode
{
public:
  LeafASTNode *name;
  LeafASTNode *ret_type;
  std::vector<BinaryASTNode *> args;
  ASTNode *body = nullptr;

  FuncDefASTNode(ASTNode *name, unsigned int row, unsigned int col);
  ~FuncDefASTNode();

  void add_arg(BinaryASTNode *node);
};

#endif
