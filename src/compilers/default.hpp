#pragma once

#include <typeinfo>
#include <vector>
#include <map>

#include "../nodes/node.hpp"
#include "value.hpp"
#include "../nodes/number_node.hpp"
#include "../nodes/string_node.hpp"
#include "../nodes/unary_node.hpp"
#include "../nodes/binary_node.hpp"
#include "../nodes/put_node.hpp"
#include "../nodes/assign_node.hpp"
#include "../nodes/access_node.hpp"
#include "../nodes/block_node.hpp"

struct Compiler
{
protected:
  Node* root;
  std::vector<std::string> vars;
  std::string output_s;
  std::map<std::string, Value> symbol_table;

  Compiler(Node* root);

  Value visit(Node*);
  std::string new_var();

  virtual Value visit(NumberNode*) = 0;
  virtual Value visit(StringNode*) = 0;
  virtual Value visit(UnaryNode*) = 0;
  virtual Value visit(BinaryNode*) = 0;
  virtual Value visit(AssignNode*) = 0;
  virtual Value visit(AccessNode*) = 0;
  virtual Value visit(PutNode*) = 0;
  virtual Value visit(BlockNode*) = 0;

public:
  virtual ~Compiler() = default;

  virtual std::string compile() = 0;
};