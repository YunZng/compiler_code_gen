// Copyright (c) 2021, David H. Hovemeyer <david.hovemeyer@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#ifndef NODE_BASE_H
#define NODE_BASE_H

#include <memory>
#include "type.h"
#include "symtab.h"
#include "literal_value.h"
#include "type.h"
#include "operand.h"


enum ValueType{
  COMPUTED,
  NORMAL
};

// The Node class will inherit from this type, so you can use it
// to define any attributes and methods that Node objects should have
// (constant value, results of semantic analysis, code generation info,
// etc.)
class NodeBase{
private:
  std::shared_ptr<Type> m_type;
  std::shared_ptr<LiteralValue> m_lit;
  Symbol* m_symbol;
  ValueType value_type;
  Operand op;
  unsigned vreg;
  std::shared_ptr<Type> m_func;
  int addr;
  // copy ctor and assignment operator not supported
  NodeBase(const NodeBase&);
  NodeBase& operator=(const NodeBase&);

public:
  NodeBase();
  virtual ~NodeBase();

  void set_symbol(Symbol* symbol);
  void set_type(const std::shared_ptr<Type>& type);
  void set_func(const std::shared_ptr<Type>& type);
  void set_lit(const std::shared_ptr<LiteralValue>& lit);
  void set_value_type(ValueType type);
  void set_op(Operand ope);
  void set_addr(int);
  int get_addr();
  Operand get_op() const;
  void set_vreg(unsigned);
  unsigned get_vreg() const;
  bool has_symbol() const;
  Symbol* get_symbol() const;
  std::shared_ptr<Type> get_type() const;
  std::shared_ptr<LiteralValue> get_lit() const;
  std::shared_ptr<Type> get_func() const;
  ValueType get_value_type() const;
};

#endif // NODE_BASE_H
