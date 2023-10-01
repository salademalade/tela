#ifndef AST_NODE_HPP
#define AST_NODE_HPP

class ASTNode
{
public:
  enum class Type
  {
    N_INT,
    N_FLOAT,
    N_ID,
    N_ADD,
    N_SUB,
    N_MUL,
    N_DIV,
    N_NULL
  } type;

  ASTNode(Type type);
  virtual ~ASTNode() = 0;
};

#endif
