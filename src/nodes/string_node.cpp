#include "string_node.hpp"

StringNode::StringNode(std::string value, Position pos_start) : Node(pos_start, pos_start), value(value) {}

std::string StringNode::as_string(int depth) const
{
  depth++;
  return "<StringNode (\"" + value + "\")>";
}