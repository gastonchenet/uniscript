#include "for_node.hpp"

ForNode::ForNode(Token* var_name, Node* start, Node* end, Node* step, Node* body, Position pos_start, Position pos_end) : Node(pos_start, pos_end), var_name(var_name), start(start), end(end), step(step), body(body) {}

std::string ForNode::as_string(int depth) const
{
  depth++;
  std::string str = "<ForNode (\n";

  if (var_name != nullptr)
  {
    str += std::string(depth * 2, ' ') + var_name->as_string() + ",\n";
  }

  str += std::string(depth * 2, ' ') + start->as_string(depth) + ",\n" + std::string(depth * 2, ' ') + end->as_string(depth) + ",\n";

  if (step != nullptr)
  {
    str += std::string(depth * 2, ' ') + step->as_string(depth) + ",\n";
  }

  str += std::string(depth * 2, ' ') + body->as_string(depth) + "\n" + std::string((depth - 1) * 2, ' ') + ")>";

  return str;
}