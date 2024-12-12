#pragma once

#include <vector>

#include "node.hpp"

class ListNode : public Node
{
public:
  std::vector<Node*> elements;

  ListNode(std::vector<Node*> elements, Position pos_start, Position pos_end);

  std::string as_string(int = 0) const override;
};