#pragma once

#include "node.hpp"

struct PutNode : public Node
{
public:
  Node* expr;

  PutNode(Node*, Position, Position);

  std::string as_string(int depth = 0) const override;
};