#include "leaf-ast-node.hpp"

LeafASTNode::LeafASTNode(Type type, std::string value) : ASTNode(type), value(value) {}

LeafASTNode::~LeafASTNode() {}
