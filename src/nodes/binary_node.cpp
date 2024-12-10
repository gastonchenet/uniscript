#include "binary_node.hpp"

BinaryNode::BinaryNode(const Token* op, Node* left, Node* right, Position pos_start, Position pos_end) : Node(pos_start, pos_end), op(op), left(left), right(right) {}

std::string BinaryNode::as_string(int depth) const
{
  depth++;
  return "<BinaryNode (\n" + std::string(depth * 2, ' ') + left->as_string(depth) + ",\n" + std::string(depth * 2, ' ') + op->as_string() + ",\n" + std::string(depth * 2, ' ') + right->as_string(depth) + "\n" + std::string((depth - 1) * 2, ' ') + ")>";
}