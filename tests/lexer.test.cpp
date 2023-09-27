#include <catch2/catch.hpp>
#include "lexer/lexer.hpp"

TEST_CASE("Lexer tests", "[lexer]")
{
  SECTION("Individual tokens")
  {
    SECTION("Integers", "[int]")
    {
      SECTION("Single-digit integer")
      {
        Lexer lexer("1");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_INT, "1");
        Token out_token = output[0];

        REQUIRE(expected.type == out_token.type);
        REQUIRE(expected.value == out_token.value);
      }

      SECTION("Multi-digit integer")
      {
        Lexer lexer("1234");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_INT, "1234");
        Token out_token = output[0];

        REQUIRE(expected.type == out_token.type);
        REQUIRE(expected.value == out_token.value);
      }
    }

    SECTION("Floats", "[float]")
    {
      SECTION("Valid float")
      {
        Lexer lexer("1.2");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_FLOAT, "1.2");
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

    SECTION("Identifiers", "[id]")
    {
      SECTION("Identifier with letter")
      {
        Lexer lexer("a");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_ID, "a");
        Token out_token = output[0];

        REQUIRE(expected.type == out_token.type);
        REQUIRE(expected.value == out_token.value);
      }

      SECTION("Identifier with letter and number")
      {
        Lexer lexer("a1");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_ID, "a1");
        Token out_token = output[0];

        REQUIRE(expected.type == out_token.type);
        REQUIRE(expected.value == out_token.value);
      }

      SECTION("Identifier with underscore")
      {
        Lexer lexer("_");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_ID, "_");
        Token out_token = output[0];

        REQUIRE(expected.type == out_token.type);
        REQUIRE(expected.value == out_token.value);
      }

      SECTION("Identifier with underscore and letter")
      {
        Lexer lexer("_a");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_ID, "_a");
        Token out_token = output[0];

        REQUIRE(expected.type == out_token.type);
        REQUIRE(expected.value == out_token.value);
      }
    }

    SECTION("Addition Operator", "[add]")
    {
      Lexer lexer("+");
      std::vector<Token> output = lexer.tokenize();

      REQUIRE(output.size() == 1);

      Token expected(Token::Type::T_ADD, "+");
      Token out_token = output[0];

      REQUIRE(expected.type == out_token.type);
    }

    SECTION("Subtraction Operator", "[sub]")
    {
      Lexer lexer("-");
      std::vector<Token> output = lexer.tokenize();

      REQUIRE(output.size() == 1);

      Token expected(Token::Type::T_SUB, "-");
      Token out_token = output[0];

      REQUIRE(expected.type == out_token.type);
    }

    SECTION("Multiplication Operator", "[mul]")
    {
      Lexer lexer("*");
      std::vector<Token> output = lexer.tokenize();

      REQUIRE(output.size() == 1);

      Token expected(Token::Type::T_MUL, "*");
      Token out_token = output[0];

      REQUIRE(expected.type == out_token.type);
    }

    SECTION("Division Operator", "[div]")
    {
      Lexer lexer("/");
      std::vector<Token> output = lexer.tokenize();

      REQUIRE(output.size() == 1);

      Token expected(Token::Type::T_DIV, "/");
      Token out_token = output[0];

      REQUIRE(expected.type == out_token.type);
    }

    SECTION("Parentheses", "[paren]")
    {
      SECTION("Left parenthesis")
      {
        Lexer lexer("(");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_LPAREN, "(");
        Token out_token = output[0];

        REQUIRE(expected.type == out_token.type);
      }

      SECTION("Right parenthesis")
      {
        Lexer lexer(")");
        std::vector<Token> output = lexer.tokenize();

        REQUIRE(output.size() == 1);

        Token expected(Token::Type::T_RPAREN, ")");
        Token out_token = output[0];

        REQUIRE(expected.type == out_token.type);
      }
    }
  }

  SECTION("Combination of tokens")
  {
    Lexer lexer("1 + 23 - ( foo * bar / 4.5 )");
    std::vector<Token> output = lexer.tokenize();

    REQUIRE(output.size() == 11);

    std::vector<Token> expected = {
      Token(Token::Type::T_INT, "1"),
      Token(Token::Type::T_ADD),
      Token(Token::Type::T_INT, "23"),
      Token(Token::Type::T_SUB),
      Token(Token::Type::T_LPAREN),
      Token(Token::Type::T_ID, "foo"),
      Token(Token::Type::T_MUL),
      Token(Token::Type::T_ID, "bar"),
      Token(Token::Type::T_DIV),
      Token(Token::Type::T_FLOAT, "4.5"),
      Token(Token::Type::T_RPAREN),
    };

    for (int i = 0; i < output.size(); i++)
    {
      REQUIRE(expected[i].type == output[i].type);
      REQUIRE(expected[i].value == output[i].value);
    }
  }
}
