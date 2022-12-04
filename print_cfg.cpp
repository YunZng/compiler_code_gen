#include "cfg.h"
#include "highlevel_formatter.h"
#include "lowlevel_formatter.h"
#include "live_vregs.h"
#include "print_instruction_seq.h"
#include "print_cfg.h"

////////////////////////////////////////////////////////////////////////
// PrintHighLevelCFG implementation
////////////////////////////////////////////////////////////////////////

PrintHighLevelCFG::PrintHighLevelCFG() {
}

PrintHighLevelCFG::~PrintHighLevelCFG() {
}

void PrintHighLevelCFG::print_instructions(const std::shared_ptr<InstructionSequence> &iseq) {
  HighLevelControlFlowGraphBuilder cfg_builder(iseq);

  std::shared_ptr<ControlFlowGraph> hl_cfg = cfg_builder.build();

  print_cfg(hl_cfg);
}

void PrintHighLevelCFG::print_cfg(const std::shared_ptr<ControlFlowGraph> &hl_cfg) {
  HighLevelControlFlowGraphPrinter hl_cfg_printer(hl_cfg);
  hl_cfg_printer.print();
}

////////////////////////////////////////////////////////////////////////
// Implementation of PrintInstructionSequence which annotates
// the formatted instructions with liveness information.
////////////////////////////////////////////////////////////////////////

class PrintInstructionSequenceWithLiveness : public PrintInstructionSequence {
private:
  LiveVregs *m_live_vregs;

public:
  PrintInstructionSequenceWithLiveness(Formatter *formatter, LiveVregs *live_vregs);
  virtual ~PrintInstructionSequenceWithLiveness();

  virtual std::string get_instruction_annotation(const InstructionSequence *iseq, Instruction *ins);

  std::string format_set(const LiveVregs::FactType &live_set);
};

PrintInstructionSequenceWithLiveness::PrintInstructionSequenceWithLiveness(Formatter *formatter, LiveVregs *live_vregs)
  : PrintInstructionSequence(formatter)
  , m_live_vregs(live_vregs) {
}

PrintInstructionSequenceWithLiveness::~PrintInstructionSequenceWithLiveness() {
}

std::string PrintInstructionSequenceWithLiveness::get_instruction_annotation(const InstructionSequence *iseq, Instruction *ins) {
  // The InstructionSequence is actually a BasicBlock
  const BasicBlock *bb = static_cast<const BasicBlock *>(iseq);

  // get dataflow fact just before the instruction
  LiveVregs::FactType fact = m_live_vregs->get_fact_before_instruction(bb, ins);
  std::string s = format_set(fact);
  //printf("annotation: %s\n", s.c_str());
  return s;
}

std::string PrintInstructionSequenceWithLiveness::format_set(const LiveVregs::FactType &live_set) {
  return LiveVregs::fact_to_string(live_set);
}

////////////////////////////////////////////////////////////////////////
// LivenessHighLevelCFGPrinter implementation
//
// This class overrides HighLevelControlFlowGraphPrinter,
// in order to use an implementation of PrintInstructionSequence
// that annotates each high-level instruction with computed
// liveness dataflow facts.
////////////////////////////////////////////////////////////////////////

class LivenessHighLevelCFGPrinter : public HighLevelControlFlowGraphPrinter {
private:
  LiveVregs m_live_vregs;

public:
  LivenessHighLevelCFGPrinter(const std::shared_ptr<ControlFlowGraph> &cfg);
  virtual ~LivenessHighLevelCFGPrinter();

  virtual std::string get_block_begin_annotation(BasicBlock *bb);
  virtual std::string get_block_end_annotation(BasicBlock *bb);

  virtual void print_basic_block(BasicBlock *bb);
};

LivenessHighLevelCFGPrinter::LivenessHighLevelCFGPrinter(const std::shared_ptr<ControlFlowGraph> &cfg)
  : HighLevelControlFlowGraphPrinter(cfg)
  , m_live_vregs(cfg) {
  m_live_vregs.execute();
}

LivenessHighLevelCFGPrinter::~LivenessHighLevelCFGPrinter() {
}

std::string LivenessHighLevelCFGPrinter::get_block_begin_annotation(BasicBlock *bb) {
  LiveVregs::FactType fact = m_live_vregs.get_fact_at_beginning_of_block(bb);
  return LiveVregs::fact_to_string(fact);
}

std::string LivenessHighLevelCFGPrinter::get_block_end_annotation(BasicBlock *bb) {
  LiveVregs::FactType fact = m_live_vregs.get_fact_at_end_of_block(bb);
  return LiveVregs::fact_to_string(fact);
}

void LivenessHighLevelCFGPrinter::print_basic_block(BasicBlock *bb) {
  HighLevelFormatter hl_formatter;
  PrintInstructionSequenceWithLiveness print_iseq(&hl_formatter, &m_live_vregs);

  print_iseq.print(bb);
}

////////////////////////////////////////////////////////////////////////
// PrintHighLevelCFGWithLiveness implementation
////////////////////////////////////////////////////////////////////////

PrintHighLevelCFGWithLiveness::PrintHighLevelCFGWithLiveness() {
}

PrintHighLevelCFGWithLiveness::~PrintHighLevelCFGWithLiveness() {
}

void PrintHighLevelCFGWithLiveness::print_cfg(const std::shared_ptr<ControlFlowGraph> &hl_cfg) {
  LivenessHighLevelCFGPrinter cfg_printer(hl_cfg);
  cfg_printer.print();
}

////////////////////////////////////////////////////////////////////////
// PrintLowLevelCFG implementation
////////////////////////////////////////////////////////////////////////

PrintLowLevelCFG::PrintLowLevelCFG() {
}

PrintLowLevelCFG::~PrintLowLevelCFG() {
}

void PrintLowLevelCFG::print_instructions(const std::shared_ptr<InstructionSequence> &iseq) {
  LowLevelControlFlowGraphBuilder ll_cfg_builder(iseq);
  std::shared_ptr<ControlFlowGraph> ll_cfg = ll_cfg_builder.build();
  print_cfg(ll_cfg);
}

void PrintLowLevelCFG::print_cfg(const std::shared_ptr<ControlFlowGraph> &cfg) {
  LowLevelControlFlowGraphPrinter ll_cfg_printer(cfg);
  ll_cfg_printer.print();
}
