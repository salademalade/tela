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
  input.push_back(Token(TokenType::T_EOF, 2, 1));

  Parser parser(input);
  ASTNode *node = static_cast<StmtSeqASTNode *>(parser.parse())->statements[0];

  REQUIRE(node->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "1");
  REQUIRE(node->row == 1);
  REQUIRE(node->col == 1);
}

TEST_CASE("Parsing of float", "[parser][float]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_FLOAT, "2.3", 1, 1));
  input.push_back(Token(TokenType::T_SEMICOLON, 1, 4));
  input.push_back(Token(TokenType::T_EOF, 2, 1));

  Parser parser(input);
  ASTNode *node = static_cast<StmtSeqASTNode *>(parser.parse())->statements[0];

  REQUIRE(node->type == NodeType::N_FLOAT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "2.3");
  REQUIRE(node->row == 1);
  REQUIRE(node->col == 1);
}

TEST_CASE("Parsing of character", "[parser][char]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_CHAR, "a", 1, 1));
  input.push_back(Token(TokenType::T_SEMICOLON, 1, 4));
  input.push_back(Token(TokenType::T_EOF, 2, 1));

  Parser parser(input);
  ASTNode *node = static_cast<StmtSeqASTNode *>(parser.parse())->statements[0];

  REQUIRE(node->type == NodeType::N_CHAR);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "a");
  REQUIRE(node->row == 1);
  REQUIRE(node->col == 1);

}

TEST_CASE("Parsing of identifier", "[parser][id]")
{
  SECTION("Single identifier")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 4));
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    LeafASTNode *node = static_cast<LeafASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ID);
    REQUIRE(node->value == "foo");
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 1);
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
      input.push_back(Token(TokenType::T_EOF, 2, 1));

      Parser parser(input);
      UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

      REQUIRE(node->type == NodeType::N_DECL);
      REQUIRE(node->row == 1);
      REQUIRE(node->col == 1);
      REQUIRE(node->child->type == NodeType::N_TYPE_DECL);
      REQUIRE(node->child->row == 1);
      REQUIRE(node->child->col == 8);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_ID);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value == "foo");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->col == 5);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->type == NodeType::N_TYPE);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->right)->value == "int");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->col == 10);
    }

    SECTION("Declaration of variable with value assignment")
    {
      std::vector<Token> input;
      input.push_back(Token(TokenType::T_KEY_LET, 1, 1));
      input.push_back(Token(TokenType::T_ID, "foo", 1, 5));
      input.push_back(Token(TokenType::T_ASSIGN, 1, 9));
      input.push_back(Token(TokenType::T_INT, "1", 1, 11));
      input.push_back(Token(TokenType::T_SEMICOLON, 1, 12));
      input.push_back(Token(TokenType::T_EOF, 2, 1));

      Parser parser(input);
      UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

      REQUIRE(node->type == NodeType::N_DECL);
      REQUIRE(node->row == 1);
      REQUIRE(node->col == 1);
      REQUIRE(node->child->type == NodeType::N_ASSIGN);
      REQUIRE(node->child->row == 1);
      REQUIRE(node->child->col == 9);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_ID);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value == "foo");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->col == 5);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->type == NodeType::N_INT);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->right)->value == "1");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->col == 11);
    }

    SECTION("Declaration of variable with type and value assignment")
    {
      std::vector<Token> input;
      input.push_back(Token(TokenType::T_KEY_LET, 1, 1));
      input.push_back(Token(TokenType::T_ID, "foo", 1, 5));
      input.push_back(Token(TokenType::T_COLON, 1, 8));
      input.push_back(Token(TokenType::T_KEY_INT, 1, 10));
      input.push_back(Token(TokenType::T_ASSIGN, 1, 14));
      input.push_back(Token(TokenType::T_INT, "1", 1, 16));
      input.push_back(Token(TokenType::T_SEMICOLON, 1, 17));
      input.push_back(Token(TokenType::T_EOF, 2, 1));

      Parser parser(input);
      UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

      REQUIRE(node->type == NodeType::N_DECL);
      REQUIRE(node->row == 1);
      REQUIRE(node->col == 1);
      REQUIRE(node->child->type == NodeType::N_ASSIGN);
      REQUIRE(node->child->row == 1);
      REQUIRE(node->child->col == 14);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_TYPE_DECL);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->col == 8);
      REQUIRE(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->left->type == NodeType::N_ID);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->left)->value == "foo");
      REQUIRE(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->left->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->left->col == 5);
      REQUIRE(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->right->type == NodeType::N_TYPE);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->right)->value == "int");
      REQUIRE(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->right->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->right->col == 10);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->type == NodeType::N_INT);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->right)->value == "1");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->col == 16);
    }

    SECTION("Declaration of constant with value assignment")
    {
      std::vector<Token> input;
      input.push_back(Token(TokenType::T_KEY_CONST, 1, 1));
      input.push_back(Token(TokenType::T_ID, "foo", 1, 7));
      input.push_back(Token(TokenType::T_ASSIGN, 1, 11));
      input.push_back(Token(TokenType::T_INT, "1", 1, 13));
      input.push_back(Token(TokenType::T_SEMICOLON, 1, 14));
      input.push_back(Token(TokenType::T_EOF, 2, 1));

      Parser parser(input);
      UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

      REQUIRE(node->type == NodeType::N_DECL_CONST);
      REQUIRE(node->row == 1);
      REQUIRE(node->col == 1);
      REQUIRE(node->child->type == NodeType::N_ASSIGN);
      REQUIRE(node->child->row == 1);
      REQUIRE(node->child->col == 11);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->type == NodeType::N_ID);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->left)->value == "foo");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->left->col == 7);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->type == NodeType::N_INT);
      REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->child)->right)->value == "1");
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->row == 1);
      REQUIRE(static_cast<BinaryASTNode *>(node->child)->right->col == 13);
    }
  }

  SECTION("Value assignment without declaration")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
    input.push_back(Token(TokenType::T_ASSIGN, 1, 5));
    input.push_back(Token(TokenType::T_INT, "1", 1, 7));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 8));
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ASSIGN);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 5);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 1);
    REQUIRE(node->right->type == NodeType::N_INT);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "1");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 7);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ADD);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 5);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 1);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 7);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ADD);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 11);
    REQUIRE(node->left->type == NodeType::N_ADD);
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 5);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->col == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->col == 7);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 13);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_SUB);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 5);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 1);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 7);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_SUB);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 11);
    REQUIRE(node->left->type == NodeType::N_SUB);
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 5);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->col == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->col == 7);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 13);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_MUL);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 5);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 1);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 7);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_MUL);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 11);
    REQUIRE(node->left->type == NodeType::N_MUL);
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 5);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->col == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->col == 7);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 13);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_DIV);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 5);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 1);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 7);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_DIV);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 11);
    REQUIRE(node->left->type == NodeType::N_DIV);
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 5);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->col == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->col == 7);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 13);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_SUB);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 11);
    REQUIRE(node->left->type == NodeType::N_ADD);
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 5);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->col == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->col == 7);
    REQUIRE(node->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 13);
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
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_ADD);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 5);
    REQUIRE(node->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->left->row == 1);
    REQUIRE(node->left->col == 1);
    REQUIRE(node->right->type == NodeType::N_MUL);
    REQUIRE(node->right->row == 1);
    REQUIRE(node->right->col == 11);
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->right)->left)->value == "bar");
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->left->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->left->col == 7);
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->right)->right)->value == "baz");
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->right->row == 1);
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->right->col == 13);
  }
}

