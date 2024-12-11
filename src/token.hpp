#pragma once

#include <string>
#include <variant>
#include <optional>

#include "position.hpp"

struct Token
{
public:
  enum class Type
  {
    Number,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    And,
    Or,
    Not,
    Pow,
    LParen,
    RParen,
    LBrace,
    RBrace,
    Identifier,
    Keyword,
    Assign,
    String,
    Equals,
    NotEquals,
    GreaterThan,
    LessThan,
    GreaterThanOrEqual,
    LessThanOrEqual,
    Comma,
    End,
    Eof
  };

private:
  static std::string type_as_string(Type);

public:
  Type type;
  const Position& position;
  std::optional<std::variant<std::string, double>> value;

  Token(Type, std::string, const Position&);
  Token(Type, double, const Position&);
  Token(Type, const Position&);

  bool matches(Type) const;
  bool matches(Type, std::string) const;
  bool matches(Type, double) const;

  Token copy() const;
  std::string as_string() const;
};