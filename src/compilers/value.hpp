#pragma once

#include <string>

struct Value
{
  enum class Type
  {
    Number,
    String,
    Void
  };

  Type type;
  std::string content;

  Value(Type type, std::string content);
  Value(Type type);
  Value();
};