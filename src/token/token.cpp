#include "token.hpp"

Token::Token(Type type, unsigned int row, unsigned int col)
{
  this->type  = type;
  this->value = "";

  this->row   = row;
  this->col   = col;
}

Token::Token(Type type, std::string value, unsigned int row, unsigned int col)
{
  this->type  = type;
  this->value = value;

  this->row   = row;
  this->col   = col;
}

const char *Token::str()
{
  switch (this->type) {
  case Type::T_INT:
  case Type::T_FLOAT:
  case Type::T_CHAR:
  case Type::T_STRING:
  case Type::T_ID:
    return this->value.c_str();

  case Type::T_ADD:
    return "+";
  case Type::T_SUB:
    return "-";
  case Type::T_MUL:
    return "*";
  case Type::T_DIV:
    return "/";
  case Type::T_MOD:
    return "%";

  case Type::T_ASSIGN:
    return "=";
  case Type::T_PASSIGN:
    return "+=";
  case Type::T_SASSIGN:
    return "-=";
  case Type::T_MASSIGN:
    return "*=";
  case Type::T_DASSIGN:
    return "/=";
  case Type::T_MODASSIGN:
    return "%=";

  case Type::T_BAND:
    return "&";
  case Type::T_BOR:
    return "|";
  case Type::T_BXOR:
    return "^";

  case Type::T_ANDASSIGN:
    return "&=";
  case Type::T_ORASSIGN:
    return "|=";
  case Type::T_XORASSIGN:
    return "^=";

  case Type::T_INCR:
    return "++";
  case Type::T_DECR:
    return "--";

  case Type::T_AND:
    return "&&";
  case Type::T_OR:
    return "||";
  case Type::T_NOT:
    return "!";

  case Type::T_EQ:
    return "==";
  case Type::T_NEQ:
    return "!=";
  case Type::T_GT:
    return ">";
  case Type::T_LT:
    return "<";
  case Type::T_GEQ:
    return ">=";
  case Type::T_LEQ:
    return "<=";

  case Type::T_POINT:
    return ".";
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
  case Type::T_LBRACKET:
    return "[";
  case Type::T_RBRACKET:
    return "]";

  case Type::T_EOF:
    return "<EOF>";
  default:
    return "";
  }
}

bool Token::is_in(Type types) { return (bool)(this->type & types); }

TokenType operator&(TokenType l, TokenType r) { return (TokenType)((long)l & (long)r); }

TokenType operator|(TokenType l, TokenType r) { return (TokenType)((long)l | (long)r); }
