#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

class Token
{
public:
  enum class Type
  {
    T_INT,
    T_FLOAT,
    T_ID,

    T_KEY_DEF,   // def
    T_KEY_INT,   // int
    T_KEY_FLOAT, // float

    T_ADD,       // +
    T_SUB,       // -
    T_MUL,       // *
    T_DIV,       // /
    T_ASSIGN,    // =

    T_COLON,     // :
    T_SEMICOLON, // ;

    T_LPAREN,    // (
    T_RPAREN,    // )
    T_LCURLY,    // {
    T_RCURLY,    // }
  } type;
  std::string value;

  Token(Type type);
  Token(Type type, std::string value);
};

#endif
