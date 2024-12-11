#pragma once

#include "node.hpp"
#include "../token.hpp"

struct UnaryNode : public Node
{
public:
  const Token* op;
  Node* node;

  UnaryNode(const Token*, Node*, Position, Position);

  std::string as_string(int= 0) const override;
};