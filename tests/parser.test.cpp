#include <catch2/catch.hpp>
#include <vector>
#include "token/token.hpp"
#include "parser/parser.hpp"
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf-ast-node.hpp"
#include "ast-node/binary-ast-node.hpp"

TEST_CASE("Parsing of integer", "[parser][int]")
{
  std::vector<Token> input;
  input.push_back(Token(Token::Type::T_INT, "1"));

  Parser parser(input);
  ASTNode *node = parser.parse();

  REQUIRE(node->type == ASTNode::Type::N_INT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "1");
}

TEST_CASE("Parsing of float", "[parser][float]")
{
  std::vector<Token> input;
  input.push_back(Token(Token::Type::T_FLOAT, "2.3"));

  Parser parser(input);
  ASTNode *node = parser.parse();

  REQUIRE(node->type == ASTNode::Type::N_FLOAT);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "2.3");
}

TEST_CASE("Parsing of identifier", "[parser][id]")
{
  std::vector<Token> input;
  input.push_back(Token(Token::Type::T_ID, "a"));

  Parser parser(input);
  ASTNode *node = parser.parse();

  REQUIRE(node->type == ASTNode::Type::N_ID);
  REQUIRE(static_cast<LeafASTNode *>(node)->value == "a");
}

TEST_CASE("Parsing of addition operator", "[parser][add]")
{
  SECTION("Single operator")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_ADD));
    input.push_back(Token(Token::Type::T_ID, "bar"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_ADD);
    REQUIRE(node->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
  }

  SECTION("Chain of operators")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_ADD));
    input.push_back(Token(Token::Type::T_ID, "bar"));
    input.push_back(Token(Token::Type::T_ADD));
    input.push_back(Token(Token::Type::T_ID, "baz"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_ADD);
    REQUIRE(node->left->type == ASTNode::Type::N_ADD);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }
}

TEST_CASE("Parsing of subtraction operator", "[parser][sub]")
{
  SECTION("Single operator")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_SUB));
    input.push_back(Token(Token::Type::T_ID, "bar"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_SUB);
    REQUIRE(node->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
  }

  SECTION("Chain of operators")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_SUB));
    input.push_back(Token(Token::Type::T_ID, "bar"));
    input.push_back(Token(Token::Type::T_SUB));
    input.push_back(Token(Token::Type::T_ID, "baz"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_SUB);
    REQUIRE(node->left->type == ASTNode::Type::N_SUB);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }
}

TEST_CASE("Parsing of multiplication operator", "[parser][mul]")
{
  SECTION("Single operator")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_MUL));
    input.push_back(Token(Token::Type::T_ID, "bar"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_MUL);
    REQUIRE(node->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
  }

  SECTION("Chain of operators")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_MUL));
    input.push_back(Token(Token::Type::T_ID, "bar"));
    input.push_back(Token(Token::Type::T_MUL));
    input.push_back(Token(Token::Type::T_ID, "baz"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_MUL);
    REQUIRE(node->left->type == ASTNode::Type::N_MUL);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }
}

TEST_CASE("Parsing of division operator", "[parser][div]")
{
  SECTION("Single operator")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_DIV));
    input.push_back(Token(Token::Type::T_ID, "bar"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_DIV);
    REQUIRE(node->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "bar");
  }

  SECTION("Chain of operators")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_DIV));
    input.push_back(Token(Token::Type::T_ID, "bar"));
    input.push_back(Token(Token::Type::T_DIV));
    input.push_back(Token(Token::Type::T_ID, "baz"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_DIV);
    REQUIRE(node->left->type == ASTNode::Type::N_DIV);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }
}

TEST_CASE("Parsing of combination of operators", "[parser][add][sub][mul][div]")
{
  SECTION("Combination of addition and subtraction")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_ADD));
    input.push_back(Token(Token::Type::T_ID, "bar"));
    input.push_back(Token(Token::Type::T_SUB));
    input.push_back(Token(Token::Type::T_ID, "baz"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_SUB);
    REQUIRE(node->left->type == ASTNode::Type::N_ADD);
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->left)->value == "foo");
    REQUIRE(static_cast<BinaryASTNode *>(node->left)->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->left)->right)->value == "bar");
    REQUIRE(node->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->right)->value == "baz");
  }

  SECTION("Combination of addition and multiplication")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_ADD));
    input.push_back(Token(Token::Type::T_ID, "bar"));
    input.push_back(Token(Token::Type::T_MUL));
    input.push_back(Token(Token::Type::T_ID, "baz"));

    Parser parser(input);
    BinaryASTNode *node = static_cast<BinaryASTNode *>(parser.parse());

    REQUIRE(node->type == ASTNode::Type::N_ADD);
    REQUIRE(node->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(node->left)->value == "foo");
    REQUIRE(node->right->type == ASTNode::Type::N_MUL);
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->left->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->right)->left)->value == "bar");
    REQUIRE(static_cast<BinaryASTNode *>(node->right)->right->type == ASTNode::Type::N_ID);
    REQUIRE(static_cast<LeafASTNode *>(static_cast<BinaryASTNode *>(node->right)->right)->value == "baz");
  }
}

TEST_CASE("Parsing invalid inputs")
{
  SECTION("Unclosed parentheses")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_LPAREN));
    input.push_back(Token(Token::Type::T_ID, "foo"));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected factor")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ID, "foo"));
    input.push_back(Token(Token::Type::T_ID, "bar"));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected operator")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_ADD));
    input.push_back(Token(Token::Type::T_ADD));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }

  SECTION("Unexpected EOF")
  {
    std::vector<Token> input;
    input.push_back(Token(Token::Type::T_LPAREN));

    Parser parser(input);
    REQUIRE_THROWS_AS(parser.parse(), Error);
  }
}
