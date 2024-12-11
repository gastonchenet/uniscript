#include <iostream>
#include <memory>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "nodes/node.hpp"
#include "compilers/bash.hpp"
#include "compilers/powershell.hpp"
#include "parse_args.hpp"

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
  Args args = parse_args(argc, argv);

  std::string out_bash = args.output_file + ".sh";
  std::string out_powershell = args.output_file + ".ps1";
  std::string code = read_file(&args.input_file);


  Lexer lexer = Lexer(code);
  std::vector<Token> tokens = lexer.tokenize();

  if (tokens[0].matches(Token::Type::Eof))
  {
    std::cerr << "Error: empty file" << std::endl;
    exit(1);
  }

  Parser parser = Parser(&tokens);
  Node* node = parser.parse();

  if (args.bash)
  {
    BashCompiler compiler = BashCompiler(node);
    std::string compiled = compiler.compile();
    write_file(&out_bash, &compiled);
  }

  if (args.powershell)
  {
    PowershellCompiler compiler = PowershellCompiler(node);
    std::string compiled = compiler.compile();
    write_file(&out_powershell, &compiled);
  }

  return 0;
}