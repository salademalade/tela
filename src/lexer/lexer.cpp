#include "lexer.hpp"

const char WHITESPACES[4] = " \t\r";
const char DIGITS[11] = "0123456789";
const char LETTERS[53] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

bool char_is_in(char c, const char *list)
{
  for (unsigned int i = 0; i < strlen(list); i++)
  {
    if (c == list[i]) return true;
  }
  return false;
}

char get_char(std::string::iterator &i, unsigned int &col)
{
  if (*i == '\\')
  {
    i++;
    col++;
    switch (*i)
    {
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
      return *i;
    }
  }

  return *i;
}

Lexer::Lexer(std::string input)
{
  this->input = input;
}

std::vector<Token> Lexer::tokenize()
{
  unsigned int row = 1, col = 1;

  std::string::iterator i = input.begin();
  std::vector<Token> output;

  while (i != input.end())
  {
    if (*i == '\n')
    {
      i++;
      row++;
      col = 1;
    }
    else if (char_is_in(*i, WHITESPACES))
    {
      i++;
      col++;
    }
    else if (*i == '/' && *(i+1) == '/')
    {
      do
      {
        i++;
        col++;
      }
      while (*i != '\n');
      row++;
      col = 1;
    }
    else if (char_is_in(*i, DIGITS) || *i == '.')
    {
      std::string num("");
      unsigned int pos = col;
      bool has_p = false;
      while (char_is_in(*i, DIGITS) || *i == '.')
      {
        if (*i == '.')
        {
          if (has_p) throw Error(row, col, "Unexpected token: '.'");
          else has_p = true;
        }
        num.push_back(*i++);
        col++;
      }
      if (has_p) output.push_back(Token(TokenType::T_FLOAT, num, row, pos));
      else output.push_back(Token(TokenType::T_INT, num, row, pos));
    }
    else if (char_is_in(*i, LETTERS) || *i == '_')
    {
      std::string id("");
      unsigned int pos = col;
      while (char_is_in(*i, LETTERS) || char_is_in(*i, DIGITS) || *i == '_')
      {
        id.push_back(*i++);
        col++;
      }
      if (id == "def") output.push_back(Token(TokenType::T_KEY_DEF, row, pos));
      else if (id == "let") output.push_back(Token(TokenType::T_KEY_LET, row, pos));
      else if (id == "const") output.push_back(Token(TokenType::T_KEY_CONST, row, pos));
      else if (id == "int") output.push_back(Token(TokenType::T_KEY_INT, row, pos));
      else if (id == "float") output.push_back(Token(TokenType::T_KEY_FLOAT, row, pos));
      else if (id == "char") output.push_back(Token(TokenType::T_KEY_CHAR, row, pos));
      else if (id == "string") output.push_back(Token(TokenType::T_KEY_STRING, row, pos));
      else if (id == "void") output.push_back(Token(TokenType::T_KEY_VOID, row, pos));
      else if (id == "return") output.push_back(Token(TokenType::T_KEY_RETURN, row, pos));
      else if (id == "import") output.push_back(Token(TokenType::T_KEY_IMPORT, row, col));
      else output.push_back(Token(TokenType::T_ID, id, row, pos));
    }
    else if (*i == '\'')
    {
      unsigned int pos = col;
      i++;
      col++;
      char ch = get_char(i, col);
      i++;
      col++;
      if (*i != '\'') throw Error(row, col, "Invalid character: \'%c%c\'.", ch, *i);
      output.push_back(Token(TokenType::T_CHAR, std::string(1, ch), row, pos));
      i++;
      col++;
    }
    else if (*i == '\"')
    {
      unsigned int pos = col;
      std::string str("");
      while (col++, *++i != '\"') str.push_back(get_char(i, col));
      output.push_back(Token(TokenType::T_STRING, str, row, pos));
      i++;
      col++;
    }
    else if (*i == '+')
    {
      output.push_back(Token(TokenType::T_ADD, row, col));
      i++;
      col++;
    }
    else if (*i == '-')
    {
      output.push_back(Token(TokenType::T_SUB, row, col));
      i++;
      col++;
    }
    else if (*i == '*')
    {
      output.push_back(Token(TokenType::T_MUL, row, col));
      i++;
      col++;
    }
    else if (*i == '/')
    {
      output.push_back(Token(TokenType::T_DIV, row, col));
      i++;
      col++;
    }
    else if (*i == '=')
    {
      output.push_back(Token(TokenType::T_ASSIGN, row, col));
      i++;
      col++;
    }
    else if (*i == ',')
    {
      output.push_back(Token(TokenType::T_COMMA, row, col));
      i++;
      col++;
    }
    else if (*i == ':')
    {
      output.push_back(Token(TokenType::T_COLON, row, col));
      i++;
      col++;
    }
    else if (*i == ';')
    {
      output.push_back(Token(TokenType::T_SEMICOLON, row, col));
      i++;
      col++;
    }
    else if (*i == '(')
    {
      output.push_back(Token(TokenType::T_LPAREN, row, col));
      i++;
      col++;
    }
    else if (*i == ')')
    {
      output.push_back(Token(TokenType::T_RPAREN, row, col));
      i++;
      col++;
    }
    else if (*i == '{')
    {
      output.push_back(Token(TokenType::T_LCURLY, row, col));
      i++;
      col++;
    }
    else if (*i == '}')
    {
      output.push_back(Token(TokenType::T_RCURLY, row, col));
      i++;
      col++;
    }
    else throw Error(row, col, "Unexpected token: %c", *i);
  }

  output.push_back(Token(TokenType::T_EOF, row, col));

  return output;
}
