#include "lexer.hpp"

const char WHITESPACES[5] = " \n\t\r";
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
  std::string::iterator i = input.begin();
  std::vector<Token> output;

  while (i != input.end())
  {
    if (char_is_in(*i, WHITESPACES)) i++;
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
      }
      if (has_p) output.push_back(Token(Token::Type::T_FLOAT, num));
      else output.push_back(Token(Token::Type::T_INT, num));
    }
    else if (char_is_in(*i, LETTERS) || *i == '_')
    {
      std::string id("");
      while (char_is_in(*i, LETTERS) || char_is_in(*i, DIGITS) || *i == '_')
      {
        id.push_back(*i++);
      }
      if (id == "def")
      {
        output.push_back(Token(Token::Type::T_KEY_DEF));
      }
      else if (id == "int")
      {
        output.push_back(Token(Token::Type::T_KEY_INT));
      }
      else if (id == "float")
      {
        output.push_back(Token(Token::Type::T_KEY_FLOAT));
      }
      else
      {
        output.push_back(Token(Token::Type::T_ID, id));
      }
    }
    else if (*i == '+')
    {
      output.push_back(Token(Token::Type::T_ADD));
      i++;
    }
    else if (*i == '-')
    {
      output.push_back(Token(Token::Type::T_SUB));
      i++;
    }
    else if (*i == '*')
    {
      output.push_back(Token(Token::Type::T_MUL));
      i++;
    }
    else if (*i == '/')
    {
      output.push_back(Token(Token::Type::T_DIV));
      i++;
    }
    else if (*i == '=')
    {
      output.push_back(Token(Token::Type::T_ASSIGN));
      i++;
    }
    else if (*i == ':')
    {
      output.push_back(Token(Token::Type::T_COLON));
      i++;
    }
    else if (*i == ';')
    {
      output.push_back(Token(Token::Type::T_SEMICOLON));
      i++;
    }
    else if (*i == '(')
    {
      output.push_back(Token(Token::Type::T_LPAREN));
      i++;
    }
    else if (*i == ')')
    {
      output.push_back(Token(Token::Type::T_RPAREN));
      i++;
    }
    else if (*i == '{')
    {
      output.push_back(Token(Token::Type::T_LCURLY));
      i++;
    }
    else if (*i == '}')
    {
      output.push_back(Token(Token::Type::T_RCURLY));
      i++;
    }
    else throw new Error("Unexpected token: %c", *i);
  }

  return output;
}
