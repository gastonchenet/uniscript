#include "token.hpp"

Token::Token(Type type, std::string value, const Position& position) : type(type), position(position), value(std::make_optional<std::variant<std::string, double>>(value)) {}
Token::Token(Type type, double value, const Position& position) : type(type), position(position), value(std::make_optional<std::variant<std::string, double>>(value)) {}
Token::Token(Type type, const Position& position) : type(type), position(position) {}

std::string Token::type_as_string(Token::Type type)
{
  switch (type)
  {
  case Token::Type::Number:             return "Number";
  case Token::Type::Plus:               return "Plus";
  case Token::Type::Minus:              return "Minus";
  case Token::Type::Multiply:           return "Multiply";
  case Token::Type::Divide:             return "Divide";
  case Token::Type::Modulo:             return "Modulo";
  case Token::Type::And:                return "And";
  case Token::Type::Or:                 return "Or";
  case Token::Type::Not:                return "Not";
  case Token::Type::Pow:                return "Pow";
  case Token::Type::LParen:             return "LParen";
  case Token::Type::RParen:             return "RParen";
  case Token::Type::End:                return "End";
  case Token::Type::Identifier:         return "Identifier";
  case Token::Type::Keyword:            return "Keyword";
  case Token::Type::Assign:             return "Assign";
  case Token::Type::String:             return "String";
  case Token::Type::Equals:             return "Equals";
  case Token::Type::NotEquals:          return "NotEquals";
  case Token::Type::GreaterThan:        return "GreaterThan";
  case Token::Type::LessThan:           return "LessThan";
  case Token::Type::GreaterThanOrEqual: return "GreaterThanOrEqual";
  case Token::Type::LessThanOrEqual:    return "LessThanOrEqual";
  case Token::Type::Eof:                return "Eof";

  default:                              return "Unknown";
  }
}

bool Token::matches(Type type) const
{
  return this->type == type;
}

bool Token::matches(Type type, std::string value) const
{
  return this->type == type && this->value.has_value() && std::get<std::string>(this->value.value()) == value;
}

bool Token::matches(Type type, double value) const
{
  return this->type == type && this->value.has_value() && std::get<double>(this->value.value()) == value;
}

Token Token::copy() const
{
  if (this->value.has_value() && std::holds_alternative<double>(this->value.value()))
  {
    return Token(this->type, std::get<double>(this->value.value()), this->position);
  }
  else if (this->value.has_value() && std::holds_alternative<std::string>(this->value.value()))
  {
    return Token(this->type, std::get<std::string>(this->value.value()), this->position);
  }
  else
  {
    return Token(this->type, this->position);
  }
}

std::string Token::as_string() const 
{
  std::string value = "";

  if (this->value.has_value())
  {
    std::visit([&value](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;

      if constexpr (std::is_same_v<T, std::string>)
      {
        value = arg;
      }
      else if constexpr (std::is_same_v<T, double>)
      {
        value = std::to_string(arg);
      }
    }, this->value.value());

    return "<" + type_as_string(type) + ", " + value + ">";
  }
  else
  {
    return "<" + type_as_string(type) + ">";
  }
}
