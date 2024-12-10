#include "position.hpp"

Position::Position(size_t index, size_t line, size_t column, std::string* filename) : index(index), line(line), column(column), filename(filename) {}
Position::Position() : index(0), line(1), column(1), filename(nullptr) {}

void Position::advance(char current_char)
{
  index++;
  column++;

  if (current_char == '\n')
  {
    line++;
    column = 1;
  }
}

Position Position::copy() const
{
  return Position(index, line, column, filename);
}