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
#include "../nodes/if_node.hpp"
#include "../nodes/while_node.hpp"
#include "../nodes/for_node.hpp"
#include "../nodes/call_node.hpp"
#include "../nodes/funcdef_node.hpp"
#include "../nodes/break_node.hpp"
#include "../nodes/continue_node.hpp"
#include "../nodes/return_node.hpp"

struct Compiler
{
protected:
  Node* root;
  std::vector<std::string> vars;
  std::string output_s;
  std::map<std::string, Value> symbol_table;

  Compiler(Node*);

  Value visit(Node*, int);
  std::string new_var();

  virtual Value visit(NumberNode*, int) = 0;
  virtual Value visit(StringNode*, int) = 0;
  virtual Value visit(UnaryNode*, int) = 0;
  virtual Value visit(BinaryNode*, int) = 0;
  virtual Value visit(AssignNode*, int) = 0;
  virtual Value visit(AccessNode*, int) = 0;
  virtual Value visit(PutNode*, int) = 0;
  virtual Value visit(BlockNode*, int) = 0;
  virtual Value visit(IfNode*, int) = 0;
  virtual Value visit(WhileNode*, int) = 0;
  virtual Value visit(ForNode*, int) = 0;
  virtual Value visit(CallNode*, int) = 0;
  virtual Value visit(FuncdefNode*, int) = 0;
  virtual Value visit(BreakNode*, int) = 0;
  virtual Value visit(ContinueNode*, int) = 0;
  virtual Value visit(ReturnNode*, int) = 0;

public:
  virtual ~Compiler() = default;

  virtual std::string compile() = 0;
};
