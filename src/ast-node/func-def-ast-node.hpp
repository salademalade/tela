#ifndef FUNC_DEF_AST_NODE_HPP
#define FUNC_DEF_AST_NODE_HPP

#include <map>
#include "error/error.hpp"
#include "ast-node.hpp"
#include "leaf-ast-node.hpp"

class FuncDefASTNode: public ASTNode
{
public:
  enum class ReturnType
  {
    R_INT,
    R_FLOAT,
  };
  LeafASTNode *name;
  std::map<std::string, ReturnType> args;
  ASTNode *body;

  FuncDefASTNode(ASTNode *name);
  ~FuncDefASTNode();

  void add_arg(std::string id, ReturnType type);
};

typedef FuncDefASTNode::ReturnType FuncRetType;

#endif
