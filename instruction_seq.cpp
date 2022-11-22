#include <cassert>
#include "instruction.h"
#include "instruction_seq.h"

InstructionSequence::InstructionSequence() {
}

InstructionSequence::~InstructionSequence() {
}

void InstructionSequence::append(Instruction *ins) {
  m_instructions.push_back({ label: m_next_label, ins: ins });
  m_next_label = "";
}

unsigned InstructionSequence::get_length() const {
  return unsigned(m_instructions.size());
}

Instruction *InstructionSequence::get_instruction(unsigned index) const {
  return m_instructions.at(index).ins;
}

void InstructionSequence::define_label(const std::string &label) {
  assert(m_next_label.empty());
  m_next_label = label;
}

bool InstructionSequence::has_label_at_end() const {
  return !m_next_label.empty();
}

Instruction *InstructionSequence::find_labeled_instruction(const std::string &label) const {
  for (auto i = m_instructions.cbegin(); i != m_instructions.cend(); i++) {
    if (i->label == label) {
      return i->ins;
    }
  }
  return nullptr;
}
