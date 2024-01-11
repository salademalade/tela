#include "error/error.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstring>

TEST_CASE("Error class tests", "[error]")
{
  SECTION("Error without filename and position in file")
  {
    Error e("Hello world!");

    REQUIRE(e.filename == nullptr);
    REQUIRE(e.row == 0);
    REQUIRE(e.col == 0);
  }

  SECTION("Error with filename and position in file")
  {
    Error e("main.tela", 1, 1, "Hello world!");

    REQUIRE_FALSE(strcmp(e.filename, "main.tela"));
    REQUIRE(e.row == 1);
    REQUIRE(e.col == 1);
  }

  SECTION("Error with no arguments in format")
  {
    Error e("Hello!");

    REQUIRE_FALSE(strcmp(e.what(), "Hello!"));
  }

  SECTION("Error with char in format")
  {
    Error e("Hello %c!", 'a');

    REQUIRE_FALSE(strcmp(e.what(), "Hello a!"));
  }

  SECTION("Error with string in format")
  {
    Error e("Hello %s!", "world");

    REQUIRE_FALSE(strcmp(e.what(), "Hello world!"));
  }
}
