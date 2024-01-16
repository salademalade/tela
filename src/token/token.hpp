#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

class Token {
  public:
  enum class Type : long {
    T_NUMBER    = 0x0000000000000001,
    T_CHAR      = 0x0000000000000002,
    T_STRING    = 0x0000000000000004,
    T_ID        = 0x0000000000000008,

    T_ADD       = 0x0000000000000010, // +
    T_SUB       = 0x0000000000000020, // -
    T_MUL       = 0x0000000000000040, // *
    T_DIV       = 0x0000000000000080, // /
    T_MOD       = 0x0000000000000100, // %

    T_ASSIGN    = 0x0000000000000200, // =
    T_PASSIGN   = 0x0000000000000400, // +=
    T_SASSIGN   = 0x0000000000000800, // -=
    T_MASSIGN   = 0x0000000000001000, // *=
    T_DASSIGN   = 0x0000000000002000, // /=
    T_MODASSIGN = 0x0000000000004000, // %=

    T_BAND      = 0x0000000000008000, // &
    T_BOR       = 0x0000000000010000, // |
    T_BXOR      = 0x0000000000020000, // ^

    T_ANDASSIGN = 0x0000000000040000, // &=
    T_ORASSIGN  = 0x0000000000080000, // |=
    T_XORASSIGN = 0x0000000000100000, // ^=

    T_INCR      = 0x0000000000200000, // ++
    T_DECR      = 0x0000000000400000, // --

    T_AND       = 0x0000000000800000, // &&
    T_OR        = 0x0000000001000000, // ||
    T_NOT       = 0x0000000002000000, // !

    T_EQ        = 0x0000000004000000, // ==
    T_NEQ       = 0x0000000008000000, // !=
    T_GT        = 0x0000000010000000, // >
    T_LT        = 0x0000000020000000, // <
    T_GEQ       = 0x0000000040000000, // >=
    T_LEQ       = 0x0000000080000000, // <=

    T_POINT     = 0x0000000100000000, // .
    T_COMMA     = 0x0000000200000000, // ,
    T_COLON     = 0x0000000400000000, // :
    T_SEMICOLON = 0x0000000800000000, // ;

    T_LPAREN    = 0x0000001000000000, // (
    T_RPAREN    = 0x0000002000000000, // )
    T_LCURLY    = 0x0000004000000000, // {
    T_RCURLY    = 0x0000008000000000, // }
    T_LBRACKET  = 0x0000010000000000, // [
    T_RBRACKET  = 0x0000020000000000, // ]

    T_EOF       = 0x0000040000000000,
  } type;
  std::string value;

  unsigned int row;
  unsigned int col;

  Token(Type type, unsigned int row, unsigned int col);
  Token(Type type, std::string value, unsigned int row, unsigned int col);

  const char *str();
  bool is_in(Type types);
};

typedef Token::Type TokenType;

TokenType operator&(TokenType l, TokenType r);
TokenType operator|(TokenType l, TokenType r);

#endif
