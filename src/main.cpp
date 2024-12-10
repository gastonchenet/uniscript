#include <iostream>
#include <memory>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "nodes/node.hpp"
#include "compilers/bash.hpp"

std::string read_file(const std::string* filename) {
  std::ifstream file(*filename);
  std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return code;
}

void write_file(const std::string* filename, const std::string* content) {
  std::ofstream file(*filename);
  file << *content;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::string in = argv[1];
  std::string out = "out.sh";

  std::string code = read_file(&in);

  Lexer lexer = Lexer(code);
  std::vector<Token> tokens = lexer.tokenize();

  // for (Token token : tokens)
  // {
  //   std::cout << token.as_string() << std::endl;
  // }

  Parser parser = Parser(&tokens);
  Node* node = parser.parse();

  std::cout << node->as_string() << std::endl;

  BashCompiler compiler = BashCompiler(node);
  std::string compiled = compiler.compile();

  write_file(&out, &compiled);

  return 0;
}