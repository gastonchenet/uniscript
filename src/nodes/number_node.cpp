#include "number_node.hpp"

NumberNode::NumberNode(double value, Position pos_start) : Node(pos_start, pos_start), value(value) {}

std::string NumberNode::as_string(int depth) const
{
  depth++;
  return "<NumberNode (" + std::to_string(value) + ")>";
}