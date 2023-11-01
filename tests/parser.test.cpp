#include <catch2/catch.hpp>
#include <vector>
#include "token/token.hpp"
#include "parser/parser.hpp"
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf/leaf-ast-node.hpp"
#include "ast-node/binary/binary-ast-node.hpp"
#include "ast-node/unary/unary-ast-node.hpp"
#include "ast-node/func-def/func-def-ast-node.hpp"
#include "ast-node/func-call/func-call-ast-node.hpp"
#include "ast-node/stmt-seq/stmt-seq-ast-node.hpp"
#include "ast-node/null/null-ast-node.hpp"

TEST_CASE("Parsing of integer", "[parser][int]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_INT, "1"));
  input.push_back(Token(TokenType::T_SEMICOLON));

  Parser parser(input);
  ASTNode *node = static_cast<StmtSeqASTNode *>(parser.parse())->statements[0];

  REQUIRE(node->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "1");
}

TEST_CASE("Parsing of float", "[parser][float]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_FLOAT, "2.3"));
  input.push_back(Token(TokenType::T_SEMICOLON));

  Parser parser(input);
  ASTNode *node = static_cast<StmtSeqASTNode *>(parser.parse())->statements[0];

  REQUIRE(node->type == NodeType::N_FLOAT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "2.3");
}

TEST_CASE("Parsing of identifier", "[parser][id]")
{
  SECTION("Single identifier")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    LeafASTNode *node = static_cast<LeafASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ID);
    REQUIRE(node->value == "foo");
  }

  SECTION("Declaration of identifier")
  {
    SECTION("Declaration of variable without value assignment")
    {
      std::vector<Token> input;
      input.push_back(Token(TokenType::T_KEY_LET));
      input.push_back(Token(TokenType::T_ID, "foo"));
      input.push_back(Token(TokenType::T_COLON));
      input.push_back(Token(TokenType::T_KEY_INT));
      input.push_back(Token(TokenType::T_SEMICOLON));

      Parser parser(input);
      UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

      REQUIRE(node->type == NodeType::N_DECL);
      REQUIRE(node->child->type == NodeType::N_TYPE_DECL);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_ID);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value == "foo");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->type == NodeType::N_TYPE);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->right)->value == "int");
    }

    SECTION("Declaration of variable with value assignment")
    {
      std::vector<Token> input;
      input.push_back(Token(TokenType::T_KEY_LET));
      input.push_back(Token(TokenType::T_ID, "foo"));
      input.push_back(Token(TokenType::T_ASSIGN));
      input.push_back(Token(TokenType::T_INT, "1"));
      input.push_back(Token(TokenType::T_SEMICOLON));

      Parser parser(input);
      UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

      REQUIRE(node->type == NodeType::N_DECL);
      REQUIRE(node->child->type == NodeType::N_ASSIGN);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_ID);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value == "foo");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->type == NodeType::N_INT);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->right)->value == "1");
    }

    SECTION("Declaration of constant with value assignment")
    {
      std::vector<Token> input;
      input.push_back(Token(TokenType::T_KEY_CONST));
      input.push_back(Token(TokenType::T_ID, "foo"));
      input.push_back(Token(TokenType::T_ASSIGN));
      input.push_back(Token(TokenType::T_INT, "1"));
      input.push_back(Token(TokenType::T_SEMICOLON));

      Parser parser(input);
      UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

      REQUIRE(node->type == NodeType::N_DECL_CONST);
      REQUIRE(node->child->type == NodeType::N_ASSIGN);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_ID);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value == "foo");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->type == NodeType::N_INT);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->right)->value == "1");
    }
  }

  SECTION("Value assignment without declaration")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_ASSIGN));
    input.push_back(Token(TokenType::T_INT, "1"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ASSIGN);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == NodeType::N_INT);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "1");
  }
}

TEST_CASE("Parsing of addition operator", "[parser][add]")
{
  SECTION("Single operator")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_ADD));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ADD);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
  }

  SECTION("Chain of operators")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_ADD));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_ADD));
    input.push_back(Token(TokenType::T_ID, "baz"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ADD);
    REQUIRE(node->left->type == NodeType::N_ADD);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }
}

TEST_CASE("Parsing of subtraction operator", "[parser][sub]")
{
  SECTION("Single operator")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_SUB));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_SUB);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
  }

  SECTION("Chain of operators")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_SUB));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_SUB));
    input.push_back(Token(TokenType::T_ID, "baz"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_SUB);
    REQUIRE(node->left->type == NodeType::N_SUB);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }
}

TEST_CASE("Parsing of multiplication operator", "[parser][mul]")
{
  SECTION("Single operator")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_MUL));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_MUL);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
  }

  SECTION("Chain of operators")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_MUL));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_MUL));
    input.push_back(Token(TokenType::T_ID, "baz"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_MUL);
    REQUIRE(node->left->type == NodeType::N_MUL);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }
}

TEST_CASE("Parsing of division operator", "[parser][div]")
{
  SECTION("Single operator")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_DIV));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_DIV);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
  }

  SECTION("Chain of operators")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_DIV));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_DIV));
    input.push_back(Token(TokenType::T_ID, "baz"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_DIV);
    REQUIRE(node->left->type == NodeType::N_DIV);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }
}

