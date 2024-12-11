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

public:
  BashCompiler(Node*);

  std::string compile() override;
};
