#pragma once

#include "node.hpp"

struct ReturnNode : public Node
{
public:
  Node* node;

  ReturnNode(Node*, Position, Position);

  std::string as_string(int = 0) const override;
};