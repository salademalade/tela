#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include "error/error.hpp"

TEST_CASE("Error class tests", "[error]")
{
  SECTION("Error without position in file")
  {
    Error e("Hello %s!", "world");

    REQUIRE(strcmp(e.what(), "Hello world!") == 0);
  }

  SECTION("Error with position in file")
  {
     Error e(1, 1, "Hello %s!", "world");

    REQUIRE(strcmp(e.what(), "Hello world!") == 0);
    REQUIRE(e.row == 1);
    REQUIRE(e.col == 1);
  }
}
