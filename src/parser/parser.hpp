#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "error/error.hpp"
#include "token/token.hpp"
#include "ast-node/ast-node.hpp"
#include "ast-node/leaf-ast-node.hpp"
#include "ast-node/binary-ast-node.hpp"
#include "ast-node/unary-ast-node.hpp"
#include "ast-node/func-def-ast-node.hpp"
#include "ast-node/func-call-ast-node.hpp"
#include "ast-node/stmt-seq-ast-node.hpp"
#include "ast-node/null-ast-node.hpp"

class Parser
{
  std::vector<Token> input;
  std::vector<Token>::iterator i;
public:
  Parser(std::vector<Token> input);

  ASTNode *parse();
private:
  ASTNode *parse_block();
  ASTNode *parse_statement();
  ASTNode *parse_expression();
  ASTNode *parse_term();
  ASTNode *parse_factor();

  bool check_next(std::vector<TokenType> types);
};

#endif
