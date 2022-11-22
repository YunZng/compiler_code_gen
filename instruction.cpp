#include <cassert>
#include "instruction.h"

Instruction::Instruction(int opcode)
  : Instruction(opcode, Operand(), Operand(), Operand(), 0) {
}

Instruction::Instruction(int opcode, const Operand &op1)
  : Instruction(opcode, op1, Operand(), Operand(), 1) {
}

Instruction::Instruction(int opcode, const Operand &op1, const Operand &op2)
  : Instruction(opcode, op1, op2, Operand(), 2) {
}

Instruction::Instruction(int opcode, const Operand &op1, const Operand &op2, const Operand &op3, unsigned num_operands)
  : m_opcode(opcode)
  , m_num_operands(num_operands)
  , m_operands { op1, op2, op3 } {
}

Instruction::~Instruction() {
}

int Instruction::get_opcode() const {
  return m_opcode;
}

unsigned Instruction::get_num_operands() const {
  return m_num_operands;
}

const Operand &Instruction::get_operand(unsigned index) const {
  assert(index < m_num_operands);
  return m_operands[index];
}

