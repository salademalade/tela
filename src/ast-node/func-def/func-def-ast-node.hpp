#ifndef FUNC_DEF_AST_NODE_HPP
#define FUNC_DEF_AST_NODE_HPP

#include <utility>
#include <vector>
#include <map>
#include "error/error.hpp"
#include "../ast-node.hpp"
#include "../leaf/leaf-ast-node.hpp"

class FuncDefASTNode: public ASTNode
{
public:
  LeafASTNode *name;
  LeafASTNode *ret_type;
  std::vector<std::pair<std::string, LeafASTNode *>> args;
  ASTNode *body;

  FuncDefASTNode(ASTNode *name);
  ~FuncDefASTNode();

  void add_arg(std::string id, std::string type);
};

#endif
