#pragma once

#include "node.hpp"

struct NumberNode : public Node
{
public:
  double value;

  NumberNode(double, Position);

  std::string as_string(int depth = 0) const override;
};