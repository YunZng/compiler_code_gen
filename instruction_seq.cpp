#include <cassert>
#include "instruction.h"
#include "exceptions.h"
#include "instruction_seq.h"

InstructionSequence::InstructionSequence()
  : m_funcdef_ast(nullptr) {
}

InstructionSequence::~InstructionSequence() {
  // delete the Instructions
  for (auto i = m_instructions.begin(); i != m_instructions.end(); ++i)
    delete i->ins;
}

InstructionSequence *InstructionSequence::duplicate() const {
  InstructionSequence *dup = new InstructionSequence();
  dup->m_next_label = m_next_label;
  dup->m_funcdef_ast = m_funcdef_ast;

  for (auto i = m_instructions.begin(); i != m_instructions.end(); ++i) {
    const Slot &slot = *i;
    if (!slot.label.empty())
      dup->define_label(slot.label);
    dup->append(slot.ins->duplicate());
  }

  return dup;
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

Instruction *InstructionSequence::get_last_instruction() const {
  assert(!m_instructions.empty());
  return m_instructions.back().ins;
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

unsigned InstructionSequence::get_index_of_labeled_instruction(const std::string &label) const {
  unsigned index = 0;
  for (auto i = m_instructions.cbegin(); i != m_instructions.cend(); i++) {
    if (i->label == label) {
      return index;
    }
    ++index;
  }
  RuntimeError::raise("no instruction has label '%s'", label.c_str());
}
