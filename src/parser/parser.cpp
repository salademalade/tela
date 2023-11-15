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
  StmtSeqASTNode *seq = new StmtSeqASTNode(i->row, i->col);
  while (!check_next({TokenType::T_RCURLY}))
  {
    if (i->type == TokenType::T_KEY_DEF)
    {
      unsigned int row = i->row, col = i->col;
      i++;
      ASTNode *name = new LeafASTNode(NodeType::N_ID, i->value, i->row, i->col);
      FuncDefASTNode *func = new FuncDefASTNode(name, row, col);

      i++;
      if (i->type != TokenType::T_LPAREN) throw Error("Expected parenthesis.");

      i++;
      if (i->type != TokenType::T_RPAREN)
      {
        while (true)
        {
          BinaryASTNode *arg = static_cast<BinaryASTNode *>(parse_typedecl());
          func->add_arg(arg);

          if (i->type == TokenType::T_RPAREN) break;
          if (i->type != TokenType::T_COMMA) throw Error("Expected comma.");

          i++;
        }
      }

      i++;
      if (i->type != TokenType::T_COLON) throw Error("Expected colon.");

      i++;
      switch (i->type)
      {
      case TokenType::T_KEY_INT:
        func->ret_type = new LeafASTNode(NodeType::N_TYPE, "int", i->row, i->col);
        break;
      case TokenType::T_KEY_FLOAT:
        func->ret_type = new LeafASTNode(NodeType::N_TYPE, "float", i->row, i->col);
        break;
      default:
        throw Error("Expected type specifier.");
      }

      i++;
      if (i->type != TokenType::T_LCURLY) throw Error("Expected bracket.");

      i++;
      func->body = parse_block();

      seq->statements.push_back(func);
    }
    else
    {
      ASTNode *stmt = parse_statement();
      if (i->type != TokenType::T_SEMICOLON)
      {
        throw Error("Expected semicolon.");
      }
      seq->statements.push_back(stmt);
    }
    i++;
  }

  return seq;
}

ASTNode *Parser::parse_statement()
{
  NodeType n_type;
  unsigned int row = i->row, col = i->col;
  switch (i->type)
  {
  case TokenType::T_KEY_LET:
    n_type = NodeType::N_DECL;
    break;
  case TokenType::T_KEY_CONST:
    n_type = NodeType::N_DECL_CONST;
    break;
  case TokenType::T_KEY_RETURN:
    n_type = NodeType::N_RET;
    break;
  default:
    return parse_assignment();
  }

  i++;
  ASTNode *expr = parse_typedecl();

  return new UnaryASTNode(n_type, expr, row, col);
}

ASTNode *Parser::parse_typedecl()
{
  ASTNode *ass = parse_assignment();
  if (i->type != TokenType::T_COLON) return ass;
  unsigned int row = i->row, col = i->col;
  i++;
  LeafASTNode *type = new LeafASTNode(NodeType::N_TYPE, "", i->row, i->col);

  switch (i->type)
  {
  case TokenType::T_KEY_INT:
    type->value = "int";
    break;
  case TokenType::T_KEY_FLOAT:
    type->value = "float";
    break;
  default:
    throw Error("Invalid type.");
  }

  i++;
  return new BinaryASTNode(NodeType::N_TYPE_DECL, ass, type, row, col);
}

ASTNode *Parser::parse_assignment()
{
  ASTNode *expr = parse_expression();
  while (true)
  {
    if (i->type == TokenType::T_ASSIGN)
    {
      unsigned int row = i->row, col = i->col;
      i++;
      if (expr->type != NodeType::N_ID) throw Error("Cannot assign value to expression.");
      ASTNode *left = expr;
      ASTNode *right = parse_expression();
      expr = new BinaryASTNode(NodeType::N_ASSIGN, left, right, row, col);
    }
    else if (check_next({TokenType::T_SEMICOLON, TokenType::T_COLON})) return expr;
    else throw Error("Unexpected token: %s", i->value.c_str());
  }
}

ASTNode *Parser::parse_expression()
{
  ASTNode *term = parse_term();
  while (true)
  {
    if (i->type == TokenType::T_ADD)
    {
      unsigned int row = i->row, col = i->col;
      i++;
      ASTNode *left = term;
      ASTNode *right = parse_term();
      term = new BinaryASTNode(NodeType::N_ADD, left, right, row, col);
    }
    else if (i->type == TokenType::T_SUB)
    {
      unsigned int row = i->row, col = i->col;
      i++;
      ASTNode *left = term;
      ASTNode *right = parse_term();
      term = new BinaryASTNode(NodeType::N_SUB, left, right, row, col);
    }
    else if (check_next({TokenType::T_ASSIGN, TokenType::T_SEMICOLON, TokenType::T_COLON, TokenType::T_COMMA, TokenType::T_RPAREN})) return term;
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
      unsigned int row = i->row, col = i->col;
      i++;
      ASTNode *left = factor;
      ASTNode *right = parse_factor();
      factor = new BinaryASTNode(NodeType::N_MUL, left, right, row, col);
    }
    else if (i->type == TokenType::T_DIV)
    {
      unsigned int row = i->row, col = i->col;
      i++;
      ASTNode *left = factor;
      ASTNode *right = parse_factor();
      factor = new BinaryASTNode(NodeType::N_DIV, left, right, row, col);
    }
    else if (check_next({TokenType::T_ADD, TokenType::T_SUB, TokenType::T_ASSIGN, TokenType::T_SEMICOLON, TokenType::T_COLON, TokenType::T_COMMA, TokenType::T_RPAREN})) return factor;
    else throw Error("Unexpected token: %s", i->value.c_str());
  }
}

ASTNode *Parser::parse_factor()
{
  if (i->type == TokenType::T_INT)
  {
    LeafASTNode *node = new LeafASTNode(NodeType::N_INT, i->value, i->row, i->col);
    i++;
    return node;
  }
  else if (i->type == TokenType::T_FLOAT)
  {
    LeafASTNode *node = new LeafASTNode(NodeType::N_FLOAT, i->value, i->row, i->col);
    i++;
    return node;
  }
  else if (i->type == TokenType::T_ID)
  {
    ASTNode *id = new LeafASTNode(NodeType::N_ID, i->value, i->row, i->col);
    i++;

    if ((i)->type != TokenType::T_LPAREN) return id;

    FuncCallASTNode *func = new FuncCallASTNode(id);

    while (i->type != TokenType::T_RPAREN)
    {
      i++;
      ASTNode *arg = parse_expression();
      func->add_arg(arg);
    }

    i++;

    return func;
  }
  else if (i->type == TokenType::T_LPAREN)
  {
    i++;
    ASTNode *expr = parse_expression();
    if (i->type != TokenType::T_RPAREN) throw Error("Unclosed parenthesis.");
    return expr;
  }
  else if (i == input.end()) return new NullASTNode((i-1)->row+1, 1);
  else throw Error("Unexpected token: %s", i->value.c_str());
}

bool Parser::check_next(std::vector<TokenType> types)
{
  if (i == input.end()) return true;

  for (auto type : types)
  {
    if (i->type == type) return true;
  }

  return false;
}
