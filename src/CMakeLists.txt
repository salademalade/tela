ADD_LIBRARY(tela-lib
  error/error.hpp
  error/error.cpp
  token/token.hpp
  token/token.cpp
)

ADD_EXECUTABLE(tela main.cpp)
TARGET_LINK_LIBRARIES(tela PRIVATE tela-lib)