TEST_CASE("Parsing of signed number", "[parser]")
{
  SECTION("Positive number")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_ADD, 1, 1));
    input.push_back(Token(TokenType::T_INT, "1", 1, 2));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 3));
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_POS);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 1);
    REQUIRE(node->child->type == NodeType::N_INT);
    REQUIRE(static_cast<LeafASTNode *>(node->child)->value == "1");
    REQUIRE(node->child->row == 1);
    REQUIRE(node->child->col == 2);
  }

  SECTION("Negative number")
  {
    std::vector<Token> input;
    input.push_back(Token(TokenType::T_SUB, 1, 1));
    input.push_back(Token(TokenType::T_INT, "1", 1, 2));
    input.push_back(Token(TokenType::T_SEMICOLON, 1, 3));
    input.push_back(Token(TokenType::T_EOF, 2, 1));

    Parser parser(input);
    UnaryASTNode *node = static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_NEG);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 1);
    REQUIRE(node->child->type == NodeType::N_INT);
    REQUIRE(static_cast<LeafASTNode *>(node->child)->value == "1");
    REQUIRE(node->child->row == 1);
    REQUIRE(node->child->col == 2);
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
    input.push_back(Token(TokenType::T_EOF, 4, 1));

    Parser parser(input);
    FuncDefASTNode *node = static_cast<FuncDefASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_FUNC_DEF);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 1);
    REQUIRE(node->name->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->name)->value == "func");
    REQUIRE(node->name->row == 1);
    REQUIRE(node->name->col == 5);
    REQUIRE(node->ret_type->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->ret_type)->value == "int");
    REQUIRE(node->ret_type->row == 1);
    REQUIRE(node->ret_type->col == 13);
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
    input.push_back(Token(TokenType::T_EOF, 4, 1));

    Parser parser(input);
    FuncDefASTNode *node = static_cast<FuncDefASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(node->type == NodeType::N_FUNC_DEF);
    REQUIRE(node->row == 1);
    REQUIRE(node->col == 1);
    REQUIRE(node->name->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->name)->value == "func");
    REQUIRE(node->name->row == 1);
    REQUIRE(node->name->col == 5);
    REQUIRE(node->ret_type->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->ret_type)->value == "int");
    REQUIRE(node->ret_type->row == 1);
    REQUIRE(node->ret_type->col == 35);
    REQUIRE(node->args.size() == 2);
    REQUIRE(node->args[0]->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->args[0]->left)->value == "arg1");
    REQUIRE(node->args[0]->left->row == 1);
    REQUIRE(node->args[0]->left->col == 10);
    REQUIRE(node->args[0]->right->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->args[0]->right)->value == "int");
    REQUIRE(node->args[0]->right->row == 1);
    REQUIRE(node->args[0]->right->col == 16);
    REQUIRE(node->args[1]->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->args[1]->left)->value == "arg2");
    REQUIRE(node->args[1]->left->row == 1);
    REQUIRE(node->args[1]->left->col == 21);
    REQUIRE(node->args[1]->right->type == NodeType::N_TYPE);
    REQUIRE(static_cast<LeafASTNode *>(node->args[1]->right)->value == "float");
    REQUIRE(node->args[1]->right->row == 1);
    REQUIRE(node->args[1]->right->col == 27);
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
    input.push_back(Token(TokenType::T_EOF, 5, 1));

    Parser parser(input);
    FuncDefASTNode *node = static_cast<FuncDefASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements.size() == 2);
    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[0]->type == NodeType::N_ADD);
    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[0]->row == 2);
    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[0]->col == 7);
    REQUIRE(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->left->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->left)->value == "arg1");
    REQUIRE(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->left->row == 2);
    REQUIRE(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->left->col == 2);
    REQUIRE(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->right->type == NodeType::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->right)->value == "arg2");
    REQUIRE(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->right->row == 2);
    REQUIRE(static_cast<BinaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[0])->right->col == 9);
    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[1]->type == NodeType::N_RET);
    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[1]->row == 3);
    REQUIRE(static_cast<StmtSeqASTNode *>(node->body)->statements[1]->col == 2);
    REQUIRE(static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[1])->child->type == NodeType::N_INT);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[1])->child)->value == "1");
    REQUIRE(static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[1])->child->row == 3);
    REQUIRE(static_cast<UnaryASTNode *>(static_cast<StmtSeqASTNode *>(node->body)->statements[1])->child->col == 9);
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
  input.push_back(Token(TokenType::T_EOF, 2, 1));

  Parser parser(input);
  FuncCallASTNode *node = static_cast<FuncCallASTNode *>(static_cast<StmtSeqASTNode *>(parser.parse())->statements[0]);

  REQUIRE(node->type == NodeType::N_FUNC_CALL);
  REQUIRE(node->row == 1);
  REQUIRE(node->col == 1);
  REQUIRE(node->name->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->name)->value == "func");
  REQUIRE(node->name->row == 1);
  REQUIRE(node->name->col == 1);
  REQUIRE(node->args[0]->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->args[0])->value == "foo");
  REQUIRE(node->args[0]->row == 1);
  REQUIRE(node->args[0]->col == 6);
  REQUIRE(node->args[1]->type == NodeType::N_ADD);
  REQUIRE(node->args[1]->row == 1);
  REQUIRE(node->args[1]->col == 13);
  REQUIRE(static_cast<BinaryASTNode *>(node->args[1])->left->type == NodeType::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->args[1])->left)->value == "1");
  REQUIRE(static_cast<BinaryASTNode *>(node->args[1])->left->row == 1);
  REQUIRE(static_cast<BinaryASTNode *>(node->args[1])->left->col == 11);
  REQUIRE(static_cast<BinaryASTNode *>(node->args[1])->right->type == NodeType::N_FLOAT);
  REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->args[1])->right)->value == "2.3");
  REQUIRE(static_cast<BinaryASTNode *>(node->args[1])->right->row == 1);
  REQUIRE(static_cast<BinaryASTNode *>(node->args[1])->right->col == 15);
}

TEST_CASE("Parsing of statement sequences", "[parser]")
{
  std::vector<Token> input;
  input.push_back(Token(TokenType::T_ID, "foo", 1, 1));
  input.push_back(Token(TokenType::T_SEMICOLON, 1, 5));
  input.push_back(Token(TokenType::T_ID, "bar", 2, 1));
  input.push_back(Token(TokenType::T_SEMICOLON, 2, 5));
  input.push_back(Token(TokenType::T_EOF, 3, 1));

  Parser parser(input);
  StmtSeqASTNode *node = static_cast<StmtSeqASTNode *>(parser.parse());

  REQUIRE(node->statements.size() == 2);
  REQUIRE(node->row == 1);
  REQUIRE(node->col == 1);
  REQUIRE(node->statements[0]->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->statements[0])->value == "foo");
  REQUIRE(node->statements[0]->row == 1);
  REQUIRE(node->statements[0]->col == 1);
  REQUIRE(node->statements[1]->type == NodeType::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node->statements[1])->value == "bar");
  REQUIRE(node->statements[1]->row == 2);
  REQUIRE(node->statements[1]->col == 1);
}
