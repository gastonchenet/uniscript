#pragma once

#include "node.hpp"

struct ContinueNode : public Node
{
  ContinueNode(Position, Position);

  std::string as_string(int = 0) const override;
};