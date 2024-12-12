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

Value Compiler::visit(Node* node, int depth)
{
  if (auto numberNode = dynamic_cast<NumberNode*>(node))     return visit(numberNode, depth);
  if (auto stringNode = dynamic_cast<StringNode*>(node))     return visit(stringNode, depth);
  if (auto unaryNode = dynamic_cast<UnaryNode*>(node))       return visit(unaryNode, depth);
  if (auto binaryNode = dynamic_cast<BinaryNode*>(node))     return visit(binaryNode, depth);
  if (auto assignNode = dynamic_cast<AssignNode*>(node))     return visit(assignNode, depth);
  if (auto accessNode = dynamic_cast<AccessNode*>(node))     return visit(accessNode, depth);
  if (auto putNode = dynamic_cast<PutNode*>(node))           return visit(putNode, depth);
  if (auto blockNode = dynamic_cast<BlockNode*>(node))       return visit(blockNode, depth);
  if (auto ifNode = dynamic_cast<IfNode*>(node))             return visit(ifNode, depth);
  if (auto whileNode = dynamic_cast<WhileNode*>(node))       return visit(whileNode, depth);
  if (auto forNode = dynamic_cast<ForNode*>(node))           return visit(forNode, depth);
  if (auto callNode = dynamic_cast<CallNode*>(node))         return visit(callNode, depth);
  if (auto funcdefNode = dynamic_cast<FuncdefNode*>(node))   return visit(funcdefNode, depth);
  if (auto breakNode = dynamic_cast<BreakNode*>(node))       return visit(breakNode, depth);
  if (auto continueNode = dynamic_cast<ContinueNode*>(node)) return visit(continueNode, depth);
  if (auto returnNode = dynamic_cast<ReturnNode*>(node))     return visit(returnNode, depth);
  if (auto listNode = dynamic_cast<ListNode*>(node))         return visit(listNode, depth);

  throw std::runtime_error("Unknown node type");
}
