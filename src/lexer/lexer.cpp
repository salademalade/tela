#include "lexer.hpp"

const char WHITESPACES[5] = " \t\r";
const char DIGITS[11] = "0123456789";
const char LETTERS[53] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

bool char_is_in(char c, const char *list)
{
  for (int i = 0; i < strlen(list); i++)
  {
    if (c == list[i]) return true;
  }
  return false;
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
      row++;
      col = 1;
    }
    else if (char_is_in(*i, WHITESPACES))
    {
      i++;
      col++;
    }
    else if (char_is_in(*i, DIGITS) || *i == '.')
    {
      std::string num("");
      bool has_p = false;
      while (char_is_in(*i, DIGITS) || *i == '.')
      {
        if (*i == '.')
        {
          if (has_p) throw Error("Unexpected token: '.'");
          else has_p = true;
        }
        num.push_back(*i++);
        col++;
      }
      if (has_p) output.push_back(Token(TokenType::T_FLOAT, num, row, col));
      else output.push_back(Token(TokenType::T_INT, num, row, col));
    }
    else if (char_is_in(*i, LETTERS) || *i == '_')
    {
      std::string id("");
      while (char_is_in(*i, LETTERS) || char_is_in(*i, DIGITS) || *i == '_')
      {
        id.push_back(*i++);
        col++;
      }
      if (id == "def")
      {
        output.push_back(Token(TokenType::T_KEY_DEF, row, col));
        col++;
      }
      else if (id == "let")
      {
        output.push_back(Token(TokenType::T_KEY_LET, row, col));
        col++;
      }
      else if (id == "const")
      {
        output.push_back(Token(TokenType::T_KEY_CONST, row, col));
        col++;
      }
      else if (id == "int")
      {
        output.push_back(Token(TokenType::T_KEY_INT, row, col));
        col++;
      }
      else if (id == "float")
      {
        output.push_back(Token(TokenType::T_KEY_FLOAT, row, col));
        col++;
      }
      else if (id == "return")
      {
        output.push_back(Token(TokenType::T_KEY_RETURN, row, col));
        col++;
      }
      else
      {
        output.push_back(Token(TokenType::T_ID, id, row, col));
        col++;
      }
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
    else throw new Error("Unexpected token: %c", *i);
  }

  return output;
}
