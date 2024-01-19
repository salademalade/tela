#include "lexer.hpp"

const char WHITESPACES[5] = " \n\t\r";
const char DIGITS[11]     = "0123456789";
const char LETTERS[53]    = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

bool char_is_in(char c, const char *list);

Lexer::Lexer(std::string filename, std::string input)
{
  this->filename = filename.c_str();
  this->input    = input;
  this->i        = this->input.begin();

  this->row      = 1;
  this->col      = 1;
}

std::vector<Token> Lexer::tokenize()
{
  std::vector<Token> output;

  while (this->i != this->input.end()) {
    if (*this->i == '\n') {
      this->i++;
      this->row++;
      this->col = 1;
    } else if (char_is_in(*this->i, WHITESPACES)) {
      this->i++;
      this->col++;
    } else if (char_is_in(*this->i, DIGITS)) {
      output.push_back(this->lex_number());
    } else if (char_is_in(*this->i, LETTERS) || *this->i == '_') {
      std::string id("");
      unsigned int pos = this->col;
      while (char_is_in(*this->i, LETTERS) || char_is_in(*this->i, DIGITS) || *this->i == '_') {
        id.push_back(*this->i++);
        this->col++;
      }
      output.push_back(Token(TokenType::T_ID, id, this->row, pos));
    } else if (*this->i == '\'') {
      unsigned int pos = this->col;
      this->i++;
      this->col++;
      char ch = this->lex_char();
      this->i++;
      this->col++;
      if (*this->i != '\'')
        throw Error(
            this->filename, this->row, this->col, "Invalid character: \'%c%c\'.", ch, *this->i);
      output.push_back(Token(TokenType::T_CHAR, std::string(1, ch), this->row, pos));
      this->i++;
      this->col++;
    } else if (*this->i == '\"') {
      unsigned int pos = this->col;
      std::string str("");
      while (this->col++, *++this->i != '\"')
        str.push_back(this->lex_char());
      output.push_back(Token(TokenType::T_STRING, str, this->row, pos));
      this->i++;
      this->col++;
    } else if (*this->i == '+') {
      if (*(this->i + 1) == '+') {
        output.push_back(Token(TokenType::T_INCR, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_ADDASSIGN, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_ADD, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '-') {
      if (*(this->i + 1) == '-') {
        output.push_back(Token(TokenType::T_DECR, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_SUBASSIGN, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_SUB, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '*') {
      if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_MULASSIGN, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_MUL, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '/') {
      if (*(this->i + 1) == '/') {
        while (this->i != input.end() && *this->i != '\n') {
          this->i++;
          this->col++;
        }
      } else if (*(this->i + 1) == '*') {
        while (*this->i != '*' || *(this->i + 1) != '/') {
          if (this->i == this->input.end())
            throw Error(this->filename, this->row, this->col, "Unclosed comment.");
          else if (*this->i == '\n') {
            this->col = 1;
            this->row++;
          } else
            this->col++;
          this->i++;
        }
        this->col += 2;
        this->i += 2;
      } else if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_DIVASSIGN, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_DIV, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '%') {
      if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_MODASSIGN, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_MOD, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '&') {
      if (*(this->i + 1) == '&') {
        output.push_back(Token(TokenType::T_AND, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_ANDASSIGN, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_BAND, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '|') {
      if (*(this->i + 1) == '|') {
        output.push_back(Token(TokenType::T_OR, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_ORASSIGN, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_BOR, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '^') {
      if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_XORASSIGN, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_BXOR, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '=') {
      if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_EQ, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_ASSIGN, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '!') {
      if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_NEQ, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_NOT, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '>') {
      if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_GEQ, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_GT, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '<') {
      if (*(this->i + 1) == '=') {
        output.push_back(Token(TokenType::T_LEQ, this->row, this->col));
        this->i += 2;
        this->col += 2;
      } else {
        output.push_back(Token(TokenType::T_LT, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == '.') {
      if (*(this->i + 1) == '.' && *(this->i + 2) == '.') {
        output.push_back(Token(TokenType::T_ELLIPSIS, this->row, this->col));
        this->i += 3;
        this->col += 3;
      } else {
        output.push_back(Token(TokenType::T_POINT, this->row, this->col++));
        this->i++;
      }
    } else if (*this->i == ',') {
      output.push_back(Token(TokenType::T_COMMA, this->row, this->col++));
      this->i++;
    } else if (*this->i == ':') {
      output.push_back(Token(TokenType::T_COLON, this->row, this->col++));
      this->i++;
    } else if (*this->i == ';') {
      output.push_back(Token(TokenType::T_SEMICOLON, this->row, this->col++));
      this->i++;
    } else if (*this->i == '(') {
      output.push_back(Token(TokenType::T_LPAREN, this->row, this->col++));
      this->i++;
    } else if (*this->i == ')') {
      output.push_back(Token(TokenType::T_RPAREN, this->row, this->col++));
      this->i++;
    } else if (*this->i == '{') {
      output.push_back(Token(TokenType::T_LCURLY, this->row, this->col++));
      this->i++;
    } else if (*this->i == '}') {
      output.push_back(Token(TokenType::T_RCURLY, this->row, this->col++));
      this->i++;
    } else if (*this->i == '[') {
      output.push_back(Token(TokenType::T_LBRACKET, this->row, this->col++));
      this->i++;
    } else if (*this->i == ']') {
      output.push_back(Token(TokenType::T_RBRACKET, this->row, this->col++));
      this->i++;
    } else
      throw Error(this->filename, this->row, this->col, "Unexpected token: %c", *this->i);
  }

  output.push_back(Token(TokenType::T_EOF, this->row, this->col));

  return output;
}

Token Lexer::lex_number()
{
  std::string num("");
  unsigned int pos = this->col;

  if (*this->i == '0') {
    if (*(this->i + 1) == 'x') {
      this->i += 2;
      this->col += 2;

      num = "0x";

      while (true) {
        if (*this->i == '\'') {
          this->i++;
          this->col++;
        } else if (char_is_in(*this->i, WHITESPACES) || this->i == this->input.end()) {
          return Token(TokenType::T_NUMBER, num, this->row, pos);
        } else if (char_is_in(*this->i, DIGITS) || char_is_in(*this->i, "ABCDEFabcdef")) {
          num.push_back(*this->i++);
          this->col++;
        } else
          throw Error(this->filename, this->row, this->col, "Unexpected token: %c", *this->i);
      }
    } else if (*(this->i + 1) == 'b') {
      this->i += 2;
      this->col += 2;

      num = "0b";

      while (true) {
        if (*this->i == '\'') {
          this->i++;
          this->col++;
        } else if (char_is_in(*this->i, WHITESPACES) || this->i == this->input.end()) {
          return Token(TokenType::T_NUMBER, num, this->row, pos);
        } else if (*this->i == '0' || *this->i == '1') {
          num.push_back(*this->i++);
          this->col++;
        } else
          throw Error(this->filename, this->row, this->col, "Unexpected token: %c", *this->i);
      }
    }
  }

  bool has_p = false;
  while (true) {
    if (*this->i == '\'') {
      this->i++;
      this->col++;
    } else if (char_is_in(*this->i, WHITESPACES) || this->i == this->input.end()) {
      return Token(TokenType::T_NUMBER, num, this->row, pos);
    } else if (*this->i == '.') {
      if (has_p)
        throw Error(this->filename, this->row, this->col, "Unexpected token: %c", *this->i);

      has_p = true;
      num.push_back(*this->i++);
      this->col++;
    } else if (char_is_in(*this->i, DIGITS)) {
      num.push_back(*this->i++);
      this->col++;
    } else
      throw Error(this->filename, this->row, this->col, "Unexpected token: %c", *this->i);
  }

  return Token(TokenType::T_NUMBER, num, row, pos);
}

char Lexer::lex_char()
{
  if (*this->i == '\\') {
    this->i++;
    this->col++;
    switch (*this->i) {
    case 'a':
      return '\a';
    case 'b':
      return '\b';
    case 'e':
      return '\x1B';
    case 'f':
      return '\f';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case 'v':
      return '\v';
    case '\\':
      return '\\';
    case '\'':
      return '\'';
    case '\"':
      return '\"';
    case '\?':
      return '\?';
    default:
      return *this->i;
    }
  }

  return *this->i;
}

bool char_is_in(char c, const char *list)
{
  for (int i = 0; i < strlen(list); i++) {
    if (c == list[i])
      return true;
  }
  return false;
}
