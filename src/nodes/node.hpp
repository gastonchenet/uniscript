#pragma once

#include <string>

#include "position.hpp"

struct Node
{
public:
  Position pos_start;
  Position pos_end;

  Node(Position, Position);

  virtual ~Node() = default;
  virtual std::string as_string(int = 0) const = 0;
};