#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

const char *hello()
{
  return "Hello world!";
}

TEST_CASE("Hello!", "[hello]")
{
  REQUIRE(hello() == "Hello world!");
}
