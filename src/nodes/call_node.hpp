#pragma once

#include <vector>

#include "node.hpp"
#include "../token.hpp"

struct CallNode : public Node
{
public:
  const Token* name;
  std::vector<Node*> args;

  CallNode(const Token*, std::vector<Node*>, Position, Position);

  std::string as_string(int = 0) const override;
};