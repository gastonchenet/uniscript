#pragma once

#include "node.hpp"

struct NumberNode : public Node
{
public:
  double value;

  NumberNode(double, Position);

  std::string as_string(int= 0) const override;
};