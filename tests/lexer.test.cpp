#include <catch2/catch.hpp>
#include "lexer/lexer.hpp"

TEST_CASE("Tokenization of integer", "[lexer][int]")
{
  SECTION("Single-digit integer")
  {
    Lexer lexer("1");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_INT, "1", 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("Multi-digit integer")
  {
    Lexer lexer("1234");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_INT, "1234", 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }
}

TEST_CASE("Tokenization of float", "[lexer][float]")
{
  SECTION("Valid float")
  {
    Lexer lexer("1.2");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_FLOAT, "1.2", 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("Invalid float")
  {
    Lexer lexer("1.2.3");
    REQUIRE_THROWS(lexer.tokenize());
  }
}

TEST_CASE("Tokenization of character", "[lexer][char]")
{
  SECTION("Valid character")
  {
    SECTION("Normal character")
    {
      Lexer lexer("\'a\'");
      std::vector<Token> output = lexer.tokenize();

      REQUIRE(output.size() == 2);

      Token expected(TokenType::T_CHAR, "a", 1, 1);
      Token out_token = output[0];

      REQUIRE(expected.type == out_token.type);
      REQUIRE(expected.value == out_token.value);
      REQUIRE(expected.row == 1);
      REQUIRE(expected.col == 1);
    }

    SECTION("Escaped character")
    {
      Lexer lexer("\'\n\'");
      std::vector<Token> output = lexer.tokenize();

      REQUIRE(output.size() == 2);

      Token expected(TokenType::T_CHAR, "\n", 1, 1);
      Token out_token = output[0];

      REQUIRE(expected.type == out_token.type);
      REQUIRE(expected.value == out_token.value);
      REQUIRE(expected.row == 1);
      REQUIRE(expected.col == 1);
    }
  }
}

TEST_CASE("Tokenization of string", "[lexer][str]")
{
  Lexer lexer("\"foo\"");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_STRING, "foo", 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.value == out_token.value);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of identifier", "[lexer][id]")
{
  SECTION("Identifier with letter")
  {
    Lexer lexer("a");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_ID, "a", 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("Identifier with letter and number")
  {
    Lexer lexer("a1");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_ID, "a1", 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("Identifier with underscore")
  {
    Lexer lexer("_");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_ID, "_", 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("Identifier with underscore and letter")
  {
    Lexer lexer("_a");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_ID, "_a", 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.value == out_token.value);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }
}

TEST_CASE("Tokenization of keywords", "[lexer]")
{
  SECTION("def")
  {
    Lexer lexer("def");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_DEF, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("let")
  {
    Lexer lexer("let");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_LET, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("const")
  {
    Lexer lexer("const");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_CONST, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("int")
  {
    Lexer lexer("int");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_INT, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("float")
  {
    Lexer lexer("float");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_FLOAT, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("char")
  {
    Lexer lexer("char");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_CHAR, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("string")
  {
    Lexer lexer("string");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_STRING, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("void")
  {
    Lexer lexer("void");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_VOID, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("return")
  {
    Lexer lexer("return");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_RETURN, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("import")
  {
    Lexer lexer("import");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_KEY_IMPORT, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }
}

TEST_CASE("Tokenization of addition operator", "[lexer][add]")
{
  Lexer lexer("+");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_ADD, 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of subtraction operator", "[lexer][sub]")
{
  Lexer lexer("-");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_SUB, 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of multiplication operator", "[lexer][mul]")
{
  Lexer lexer("*");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_MUL, 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of division operator", "[lexer][div]")
{
  Lexer lexer("/");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_DIV, 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of assignment operator", "[lexer][assign]")
{
  Lexer lexer("=");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_ASSIGN, 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of comma", "[lexer]")
{
  Lexer lexer(",");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_COMMA, 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of colon", "[lexer]")
{
  Lexer lexer(":");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_COLON, 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of semicolon", "[lexer]")
{
  Lexer lexer(";");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 2);

  Token expected(TokenType::T_SEMICOLON, 1, 1);
  Token out_token = output[0];

  REQUIRE(expected.type == out_token.type);
  REQUIRE(expected.row == 1);
  REQUIRE(expected.col == 1);
}

TEST_CASE("Tokenization of parentheses", "[lexer][paren]")
{
  SECTION("Left parenthesis")
  {
    Lexer lexer("(");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_LPAREN, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("Right parenthesis")
  {
    Lexer lexer(")");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_RPAREN, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }
}

TEST_CASE("Tokenization of curly brackets", "[lexer]")
{
  SECTION("Left bracket")
  {
    Lexer lexer("{");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_LCURLY, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }

  SECTION("Right bracket")
  {
    Lexer lexer("}");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 2);

    Token expected(TokenType::T_RCURLY, 1, 1);
    Token out_token = output[0];

    REQUIRE(expected.type == out_token.type);
    REQUIRE(expected.row == 1);
    REQUIRE(expected.col == 1);
  }
}

TEST_CASE("General lexer tests", "[lexer]")
{
  Lexer lexer("a = 1 + 23 - ( foo * bar / 4.5 ) ;");
  std::vector<Token> output = lexer.tokenize();

  REQUIRE(output.size() == 15);

  std::vector<Token> expected = {
    Token(TokenType::T_ID, "a", 1, 1),
    Token(TokenType::T_ASSIGN, 1, 3),
    Token(TokenType::T_INT, "1", 1, 5),
    Token(TokenType::T_ADD, 1, 7),
    Token(TokenType::T_INT, "23", 1, 9),
    Token(TokenType::T_SUB, 1, 12),
    Token(TokenType::T_LPAREN, 1, 14),
    Token(TokenType::T_ID, "foo", 1, 16),
    Token(TokenType::T_MUL, 1, 20),
    Token(TokenType::T_ID, "bar", 1, 22),
    Token(TokenType::T_DIV, 1, 26),
    Token(TokenType::T_FLOAT, "4.5", 1, 28),
    Token(TokenType::T_RPAREN, 1, 32),
    Token(TokenType::T_SEMICOLON, 1, 34),
  };

  for (int i = 0; i < output.size() - 1; i++)
  {
    CHECK(expected[i].type == output[i].type);
    CHECK(expected[i].value == output[i].value);
    CHECK(expected[i].row == output[i].row);
    CHECK(expected[i].col == output[i].col);
  }
}
