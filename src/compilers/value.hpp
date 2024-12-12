#pragma once

#include <string>

struct Value
{
  enum class Type
  {
    Number,
    String,
    Void,
    List,
  };

  Type type;
  std::string content;

  Value(Type, std::string);
  Value(Type);
  Value();
};