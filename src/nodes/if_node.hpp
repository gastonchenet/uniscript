#pragma once

#include "node.hpp"

struct IfNode : Node
{
  Node* condition;
  Node* body;
  Node* else_body;

  IfNode(Node*, Node*, Node*, Position, Position);
  IfNode(Node*, Node*, Position, Position);

  std::string as_string(int depth = 0) const override;
};
