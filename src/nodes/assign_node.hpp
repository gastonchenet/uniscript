#pragma once

#include "node.hpp"
#include "../token.hpp"

struct AssignNode : public Node
{
public:
  const Token* left;
  Node* right;

  AssignNode(const Token*, Node*, Position, Position);

  std::string as_string(int depth = 0) const override;
};