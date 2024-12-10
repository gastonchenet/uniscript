#include "block_node.hpp"

BlockNode::BlockNode(std::vector<Node*> nodes, Position pos_start, Position pos_end) : Node(pos_start, pos_end), nodes(nodes) {}

std::string BlockNode::as_string(int depth) const
{
  depth++;
  std::string str = "<BlockNode (";

  for (Node* node : nodes)
  {
    str += "\n" + std::string(depth * 2, ' ') + node->as_string(depth) + ",";
  }

  str += "\n" + std::string((depth - 1) * 2, ' ') + ")>";

  return str;
}