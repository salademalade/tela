#include <catch2/catch.hpp>
#include <memory>
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf-ast-node.hpp"
#include "ast-node/binary-ast-node.hpp"
#include "ast-node/stmt-seq-ast-node.hpp"

TEST_CASE("Leaf AST Node tests", "[ast]")
{
  ASTNode *node = new LeafASTNode(NodeType::N_INT, "1");

  REQUIRE(node->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "1");
}

TEST_CASE("Binary AST Node tests", "[ast]")
{
  ASTNode *left = new LeafASTNode(NodeType::N_ID, "foo");
  ASTNode *right = new LeafASTNode(NodeType::N_ID, "bar");

  ASTNode *node = new BinaryASTNode(NodeType::N_ADD, left, right);

  REQUIRE(node->type == NodeType::N_ADD);
  REQUIRE(static_cast<BinaryASTNode *>(node)->left == left);
  REQUIRE(static_cast<BinaryASTNode *>(node)->right == right);
}

TEST_CASE("Statement Sequence Node tests", "[ast]")
{
  StmtSeqASTNode *seq = new StmtSeqASTNode();

  ASTNode *el = new LeafASTNode(NodeType::N_ID, "foo");

  seq->statements.push_back(el);
  REQUIRE(seq->statements[0] == el);
}
