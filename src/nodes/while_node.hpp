#pragma once

#include "node.hpp"

struct WhileNode : public Node
{
public:
  Node* condition;
  Node* body;

  WhileNode(Node*, Node*, Position, Position);

  std::string as_string(int = 0) const override;
};