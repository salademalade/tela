#include "stmt-seq-ast-node.hpp"

StmtSeqASTNode::StmtSeqASTNode(unsigned int row, unsigned int col) : ASTNode(NodeType::N_STMT_SEQ, row, col) {}

StmtSeqASTNode::~StmtSeqASTNode()
{
  for (auto i : statements) delete i;
}
