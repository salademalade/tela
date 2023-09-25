#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstring>
#include <string>
#include <vector>
#include "error/error.hpp"
#include "token/token.hpp"

class Lexer
{
public:
  std::string input;

  Lexer(std::string input);

  std::vector<Token> tokenize();
};

#endif
