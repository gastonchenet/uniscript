#include "parser.hpp"
#include <stdexcept>
#include "nodes/number_node.hpp"
#include "nodes/string_node.hpp"
#include "nodes/unary_node.hpp"
#include "nodes/binary_node.hpp"
#include "nodes/put_node.hpp"
#include "nodes/assign_node.hpp"
#include "nodes/access_node.hpp"
#include "nodes/block_node.hpp"
#include "nodes/if_node.hpp"
#include "nodes/while_node.hpp"
#include "nodes/for_node.hpp"
#include "nodes/funcdef_node.hpp"
#include "nodes/call_node.hpp"
#include "nodes/return_node.hpp"
#include "nodes/break_node.hpp"
#include "nodes/continue_node.hpp"
#include "nodes/list_node.hpp"

#include <iostream>

Parser::Parser(std::vector<Token>* tokens) : tokens(*tokens), current_token(nullptr), index(0) {}

Node* Parser::parse()
{
  current_token = &tokens[index];

  Node* node = parse_block();

  if (node == nullptr)
  {
    throw std::runtime_error("Unexpected token: " + current_token->as_string());
  }

  return node;
}

void Parser::advance()
{
  current_token = &tokens[++index];  
}

Node* Parser::parse_atom()
{
  bool is_true;

  if (current_token->matches(Token::Type::Number))
  {
    Node* node = new NumberNode(std::get<double>(current_token->value.value()), current_token->position.copy());
    advance();
    return node;
  }
  else if (
    (is_true = current_token->matches(Token::Type::Keyword, "true")) ||
    current_token->matches(Token::Type::Keyword, "false")
  )
  {
    Node* node = new NumberNode(is_true, current_token->position.copy());
    advance();
    return node;
  }
  else if (current_token->matches(Token::Type::String))
  {
    Node* node = new StringNode(std::get<std::string>(current_token->value.value()), current_token->position.copy());
    advance();
    return node;
  }
  else if (current_token->matches(Token::Type::LParen))
  {
    advance();
    Node* node = parse_expr();

    if (!current_token->matches(Token::Type::RParen))
    {
      throw std::runtime_error("Expected ')', got: " + current_token->as_string());
    }

    advance();
    return node;
  }
  else if (current_token->matches(Token::Type::Identifier))
  {
    const Token* var_name = current_token;
    advance();

    if (current_token->matches(Token::Type::Assign))
    {
      advance();
      Node* right = parse_expr();
      return new AssignNode(var_name, right, var_name->position.copy(), current_token->position.copy());
    }
    else if (current_token->matches(Token::Type::LParen))
    {
      advance();
      std::vector<Node*> args;

      while (!current_token->matches(Token::Type::RParen))
      {
        Node* arg = parse_expr();
        args.push_back(arg);

        if (current_token->matches(Token::Type::Comma))
        {
          advance();
        }
        else if (!current_token->matches(Token::Type::RParen))
        {
          throw std::runtime_error("Expected ')' or ',' got: " + current_token->as_string());
        }
      }

      advance();
      return new CallNode(var_name, args, var_name->position.copy(), current_token->position.copy());
    }

    return new AccessNode(var_name, var_name->position.copy(), current_token->position.copy());
  }
  else if (current_token->matches(Token::Type::LBrace))
  {
    return parse_list();
  }
  else
  {
    throw std::runtime_error("Unexpected token: " + current_token->as_string());
  }
}

Node* Parser::parse_unary()
{
  bool is_not = current_token->matches(Token::Type::Not);

  if (current_token->matches(Token::Type::Plus) || current_token->matches(Token::Type::Minus) || is_not)
  {
    Position pos_start = current_token->position.copy();
    const Token* op_token = current_token;
    advance();
    Node* node;
    
    if (is_not)
    {
      node = parse_comp();
    }
    else
    {
      node = parse_unary();
    }

    return new UnaryNode(op_token, node, pos_start, current_token->position.copy());
  }

  return parse_power();
}

Node* Parser::parse_power()
{
  Node* left = parse_atom();

  if (current_token->matches(Token::Type::Pow))
  {
    Position pos_start = current_token->position.copy();
    const Token* op_token = current_token;
    advance();
    Node* right = parse_unary();
    return new BinaryNode(op_token, left, right, pos_start, current_token->position.copy());
  }

  return left;
}

Node* Parser::parse_term()
{
  Node* left = parse_unary();

  while (
    current_token->matches(Token::Type::Multiply) ||
    current_token->matches(Token::Type::Divide) ||
    current_token->matches(Token::Type::Modulo)
  )
  {
    Position pos_start = current_token->position.copy();
    const Token* op_token = current_token;
    advance();
    Node* right = parse_unary();
    left = new BinaryNode(op_token, left, right, pos_start, current_token->position.copy());
  }

  return left;
}

