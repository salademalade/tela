#include <catch2/catch.hpp>
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf-ast-node.hpp"
#include "ast-node/binary-ast-node.hpp"

TEST_CASE("Leaf AST Node tests", "[ast]")
{
  ASTNode *node = new LeafASTNode(ASTNode::Type::N_INT, "1");

  REQUIRE(node->type == ASTNode::Type::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "1");
}

TEST_CASE("Binary AST Node tests", "[ast]")
{
  ASTNode *left = new LeafASTNode(ASTNode::Type::N_ID, "foo");
  ASTNode *right = new LeafASTNode(ASTNode::Type::N_ID, "bar");

  ASTNode *node = new BinaryASTNode(ASTNode::Type::N_ADD, left, right);

  REQUIRE(node->type == ASTNode::Type::N_ADD);
  REQUIRE(static_cast<BinaryASTNode *>(node)->left == left);
  REQUIRE(static_cast<BinaryASTNode *>(node)->right == right);
}