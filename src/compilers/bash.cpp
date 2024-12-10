#include "bash.hpp"

#include <iostream>

BashCompiler::BashCompiler(Node* node) : Compiler(node) {}

std::string BashCompiler::compile()
{
  Value result = Compiler::visit(root);
  return output_s;
}

Value BashCompiler::visit(NumberNode* node)
{
  std::string value = std::to_string(node->value);

  value.erase(value.find_last_not_of('0') + 1, std::string::npos);
  value.erase(value.find_last_not_of('.') + 1, std::string::npos);

  return Value(Value::Type::Number, value);
}

Value BashCompiler::visit(StringNode* node)
{
  return Value(Value::Type::String, node->value);
}

Value BashCompiler::visit(UnaryNode* node)
{
  Token::Type op_token_type = node->op->type;
  Value value = Compiler::visit(node->node);
  
  if (value.type != Value::Type::Number)
  {
    throw std::runtime_error("Expected number, got: " + value.content);
  }

  switch (op_token_type)
  { 
  case Token::Type::Plus:      return value;
  case Token::Type::Minus:     return Value(Value::Type::Number, "-" + value.content);
  default:                     throw std::runtime_error("Unknown unary operator");
  }

  return Value();
}

Value BashCompiler::visit(BinaryNode* node)
{
  Node* left_n = node->left;
  Node* right_n = node->right;

  Value left_v = Compiler::visit(left_n);
  Value right_v = Compiler::visit(right_n);

  if (left_v.type == Value::Type::Number && right_v.type == Value::Type::Number)
  {
    std::string result, operation = "";

    result += "$(echo \"";
    result += left_v.content;

    switch (node->op->type)
    {
    case Token::Type::Plus:
      result += '+';
      break;

    case Token::Type::Minus:
      result += '-';
      break;

    case Token::Type::Multiply:
      result += '*';
      break;

    case Token::Type::Divide:
      result += '/';
      break;

    case Token::Type::Modulo:
      result += '%';
      break;
    
    default:
      throw std::runtime_error("Invalid binary operation");
    }

    result += right_v.content;
    result += "\"|bc)";

    return Value(Value::Type::Number, result);
  }
  else if (node->op->type == Token::Type::Plus)
  {
    return Value(Value::Type::String, left_v.content + right_v.content);
  }
  else
  {
    throw std::runtime_error("Invalid binary operation");
  }
}

Value BashCompiler::visit(AssignNode* node)
{
  Value result = Compiler::visit(node->right);
  std::string var_name = std::get<std::string>(node->left->value.value());
  std::string var = new_var();
  symbol_table[var_name] = Value(result.type, "${" + var + '}');
  output_s += var + "=" + result.content + "\n";
  return Value();
}

Value BashCompiler::visit(AccessNode* node)
{
  return symbol_table[std::get<std::string>(node->token->value.value())];
}

Value BashCompiler::visit(PutNode* node)
{
  Value result = Compiler::visit(node->expr);
  output_s += "echo \"" + result.content + "\"\n";
  return Value();
}

Value BashCompiler::visit(BlockNode* node)
{
  for (Node* child : node->nodes)
  {
    Value result = Compiler::visit(child);
  }

  return Value();
}