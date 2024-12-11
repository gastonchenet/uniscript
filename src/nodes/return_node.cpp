#include "return_node.hpp"

ReturnNode::ReturnNode(Node* node, Position pos_start, Position pos_end) : Node(pos_start, pos_end), node(node) {}

std::string ReturnNode::as_string(int depth) const
{
  depth++;
  std::string str = "<ReturnNode (\n";
  str += std::string(depth * 2, ' ') + node->as_string(depth) + "\n";
  str += std::string((depth - 1) * 2, ' ') + ")>";
  return str;
}