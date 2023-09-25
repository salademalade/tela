#include <catch2/catch.hpp>
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf-ast-node.hpp"
#include "ast-node/binary-ast-node.hpp"

TEST_CASE("AST Node class tests", "[ast]")
{
  SECTION("Leaf node")
  {
    SECTION("Integer type")
    {
      ASTNode *node = new LeafASTNode(ASTNode::Type::N_INT, "1");

      REQUIRE(node->type == ASTNode::Type::N_INT);
      REQUIRE(static_cast<LeafASTNode *>(node)->value == "1");
    }

    SECTION("Float type")
    {
      ASTNode *node = new LeafASTNode(ASTNode::Type::N_FLOAT, "2.3");

      REQUIRE(node->type == ASTNode::Type::N_FLOAT);
      REQUIRE(static_cast<LeafASTNode *>(node)->value == "2.3");
    }

    SECTION("ID type")
    {
      ASTNode *node = new LeafASTNode(ASTNode::Type::N_ID, "foo");

      REQUIRE(node->type == ASTNode::Type::N_ID);
      REQUIRE(static_cast<LeafASTNode *>(node)->value == "foo");
    }
  }

  SECTION("Binary node")
  {
    ASTNode *left = new LeafASTNode(ASTNode::Type::N_ID, "foo");
    ASTNode *right = new LeafASTNode(ASTNode::Type::N_ID, "bar");

    SECTION("Addition")
    {
      ASTNode *node = new BinaryASTNode(ASTNode::Type::N_ADD, left, right);

      REQUIRE(node->type == ASTNode::Type::N_ADD);
      REQUIRE(static_cast<BinaryASTNode *>(node)->left == left);
      REQUIRE(static_cast<BinaryASTNode *>(node)->right == right);
    }

    SECTION("Subtraction")
    {
      ASTNode *node = new BinaryASTNode(ASTNode::Type::N_SUB, left, right);

      REQUIRE(node->type == ASTNode::Type::N_SUB);
      REQUIRE(static_cast<BinaryASTNode *>(node)->left == left);
      REQUIRE(static_cast<BinaryASTNode *>(node)->right == right);
    }

    SECTION("Multiplication")
    {
      ASTNode *node = new BinaryASTNode(ASTNode::Type::N_MUL, left, right);

      REQUIRE(node->type == ASTNode::Type::N_MUL);
      REQUIRE(static_cast<BinaryASTNode *>(node)->left == left);
      REQUIRE(static_cast<BinaryASTNode *>(node)->right == right);
    }

    SECTION("Division")
    {
      ASTNode *node = new BinaryASTNode(ASTNode::Type::N_DIV, left, right);

      REQUIRE(node->type == ASTNode::Type::N_DIV);
      REQUIRE(static_cast<BinaryASTNode *>(node)->left == left);
      REQUIRE(static_cast<BinaryASTNode *>(node)->right == right);
    }
  }
}
