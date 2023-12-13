#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include "token/token.hpp"

TEST_CASE("Token class tests", "[token]")
{
  SECTION("Token without value")
  {
    Token tok(TokenType::T_ADD, 1, 1);

    REQUIRE(tok.type == TokenType::T_ADD);
    REQUIRE(tok.value == "");
    REQUIRE(tok.row == 1);
    REQUIRE(tok.col == 1);
    REQUIRE(strcmp(tok.str(), "+") == 0);
  }

  SECTION("Token with value")
  {
    Token tok(TokenType::T_INT, "1", 1, 1);

    REQUIRE(tok.type == TokenType::T_INT);
    REQUIRE(tok.value == "1");
    REQUIRE(tok.row == 1);
    REQUIRE(tok.col == 1);
    REQUIRE(strcmp(tok.str(), "1") == 0);
  }
}
