#include <catch2/catch.hpp>
#include "token/token.hpp"

TEST_CASE("Token class tests", "[token]")
{
  SECTION("Token without value")
  {
    Token tok(TokenType::T_ADD);

    REQUIRE(tok.type == TokenType::T_ADD);
    REQUIRE(tok.value == "");
  }

  SECTION("Token with value")
  {
    Token tok(TokenType::T_INT, "1");

    REQUIRE(tok.type == TokenType::T_INT);
    REQUIRE(tok.value == "1");
  }
}
