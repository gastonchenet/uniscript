#pragma once

#include <cstdint>
#include <string>

struct Position
{
public:
  size_t index, line, column;
  std::string* filename;

  Position(size_t, size_t, size_t, std::string*);
  Position();

  void advance(char);
  Position copy() const;
};