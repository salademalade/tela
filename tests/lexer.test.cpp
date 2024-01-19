#include "lexer/lexer.hpp"
#include "error/error.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

TEST_CASE("Tokenization of number", "[lexer]")
{
  SECTION("Decimal integer")
  {
    Lexer lexer("test.tl", "10000");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_NUMBER);
    REQUIRE(tokens[0].value == "10000");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 6);
  }

  SECTION("Integer with apostrophe")
  {
    Lexer lexer("test.tl", "10'000");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_NUMBER);
    REQUIRE(tokens[0].value == "10000");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 7);
  }

  SECTION("Binary integer")
  {
    Lexer lexer("test.tl", "0b1000");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_NUMBER);
    REQUIRE(tokens[0].value == "0b1000");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 7);
  }

  SECTION("Hexadecimal integer without higher-value digits")
  {
    Lexer lexer("test.tl", "0x1000");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_NUMBER);
    REQUIRE(tokens[0].value == "0x1000");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 7);
  }

  SECTION("Hexadecimal integer with higher-value digits")
  {
    Lexer lexer("test.tl", "0xaB00");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_NUMBER);
    REQUIRE(tokens[0].value == "0xaB00");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 7);
  }

  SECTION("Floating-point number")
  {
    Lexer lexer("test.tl", "1.0");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_NUMBER);
    REQUIRE(tokens[0].value == "1.0");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 4);
  }

  SECTION("Integer with illegal character")
  {
    Lexer lexer("test.tl", "0g000");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }

  SECTION("Binary integer with illegal character")
  {
    Lexer lexer("test.tl", "0b2000");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }

  SECTION("Hexadecimal integer with illegal character")
  {
    Lexer lexer("test.tl", "0xg000");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }

  SECTION("Float with illegal character")
  {
    Lexer lexer("test.tl", "1.0.1");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }

  SECTION("Non-decimal float")
  {
    Lexer lexer("test.tl", "0x0.1");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }
}

TEST_CASE("Tokenization of characters", "[lexer]")
{
  SECTION("Normal character")
  {
    Lexer lexer("test.tl", "\'a\'");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_CHAR);
    REQUIRE(tokens[0].value == "a");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 4);
  }

  SECTION("Escaped character")
  {
    Lexer lexer("test.tl", "\'\\a\'");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_CHAR);
    REQUIRE(tokens[0].value == "\a");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 5);
  }

  SECTION("Unclosed character")
  {
    Lexer lexer("test.tl", "\'a");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }
}

TEST_CASE("Tokenization of strings", "[lexer]")
{
  SECTION("Normal string")
  {
    Lexer lexer("test.tl", "\"Hello\"");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_STRING);
    REQUIRE(tokens[0].value == "Hello");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 8);
  }

  SECTION("String with escaped character")
  {
    Lexer lexer("test.tl", "\"Hello\\n\"");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_STRING);
    REQUIRE(tokens[0].value == "Hello\n");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 10);
  }

  SECTION("Unclosed string")
  {
    Lexer lexer("test.tl", "\'a");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }
}

TEST_CASE("Tokenization of identifiers", "[lexer]")
{
  SECTION("Identifier with letters")
  {
    Lexer lexer("test.tl", "abc");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_ID);
    REQUIRE(tokens[0].value == "abc");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 4);
  }

  SECTION("Identifier with numbers")
  {
    Lexer lexer("test.tl", "a123");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_ID);
    REQUIRE(tokens[0].value == "a123");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 5);
  }

  SECTION("Identifier with underscore")
  {
    Lexer lexer("test.tl", "_a");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::T_ID);
    REQUIRE(tokens[0].value == "_a");
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 1);

    REQUIRE(tokens[1].type == TokenType::T_EOF);
    REQUIRE(tokens[1].row == 1);
    REQUIRE(tokens[1].col == 3);
  }

  SECTION("Identifier with invalid character")
  {
    Lexer lexer("test.tl", "a$");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }
}

