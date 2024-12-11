#pragma once

#include "node.hpp"
#include "../token.hpp"

struct AccessNode : public Node
{
public:
  const Token* token;

  AccessNode(const Token*, Position, Position);

  std::string as_string(int = 0) const override;
};