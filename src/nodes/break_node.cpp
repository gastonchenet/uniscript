#include "break_node.hpp"

BreakNode::BreakNode(Position pos_start, Position pos_end) : Node(pos_start, pos_end) {}

std::string BreakNode::as_string(int depth) const
{
  depth++;
  return "<BreakNode>";
}