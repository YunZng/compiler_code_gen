#include <cassert>
#include "cfg.h"
#include "cfg_transform.h"
#include "highlevel_defuse.h"


ControlFlowGraphTransform::ControlFlowGraphTransform(const std::shared_ptr<ControlFlowGraph>& cfg)
  : m_cfg(cfg){
}

ControlFlowGraphTransform::~ControlFlowGraphTransform(){
}

std::shared_ptr<ControlFlowGraph> ControlFlowGraphTransform::get_orig_cfg(){
  return m_cfg;
}

std::shared_ptr<ControlFlowGraph> ControlFlowGraphTransform::transform_cfg(){
  std::shared_ptr<ControlFlowGraph> result(new ControlFlowGraph());

  // map of basic blocks of original CFG to basic blocks in transformed CFG
  std::map<BasicBlock*, BasicBlock*> block_map;

  // iterate over all basic blocks, transforming each one
  for(auto i = m_cfg->bb_begin(); i != m_cfg->bb_end(); i++){
    BasicBlock* orig = *i;

    // Transform the instructions
    std::shared_ptr<InstructionSequence> transformed_bb = transform_basic_block(orig);

    // Create transformed basic block; note that we set its
    // code order to be the same as the code order of the original
    // block (with the hope of eventually reconstructing an InstructionSequence
    // with the transformed blocks in an order that matches the original
    // block order)
    BasicBlock* result_bb = result->create_basic_block(orig->get_kind(), orig->get_code_order(), orig->get_label());
    for(auto j = transformed_bb->cbegin(); j != transformed_bb->cend(); ++j)
      result_bb->append((*j)->duplicate());

    block_map[orig] = result_bb;
  }

  // add edges to transformed CFG
  for(auto i = m_cfg->bb_begin(); i != m_cfg->bb_end(); i++){
    BasicBlock* orig = *i;
    const ControlFlowGraph::EdgeList& outgoing_edges = m_cfg->get_outgoing_edges(orig);
    for(auto j = outgoing_edges.cbegin(); j != outgoing_edges.cend(); j++){
      Edge* orig_edge = *j;

      BasicBlock* transformed_source = block_map[orig_edge->get_source()];
      BasicBlock* transformed_target = block_map[orig_edge->get_target()];

      result->create_edge(transformed_source, transformed_target, orig_edge->get_kind());
    }
  }

  return result;
}

MyOptimization::MyOptimization(const std::shared_ptr<ControlFlowGraph>& cfg)
  : ControlFlowGraphTransform(cfg)
  , m_live_vregs(cfg){
  m_live_vregs.execute();
}

MyOptimization::~MyOptimization(){
}

std::shared_ptr<InstructionSequence>
MyOptimization::transform_basic_block(const InstructionSequence* orig_bb){
  const BasicBlock* orig_bb_as_basic_block = static_cast<const BasicBlock*>(orig_bb);

  std::shared_ptr<InstructionSequence> result_iseq(new InstructionSequence());

  for(auto i = orig_bb->cbegin(); i != orig_bb->cend(); ++i){
    Instruction* orig_ins = *i;
    bool preserve_instruction = true;

    // if has destination, get destination, 
    if(HighLevel::is_def(orig_ins)){
      Operand dest = orig_ins->get_operand(0);

      LiveVregs::FactType live_after = m_live_vregs.get_fact_after_instruction(orig_bb_as_basic_block, orig_ins);
      printf("dest.get_base_reg %d\n", dest.get_base_reg());
      // printf("dest.get_base_reg %d\n", dest.get_base_reg());

      if(!live_after.test(dest.get_base_reg()))
        preserve_instruction = false;
      // destination register is dead immediately after this instruction,
      // so it can be eliminated
    }

    if(preserve_instruction)
      result_iseq->append(orig_ins->duplicate());
  }

  return result_iseq;

}