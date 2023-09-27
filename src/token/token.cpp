#include "token.hpp"

Token::Token(Type type)
{
  this->type = type;
  this->value = "";
}

Token::Token(Type type, std::string value)
{
  this->type = type;
  this->value = value;
}
