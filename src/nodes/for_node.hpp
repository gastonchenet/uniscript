#pragma once

#include "node.hpp"
#include "../token.hpp"

struct ForNode : Node
{
  Token* var_name;
  Node* start;
  Node* end;
  Node* step;
  Node* body;

  ForNode(Token*, Node*, Node*, Node*, Node*, Position, Position);

  std::string as_string(int = 0) const override;
};