TEST_CASE("Tokenization of addition operator", "[lexer]")
{
  Lexer lexer("test.tl", "+");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_ADD);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of subtraction operator", "[lexer]")
{
  Lexer lexer("test.tl", "-");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_SUB);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of multiplication operator", "[lexer]")
{
  Lexer lexer("test.tl", "*");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_MUL);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of division operator", "[lexer]")
{
  Lexer lexer("test.tl", "/");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_DIV);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of modulo operator", "[lexer]")
{
  Lexer lexer("test.tl", "%");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_MOD);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_ASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of addition assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "+=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_ADDASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of subtraction assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "-=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_SUBASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of multiplication assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "*=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_MULASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of division assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "/=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_DIVASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of modulo assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "%=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_MODASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of bitwise and operator", "[lexer]")
{
  Lexer lexer("test.tl", "&");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_BAND);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of bitwise or operator", "[lexer]")
{
  Lexer lexer("test.tl", "|");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_BOR);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of bitwise xor operator", "[lexer]")
{
  Lexer lexer("test.tl", "^");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_BXOR);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of bitwise and assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "&=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_ANDASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of bitwise or assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "|=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_ORASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of bitwise xor assignment operator", "[lexer]")
{
  Lexer lexer("test.tl", "^=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_XORASSIGN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of incrementation operator", "[lexer]")
{
  Lexer lexer("test.tl", "++");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_INCR);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of decrementation operator", "[lexer]")
{
  Lexer lexer("test.tl", "--");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_DECR);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of boolean and operator", "[lexer]")
{
  Lexer lexer("test.tl", "&&");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_AND);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of boolean or operator", "[lexer]")
{
  Lexer lexer("test.tl", "||");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_OR);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of boolean not operator", "[lexer]")
{
  Lexer lexer("test.tl", "!");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_NOT);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of equality operator", "[lexer]")
{
  Lexer lexer("test.tl", "==");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_EQ);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of inequality operator", "[lexer]")
{
  Lexer lexer("test.tl", "!=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_NEQ);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of greater than operator", "[lexer]")
{
  Lexer lexer("test.tl", ">");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_GT);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of lesser than operator", "[lexer]")
{
  Lexer lexer("test.tl", "<");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_LT);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of greater than equality operator", "[lexer]")
{
  Lexer lexer("test.tl", ">=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_GEQ);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of lesser than equality operator", "[lexer]")
{
  Lexer lexer("test.tl", "<=");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_LEQ);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 3);
}

TEST_CASE("Tokenization of point", "[lexer]")
{
  Lexer lexer("test.tl", ".");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_POINT);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of comma", "[lexer]")
{
  Lexer lexer("test.tl", ",");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_COMMA);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of colon", "[lexer]")
{
  Lexer lexer("test.tl", ":");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_COLON);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of semicolon", "[lexer]")
{
  Lexer lexer("test.tl", ";");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_SEMICOLON);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of ellipsis", "[lexer]")
{
  Lexer lexer("test.tl", "...");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_ELLIPSIS);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 4);
}

TEST_CASE("Tokenization of left parenthesis", "[lexer]")
{
  Lexer lexer("test.tl", "(");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_LPAREN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of right parenthesis", "[lexer]")
{
  Lexer lexer("test.tl", ")");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_RPAREN);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of left curly bracket", "[lexer]")
{
  Lexer lexer("test.tl", "{");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_LCURLY);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of right curly bracket", "[lexer]")
{
  Lexer lexer("test.tl", "}");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_RCURLY);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of left square bracket", "[lexer]")
{
  Lexer lexer("test.tl", "[");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_LBRACKET);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of right square bracket", "[lexer]")
{
  Lexer lexer("test.tl", "]");
  auto tokens = lexer.tokenize();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::T_RBRACKET);
  REQUIRE(tokens[0].row == 1);
  REQUIRE(tokens[0].col == 1);

  REQUIRE(tokens[1].type == TokenType::T_EOF);
  REQUIRE(tokens[1].row == 1);
  REQUIRE(tokens[1].col == 2);
}

TEST_CASE("Tokenization of comments", "[lexer]")
{
  SECTION("Single-line comment")
  {
    Lexer lexer("test.tl", "// Hello!");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens[0].type == TokenType::T_EOF);
    REQUIRE(tokens[0].row == 1);
    REQUIRE(tokens[0].col == 10);
  }

  SECTION("Multi-line comment")
  {
    Lexer lexer("test.tl", "/* Hello\nworld! */");
    auto tokens = lexer.tokenize();

    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens[0].type == TokenType::T_EOF);
    REQUIRE(tokens[0].row == 2);
    REQUIRE(tokens[0].col == 10);
  }

  SECTION("Unclosed multi-line comment")
  {
    Lexer lexer("test.tl", "/* Hello");

    REQUIRE_THROWS_AS(lexer.tokenize(), Error);
  }
}
