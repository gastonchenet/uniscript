#include "lexer.hpp"
#include <stdexcept>
#include "keywords.hpp"

#include <iostream>

Lexer::Lexer(const std::string& code) : code(code), position(Position()) {}

std::vector<Token> Lexer::tokenize()
{
  size_t code_length = code.length();
  char current_char;
  std::string current_token = "";

  while (position.index < code_length)
  {
    current_char = code[position.index];

    if (current_char == ' ' || current_char == '\t' || current_char == '\n')
    {
      position.advance(current_char);
    }
    else if (
      (current_char >= 'a' && current_char <= 'z') ||
      (current_char >= 'A' && current_char <= 'Z') ||
      current_char == '_'
    )
    {
      make_identifier_or_keyword();
    }
    else if (current_char >= '0' && current_char <= '9')
    {
      make_number();
    }
    else if (current_char == '"')
    {
      make_string();
    }
    else if (current_char == ';')
    {
      Token token = Token(Token::Type::End, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == ',')
    {
      Token token = Token(Token::Type::Comma, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '!')
    {
      make_not_or_not_equals();
    }
    else if (current_char == '+')
    {
      Token token = Token(Token::Type::Plus, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '-')
    {
      make_minus_or_comment();
    }
    else if (current_char == '*')
    {
      Token token = Token(Token::Type::Multiply, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '/')
    {
      Token token = Token(Token::Type::Divide, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '^')
    {
      Token token = Token(Token::Type::Pow, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '%')
    {
      Token token = Token(Token::Type::Modulo, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '(')
    {
      Token token = Token(Token::Type::LParen, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == ')')
    {
      Token token = Token(Token::Type::RParen, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '{')
    {
      Token token = Token(Token::Type::LBrace, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '}')
    {
      Token token = Token(Token::Type::RBrace, position.copy());
      tokens.push_back(token);
      position.advance(current_char);
    }
    else if (current_char == '|')
    {
      make_or();
    }
    else if (current_char == '&')
    {
      make_and();
    }
    else if (current_char == '=')
    {
      make_equals_or_assign();
    }
    else if (current_char == '>')
    {
      make_greater_than_or_greater_than_equals();
    }
    else if (current_char == '<')
    {
      make_less_than_or_less_than_equals();
    }
    else
    {
      std::string message = "Unexpected character: ";
      message += current_char;
      throw std::runtime_error(message);
    }
  }

  Token eof_token = Token(Token::Type::Eof, position.copy());
  tokens.push_back(eof_token);

  return tokens;
}

void Lexer::make_identifier_or_keyword()
{
  std::string value = "";
  char current_char = code[position.index];

  while (
    (current_char >= 'a' && current_char <= 'z') ||
    (current_char >= 'A' && current_char <= 'Z') ||
    (current_char >= '0' && current_char <= '9') ||
    current_char == '_'
  )
  {
    value += current_char;
    position.advance(current_char);
    current_char = code[position.index];
  }

  Token::Type token_type = Token::Type::Identifier;

  for (size_t i = 0; i < KeywordsLength; i++)
  {
    if (value == Keywords[i])
    {
      token_type = Token::Type::Keyword;
      break;
    }
  }

  Token token = Token(token_type, value, position.copy());
  tokens.push_back(token);
}

void Lexer::make_number()
{
  std::string value = "";
  char current_char = code[position.index];
  bool has_dot = false;

  while ((current_char >= '0' && current_char <= '9') || current_char == '.')
  {
    if (current_char == '.')
    {
      if (has_dot)
      {
        throw std::runtime_error("Unexpected character: .");
      }

      has_dot = true;
    }

    value += current_char;
    position.advance(current_char);
    current_char = code[position.index];
  }

  Token token = Token(Token::Type::Number, std::stod(value), position.copy());
  tokens.push_back(token);
}

void Lexer::make_string()
{
  std::string value = "";
  char current_char = code[position.index];
  bool was_escape = false;

  position.advance(current_char);
  current_char = code[position.index];

  while (current_char != '"' || was_escape)
  {
    value += current_char;
    position.advance(current_char);

    if (current_char == '\\' && !was_escape)
    {
      was_escape = true;
    }
    else
    {
      was_escape = false;
    }

    current_char = code[position.index];
  }

  position.advance(current_char);

  Token token = Token(Token::Type::String, value, position.copy());
  tokens.push_back(token);
}

void Lexer::make_not_or_not_equals()
{
  char current_char = code[position.index];
  position.advance(current_char);
  current_char = code[position.index];

  if (current_char == '=')
  {
    Token token = Token(Token::Type::NotEquals, position.copy());
    tokens.push_back(token);
    position.advance(current_char);
  }
  else
  {
    Token token = Token(Token::Type::Not, position.copy());
    tokens.push_back(token);
  }
}

void Lexer::make_or()
{
  char current_char = code[position.index];
  position.advance(current_char);
  current_char = code[position.index];

  if (current_char == '|')
  {
    Token token = Token(Token::Type::Or, position.copy());
    tokens.push_back(token);
    position.advance(current_char);
  }
  else
  {
    throw std::runtime_error("Unexpected character: " + current_char);
  }
}

void Lexer::make_and()
{
  char current_char = code[position.index];
  position.advance(current_char);
  current_char = code[position.index];

  if (current_char == '&')
  {
    Token token = Token(Token::Type::And, position.copy());
    tokens.push_back(token);
    position.advance(current_char);
  }
  else
  {
    throw std::runtime_error("Unexpected character: " + current_char);
  }
}

void Lexer::make_equals_or_assign()
{
  char current_char = code[position.index];
  position.advance(current_char);
  current_char = code[position.index];

  if (current_char == '=')
  {
    Token token = Token(Token::Type::Equals, position.copy());
    tokens.push_back(token);
    position.advance(current_char);
  }
  else
  {
    Token token = Token(Token::Type::Assign, position.copy());
    tokens.push_back(token);
  }
}

void Lexer::make_greater_than_or_greater_than_equals()
{
  char current_char = code[position.index];
  position.advance(current_char);
  current_char = code[position.index];

  if (current_char == '=')
  {
    Token token = Token(Token::Type::GreaterThanOrEqual, position.copy());
    tokens.push_back(token);
    position.advance(current_char);
  }
  else
  {
    Token token = Token(Token::Type::GreaterThan, position.copy());
    tokens.push_back(token);
  }
}

void Lexer::make_less_than_or_less_than_equals()
{
  char current_char = code[position.index];
  position.advance(current_char);
  current_char = code[position.index];

  if (current_char == '=')
  {
    Token token = Token(Token::Type::LessThanOrEqual, position.copy());
    tokens.push_back(token);
    position.advance(current_char);
  }
  else
  {
    Token token = Token(Token::Type::LessThan, position.copy());
    tokens.push_back(token);
  }
}

void Lexer::make_minus_or_comment()
{
  char current_char = code[position.index];
  position.advance(current_char);
  current_char = code[position.index];

  if (current_char == '-')
  {
    while (current_char != '\n' && current_char != '\0')
    {
      position.advance(current_char);
      current_char = code[position.index];
    }
  }
  else
  {
    Token token = Token(Token::Type::Minus, position.copy());
    tokens.push_back(token);
  }
}