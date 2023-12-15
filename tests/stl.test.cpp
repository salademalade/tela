#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <cstdlib>

extern "C" void tela_print(char *);
extern "C" int tela_length(char *);
extern "C" char *tela_concat(char *, char*);

TEST_CASE("Tests for std/io.tela", "[stl]")
{
  SECTION("print(str: string): void")
  {
    SUCCEED("No test for print(str: string): void");
  }
}

TEST_CASE("Tests for std/string.tela", "[stl]")
{
  SECTION("length(str: string): int")
  {
    char *str = strdup("foo");
    CHECK(tela_length(str) == 3);
    free(str);
  }

  SECTION("concat(str1: string, str2: string): string")
  {
    char *str1 = strdup("foo");
    char *str2 = strdup("bar");
    CHECK(!strcmp("foobar", tela_concat(str1, str2)));
    free(str1);
    free(str2);
  }
}
