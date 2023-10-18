#include "parser/parser.hpp"

Parser::Parser(std::vector<Token> input)
{
  this->input = input;
  this->i = this->input.begin();
}

ASTNode *Parser::parse()
{
  return parse_block();
}

ASTNode *Parser::parse_block()
{
  StmtSeqASTNode *seq = new StmtSeqASTNode();
  while (i != input.end())
  {
    ASTNode *stmt = parse_statement();
    if (i->type != TokenType::T_SEMICOLON)
    {
      throw Error("Expected semicolon.");
    }
    seq->statements.push_back(stmt);
    i++;
  }

  return seq;
}

ASTNode *Parser::parse_statement()
{
  if (i->type == TokenType::T_ID && (i+1)->type == TokenType::T_ASSIGN)
  {
    LeafASTNode *var = new LeafASTNode(NodeType::N_ID, i->value);
    i += 2;
    ASTNode *expr = parse_expression();
    return new BinaryASTNode(NodeType::N_ASSIGN, var, expr);
  }
  else return parse_expression();
}

ASTNode *Parser::parse_expression()
{
  ASTNode *term = parse_term();
  while (true)
  {
    if (i->type == TokenType::T_ADD)
    {
      i++;
      ASTNode *left = term;
      ASTNode *right = parse_term();
      term = new BinaryASTNode(NodeType::N_ADD, left, right);
    }
    else if (i->type == TokenType::T_SUB)
    {
      i++;
      ASTNode *left = term;
      ASTNode *right = parse_term();
      term = new BinaryASTNode(NodeType::N_SUB, left, right);
    }
    else if (i == input.end() || i->type == TokenType::T_SEMICOLON) return term;
    else throw Error("Unexpected token: %s", i->value.c_str());
  }
}

ASTNode *Parser::parse_term()
{
  ASTNode *factor = parse_factor();
  while (true)
  {
    if (i->type == TokenType::T_MUL)
    {
      i++;
      ASTNode *left = factor;
      ASTNode *right = parse_factor();
      factor = new BinaryASTNode(NodeType::N_MUL, left, right);
    }
    else if (i->type == TokenType::T_DIV)
    {
      i++;
      ASTNode *left = factor;
      ASTNode *right = parse_factor();
      factor = new BinaryASTNode(NodeType::N_DIV, left, right);
    }
    else if (i == input.end() || i->type == TokenType::T_ADD || i->type == TokenType::T_SUB || i->type == TokenType::T_SEMICOLON) return factor;
    else throw Error("Unexpected token: %s", i->value.c_str());
  }
}

ASTNode *Parser::parse_factor()
{
  if (i->type == TokenType::T_INT) return new LeafASTNode(NodeType::N_INT, (i++)->value);
  else if (i->type == TokenType::T_FLOAT) return new LeafASTNode(NodeType::N_FLOAT, (i++)->value);
  else if (i->type == TokenType::T_ID) return new LeafASTNode(NodeType::N_ID, (i++)->value);
  else if (i->type == TokenType::T_LPAREN)
  {
    i++;
    ASTNode *expr = parse_expression();
    if (i->type != TokenType::T_RPAREN) throw Error("Unclosed parenthesis.");
    return expr;
  }
  else if (i == input.end()) return new NullASTNode();
  else throw Error("Unexpected token: %s", i->value.c_str());
}
