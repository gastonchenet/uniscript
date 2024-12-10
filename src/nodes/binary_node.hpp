#pragma once

#include "node.hpp"
#include "../token.hpp"

struct BinaryNode : public Node
{
public:
  const Token* op;
  Node* left;
  Node* right;

  BinaryNode(const Token*, Node*, Node*, Position, Position);

  std::string as_string(int depth = 0) const override;
};