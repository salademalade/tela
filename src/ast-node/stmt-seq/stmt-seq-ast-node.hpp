#ifndef STMT_SEQ_AST_NODE_HPP
#define STMT_SEQ_AST_NODE_HPP

#include <vector>
#include <memory>
#include "../ast-node.hpp"

class StmtSeqASTNode: public ASTNode
{
public:
  std::vector<ASTNode *> statements;

  StmtSeqASTNode(unsigned int row, unsigned int col);
  ~StmtSeqASTNode();
};

#endif
