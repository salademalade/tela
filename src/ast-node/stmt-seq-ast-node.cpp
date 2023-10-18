#include "stmt-seq-ast-node.hpp"

StmtSeqASTNode::StmtSeqASTNode() : ASTNode(NodeType::N_STMT_SEQ) {}

StmtSeqASTNode::~StmtSeqASTNode()
{
  for (auto i : statements) delete i;
}
