#include "token/token.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Token class tests", "[token]")
{
  SECTION("Token initialization without value")
  {
    Token tok(TokenType::T_ADD, 1, 1);

    REQUIRE(tok.type == TokenType::T_ADD);
    REQUIRE(tok.value == "");
    REQUIRE(tok.row == 1);
    REQUIRE(tok.col == 1);
    REQUIRE_FALSE(strcmp(tok.str(), "+"));
  }

  SECTION("Token initialization with value")
  {
    Token tok(TokenType::T_INT, "1", 1, 1);

    REQUIRE(tok.type == TokenType::T_INT);
    REQUIRE(tok.value == "1");
    REQUIRE(tok.row == 1);
    REQUIRE(tok.col == 1);
    REQUIRE_FALSE(strcmp(tok.str(), "1"));
  }

  SECTION("Token::is_in")
  {
    Token tok(TokenType::T_ADD, 1, 1);

    REQUIRE(tok.is_in(TokenType::T_ADD | TokenType::T_EOF));
    REQUIRE(tok.is_in(TokenType::T_ID | TokenType::T_ADD | TokenType::T_EOF));
    REQUIRE_FALSE(tok.is_in(TokenType::T_SUB | TokenType::T_EOF));
    REQUIRE_FALSE(tok.is_in(TokenType::T_SUB | TokenType::T_DIV | TokenType::T_EOF));
  }
}
