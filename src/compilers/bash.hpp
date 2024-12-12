#pragma once

#include "default.hpp"

struct BashCompiler : public Compiler
{
protected:
  Value visit(NumberNode*, int) override;
  Value visit(StringNode*, int) override;
  Value visit(UnaryNode*, int) override;
  Value visit(BinaryNode*, int) override;
  Value visit(AssignNode*, int) override;
  Value visit(AccessNode*, int) override;
  Value visit(PutNode*, int) override;
  Value visit(BlockNode*, int) override;
  Value visit(IfNode*, int) override;
  Value visit(WhileNode*, int) override;
  Value visit(ForNode*, int) override;
  Value visit(CallNode*, int) override;
  Value visit(FuncdefNode*, int) override;
  Value visit(BreakNode*, int) override;
  Value visit(ContinueNode*, int) override;
  Value visit(ReturnNode*, int) override;
  Value visit(ListNode*, int) override;

public:
  BashCompiler(Node*);

  std::string compile() override;
};
