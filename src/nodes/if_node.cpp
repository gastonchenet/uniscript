#include "if_node.hpp"

IfNode::IfNode(Node* condition, Node* body, Node* else_body, Position pos_start, Position pos_end) : Node(pos_start, pos_end), condition(condition), body(body), else_body(else_body) {}
IfNode::IfNode(Node* condition, Node* body, Position pos_start, Position pos_end) : Node(pos_start, pos_end), condition(condition), body(body), else_body(nullptr) {}

std::string IfNode::as_string(int depth) const
{
  depth++;
  std::string str = "IfNode(\n";
  str += std::string(depth * 2, ' ') + condition->as_string(depth);
  str += ",\n" + std::string(depth * 2, ' ') + body->as_string(depth);

  if (else_body != nullptr)
  {
    str += ",\n" + std::string(depth * 2, ' ') + else_body->as_string(depth);
  }

  str += "\n";
  str += std::string((depth - 1) * 2, ' ');
  str += ")";
  return str;
}
