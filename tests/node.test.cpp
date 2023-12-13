#include <catch2/catch_test_macros.hpp>
#include <memory>
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf/leaf-ast-node.hpp"
#include "ast-node/binary/binary-ast-node.hpp"
#include "ast-node/unary/unary-ast-node.hpp"
#include "ast-node/func-def/func-def-ast-node.hpp"
#include "ast-node/func-call/func-call-ast-node.hpp"
#include "ast-node/stmt-seq/stmt-seq-ast-node.hpp"
#include "ast-node/null/null-ast-node.hpp"

TEST_CASE("Leaf AST Node tests", "[ast]")
{
  ASTNode *node = new LeafASTNode(NodeType::N_INT, "1", 1, 1);

  REQUIRE(node->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "1");
  REQUIRE(node->row == 1);
  REQUIRE(node->col == 1);
}

TEST_CASE("Binary AST Node tests", "[ast]")
{
  ASTNode *left = new LeafASTNode(NodeType::N_ID, "foo", 1, 1);
  ASTNode *right = new LeafASTNode(NodeType::N_ID, "bar", 1, 7);

  ASTNode *node = new BinaryASTNode(NodeType::N_ADD, left, right, 1, 5);

  REQUIRE(node->type == NodeType::N_ADD);
  REQUIRE(static_cast<BinaryASTNode *>(node)->left == left);
  REQUIRE(static_cast<BinaryASTNode *>(node)->right == right);
  REQUIRE(node->row == 1);
  REQUIRE(node->col == 5);
}

TEST_CASE("Unary AST Node tests", "[ast]")
{
  ASTNode *child = new LeafASTNode(NodeType::N_ID, "foo", 1, 5);

  ASTNode *node = new UnaryASTNode(NodeType::N_RET, child, 1, 1);

  REQUIRE(node->type == NodeType::N_RET);
  REQUIRE(static_cast<UnaryASTNode *>(node)->child == child);
  REQUIRE(node->row == 1);
  REQUIRE(node->col == 1);
}

TEST_CASE("Function Definition Node tests", "[ast][func]")
{
  ASTNode *name = new LeafASTNode(NodeType::N_ID, "func", 1, 5);
  ASTNode *body = new LeafASTNode(NodeType::N_INT, "1", 1, 37);
  FuncDefASTNode *func = new FuncDefASTNode(name, 1, 1);

  func->body = body;

  LeafASTNode *a_name1 = new LeafASTNode(NodeType::N_ID, "foo", 1, 10);
  LeafASTNode *a_type1 = new LeafASTNode(NodeType::N_TYPE, "int", 1, 15);
  BinaryASTNode *arg1 = new BinaryASTNode(NodeType::N_TYPE_DECL, a_name1, a_type1, 1, 13);

  LeafASTNode *a_name2 = new LeafASTNode(NodeType::N_ID, "bar", 1, 20);
  LeafASTNode *a_type2 = new LeafASTNode(NodeType::N_TYPE, "float", 1, 25);
  BinaryASTNode *arg2 = new BinaryASTNode(NodeType::N_TYPE_DECL, a_name2, a_type2, 1, 23);

  func->add_arg(arg1);
  func->add_arg(arg2);

  REQUIRE(func->type == NodeType::N_FUNC_DEF);
  REQUIRE(func->row == 1);
  REQUIRE(func->col == 1);

  REQUIRE(static_cast<LeafASTNode *>(func->name)->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(func->name)->value == "func");

  REQUIRE(func->args[0]->left->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(func->args[0]->left)->value == "foo");
  REQUIRE(func->args[0]->right->type == NodeType::N_TYPE);
  REQUIRE(static_cast<LeafASTNode *>(func->args[0]->right)->value == "int");
  REQUIRE(func->args[1]->left->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(func->args[1]->left)->value == "bar");
  REQUIRE(func->args[1]->right->type == NodeType::N_TYPE);
  REQUIRE(static_cast<LeafASTNode *>(func->args[1]->right)->value == "float");

  REQUIRE(static_cast<LeafASTNode *>(func->body)->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(func->body)->value == "1");
}

TEST_CASE("Function Call Node tests", "[ast][func]")
{
  ASTNode *name = new LeafASTNode(NodeType::N_ID, "func", 1, 1);

  FuncCallASTNode *func = new FuncCallASTNode(name);

  ASTNode *arg1 = new LeafASTNode(NodeType::N_INT, "1", 1, 6);

  ASTNode *arg2_l = new LeafASTNode(NodeType::N_ID, "foo", 1, 9);
  ASTNode *arg2_r = new LeafASTNode(NodeType::N_FLOAT, "1.2", 1, 15);
  ASTNode *arg2 = new BinaryASTNode(NodeType::N_ADD, arg2_l, arg2_r, 1, 13);

  func->add_arg(arg1);
  func->add_arg(arg2);

  REQUIRE(func->type == NodeType::N_FUNC_CALL);
  REQUIRE(func->row == 1);
  REQUIRE(func->col == 1);

  REQUIRE(static_cast<LeafASTNode *>(func->name)->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(func->name)->value == "func");

  REQUIRE(func->args[0]->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(func->args[0])->value == "1");

  REQUIRE(func->args[1]->type == NodeType::N_ADD);
  REQUIRE(static_cast<BinaryASTNode *>(func->args[1])->left->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(func->args[1])->left)->value == "foo");
  REQUIRE(static_cast<BinaryASTNode *>(func->args[1])->right->type == NodeType::N_FLOAT);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(func->args[1])->right)->value == "1.2");
}

TEST_CASE("Statement Sequence Node tests", "[ast]")
{
  StmtSeqASTNode *seq = new StmtSeqASTNode(1, 1);

  ASTNode *el = new LeafASTNode(NodeType::N_ID, "foo", 1, 1);

  seq->statements.push_back(el);
  REQUIRE(seq->statements[0] == el);
  REQUIRE(seq->row == 1);
  REQUIRE(seq->col == 1);
}
