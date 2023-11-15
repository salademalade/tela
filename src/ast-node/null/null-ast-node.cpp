#include "null-ast-node.hpp"

NullASTNode::NullASTNode(unsigned int row, unsigned int col): ASTNode(NodeType::N_NULL, row, col) {}

NullASTNode::~NullASTNode() {}
