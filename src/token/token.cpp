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

const char *Token::str()
{
  switch (this->type)
  {
  case Type::T_INT:
  case Type::T_FLOAT:
  case Type::T_ID:
    return this->value.c_str();
  case Type::T_KEY_DEF:
    return "def";
  case Type::T_KEY_LET:
    return "let";
  case Type::T_KEY_CONST:
    return "const";
  case Type::T_KEY_INT:
    return "int";
  case Type::T_KEY_FLOAT:
    return "float";
  case Type::T_KEY_RETURN:
    return "return";
  case Type::T_ADD:
    return "+";
  case Type::T_SUB:
    return "-";
  case Type::T_MUL:
    return "*";
  case Type::T_DIV:
    return ":";
  case Type::T_ASSIGN:
    return "=";
  case Type::T_COMMA:
    return ",";
  case Type::T_COLON:
    return ":";
  case Type::T_SEMICOLON:
    return ";";
  case Type::T_LPAREN:
    return "(";
  case Type::T_RPAREN:
    return ")";
  case Type::T_LCURLY:
    return "{";
  case Type::T_RCURLY:
    return "}";
  case Type::T_EOF:
    return "EOF";
  default:
    return "";
  }
}
