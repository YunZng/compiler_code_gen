#ifndef PRINT_INSTRUCTION_SEQ_H
#define PRINT_INSTRUCTION_SEQ_H

class Formatter;
class InstructionSequence;

class PrintInstructionSequence {
private:
  const Formatter *m_formatter;

public:
  PrintInstructionSequence(const Formatter *formatter);
  ~PrintInstructionSequence();

  void print(const InstructionSequence *iseq);
};

#endif // PRINT_INSTRUCTION_SEQ_H
