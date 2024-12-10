#include "access_node.hpp"

AccessNode::AccessNode(const Token* token, Position pos_start, Position pos_end) : Node(pos_start, pos_end), token(token) {}

std::string AccessNode::as_string(int depth) const
{
  depth++;
  return "<AccessNode (\n" + std::string(depth * 2, ' ') + token->as_string() + "\n" + std::string((depth - 1) * 2, ' ') + ")>";
}