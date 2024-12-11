#pragma once

#include <vector>

#include "node.hpp"
#include "../token.hpp"

struct FuncdefNode : public Node
{
  const Token* name;
  std::vector<const Token*> args;
  Node* body;

  FuncdefNode(const Token*, std::vector<const Token*>, Node*, Position, Position);

  std::string as_string(int = 0) const override;
};