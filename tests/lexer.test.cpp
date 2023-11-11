#include <catch2/catch.hpp>
#include "lexer/lexer.hpp"

TEST_CASE("Tokenization of integer", "[lexer][int]")
{
  SECTION("Single-digit integer")
  {
    Lexer lexer("1");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_INT, "1");
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
  }

  SECTION("Multi-digit integer")
  {
    Lexer lexer("1234");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_INT, "1234");
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
  }
}

TEST_CASE("Tokenization of float", "[lexer][float]")
{
  SECTION("Valid float")
  {
    Lexer lexer("1.2");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_FLOAT, "1.2");
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
  }

  SECTION("Invalid float")
  {
    Lexer lexer("1.2.3");
    REQUIRE_THROWS(lexer.tokenize());
  }
}

TEST_CASE("Tokenization of identifier", "[lexer][id]")
{
  SECTION("Identifier with letter")
  {
    Lexer lexer("a");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_ID, "a");
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
  }

  SECTION("Identifier with letter and number")
  {
    Lexer lexer("a1");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_ID, "a1");
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
  }

  SECTION("Identifier with underscore")
  {
    Lexer lexer("_");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_ID, "_");
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
  }

  SECTION("Identifier with underscore and letter")
  {
    Lexer lexer("_a");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_ID, "_a");
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
  }
}

TEST_CASE("Tokenization of keywords", "[lexer]")
{
  SECTION("def")
  {
    Lexer lexer("def");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_KEY_DEF);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }

  SECTION("let")
  {
    Lexer lexer("let");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_KEY_LET);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }

  SECTION("const")
  {
    Lexer lexer("const");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_KEY_CONST);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }

  SECTION("int")
  {
    Lexer lexer("int");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_KEY_INT);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }

  SECTION("float")
  {
    Lexer lexer("float");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_KEY_FLOAT);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }

  SECTION("return")
  {
    Lexer lexer("return");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_KEY_RETURN);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }
}

TEST_CASE("Tokenization of addition operator", "[lexer][add]")
{
  Lexer lexer("+");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 1);

  Token expected(TokenType::T_ADD);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
}

TEST_CASE("Tokenization of subtraction operator", "[lexer][sub]")
{
  Lexer lexer("-");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 1);

  Token expected(TokenType::T_SUB);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
}

TEST_CASE("Tokenization of multiplication operator", "[lexer][mul]")
{
  Lexer lexer("*");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 1);

  Token expected(TokenType::T_MUL);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
}

TEST_CASE("Tokenization of division operator", "[lexer][div]")
{
  Lexer lexer("/");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 1);

  Token expected(TokenType::T_DIV);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
}

TEST_CASE("Tokenization of assignment operator", "[lexer][assign]")
{
  Lexer lexer("=");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 1);

  Token expected(TokenType::T_ASSIGN);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
}

TEST_CASE("Tokenization of comma", "[lexer]")
{
  Lexer lexer(",");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 1);

  Token expected(TokenType::T_COMMA);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
}

TEST_CASE("Tokenization of colon", "[lexer]")
{
  Lexer lexer(":");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 1);

  Token expected(TokenType::T_COLON);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
}

TEST_CASE("Tokenization of semicolon", "[lexer]")
{
  Lexer lexer(";");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 1);

  Token expected(TokenType::T_SEMICOLON);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
}

TEST_CASE("Tokenization of parentheses", "[lexer][paren]")
{
  SECTION("Left parenthesis")
  {
    Lexer lexer("(");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_LPAREN);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }

  SECTION("Right parenthesis")
  {
    Lexer lexer(")");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_RPAREN);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }
}

TEST_CASE("Tokenization of curly brackets", "[lexer]")
{
  SECTION("Left bracket")
  {
    Lexer lexer("{");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_LCURLY);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }

  SECTION("Right bracket")
  {
    Lexer lexer("}");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 1);

    Token expected(TokenType::T_RCURLY);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
  }
}

TEST_CASE("General lexer tests", "[lexer]")
{
  Lexer lexer("a = 1 + 23 - ( foo * bar / 4.5 ) ;");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 14);

  std::vector<Token> expected = {
    Token(TokenType::T_ID, "a"),
    Token(TokenType::T_ASSIGN),
    Token(TokenType::T_INT, "1"),
    Token(TokenType::T_ADD),
    Token(TokenType::T_INT, "23"),
    Token(TokenType::T_SUB),
    Token(TokenType::T_LPAREN),
    Token(TokenType::T_ID, "foo"),
    Token(TokenType::T_MUL),
    Token(TokenType::T_ID, "bar"),
    Token(TokenType::T_DIV),
    Token(TokenType::T_FLOAT, "4.5"),
    Token(TokenType::T_RPAREN),
    Token(TokenType::T_SEMICOLON),
  };

  for (int i = 0; i < output.size(); i++)
  {
    REQUIRE(expected[i].type == output[i].type);
    REQUIRE(expected[i].value == output[i].value);
  }
}
