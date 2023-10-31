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
  while (!check_next({Token::Type::T_RCURLY}))
  {
    if (i->type == Token::Type::T_KEY_DEF)
    {
      ASTNode *name = new LeafASTNode(ASTNode::Type::N_ID, (++i)->value);
      FuncDefASTNode *func = new FuncDefASTNode(name);

      if ((++i)->type != Token::Type::T_LPAREN) throw Error("Expected parenthesis.");

      while (i->type != Token::Type::T_RPAREN)
      {
        if ((++i)->type != Token::Type::T_ID) throw Error("Expected identifier.");
        std::string a_name = i->value;

        if ((++i)->type != Token::Type::T_COLON) throw Error("Expected colon.");

        FuncDefASTNode::ReturnType a_type;
        switch ((++i)->type)
        {
        case Token::Type::T_KEY_INT:
          a_type = FuncDefASTNode::ReturnType::R_INT;
          break;
        case Token::Type::T_KEY_FLOAT:
          a_type = FuncDefASTNode::ReturnType::R_FLOAT;
          break;
        default:
          throw Error("Expected type specifier.");
        }

        func->add_arg(a_name, a_type);

        i++;
        if (i->type != Token::Type::T_COMMA && i->type != Token::Type::T_RPAREN) throw Error("Expected comma.");
      }

      if ((++i)->type != Token::Type::T_COLON) throw Error("Expected colon.");

      switch ((++i)->type)
      {
      case Token::Type::T_KEY_INT:
        func->ret_type = FuncDefASTNode::ReturnType::R_INT;
        break;
      case Token::Type::T_KEY_FLOAT:
        func->ret_type = FuncDefASTNode::ReturnType::R_FLOAT;
        break;
      default:
        throw Error("Expected type specifier.");
      }

      if ((++i)->type != Token::Type::T_LCURLY) throw Error("Expected bracket.");

      i++;
      func->body = parse_block();

      seq->statements.push_back(func);
    }
    else
    {
      ASTNode *stmt = parse_statement();
      if (i->type != Token::Type::T_SEMICOLON)
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
  if (i->type == TokenType::T_ID && (i+1)->type == TokenType::T_ASSIGN)
  {
    LeafASTNode *var = new LeafASTNode(NodeType::N_ID, i->value);
    i += 2;
    ASTNode *expr = parse_expression();
    return new BinaryASTNode(NodeType::N_ASSIGN, var, expr);
  }
  else if (i->type == Token::Type::T_KEY_RETURN)
  {
    i++;
    ASTNode *expr = parse_expression();
    return new UnaryASTNode(ASTNode::Type::N_RET, expr);
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
    else if (check_next({Token::Type::T_SEMICOLON, Token::Type::T_COMMA, Token::Type::T_RPAREN})) return term;
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
    else if (check_next({Token::Type::T_ADD, Token::Type::T_SUB, Token::Type::T_SEMICOLON, Token::Type::T_COMMA, Token::Type::T_RPAREN})) return factor;
    else throw Error("Unexpected token: %s", i->value.c_str());
  }
}

ASTNode *Parser::parse_factor()
{
  if (i->type == Token::Type::T_INT) return new LeafASTNode(ASTNode::Type::N_INT, (i++)->value);
  else if (i->type == Token::Type::T_FLOAT) return new LeafASTNode(ASTNode::Type::N_FLOAT, (i++)->value);
  else if (i->type == Token::Type::T_ID)
  {
    ASTNode *id = new LeafASTNode(ASTNode::Type::N_ID, (i++)->value);

    if ((i)->type != Token::Type::T_LPAREN) return id;

    FuncCallASTNode *func = new FuncCallASTNode(id);

    while (i->type != Token::Type::T_RPAREN)
    {
      i++;
      ASTNode *arg = parse_expression();
      func->add_arg(arg);
    }

    i++;

    return func;
  }
  else if (i->type == Token::Type::T_LPAREN)
  {
    i++;
    ASTNode *expr = parse_expression();
    if (i->type != TokenType::T_RPAREN) throw Error("Unclosed parenthesis.");
    return expr;
  }
  else if (i == input.end()) return new NullASTNode();
  else throw Error("Unexpected token: %s", i->value.c_str());
}

bool Parser::check_next(std::vector<Token::Type> types)
{
  if (i == input.end()) return true;

  for (auto type : types)
  {
    if (i->type == type) return true;
  }

  return false;
}
