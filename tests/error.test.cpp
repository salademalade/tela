#include <catch2/catch.hpp>
#include <cstring>
#include "error/error.hpp"

TEST_CASE("Error class tests", "[error]")
{
  Error e("Hello %s!", "world");

  REQUIRE_FALSE(strcmp(e.what(), "Hello world!"));
}
