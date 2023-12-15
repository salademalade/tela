#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstring>
#include <cstdlib>
#include <cmath>

extern "C" void tela_print(char *);
extern "C" void tela_pint(int);
extern "C" void tela_pfloat(float);
extern "C" void tela_pchar(char);
extern "C" int tela_length(char *);
extern "C" char *tela_concat(char *, char*);
extern "C" float tela_cos(float);
extern "C" float tela_sin(float);
extern "C" float tela_tan(float);
extern "C" float tela_acos(float);
extern "C" float tela_asin(float);
extern "C" float tela_atan(float);
extern "C" float tela_atan2(float, float);

TEST_CASE("Tests for std/io.tela", "[stl]")
{
  SECTION("print(str: string): void")
  {
    SUCCEED("No test for print(str: string): void");
  }

  SECTION("pint(i: int): void")
  {
    SUCCEED("No test for pint(i: int): void");
  }

  SECTION("pfloat(f: float): void")
  {
    SUCCEED("No test for pfloat(f: float): void");
  }

  SECTION("pchar(c: char): void")
  {
    SUCCEED("No test for pchar(c: char): void");
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

TEST_CASE("Tests for std/math.tela", "[stl]")
{
  SECTION("cos(x: float): float")
  {
    CHECK_THAT(tela_cos(0), Catch::Matchers::WithinAbs(1.0f, 0.00001f));
    CHECK_THAT(tela_cos(1), Catch::Matchers::WithinAbs(0.54030f, 0.00001f));
    CHECK_THAT(tela_cos(M_PI), Catch::Matchers::WithinAbs(-1.0f, 0.00001f));
  }

  SECTION("sin(x: float): float")
  {
    CHECK_THAT(tela_sin(0.0f), Catch::Matchers::WithinAbs(0.0f, 0.00001f));
    CHECK_THAT(tela_sin(1.0f), Catch::Matchers::WithinAbs(0.84147f, 0.00001f));
    CHECK_THAT(tela_sin(M_PI), Catch::Matchers::WithinAbs(0.0f, 0.00001f));
  }

  SECTION("tan(x: float): float")
  {
    CHECK_THAT(tela_tan(0.0f), Catch::Matchers::WithinAbs(0.0f, 0.00001f));
    CHECK_THAT(tela_tan(1.0f), Catch::Matchers::WithinAbs(1.55740f, 0.00001f));
    CHECK_THAT(tela_tan(M_PI), Catch::Matchers::WithinAbs(0.0f, 0.00001f));
  }

  SECTION("acos(x: float): float")
  {
    CHECK_THAT(tela_acos(0.0f), Catch::Matchers::WithinAbs(1.57079f, 0.00001f));
    CHECK_THAT(tela_acos(0.5f), Catch::Matchers::WithinAbs(1.04719f, 0.00001f));
    CHECK_THAT(tela_acos(1.0f), Catch::Matchers::WithinAbs(0.0f, 0.00001f));
  }

  SECTION("asin(x: float): float")
  {
    CHECK_THAT(tela_asin(0.0f), Catch::Matchers::WithinAbs(0.0f, 0.00001f));
    CHECK_THAT(tela_asin(0.5f), Catch::Matchers::WithinAbs(0.52359f, 0.00001f));
    CHECK_THAT(tela_asin(1.0f), Catch::Matchers::WithinAbs(1.57079f, 0.00001f));
  }

  SECTION("atan(x: float): float")
  {
    CHECK_THAT(tela_atan(0.0f), Catch::Matchers::WithinAbs(0.0f, 0.00001f));
    CHECK_THAT(tela_atan(0.5f), Catch::Matchers::WithinAbs(0.46364f, 0.00001f));
    CHECK_THAT(tela_atan(1.0f), Catch::Matchers::WithinAbs(0.78539f, 0.00001f));
  }

  SECTION("atan2(x: float, y: float): float")
  {
    CHECK_THAT(tela_atan2(0.0f, 1.0f), Catch::Matchers::WithinAbs(0.0f, 0.00001f));
    CHECK_THAT(tela_atan2(1.0f, 2.0f), Catch::Matchers::WithinAbs(0.46364f, 0.00001f));
    CHECK_THAT(tela_atan2(1.0f, 1.0f), Catch::Matchers::WithinAbs(0.78539f, 0.00001f));
  }
}
