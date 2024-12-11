#pragma once

#include <string>
#include <vector>

#include "position.hpp"
#include "token.hpp"

struct Lexer
{
private:
  const std::string& code;
  Position position;
  std::vector<Token> tokens;

public:
  Lexer(const std::string&);

  std::vector<Token> tokenize();

  void make_identifier_or_keyword();
  void make_number();
  void make_string();
  void make_not_or_not_equals();
  void make_or();
  void make_and();
  void make_equals_or_assign();
  void make_greater_than_or_greater_than_equals();
  void make_less_than_or_less_than_equals();
  void make_minus_or_comment();
};