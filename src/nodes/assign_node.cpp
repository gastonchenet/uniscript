#include "assign_node.hpp"

AssignNode::AssignNode(const Token* left, Node* right, Position pos_start, Position pos_end) : Node(pos_start, pos_end), left(left), right(right) {}

std::string AssignNode::as_string(int depth) const
{
  depth++;
  return "<AssignNode (\n" + std::string(depth * 2, ' ') + left->as_string() + ",\n" + std::string(depth * 2, ' ') + right->as_string(depth) + "\n" + std::string((depth - 1) * 2, ' ') + ")>";
}