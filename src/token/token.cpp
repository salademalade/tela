#include "token.hpp"

Token::Token(Type type, unsigned int row, unsigned int col)
{
  this->type = type;
  this->value = "";

  this->row = row;
  this->col = col;
}

Token::Token(Type type, std::string value, unsigned int row, unsigned int col)
{
  this->type = type;
  this->value = value;

  this->row = row;
  this->col = col;
}
