#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "operand.h"

// Instruction object type.
// Can be used for either high-level or low-level code.
class Instruction {
private:
  int m_opcode;
  unsigned m_num_operands;
  Operand m_operands[3];

public:
  Instruction(int opcode);
  Instruction(int opcode, const Operand &op1);
  Instruction(int opcode, const Operand &op1, const Operand &op2);
  Instruction(int opcode, const Operand &op1, const Operand &op2, const Operand &op3, unsigned num_operands = 3);

  ~Instruction();

  int get_opcode() const;

  unsigned get_num_operands() const;

  const Operand &get_operand(unsigned index) const;
};

#endif // INSTRUCTION_H
