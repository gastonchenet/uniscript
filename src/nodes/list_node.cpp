#include "list_node.hpp"

ListNode::ListNode(std::vector<Node*> elements, Position pos_start, Position pos_end) : Node(pos_start, pos_end), elements(elements) {}

std::string ListNode::as_string(int depth) const
{
  depth++;
  std::string str = "<ListNode (\n";

  for (const Node* element : elements)
  {
    str += std::string(depth * 2, ' ') + element->as_string(depth) + ",\n";
  }

  str.pop_back();
  str.pop_back();
  str += "\n" + std::string((depth - 1) * 2, ' ') + ")>";

  return str;
}