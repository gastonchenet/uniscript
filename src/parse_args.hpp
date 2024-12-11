#pragma once

#include <string>

struct Args {
  std::string input_file;
  std::string output_file = "a";
  bool bash = false, powershell = false;
};

Args parse_args(int argc, char** argv);