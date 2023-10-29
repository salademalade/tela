#include <catch2/catch.hpp>
#include <memory>
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf-ast-node.hpp"
#include "ast-node/binary-ast-node.hpp"
#include "ast-node/func-def-ast-node.hpp"
#include "ast-node/func-call-ast-node.hpp"
#include "ast-node/stmt-seq-ast-node.hpp"

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

TEST_CASE("Function Definition Node tests", "[ast][func]")
{
  ASTNode *name = new LeafASTNode(ASTNode::Type::N_ID, "func");
  ASTNode *body = new LeafASTNode(ASTNode::Type::N_INT, "1");
  FuncDefASTNode *func = new FuncDefASTNode(name, FuncRetType::R_INT);

  func->body = body;

  std::string a_name1 = "foo";
  FuncRetType a_type1 = FuncRetType::R_INT;

  std::string a_name2 = "bar";
  FuncRetType a_type2 = FuncRetType::R_FLOAT;

  func->add_arg(a_name1, a_type1);
  func->add_arg(a_name2, a_type2);

  REQUIRE(func->type == ASTNode::Type::N_FUNC_DEF);
  REQUIRE(func->ret_type == FuncRetType::R_INT);

  REQUIRE(static_cast<LeafASTNode *>(func->name)->type == ASTNode::Type::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(func->name)->value == "func");

  REQUIRE(func->args["foo"] == FuncRetType::R_INT);
  REQUIRE(func->args["bar"] == FuncRetType::R_FLOAT);

  REQUIRE(static_cast<LeafASTNode *>(func->body)->type == ASTNode::Type::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(func->body)->value == "1");
}

TEST_CASE("Function Call Node tests", "[ast][func]")
{
  ASTNode *name = new LeafASTNode(ASTNode::Type::N_ID, "func");

  FuncCallASTNode *func = new FuncCallASTNode(name);

  ASTNode *arg1 = new LeafASTNode(ASTNode::Type::N_INT, "1");

  ASTNode *arg2_l = new LeafASTNode(ASTNode::Type::N_ID, "foo");
  ASTNode *arg2_r = new LeafASTNode(ASTNode::Type::N_FLOAT, "1.2");
  ASTNode *arg2 = new BinaryASTNode(ASTNode::Type::N_ADD, arg2_l, arg2_r);

  func->add_arg(arg1);
  func->add_arg(arg2);

  REQUIRE(func->type == ASTNode::Type::N_FUNC_CALL);

  REQUIRE(static_cast<LeafASTNode *>(func->name)->type == ASTNode::Type::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(func->name)->value == "func");

  REQUIRE(func->args[0]->type == ASTNode::Type::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(func->args[0])->value == "1");

  REQUIRE(func->args[1]->type == ASTNode::Type::N_ADD);
  REQUIRE(static_cast<BinaryASTNode *>(func->args[1])->left->type == ASTNode::Type::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(func->args[1])->left)->value == "foo");
  REQUIRE(static_cast<BinaryASTNode *>(func->args[1])->right->type == ASTNode::Type::N_FLOAT);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(func->args[1])->right)->value == "1.2");
}

TEST_CASE("Statement Sequence Node tests", "[ast]")
{
  StmtSeqASTNode *seq = new StmtSeqASTNode();

  ASTNode *el = new LeafASTNode(ASTNode::Type::N_ID, "foo");

  seq->statements.push_back(el);
  REQUIRE(seq->statements[0] == el);
}