Node* Parser::parse_arith()
{
  Node* left = parse_term();

  while (
    current_token->matches(Token::Type::Plus) ||
    current_token->matches(Token::Type::Minus)
  )
  {
    Position pos_start = current_token->position.copy();
    const Token* op_token = current_token;
    advance();
    Node* right = parse_term();
    left = new BinaryNode(op_token, left, right, pos_start, current_token->position.copy());
  }

  return left;
}

Node* Parser::parse_comp()
{
  Node* left = parse_arith();

  while (
    current_token->matches(Token::Type::Equals) ||
    current_token->matches(Token::Type::NotEquals) ||
    current_token->matches(Token::Type::GreaterThan) ||
    current_token->matches(Token::Type::LessThan) ||
    current_token->matches(Token::Type::GreaterThanOrEqual) ||
    current_token->matches(Token::Type::LessThanOrEqual)
  )
  {
    Position pos_start = current_token->position.copy();
    const Token* op_token = current_token;
    advance();
    Node* right = parse_arith();
    left = new BinaryNode(op_token, left, right, pos_start, current_token->position.copy());
  }

  return left;
}

Node* Parser::parse_expr()
{
  Node* left = parse_comp();

  while (
    current_token->matches(Token::Type::And) ||
    current_token->matches(Token::Type::Or)
  )
  {
    Position pos_start = current_token->position.copy();
    const Token* op_token = current_token;
    advance();
    Node* right = parse_comp();
    left = new BinaryNode(op_token, left, right, pos_start, current_token->position.copy());
  }

  return left;
}

Node* Parser::parse_list()
{
  if (!current_token->matches(Token::Type::LBrace))
  {
    throw std::runtime_error("Expected '{' got: " + current_token->as_string());
  }

  Position pos_start = current_token->position.copy();
  advance();

  std::vector<Node*> elements;

  while (!current_token->matches(Token::Type::RBrace))
  {
    Node* element = parse_expr();
    elements.push_back(element);

    if (current_token->matches(Token::Type::Comma))
    {
      advance();
    }
    else if (!current_token->matches(Token::Type::RBrace))
    {
      throw std::runtime_error("Expected '}' or ',' got: " + current_token->as_string());
    }
  }

  advance();

  return new ListNode(elements, pos_start, current_token->position.copy());
}

Node* Parser::parse_statement()
{
  if (current_token->matches(Token::Type::Keyword, "put"))
  {
    advance();
    Node* expr = parse_expr();

    if (!current_token->matches(Token::Type::End))
    {
      throw std::runtime_error("Expected ';' got: " + current_token->as_string());
    }

    advance();

    return new PutNode(expr, expr->pos_start, current_token->position.copy());
  }

  if (current_token->matches(Token::Type::Keyword, "if"))
  {
    return parse_if();
  }

  if (current_token->matches(Token::Type::Keyword, "while"))
  {
    return parse_while();
  }

  if (current_token->matches(Token::Type::Keyword, "for"))
  {
    return parse_for();
  }

  if (current_token->matches(Token::Type::Keyword, "fn"))
  {
    return parse_func();
  }

  if (current_token->matches(Token::Type::Keyword, "continue"))
  {
    advance();

    if (!current_token->matches(Token::Type::End))
    {
      throw std::runtime_error("Expected ';' got: " + current_token->as_string());
    }

    advance();

    return new ContinueNode(current_token->position.copy(), current_token->position.copy());
  }

  if (current_token->matches(Token::Type::Keyword, "break"))
  {
    advance();

    if (!current_token->matches(Token::Type::End))
    {
      throw std::runtime_error("Expected ';' got: " + current_token->as_string());
    }

    advance();

    return new BreakNode(current_token->position.copy(), current_token->position.copy());
  }

  if (current_token->matches(Token::Type::Keyword, "return"))
  {
    advance();
    Node* expr = parse_expr();

    if (!current_token->matches(Token::Type::End))
    {
      throw std::runtime_error("Expected ';' got: " + current_token->as_string());
    }

    advance();

    return new ReturnNode(expr, expr->pos_start, current_token->position.copy());
  }

  Node* expr = parse_expr();

  if (!current_token->matches(Token::Type::End))
  {
    throw std::runtime_error("Expected ';' got: " + current_token->as_string());
  }

  advance();

  return expr;
}

Node* Parser::parse_block()
{
  std::vector<Node*> nodes;

  while (!current_token->matches(Token::Type::RBrace) && !current_token->matches(Token::Type::Eof))
  {
    Node* node = parse_statement();
    nodes.push_back(node);
  }

  return new BlockNode(nodes, nodes[0]->pos_start, nodes[nodes.size() - 1]->pos_end);
}

