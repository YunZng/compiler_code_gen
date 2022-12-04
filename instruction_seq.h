#ifndef INSTRUCTION_SEQ_H
#define INSTRUCTION_SEQ_H

#include <vector>
#include <string>

class Instruction;
class Node;

class InstructionSequence {
private:
  struct Slot {
    std::string label;
    Instruction *ins;
  };

  std::vector<Slot> m_instructions;
  std::string m_next_label;
  Node *m_funcdef_ast; // pointer to function definition AST node
  // TODO: could map labels to indexes for faster lookup

  // copy constructor and assignment operator are not allowed
  InstructionSequence(const InstructionSequence &);
  InstructionSequence &operator=(const InstructionSequence &);

public:
  // Generic const_iterator type, for navigating through the pointers to
  // Instruction objects. It is parametized with the underlying
  // vector const iterator type, to allow forward and reverse versions
  // to be defined easily.
  template<typename It>
  class ISeqIterator {
  private:
    It slot_iter;

  public:
    ISeqIterator() { }

    ISeqIterator(It i) : slot_iter(i) { }

    ISeqIterator(const ISeqIterator<It> &other) : slot_iter(other.slot_iter) { }

    ISeqIterator<It> &operator=(const ISeqIterator<It> &rhs) {
      if (this != &rhs) { slot_iter = rhs.slot_iter; }
      return *this;
    }

    bool operator==(const ISeqIterator<It> &rhs) const { return slot_iter == rhs.slot_iter; }
    bool operator!=(const ISeqIterator<It> &rhs) const { return slot_iter != rhs.slot_iter; }

    Instruction* operator*() const { return slot_iter->ins; }
    bool has_label() const { return !slot_iter->label.empty(); }
    std::string get_label() const { return slot_iter->label; }

    ISeqIterator<It> &operator++() {
      slot_iter++;
      return *this;
    }

    ISeqIterator<It> operator++(int) {
      ISeqIterator<It> copy(*this);
      slot_iter++;
      return copy;
    }
  };

  typedef ISeqIterator<std::vector<Slot>::const_iterator> const_iterator;
  typedef ISeqIterator<std::vector<Slot>::const_reverse_iterator> const_reverse_iterator;

  InstructionSequence();
  virtual ~InstructionSequence();

  // Return a dynamically-allocated duplicate of this InstructionSequence
  InstructionSequence *duplicate() const;

  // Access to function definition AST node
  void set_funcdef_ast(Node *funcdef_ast) { m_funcdef_ast = funcdef_ast; }
  Node *get_funcdef_ast() const { return m_funcdef_ast; }

  // get begin and end const_iterators
  const_iterator cbegin() const { return const_iterator(m_instructions.cbegin()); }
  const_iterator cend() const { return const_iterator(m_instructions.cend()); }

  // get begin and end const_reverse_iterators
  const_reverse_iterator crbegin() const { return const_reverse_iterator(m_instructions.crbegin()); }
  const_reverse_iterator crend() const { return const_reverse_iterator(m_instructions.crend()); }

  // Append a pointer to an Instruction.
  // The InstructionSequence will assume responsibility for deleting the
  // Instruction object.
  void append(Instruction *ins);

  // Get number of Instructions.
  unsigned get_length() const;

  // Get Instruction at specified index.
  Instruction *get_instruction(unsigned index) const;

  // Get the last Instruction.
  Instruction *get_last_instruction() const;

  // Define a label. The next Instruction appended will be labeled with
  // this label.
  void define_label(const std::string &label);

  // Determine if Instruction at given index has a label.
  bool has_label(unsigned index) const { return !m_instructions.at(index).label.empty(); }

  // Determine if Instruction referred to by specified iterator has a label.
  bool has_label(const_iterator i) const { return i.has_label(); }

  // Determine if the InstructionSequence has a label at the end
  bool has_label_at_end() const;

  // Find Instruction labeled with specified label.
  // Returns null pointer if no Instruction has the specified label.
  Instruction *find_labeled_instruction(const std::string &label) const;

  // Return the index of instruction labeled with the specified label.
  unsigned get_index_of_labeled_instruction(const std::string &label) const;
};

#endif // INSTRUCTION_SEQ_H
