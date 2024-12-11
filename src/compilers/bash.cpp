#include "bash.hpp"

#include <iostream>

BashCompiler::BashCompiler(Node* node) : Compiler(node) {}

std::string BashCompiler::compile()
{
  output_s += "#!/bin/bash\n";
  Value result = Compiler::visit(root, -1);
  return output_s;
}

Value BashCompiler::visit(NumberNode* node, int depth)
{
  depth++;
  std::string value = std::to_string(node->value);

  value.erase(value.find_last_not_of('0') + 1, std::string::npos);
  value.erase(value.find_last_not_of('.') + 1, std::string::npos);

  return Value(Value::Type::Number, value);
}

Value BashCompiler::visit(StringNode* node, int depth)
{
  depth++;
  return Value(Value::Type::String, "\"" + node->value + "\"");
}

Value BashCompiler::visit(UnaryNode* node, int depth)
{
  Token::Type op_token_type = node->op->type;
  Value value = Compiler::visit(node->node, depth);
  
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

Value BashCompiler::visit(BinaryNode* node, int depth)
{
  Node* left_n = node->left;
  Node* right_n = node->right;

  Value left_v = Compiler::visit(left_n, depth);
  Value right_v = Compiler::visit(right_n, depth);

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

    case Token::Type::Pow:
      result += '^';
      break;

    case Token::Type::LessThan:
      result += '<';
      break;

    case Token::Type::LessThanOrEqual:
      result += "<=";
      break;

    case Token::Type::GreaterThan:
      result += '>';
      break;

    case Token::Type::GreaterThanOrEqual:
      result += ">=";
      break;

    case Token::Type::Equals:
      result += "==";
      break;

    case Token::Type::NotEquals:
      result += "!=";
      break;

    case Token::Type::And:
      result += "&&";
      break;

    case Token::Type::Or:
      result += "||";
      break;
    
    default:
      throw std::runtime_error("Invalid binary operation");
    }

    result += right_v.content;
    result += "\"|bc -l)";

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

Value BashCompiler::visit(AssignNode* node, int depth)
{
  Value result = Compiler::visit(node->right, depth);
  std::string var_name = std::get<std::string>(node->left->value.value());
  std::string var;

  if (symbol_table.find(var_name) != symbol_table.end())
  {
    var = symbol_table[var_name].content;
  }
  else
  {
    var = new_var();
  }

  symbol_table[var_name] = Value(result.type, var);
  output_s += std::string(depth * 2, ' ') + var + "=" + result.content + "\n";
  return Value();
}

Value BashCompiler::visit(AccessNode* node, int depth)
{
  depth++;
  std::string var_name = std::get<std::string>(node->token->value.value());
  Value var = symbol_table[var_name];

  if (var.type == Value::Type::Void)
  {
    throw std::runtime_error("Variable not found: " + var_name);
  }

  return Value(var.type, "${" + var.content + '}');
}

Value BashCompiler::visit(PutNode* node, int depth)
{
  Value result = Compiler::visit(node->expr, depth);
  output_s += std::string(depth * 2, ' ') + "echo \"" + result.content + "\"\n";
  return Value();
}

Value BashCompiler::visit(BlockNode* node, int depth)
{
  for (Node* child : node->nodes)
  {
    Value result = Compiler::visit(child, depth + 1);
  }

  return Value();
}

Value BashCompiler::visit(IfNode* node, int depth)
{
  Value condition = Compiler::visit(node->condition, depth);
  output_s += std::string(depth * 2, ' ') + "if [ " + condition.content + " -ne 0 ]; then\n";
  Value result = Compiler::visit(node->body, depth);

  if (node->else_body != nullptr)
  {
    output_s += std::string(depth * 2, ' ') + "else\n";
    result = Compiler::visit(node->else_body, depth);
  }

  output_s += std::string(depth * 2, ' ') + "fi\n";
  return Value();
}

Value BashCompiler::visit(WhileNode* node, int depth)
{
  Value condition = Compiler::visit(node->condition, depth);
  output_s += std::string(depth * 2, ' ') + "while [ " + condition.content + " -ne 0 ]; do\n";
  Value result = Compiler::visit(node->body, depth);
  output_s += std::string(depth * 2, ' ') + "done\n";
  return Value();
}

Value BashCompiler::visit(ForNode* node, int depth)
{
  Value start = Compiler::visit(node->start, depth);
  Value end = Compiler::visit(node->end, depth);
  Value step = node->step != nullptr ? Compiler::visit(node->step, depth) : Value(Value::Type::Number, "1");

  std::string var = new_var();

  if (node->var_name != nullptr)
  {
    std::string var_name = std::get<std::string>(node->var_name->value.value());
    symbol_table[var_name] = Value(Value::Type::Number, var);
  }

  output_s += std::string(depth * 2, ' ') + "for " + var + " in $(seq " + start.content + " " + step.content + " " + end.content + "); do\n";
  Value result = Compiler::visit(node->body, depth);
  output_s += std::string(depth * 2, ' ') + "done\n";

  return Value();
}