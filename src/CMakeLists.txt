ADD_LIBRARY(tela-lib
  error/error.hpp
  error/error.cpp
)

ADD_EXECUTABLE(tela main.cpp)
TARGET_LINK_LIBRARIES(tela PRIVATE tela-lib)