TEST_CASE("Parsing of combination of operators", "[parser][add][sub][mul][div]")
{
  SECTION("Combination of addition and subtraction")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_ADD));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_SUB));
    input.push_back(Token(TokenType::T_ID, "baz"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_SUB);
    REQUIRE(node->left->type == NodeType::N_ADD);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }

  SECTION("Combination of addition and multiplication")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_ADD));
    input.push_back(Token(TokenType::T_ID, "bar"));
    input.push_back(Token(TokenType::T_MUL));
    input.push_back(Token(TokenType::T_ID, "baz"));
    input.push_back(Token(TokenType::T_SEMICOLON));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ADD);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == NodeType::N_MUL);
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->right)->left)->value == "bar");
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->right)->right)->value == "baz");
  }
}

TEST_CASE("Parsing of function definition", "[parser][func]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_KEY_DEF));
  input.push_back(Token(TokenType::T_ID, "func"));
  input.push_back(Token(TokenType::T_LPAREN));
  input.push_back(Token(TokenType::T_ID, "arg1"));
  input.push_back(Token(TokenType::T_COLON));
  input.push_back(Token(TokenType::T_KEY_INT));
  input.push_back(Token(TokenType::T_COMMA));
  input.push_back(Token(TokenType::T_ID, "arg2"));
  input.push_back(Token(TokenType::T_COLON));
  input.push_back(Token(TokenType::T_KEY_FLOAT));
  input.push_back(Token(TokenType::T_RPAREN));
  input.push_back(Token(TokenType::T_COLON));
  input.push_back(Token(TokenType::T_KEY_INT));
  input.push_back(Token(TokenType::T_LCURLY));
  input.push_back(Token(TokenType::T_ID, "arg1"));
  input.push_back(Token(TokenType::T_ADD));
  input.push_back(Token(TokenType::T_ID, "arg2"));
  input.push_back(Token(TokenType::T_SEMICOLON));
  input.push_back(Token(TokenType::T_KEY_RETURN));
  input.push_back(Token(TokenType::T_INT, "1"));
  input.push_back(Token(TokenType::T_SEMICOLON));
  input.push_back(Token(TokenType::T_RCURLY));

  Parser parser(input);
  FuncDefASTNode *node = static_cast<FuncDefASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

  REQUIRE(node->type == NodeType::N_FUNC_DEF);
  REQUIRE(node->name->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->name)->value == "func");
  REQUIRE(node->ret_type->type == NodeType::N_TYPE);
  REQUIRE(static_cast<LeafASTNode *>(node->ret_type)->value == "int");
  REQUIRE(node->args["arg1"]->type == NodeType::N_TYPE);
  REQUIRE(node->args["arg1"]->value == "int");
  REQUIRE(node->args["arg2"]->type == NodeType::N_TYPE);
  REQUIRE(node->args["arg2"]->value == "float");
  REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements.size() == 2);
  REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[0]->type == NodeType::N_ADD);
  REQUIRE(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->left->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->left)->value == "arg1");
  REQUIRE(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->right->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->right)->value == "arg2");
  REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[1]->type == NodeType::N_RET);
  REQUIRE(static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[1])->child->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[1])->child)->value == "1");
}

TEST_CASE("Parsing of function call", "[parser][func]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_ID, "func"));
  input.push_back(Token(TokenType::T_LPAREN));
  input.push_back(Token(TokenType::T_ID, "foo"));
  input.push_back(Token(TokenType::T_COMMA));
  input.push_back(Token(TokenType::T_INT, "1"));
  input.push_back(Token(TokenType::T_ADD));
  input.push_back(Token(TokenType::T_FLOAT, "2.3"));
  input.push_back(Token(TokenType::T_RPAREN));
  input.push_back(Token(TokenType::T_SEMICOLON));

  Parser parser(input);
  FuncCallASTNode *node = static_cast<FuncCallASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

  REQUIRE(node->type == NodeType::N_FUNC_CALL);
  REQUIRE(node->name->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->name)->value == "func");
  REQUIRE(node->args[0]->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->args[0])->value == "foo");
  REQUIRE(node->args[1]->type == NodeType::N_ADD);
  REQUIRE(static_cast<BinaryASTNode *>(node->args[1])->left->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->args[1])->left)->value == "1");
  REQUIRE(static_cast<BinaryASTNode *>(node->args[1])->right->type == NodeType::N_FLOAT);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->args[1])->right)->value == "2.3");
}

TEST_CASE("Parsing of statement sequences", "[parser]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_ID, "foo"));
  input.push_back(Token(TokenType::T_SEMICOLON));
  input.push_back(Token(TokenType::T_ID, "bar"));
  input.push_back(Token(TokenType::T_SEMICOLON));

  Parser parser(input);
  StmtSeqASTNode *node = static_cast<StmtSeqASTNode *>(parser.parse());

  REQUIRE(node->statements.size() == 2);
  REQUIRE(node->statements[0]->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->statements[0])->value == "foo");
  REQUIRE(node->statements[1]->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->statements[1])->value == "bar");
}

TEST_CASE("Parsing invalid inputs", "[parser]")
{
  SECTION("Unclosed parentheses")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_LPAREN));
    input.push_back(Token(TokenType::T_ID, "foo"));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected factor")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo"));
    input.push_back(Token(TokenType::T_ID, "bar"));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected operator")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ADD));
    input.push_back(Token(TokenType::T_ADD));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected EOF")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_LPAREN));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }
}
