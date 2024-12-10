#include "put_node.hpp"

PutNode::PutNode(Node* expr, Position pos_start, Position pos_end) : Node(pos_start, pos_end), expr(expr) {}

std::string PutNode::as_string(int depth) const
{
  depth++;
  return "<PutNode (\n" + std::string(depth * 2, ' ') + expr->as_string(depth) + "\n" + std::string((depth - 1) * 2, ' ') + ")>";
}