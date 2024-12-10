#pragma once

#include <vector>

#include "node.hpp"

struct BlockNode : public Node
{
public:
  std::vector<Node*> nodes;

  BlockNode(std::vector<Node*>, Position, Position);

  std::string as_string(int depth = 0) const override;
};