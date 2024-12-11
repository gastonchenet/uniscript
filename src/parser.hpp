#pragma once

#include <vector>
#include <memory>

#include "token.hpp"
#include "nodes/node.hpp"

struct Parser
{
public:
  std::vector<Token>& tokens;

  Parser(std::vector<Token>*);

  Node* parse();

private:
  Token* current_token;
  size_t index;

  void advance();

  Node* parse_atom();
  Node* parse_unary();
  Node* parse_power();
  Node* parse_term();
  Node* parse_arith();
  Node* parse_comp();
  Node* parse_expr();
  Node* parse_statement();
  Node* parse_block();
  Node* parse_if();
  Node* parse_while();
  Node* parse_for();
  Node* parse_func();
};
