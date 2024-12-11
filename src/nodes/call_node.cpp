#include "call_node.hpp"

CallNode::CallNode(const Token* name, std::vector<Node*> args, Position pos_start, Position pos_end) : Node(pos_start, pos_end), name(name), args(args) {}

std::string CallNode::as_string(int depth) const
{
  depth++;
  std::string str = "<CallNode (\n";
  str += std::string(depth * 2, ' ') + std::get<std::string>(name->value.value()) + ",\n";

  for (Node* arg : args)
  {
    str += std::string(depth * 2, ' ') + arg->as_string(depth) + ",\n";
  }

  str += std::string((depth - 1) * 2, ' ') + ")>";
  return str;
}