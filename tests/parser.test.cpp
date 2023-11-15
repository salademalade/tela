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
  input.push_back(Token(TokenType::T_INT, "1", 1, 1));
  input.push_back(Token(TokenType::T_SEMICOLON, 1, 2));

  Parser parser(input);
  ASTNode *node = static_cast<StmtSeqASTNode *>(parser.parse())->statements[0];

  REQUIRE(node->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "1");
}

TEST_CASE("Parsing of float", "[parser][float]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_FLOAT, "2.3", 1, 1));
  input.push_back(Token(TokenType::T_SEMICOLON, 1, 4));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 4));

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
      input.push_back(Token(TokenType::T_KEY_LET, 1, 1));
      input.push_back(Token(TokenType::T_ID, "foo", 1, 5));
      input.push_back(Token(TokenType::T_COLON, 1, 8));
      input.push_back(Token(TokenType::T_KEY_INT, 1, 10));
      input.push_back(Token(TokenType::T_SEMICOLON, 1, 13));

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
      input.push_back(Token(TokenType::T_KEY_LET, 1, 1));
      input.push_back(Token(TokenType::T_ID, "foo", 1, 5));
      input.push_back(Token(TokenType::T_ASSIGN, 1, 9));
      input.push_back(Token(TokenType::T_INT, "1", 1, 11));
      input.push_back(Token(TokenType::T_SEMICOLON, 1, 12));

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
      input.push_back(Token(TokenType::T_KEY_CONST, 1, 1));
      input.push_back(Token(TokenType::T_ID, "foo", 1, 7));
      input.push_back(Token(TokenType::T_ASSIGN, 1, 11));
      input.push_back(Token(TokenType::T_INT, "1", 1, 13));
      input.push_back(Token(TokenType::T_SEMICOLON, 1, 14));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_ASSIGN, 1, 5));
    input.push_back(Token(TokenType::T_INT, "1", 1, 7));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 8));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_ADD, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 10));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_ADD, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_ADD, 1, 11));
    input.push_back(Token(TokenType::T_ID, "baz", 1, 13));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 16));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_SUB, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 10));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_SUB, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_SUB, 1, 11));
    input.push_back(Token(TokenType::T_ID, "baz", 1, 13));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 16));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_MUL, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 10));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_MUL, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_MUL, 1, 11));
    input.push_back(Token(TokenType::T_ID, "baz", 1, 13));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 16));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_DIV, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 10));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_DIV, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_DIV, 1, 11));
    input.push_back(Token(TokenType::T_ID, "baz", 1, 13));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 16));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_ADD, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_SUB, 1, 11));
    input.push_back(Token(TokenType::T_ID, "baz", 1, 13));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 16));

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
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_ADD, 1, 5));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 7));
    input.push_back(Token(TokenType::T_MUL, 1, 11));
    input.push_back(Token(TokenType::T_ID, "baz", 1, 13));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 16));

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
  SECTION("Function without arguments")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_KEY_DEF, 1, 1));
    input.push_back(Token(TokenType::T_ID, "func", 1, 5));
    input.push_back(Token(TokenType::T_LPAREN, 1, 9));
    input.push_back(Token(TokenType::T_RPAREN, 1, 10));
    input.push_back(Token(TokenType::T_COLON, 1, 11));
    input.push_back(Token(TokenType::T_KEY_INT, 1, 13));
    input.push_back(Token(TokenType::T_LCURLY, 1, 17));
    input.push_back(Token(TokenType::T_KEY_RETURN, 2, 2));
    input.push_back(Token(TokenType::T_INT, "0", 2, 9));
    input.push_back(Token(TokenType::T_SEMICOLON, 2, 10));
    input.push_back(Token(TokenType::T_RCURLY, 3, 1));

    Parser parser(input);
    FuncDefASTNode *node = static_cast<FuncDefASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_FUNC_DEF);
    REQUIRE(node->name->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->name)->value == "func");
    REQUIRE(node->ret_type->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->ret_type)->value == "int");
    REQUIRE(node->args.size() == 0);
  }

  SECTION("Function with arguments")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_KEY_DEF, 1, 1));
    input.push_back(Token(TokenType::T_ID, "func", 1, 5));
    input.push_back(Token(TokenType::T_LPAREN, 1, 9));
    input.push_back(Token(TokenType::T_ID, "arg1", 1, 10));
    input.push_back(Token(TokenType::T_COLON, 1, 14));
    input.push_back(Token(TokenType::T_KEY_INT, 1, 16));
    input.push_back(Token(TokenType::T_COMMA, 1, 19));
    input.push_back(Token(TokenType::T_ID, "arg2", 1, 21));
    input.push_back(Token(TokenType::T_COLON, 1, 25));
    input.push_back(Token(TokenType::T_KEY_FLOAT, 1, 27));
    input.push_back(Token(TokenType::T_RPAREN, 1, 32));
    input.push_back(Token(TokenType::T_COLON, 1, 33));
    input.push_back(Token(TokenType::T_KEY_INT, 1, 35));
    input.push_back(Token(TokenType::T_LCURLY, 1, 39));
    input.push_back(Token(TokenType::T_KEY_RETURN, 2, 2));
    input.push_back(Token(TokenType::T_INT, "1", 2, 9));
    input.push_back(Token(TokenType::T_SEMICOLON, 2, 10));
    input.push_back(Token(TokenType::T_RCURLY, 3, 1));

    Parser parser(input);
    FuncDefASTNode *node = static_cast<FuncDefASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_FUNC_DEF);
    REQUIRE(node->name->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->name)->value == "func");
    REQUIRE(node->ret_type->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->ret_type)->value == "int");
    REQUIRE(node->args.size() == 2);
    REQUIRE(node->args[0]->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->args[0]->left)->value == "arg1");
    REQUIRE(node->args[0]->right->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->args[0]->right)->value == "int");
    REQUIRE(node->args[1]->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->args[1]->left)->value == "arg2");
    REQUIRE(node->args[1]->right->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->args[1]->right)->value == "float");
    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements.size() == 1);
    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[0]->type == NodeType::N_RET);
    REQUIRE(static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->child->type == NodeType::N_INT);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->child)->value == "1");
  }

  SECTION("Function body")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_KEY_DEF, 1, 1));
    input.push_back(Token(TokenType::T_ID, "func", 1, 5));
    input.push_back(Token(TokenType::T_LPAREN, 1, 9));
    input.push_back(Token(TokenType::T_ID, "arg1", 1, 10));
    input.push_back(Token(TokenType::T_COLON, 1, 14));
    input.push_back(Token(TokenType::T_KEY_INT, 1, 16));
    input.push_back(Token(TokenType::T_COMMA, 1, 19));
    input.push_back(Token(TokenType::T_ID, "arg2", 1, 21));
    input.push_back(Token(TokenType::T_COLON, 1, 25));
    input.push_back(Token(TokenType::T_KEY_FLOAT, 1, 27));
    input.push_back(Token(TokenType::T_RPAREN, 1, 32));
    input.push_back(Token(TokenType::T_COLON, 1, 33));
    input.push_back(Token(TokenType::T_KEY_INT, 1, 35));
    input.push_back(Token(TokenType::T_LCURLY, 1, 39));
    input.push_back(Token(TokenType::T_ID, "arg1", 2, 2));
    input.push_back(Token(TokenType::T_ADD, 2, 7));
    input.push_back(Token(TokenType::T_ID, "arg2", 2, 9));
    input.push_back(Token(TokenType::T_SEMICOLON, 2, 13));
    input.push_back(Token(TokenType::T_KEY_RETURN, 3, 2));
    input.push_back(Token(TokenType::T_INT, "1", 3, 9));
    input.push_back(Token(TokenType::T_SEMICOLON, 3, 10));
    input.push_back(Token(TokenType::T_RCURLY, 4, 1));

    Parser parser(input);
    FuncDefASTNode *node = static_cast<FuncDefASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_FUNC_DEF);
    REQUIRE(node->name->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->name)->value == "func");
    REQUIRE(node->ret_type->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->ret_type)->value == "int");
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
}

