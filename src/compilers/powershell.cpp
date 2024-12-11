#include "powershell.hpp"

#include <iostream>

PowershellCompiler::PowershellCompiler(Node* node) : Compiler(node) {}

std::string PowershellCompiler::compile()
{
  output_s += "#!/usr/bin/env pwsh\n";
  Value result = Compiler::visit(root, -1);
  return output_s;
}

Value PowershellCompiler::visit(NumberNode* node, int depth)
{
  depth++;
  std::string value = std::to_string(node->value);

  value.erase(value.find_last_not_of('0') + 1, std::string::npos);
  value.erase(value.find_last_not_of('.') + 1, std::string::npos);

  return Value(Value::Type::Number, value);
}

Value PowershellCompiler::visit(StringNode* node, int depth)
{
  depth++;
  return Value(Value::Type::String, "\"" + node->value + "\"");
}

Value PowershellCompiler::visit(UnaryNode* node, int depth)
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

Value PowershellCompiler::visit(BinaryNode* node, int depth)
{
  Node* left_n = node->left;
  Node* right_n = node->right;

  Value left_v = Compiler::visit(left_n, depth);
  Value right_v = Compiler::visit(right_n, depth);

  if (left_v.type == Value::Type::Number && right_v.type == Value::Type::Number)
  {
    std::string result = "(";

    switch (node->op->type)
    {
    case Token::Type::Plus:                   result += left_v.content + "+" + right_v.content; break;
    case Token::Type::Minus:                  result += left_v.content + "-" + right_v.content; break;
    case Token::Type::Multiply:               result += left_v.content + "*" + right_v.content; break;
    case Token::Type::Divide:                 result += left_v.content + "/" + right_v.content; break;
    case Token::Type::Pow:                    result += "[Math]::Pow(" + left_v.content + "," + right_v.content + ")"; break;
    case Token::Type::Equals:                 result += left_v.content + "-eq" + right_v.content; break;
    case Token::Type::NotEquals:              result += left_v.content + "-ne" + right_v.content; break;
    case Token::Type::LessThan:               result += left_v.content + "-lt" + right_v.content; break;
    case Token::Type::LessThanOrEqual:        result += left_v.content + "-le" + right_v.content; break;
    case Token::Type::GreaterThan:            result += left_v.content + "-gt" + right_v.content; break;
    case Token::Type::GreaterThanOrEqual:     result += left_v.content + "-ge" + right_v.content; break;
    case Token::Type::And:                    result += left_v.content + "-and" + right_v.content; break;
    case Token::Type::Or:                     result += left_v.content + "-or" + right_v.content; break;
    default:                                  throw std::runtime_error("Invalid binary operation");
    }

    result += ")";
    return Value(Value::Type::Number, result);
  }
  else if (left_v.type == Value::Type::String && right_v.type == Value::Type::String)
  {
    std::string result = "(" + left_v.content;

    switch (node->op->type)
    {
    case Token::Type::Plus:       result += "+"; break;
    default:                      throw std::runtime_error("Invalid binary operation");
    }

    result += right_v.content + ")";
    return Value(Value::Type::String, result);
  }
  else
  {
    throw std::runtime_error("Invalid binary operation");
  }
}

Value PowershellCompiler::visit(AssignNode* node, int depth)
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
  output_s += std::string(depth * 2, ' ') + "$" + var + "=" + result.content + "\n";
  return Value();
}

Value PowershellCompiler::visit(AccessNode* node, int depth)
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

Value PowershellCompiler::visit(PutNode* node, int depth)
{
  Value result = Compiler::visit(node->expr, depth);
  output_s += std::string(depth * 2, ' ') + "Write-Output " + result.content + "\n";
  return Value();
}

Value PowershellCompiler::visit(BlockNode* node, int depth)
{
  for (Node* statement : node->nodes)
  {
    Compiler::visit(statement, depth + 1);
  }

  return Value();
}

Value PowershellCompiler::visit(IfNode* node, int depth)
{
  Value condition = Compiler::visit(node->condition, depth);
  output_s += std::string(depth * 2, ' ') + "if (" + condition.content + ") {\n";
  Compiler::visit(node->body, depth);
  output_s += std::string(depth * 2, ' ') + "}\n";

  if (node->else_body != nullptr)
  {
    output_s += std::string(depth * 2, ' ') + "else {\n";
    Compiler::visit(node->else_body, depth);
    output_s += std::string(depth * 2, ' ') + "}\n";
  }

  return Value();
}

Value PowershellCompiler::visit(WhileNode* node, int depth)
{
  Value condition = Compiler::visit(node->condition, depth);
  output_s += std::string(depth * 2, ' ') + "while (" + condition.content + ") {\n";
  Compiler::visit(node->body, depth);
  output_s += std::string(depth * 2, ' ') + "}\n";
  return Value();
}

Value PowershellCompiler::visit(ForNode* node, int depth)
{
  Value start = Compiler::visit(node->start, depth);
  Value end = Compiler::visit(node->end, depth);
  Value step = node->step != nullptr ? Compiler::visit(node->step, depth) : Value(Value::Type::Number, "1");
  std::string var_name = std::get<std::string>(node->var_name->value.value());
  std::string var = new_var();

  output_s += std::string(depth * 2, ' ') + "for ($" + var + "=" + start.content + "; $" + var + "-lt" + end.content + "; $" + var + "+=" + step.content + ") {\n";
  symbol_table[var_name] = Value(Value::Type::Number, var);
  Compiler::visit(node->body, depth);
  output_s += std::string(depth * 2, ' ') + "}\n";
  return Value();
}

Value PowershellCompiler::visit(CallNode* node, int depth)
{
  depth++;
  std::string name = std::get<std::string>(node->name->value.value());
  std::string args = "";

  for (Node* arg : node->args)
  {
    Value result = Compiler::visit(arg, depth);
    args += ' ' + result.content;
  }

  return Value(Value::Type::String, "(" + name + args + ")");
}

Value PowershellCompiler::visit(FuncdefNode* node, int depth)
{
  std::string name = std::get<std::string>(node->name->value.value());
  std::string args = "";
  std::vector<const Token*> arg_names = node->args;

  for (size_t i = 0; i < arg_names.size(); i++)
  {
    const Token* arg = arg_names[i];
    std::string var_name = std::get<std::string>(arg->value.value());
    std::string var = new_var();
    symbol_table[var_name] = Value(Value::Type::Number, var);
    args += "[Parameter(Position=" + std::to_string(i) + ")]$" + var + ",";
  }

  output_s += std::string(depth * 2, ' ') + "function " + name + " {\n";

  if (args.size() > 0)
  {
    args.pop_back();
    output_s += std::string((depth + 1) * 2, ' ') + "param(" + args + ")\n";
  }

  Compiler::visit(node->body, depth);
  output_s += std::string(depth * 2, ' ') + "}\n";

  return Value();
}

Value PowershellCompiler::visit(BreakNode* node, int depth)
{
  node = node;
  output_s += std::string(depth * 2, ' ') + "break\n";
  return Value();
}

Value PowershellCompiler::visit(ContinueNode* node, int depth)
{
  node = node;
  output_s += std::string(depth * 2, ' ') + "continue\n";
  return Value();
}

Value PowershellCompiler::visit(ReturnNode* node, int depth)
{
  Value result = Compiler::visit(node->node, depth);
  output_s += std::string(depth * 2, ' ') + "return " + result.content + "\n";
  return Value();
}