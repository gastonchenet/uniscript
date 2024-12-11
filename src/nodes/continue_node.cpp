#include "continue_node.hpp"

ContinueNode::ContinueNode(Position pos_start, Position pos_end) : Node(pos_start, pos_end) {}

std::string ContinueNode::as_string(int depth) const
{
  depth++;
  return "<ContinueNode>";
}