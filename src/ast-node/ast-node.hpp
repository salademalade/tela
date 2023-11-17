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

    N_DECL,
    N_DECL_CONST,
    N_TYPE,
    N_TYPE_DECL,
    N_ASSIGN,

    N_FUNC_DEF,
    N_FUNC_CALL,
    N_RET,

    N_STMT_SEQ,
    N_NULL,
  } type;

  unsigned int row;
  unsigned int col;

  ASTNode(Type type, unsigned int row, unsigned int col);
  virtual ~ASTNode() = 0;
};

typedef ASTNode::Type NodeType;

#endif