TEST_CASE("Parsing of function call", "[parser][func]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_ID, "func", 1, 1));
  input.push_back(Token(TokenType::T_LPAREN, 1, 5));
  input.push_back(Token(TokenType::T_ID, "foo", 1, 6));
  input.push_back(Token(TokenType::T_COMMA, 1, 9));
  input.push_back(Token(TokenType::T_INT, "1", 1, 11));
  input.push_back(Token(TokenType::T_ADD, 1, 13));
  input.push_back(Token(TokenType::T_FLOAT, "2.3", 1, 15));
  input.push_back(Token(TokenType::T_RPAREN, 1, 18));
  input.push_back(Token(TokenType::T_SEMICOLON, 1, 19));

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
  input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
  input.push_back(Token(TokenType::T_SEMICOLON, 1, 5));
  input.push_back(Token(TokenType::T_ID, "bar", 2, 1));
  input.push_back(Token(TokenType::T_SEMICOLON, 2, 5));

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
    input.push_back(Token(TokenType::T_LPAREN, 1, 1));
    input.push_back(Token(TokenType::T_ID, "foo", 1, 2));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected factor")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_ID, "bar", 1, 5));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected operator")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ADD, 1, 1));
    input.push_back(Token(TokenType::T_ADD, 1, 3));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected EOF")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_LPAREN, 1, 1));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }
}
