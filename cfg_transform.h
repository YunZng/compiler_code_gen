#ifndef CFG_TRANSFORM_H
#define CFG_TRANSFORM_H

#include <memory>
#include "cfg.h"
#include "live_vregs.h"
#include <unordered_map>

class ControlFlowGraphTransform{
private:
  std::shared_ptr<ControlFlowGraph> m_cfg;

public:
  ControlFlowGraphTransform(const std::shared_ptr<ControlFlowGraph>& cfg);

  virtual ~ControlFlowGraphTransform();

  std::shared_ptr<ControlFlowGraph> get_orig_cfg();

  virtual std::shared_ptr<ControlFlowGraph> transform_cfg();

  // Create a transformed version of the instructions in a basic block.
  // Note that an InstructionSequence "owns" the Instruction objects it contains,
  // and is responsible for deleting them. Therefore, be careful to avoid
  // having two InstructionSequences contain pointers to the same Instruction.
  // If you need to make an exact copy of an Instruction object, you can
  // do so using the duplicate() member function, as follows:
  //
  //    Instruction *orig_ins = /* an Instruction object */
  //    Instruction *dup_ins = orig_ins->duplicate();
  // virtual std::shared_ptr<InstructionSequence> transform_basic_block(const InstructionSequence* orig_bb) = 0;

  // virtual std::shared_ptr<InstructionSequence> constant_fold(const InstructionSequence* orig_bb) = 0;
  virtual std::shared_ptr<InstructionSequence> dead_store(const InstructionSequence* orig_bb) = 0;
  virtual std::shared_ptr<InstructionSequence> lvn(const InstructionSequence* orig_bb, const BasicBlock*) = 0;

};

class MyOptimization : public ControlFlowGraphTransform{
private:
  LiveVregs m_live_vregs;
  std::map<long, Operand> val_to_ival;
  std::map<long, long> op_to_val;

public:
  MyOptimization(const std::shared_ptr<ControlFlowGraph>& cfg);
  ~MyOptimization();

  // virtual std::shared_ptr<InstructionSequence> constant_fold(const InstructionSequence* orig_bb);
  virtual std::shared_ptr<InstructionSequence> dead_store(const InstructionSequence* orig_bb);
  virtual std::shared_ptr<InstructionSequence> lvn(const InstructionSequence* orig_bb, const BasicBlock*);

private:
  void loop_check(int, Instruction*&, Instruction*&, std::unordered_map<int, long>&);
  // void map_value(int, Operand, );
  long set_val(std::map<long, long>&, Operand);
  void recursive_find(Operand&);
};
#endif // CFG_TRANSFORM_H