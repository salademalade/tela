#include <catch2/catch.hpp>
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

TEST_CASE("Unary AST Node tests", "[ast]")
{
  ASTNode *child = new LeafASTNode(NodeType::N_ID, "foo");

  ASTNode *node = new UnaryASTNode(NodeType::N_ADD, child);

  REQUIRE(node->type == NodeType::N_ADD);
  REQUIRE(static_cast<UnaryASTNode *>(node)->child == child);
}

TEST_CASE("Function Definition Node tests", "[ast][func]")
{
  ASTNode *name = new LeafASTNode(NodeType::N_ID, "func");
  ASTNode *body = new LeafASTNode(NodeType::N_INT, "1");
  ASTNode *ret_type = new LeafASTNode(NodeType::N_TYPE, "int");
  FuncDefASTNode *func = new FuncDefASTNode(name);

  func->body = body;

  std::string a_name1 = "foo";
  std::string a_type1 = "int";

  std::string a_name2 = "bar";
  std::string a_type2 = "float";

  func->add_arg(a_name1, a_type1);
  func->add_arg(a_name2, a_type2);

  REQUIRE(func->type == NodeType::N_FUNC_DEF);

  REQUIRE(static_cast<LeafASTNode *>(func->name)->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(func->name)->value == "func");

  REQUIRE(func->args[0].first == "foo");
  REQUIRE(func->args[0].second->type == NodeType::N_TYPE);
  REQUIRE(func->args[0].second->value == "int");
  REQUIRE(func->args[1].first == "bar");
  REQUIRE(func->args[1].second->type == NodeType::N_TYPE);
  REQUIRE(func->args[1].second->value == "float");

  REQUIRE(static_cast<LeafASTNode *>(func->body)->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(func->body)->value == "1");
}

TEST_CASE("Function Call Node tests", "[ast][func]")
{
  ASTNode *name = new LeafASTNode(NodeType::N_ID, "func");

  FuncCallASTNode *func = new FuncCallASTNode(name);

  ASTNode *arg1 = new LeafASTNode(NodeType::N_INT, "1");

  ASTNode *arg2_l = new LeafASTNode(NodeType::N_ID, "foo");
  ASTNode *arg2_r = new LeafASTNode(NodeType::N_FLOAT, "1.2");
  ASTNode *arg2 = new BinaryASTNode(NodeType::N_ADD, arg2_l, arg2_r);

  func->add_arg(arg1);
  func->add_arg(arg2);

  REQUIRE(func->type == NodeType::N_FUNC_CALL);

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
  StmtSeqASTNode *seq = new StmtSeqASTNode();

  ASTNode *el = new LeafASTNode(NodeType::N_ID, "foo");

  seq->statements.push_back(el);
  REQUIRE(seq->statements[0] == el);
}
