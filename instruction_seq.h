#ifndef INSTRUCTION_SEQ_H
#define INSTRUCTION_SEQ_H

#include <vector>
#include <string>

class Instruction;

class InstructionSequence {
private:
  struct Slot {
    std::string label;
    Instruction *ins;
  };

  std::vector<Slot> m_instructions;
  std::string m_next_label;
  // TODO: could map labels to indexes for faster lookup

  // copy constructor and assignment operator are not allowed
  InstructionSequence(const InstructionSequence &);
  InstructionSequence &operator=(const InstructionSequence &);

public:
  // const_iterator type, for navigating through the pointers to
  // Instruction objects
  class const_iterator {
  private:
    std::vector<Slot>::const_iterator slot_iter;

  public:
    const_iterator() { }

    const_iterator(std::vector<Slot>::const_iterator i) : slot_iter(i) { }

    const_iterator(const const_iterator &other) : slot_iter(other.slot_iter) { }

    const_iterator &operator=(const const_iterator &rhs) {
      if (this != &rhs) { slot_iter = rhs.slot_iter; }
      return *this;
    }

    bool operator==(const const_iterator &rhs) const { return slot_iter == rhs.slot_iter; }
    bool operator!=(const const_iterator &rhs) const { return slot_iter != rhs.slot_iter; }

    Instruction* operator*() const { return slot_iter->ins; }
    bool has_label() const { return !slot_iter->label.empty(); }
    std::string get_label() const { return slot_iter->label; }

    const_iterator &operator++() {
      slot_iter++;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator copy(*this);
      slot_iter++;
      return copy;
    }
  };

  InstructionSequence();
  virtual ~InstructionSequence();

  // get begin and end const_iterators
  const_iterator cbegin() const { return const_iterator(m_instructions.cbegin()); }
  const_iterator cend() const { return const_iterator(m_instructions.cend()); }

  // Append a pointer to an Instruction.
  // The InstructionSequence will assume responsibility for deleting the
  // Instruction object.
  void append(Instruction *ins);

  // Get number of Instructions.
  unsigned get_length() const;

  // Get Instruction at specified index.
  Instruction *get_instruction(unsigned index) const;

  // Define a label. The next Instruction appended will be labeled with
  // this label.
  void define_label(const std::string &label);

  // Determine if Instruction at given index has a label.
  bool has_label(unsigned index) const { return m_instructions.at(index).ins; }

  // Determine if Instruction referred to by specified iterator has a label.
  bool has_label(const_iterator i) const { return i.has_label(); }

  // Determine if the InstructionSequence has a label at the end
  bool has_label_at_end() const;

  // Find Instruction labeled with specified label.
  // Returns null pointer if no Instruction has the specified label.
  Instruction *find_labeled_instruction(const std::string &label) const;
};

#endif // INSTRUCTION_SEQ_H
