#pragma once

#include "default.hpp"

struct BashCompiler : public Compiler
{
protected:
  Value visit(NumberNode*) override;
  Value visit(StringNode*) override;
  Value visit(UnaryNode*) override;
  Value visit(BinaryNode*) override;
  Value visit(AssignNode*) override;
  Value visit(AccessNode*) override;
  Value visit(PutNode*) override;
  Value visit(BlockNode*) override;

public:
  BashCompiler(Node*);

  std::string compile() override;
};