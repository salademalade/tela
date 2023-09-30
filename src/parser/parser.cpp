#include "parser/parser.hpp"

Parser::Parser(std::vector<Token> input)
{
  this->input = input;
  this->i = this->input.begin();
}

ASTNode *Parser::parse()
{
  return parse_expression();
}

ASTNode *Parser::parse_expression()
{
  ASTNode *term = parse_term();
  while (true)
  {
    if (i->type == Token::Type::T_ADD)
    {
      i++;
      ASTNode *left = term;
      ASTNode *right = parse_term();
      term = new BinaryASTNode(ASTNode::Type::N_ADD, left, right);
    }
    else if (i->type == Token::Type::T_SUB)
    {
      i++;
      ASTNode *left = term;
      ASTNode *right = parse_term();
      term = new BinaryASTNode(ASTNode::Type::N_SUB, left, right);
    }
    else return term;
  }
}

ASTNode *Parser::parse_term()
{
  ASTNode *factor = parse_factor();
  while (true)
  {
    if (i->type == Token::Type::T_MUL)
    {
      i++;
      ASTNode *left = factor;
      ASTNode *right = parse_factor();
      factor = new BinaryASTNode(ASTNode::Type::N_MUL, left, right);
    }
    else if (i->type == Token::Type::T_DIV)
    {
      i++;
      ASTNode *left = factor;
      ASTNode *right = parse_factor();
      factor = new BinaryASTNode(ASTNode::Type::N_DIV, left, right);
    }
    else return factor;
  }
}

ASTNode *Parser::parse_factor()
{
  if (i->type == Token::Type::T_INT) return new LeafASTNode(ASTNode::Type::N_INT, (i++)->value);
  else if (i->type == Token::Type::T_FLOAT) return new LeafASTNode(ASTNode::Type::N_FLOAT, (i++)->value);
  else if (i->type == Token::Type::T_ID) return new LeafASTNode(ASTNode::Type::N_ID, (i++)->value);
  else if (i->type == Token::Type::T_LPAREN)
  {
    i++;
    ASTNode *expr = parse_expression();
    if (i->type != Token::Type::T_RPAREN) throw Error("Unclosed parenthesis.");
    return expr;
  }
  else
  {
    throw Error("Unexpected token: %s", i->value.c_str());
  }
}
