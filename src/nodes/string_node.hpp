#pragma once

#include "node.hpp"

struct StringNode : public Node 
{
public:
  std::string value;

  StringNode(std::string, Position);

  std::string as_string(int= 0) const override;
};