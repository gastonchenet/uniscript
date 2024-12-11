#pragma once

#include "node.hpp"

struct BreakNode : public Node
{
  BreakNode(Position, Position);

  std::string as_string(int = 0) const override;
};