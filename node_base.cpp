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

#include <cassert>
#include "node_base.h"

NodeBase::NodeBase(){
  m_symbol = nullptr;
  m_type = nullptr;
  value_type = NORMAL;
  m_lit = nullptr;
  vreg = 0;
  m_func = nullptr;
  addr = 0;
  actually_var = 0;
}

NodeBase::~NodeBase(){
}

void NodeBase::set_symbol(Symbol* symbol){
  assert(!has_symbol());
  m_symbol = symbol;
}

void NodeBase::set_type(const std::shared_ptr<Type>& type){
  // assert(!m_type);
  m_type = type;
}

void NodeBase::set_func(const std::shared_ptr<Type>& type){
  m_func = type;
}

void NodeBase::set_lit(const std::shared_ptr<LiteralValue>& lit){
  assert(!has_symbol());
  assert(!m_lit);
  m_lit = lit;
}

void NodeBase::set_op(Operand ope){
  op = ope;
}
void NodeBase::set_vreg(unsigned vr){
  vreg = vr;
}
unsigned NodeBase::get_vreg() const{
  return vreg;
}
void NodeBase::set_value_type(ValueType type){
  value_type = type;
}

bool NodeBase::has_symbol() const{
  return m_symbol != nullptr;
}

Symbol* NodeBase::get_symbol() const{
  return m_symbol;
}

std::shared_ptr<Type> NodeBase::get_type() const{
  // this shouldn't be called unless there is actually a type
  // associated with this node

  if(has_symbol())
    return m_type; // Symbol will definitely have a valid Type
  else{
    assert(m_type); // make sure a Type object actually exists
    return m_type;
  }
}

std::shared_ptr<Type> NodeBase::get_func() const{
  return m_func;
}

ValueType NodeBase::get_value_type() const{
  return value_type;
}

Operand NodeBase::get_op() const{
  return op;
}

std::shared_ptr<LiteralValue> NodeBase::get_lit() const{
  return m_lit;
}

void NodeBase::set_addr(int addr1){
  addr = addr1;
}

int NodeBase::get_addr(){
  return addr;
}
bool NodeBase::get_actually_var(){
  return actually_var;
}
void NodeBase::set_actually_var(bool a){
  actually_var = a;
}
