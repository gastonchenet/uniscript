#include "default.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

Compiler::Compiler(Node* root) : root(root), vars(std::vector<std::string>()), output_s(""), symbol_table(std::map<std::string, Value>()) {}

std::string Compiler::new_var()
{
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(6) << std::hex << (vars.size() + 1);
  std::string var = "_var0x" + ss.str();
  vars.push_back(var);
  return var;
}

Value Compiler::visit(Node* node)
{
  if (auto numberNode = dynamic_cast<NumberNode*>(node))     return visit(numberNode);
  if (auto stringNode = dynamic_cast<StringNode*>(node))     return visit(stringNode);
  if (auto unaryNode = dynamic_cast<UnaryNode*>(node))       return visit(unaryNode);
  if (auto binaryNode = dynamic_cast<BinaryNode*>(node))     return visit(binaryNode);
  if (auto assignNode = dynamic_cast<AssignNode*>(node))     return visit(assignNode);
  if (auto accessNode = dynamic_cast<AccessNode*>(node))     return visit(accessNode);
  if (auto putNode = dynamic_cast<PutNode*>(node))           return visit(putNode);
  if (auto blockNode = dynamic_cast<BlockNode*>(node))       return visit(blockNode);

  throw std::runtime_error("Unknown node type");
}