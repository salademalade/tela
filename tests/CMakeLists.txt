FIND_PACKAGE(Catch2 CONFIG REQUIRED)

ADD_EXECUTABLE(tela-tests
  error.test.cpp
  token.test.cpp
  lexer.test.cpp
)
TARGET_LINK_LIBRARIES(tela-tests PRIVATE Catch2::Catch2WithMain)

TARGET_INCLUDE_DIRECTORIES(tela-tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
TARGET_LINK_LIBRARIES(tela-tests PRIVATE tela-lib)

INCLUDE(Catch)
CATCH_DISCOVER_TESTS(tela-tests)
