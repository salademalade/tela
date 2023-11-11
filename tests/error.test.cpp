#include <catch2/catch.hpp>
#include <cstring>
#include "error/error.hpp"

TEST_CASE("Error class tests", "[error]")
{
  SECTION("Error without position in file")
  {
    Error e("Hello %s!", "world");

    REQUIRE_FALSE(strcmp(e.what(), "Hello world!"));
  }

  SECTION("Error with position in file")
  {
     Error e(1, 1, "Hello %s!", "world");

    REQUIRE_FALSE(strcmp(e.what(), "Hello world!"));
    REQUIRE(e.row == 1);
    REQUIRE(e.col == 1);
  }
}
