#include "formatter.h"
#include "instruction.h"
#include "instruction_seq.h"
#include "print_instruction_seq.h"

PrintInstructionSequence::PrintInstructionSequence(const Formatter *formatter)
  : m_formatter(formatter) {
}

PrintInstructionSequence::~PrintInstructionSequence() {
}

void PrintInstructionSequence::print(const InstructionSequence *iseq) {
  for (auto i = iseq->cbegin(); i != iseq->cend(); i++) {
    if (i.has_label()) {
      printf("%s:\n", i.get_label().c_str());
    }
    std::string formatted_ins = m_formatter->format_instruction(*i);
    printf("\t%s\n", formatted_ins.c_str());
  }
}
