#include "parse_args.hpp"

#include <iostream>

#define USAGE "Usage: " << argv[0] << " <filename> [--help|-h] [--output|-o <output_file>] [--bash|-b] [--powershell|-p]"

Args parse_args(int argc, char** argv)
{
  Args args;

  if (argc < 2)
  {
    std::cerr << USAGE << std::endl;
    exit(1);
  }

  std::string arg;

  for (int i = 1; i < argc; i++)
  {
    arg = argv[i];

    if (arg[0] == '-' && arg[1] == '-')
    {
      if (arg == "--output")
      {
        if (i + 1 < argc)
        {
          args.output_file = argv[++i];
        }
        else
        {
          std::cerr << "Error: missing argument for --output" << std::endl;
          exit(1);
        }
      }
      else if (arg == "--bash")
      {
        args.bash = true;
      }
      else if (arg == "--powershell")
      {
        args.powershell = true;
      }
      else if (arg == "--help" || arg == "-h")
      {
        std::cout << USAGE << std::endl;
        exit(0);
      }
      else
      {
        std::cerr << "Error: unknown argument " << arg << std::endl;
        exit(1);
      }
    }
    else if (arg[0] == '-')
    {
      for (size_t j = 1; j < arg.size(); j++)
      {
        if (arg[1] == 'o')
        {
          if (arg.size() > 2)
          {
            std::cerr << "Error: cannot combine -o with other arguments" << std::endl;
            exit(1);
          }
          else if (i + 1 < argc)
          {
            args.output_file = argv[++i];
            break;
          }
          else
          {
            std::cerr << "Error: missing argument for -o" << std::endl;
            exit(1);
          }
        }
        else if (arg[j] == 'b')
        {
          args.bash = true;
        }
        else if (arg[j] == 'p')
        {
          args.powershell = true;
        }
        else
        {
          std::cerr << "Error: unknown argument -" << arg[j] << std::endl;
          exit(1);
        }
      }
    }
    else
    {
      if (!args.input_file.empty())
      {
        std::cerr << "Error: multiple input files" << std::endl;
        exit(1);
      }

      args.input_file = arg;
    }
  }

  if (args.input_file.empty())
  {
    std::cerr << "Error: missing input file" << std::endl;
    exit(1);
  }

  if (!args.bash && !args.powershell)
  {
    std::cerr << "Error: no compiler specified" << std::endl;
    exit(1);
  }

  return args;
}