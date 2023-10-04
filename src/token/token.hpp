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
    T_ADD,    // +
    T_SUB,    // -
    T_MUL,    // *
    T_DIV,    // /
    T_LPAREN, // (
    T_RPAREN, // )
    T_ASSIGN  // =
  } type;
  std::string value;

  Token(Type type);
  Token(Type type, std::string value);
};

#endif
