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
  while (!check_next({TokenType::T_RCURLY}))
  {
    if (i->type == TokenType::T_KEY_DEF)
    {
      ASTNode *name = new LeafASTNode(NodeType::N_ID, (++i)->value);
      FuncDefASTNode *func = new FuncDefASTNode(name);

      if ((++i)->type != TokenType::T_LPAREN) throw Error("Expected parenthesis.");

      while (i->type != TokenType::T_RPAREN)
      {
        if ((++i)->type != TokenType::T_ID) throw Error("Expected identifier.");
        std::string a_name = i->value;

        if ((++i)->type != TokenType::T_COLON) throw Error("Expected colon.");

        FuncRetType a_type;
        switch ((++i)->type)
        {
        case TokenType::T_KEY_INT:
          a_type = FuncRetType::R_INT;
          break;
        case TokenType::T_KEY_FLOAT:
          a_type = FuncRetType::R_FLOAT;
          break;
        default:
          throw Error("Expected type specifier.");
        }

        func->add_arg(a_name, a_type);

        i++;
        if (i->type != TokenType::T_COMMA && i->type != TokenType::T_RPAREN) throw Error("Expected comma.");
      }

      if ((++i)->type != TokenType::T_COLON) throw Error("Expected colon.");

      switch ((++i)->type)
      {
      case TokenType::T_KEY_INT:
        func->ret_type = FuncRetType::R_INT;
        break;
      case TokenType::T_KEY_FLOAT:
        func->ret_type = FuncRetType::R_FLOAT;
        break;
      default:
        throw Error("Expected type specifier.");
      }

      if ((++i)->type != TokenType::T_LCURLY) throw Error("Expected bracket.");

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

  return new UnaryASTNode(n_type, expr);
}

ASTNode *Parser::parse_typedecl()
{
  ASTNode *ass = parse_assignment();
  if (i->type != TokenType::T_COLON) return ass;
  LeafASTNode *type = new LeafASTNode(NodeType::N_TYPE, "");
  i++;
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
  return new BinaryASTNode(NodeType::N_TYPE_DECL, ass, type);
}

ASTNode *Parser::parse_assignment()
{
  ASTNode *expr = parse_expression();
  while (true)
  {
    if (i->type == TokenType::T_ASSIGN)
    {
      i++;
      if (expr->type != NodeType::N_ID) throw Error("Cannot assign value to expression.");
      ASTNode *left = expr;
      ASTNode *right = parse_expression();
      expr = new BinaryASTNode(NodeType::N_ASSIGN, left, right);
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
    else if (check_next({TokenType::T_ADD, TokenType::T_SUB, TokenType::T_ASSIGN, TokenType::T_SEMICOLON, TokenType::T_COLON, TokenType::T_COMMA, TokenType::T_RPAREN})) return factor;
    else throw Error("Unexpected token: %s", i->value.c_str());
  }
}

ASTNode *Parser::parse_factor()
{
  if (i->type == TokenType::T_INT) return new LeafASTNode(NodeType::N_INT, (i++)->value);
  else if (i->type == TokenType::T_FLOAT) return new LeafASTNode(NodeType::N_FLOAT, (i++)->value);
  else if (i->type == TokenType::T_ID)
  {
    ASTNode *id = new LeafASTNode(NodeType::N_ID, (i++)->value);

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
  else if (i == input.end()) return new NullASTNode();
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
