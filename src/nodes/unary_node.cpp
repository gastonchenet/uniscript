#include "unary_node.hpp"

UnaryNode::UnaryNode(const Token* op, Node* node, Position pos_start, Position pos_end) : Node(pos_start, pos_end), op(op), node(node) {}

std::string UnaryNode::as_string(int depth) const
{
  depth++;
  return "<UnaryNode (\n" + std::string(depth * 2, ' ') + op->as_string() + ",\n" + std::string(depth * 2, ' ') + node->as_string(depth) + "\n" + std::string((depth - 1) * 2, ' ') + ")>";
}