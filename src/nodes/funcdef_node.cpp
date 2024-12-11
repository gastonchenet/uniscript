#include "funcdef_node.hpp"

FuncdefNode::FuncdefNode(const Token* name, std::vector<const Token*> args, Node* body, Position pos_start, Position pos_end) : Node(pos_start, pos_end), name(name), args(args), body(body) {}

std::string FuncdefNode::as_string(int depth) const
{
  depth++;
  std::string str = "<FuncdefNode (\n";
  str += std::string(depth * 2, ' ') + std::get<std::string>(name->value.value()) + ",\n";

  for (const Token* arg : args)
  {
    str += std::string(depth * 2, ' ') + std::get<std::string>(arg->value.value()) + ",\n";
  }

  str += std::string(depth * 2, ' ') + body->as_string(depth) + "\n";
  str += std::string((depth - 1) * 2, ' ') + ")>";
  return str;
}