Node* Parser::parse_if()
{
  if (!current_token->matches(Token::Type::Keyword, "if"))
  {
    throw std::runtime_error("Expected 'if' got: " + current_token->as_string());
  }

  Position pos_start = current_token->position.copy();
  advance();

  Node* condition = parse_expr();

  if (!current_token->matches(Token::Type::LBrace))
  {
    throw std::runtime_error("Expected '{' got: " + current_token->as_string());
  }

  advance();

  Node* body = parse_block();
  Node* else_body = nullptr;

  if (!current_token->matches(Token::Type::RBrace))
  {
    throw std::runtime_error("Expected '}' got: " + current_token->as_string());
  }

  advance();

  if (current_token->matches(Token::Type::Keyword, "else"))
  {
    advance();

    if (current_token->matches(Token::Type::LBrace))
    {
      advance();

      else_body = parse_block();

      if (!current_token->matches(Token::Type::RBrace))
      {
        throw std::runtime_error("Expected '}' got: " + current_token->as_string());
      }

      advance();
    }
    else if (current_token->matches(Token::Type::Keyword, "if"))
    {
      else_body = parse_if();
      else_body = new BlockNode({ else_body }, else_body->pos_start, else_body->pos_end);
    }
    else
    {
      throw std::runtime_error("Expected '{' got: " + current_token->as_string());
    }
  }

  return new IfNode(condition, body, else_body, pos_start, current_token->position.copy());
}

Node* Parser::parse_while()
{
  if (!current_token->matches(Token::Type::Keyword, "while"))
  {
    throw std::runtime_error("Expected 'while' got: " + current_token->as_string());
  }

  Position pos_start = current_token->position.copy();
  advance();

  Node* condition = parse_expr();

  if (!current_token->matches(Token::Type::LBrace))
  {
    throw std::runtime_error("Expected '{' got: " + current_token->as_string());
  }

  advance();

  Node* body = parse_block();

  if (!current_token->matches(Token::Type::RBrace))
  {
    throw std::runtime_error("Expected '}' got: " + current_token->as_string());
  }

  advance();

  return new WhileNode(condition, body, pos_start, current_token->position.copy());
}

Node* Parser::parse_for()
{
  if (!current_token->matches(Token::Type::Keyword, "for"))
  {
    throw std::runtime_error("Expected 'for' got: " + current_token->as_string());
  }

  Position pos_start = current_token->position.copy();
  advance();
  
  Token* var_name = nullptr;

  if (current_token->matches(Token::Type::Identifier))
  {
    var_name = current_token;
    advance();

    if (!current_token->matches(Token::Type::Assign))
    {
      throw std::runtime_error("Expected '=' got: " + current_token->as_string());
    }

    advance();
  }

  Node* start = parse_expr();

  if (!current_token->matches(Token::Type::Keyword, "to"))
  {
    throw std::runtime_error("Expected 'to' got: " + current_token->as_string());
  }

  advance();

  Node* end = parse_expr();
  Node* step = nullptr;

  if (current_token->matches(Token::Type::Keyword, "step"))
  {
    advance();
    step = parse_expr();
  }

  if (!current_token->matches(Token::Type::LBrace))
  {
    throw std::runtime_error("Expected '{' got: " + current_token->as_string());
  }

  advance();

  Node* body = parse_block();

  if (!current_token->matches(Token::Type::RBrace))
  {
    throw std::runtime_error("Expected '}' got: " + current_token->as_string());
  }

  advance();

  return new ForNode(var_name, start, end, step, body, pos_start, current_token->position.copy());
}

Node* Parser::parse_func()
{
  Position pos_start = current_token->position.copy();
  advance();

  if (!current_token->matches(Token::Type::Identifier))
  {
    throw std::runtime_error("Expected identifier got: " + current_token->as_string());
  }

  const Token* name = current_token;
  advance();

  if (!current_token->matches(Token::Type::LParen))
  {
    throw std::runtime_error("Expected '(' got: " + current_token->as_string());
  }

  advance();

  std::vector<const Token*> args;

  while (!current_token->matches(Token::Type::RParen))
  {
    if (!current_token->matches(Token::Type::Identifier))
    {
      throw std::runtime_error("Expected identifier got: " + current_token->as_string());
    }

    args.push_back(current_token);
    advance();

    if (current_token->matches(Token::Type::Comma))
    {
      advance();
    }
    else if (!current_token->matches(Token::Type::RParen))
    {
      throw std::runtime_error("Expected ')' or ',' got: " + current_token->as_string());
    }
  }

  advance();

  if (!current_token->matches(Token::Type::LBrace))
  {
    throw std::runtime_error("Expected '{' got: " + current_token->as_string());
  }

  advance();

  Node* body = parse_block();

  if (!current_token->matches(Token::Type::RBrace))
  {
    throw std::runtime_error("Expected '}' got: " + current_token->as_string());
  }

  advance();

  return new FuncdefNode(name, args, body, pos_start, current_token->position.copy());
}