#ifndef LEXER_HPP
#define LEXER_HPP

#include "error/error.hpp"
#include "token/token.hpp"
#include <string>
#include <vector>

class Lexer {
  const char *filename;
  std::string input;

  std::string::iterator i;

  unsigned int row;
  unsigned int col;

  public:
  Lexer(std::string filename, std::string input);

  std::vector<Token> tokenize();

  private:
  Token lex_number();

  char lex_char();
};

#endif
