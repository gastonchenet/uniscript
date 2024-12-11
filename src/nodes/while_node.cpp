#include "while_node.hpp"

WhileNode::WhileNode(Node* condition, Node* body, Position pos_start, Position pos_end) : Node(pos_start, pos_end), condition(condition), body(body) {}

std::string WhileNode::as_string(int depth) const
{
  depth++;
  return "<WhileNode (\n" + std::string(depth * 2, ' ') + condition->as_string(depth) + ",\n" + std::string(depth * 2, ' ') + body->as_string(depth) + "\n" + std::string((depth - 1) * 2, ' ') + ")>";
}