#include <cassert>
#include "cfg.h"
#include "cfg_transform.h"
#include "highlevel_defuse.h"
#include <unordered_map>
#include "highlevel_formatter.h"



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
    std::shared_ptr<InstructionSequence> transformed_bb = dead_store(orig);
    // for(auto i = 0; i < 2; i++){
    transformed_bb = constant_fold(transformed_bb.get());
    // }

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
  //does the liveness analysis so that the facts can be used by transform_basic_block
  m_live_vregs.execute();
}

MyOptimization::~MyOptimization(){
}

std::shared_ptr<InstructionSequence>
MyOptimization::constant_fold(const InstructionSequence* orig_bb){
  const BasicBlock* orig_bb_as_basic_block = static_cast<const BasicBlock*>(orig_bb);

  std::shared_ptr<InstructionSequence> result_iseq(new InstructionSequence());
  std::unordered_map<int, long> vregVal;
  // idea: unless assigned new value, replace reference to that vreg with the constant val
  for(auto i = orig_bb->cbegin(); i != orig_bb->cend(); ++i){
    Instruction* orig_ins = *i;
    Instruction* new_ins = orig_ins->duplicate();

    HighLevelFormatter formatter;
    // puts("beginning");
    // std::string formatted_ins = formatter.format_instruction(new_ins);
    // printf("\t%s\n", formatted_ins.c_str());

    // if destination is changed, need to update
    if(HighLevel::is_def(orig_ins)){
      Operand dest = orig_ins->get_operand(0);
      int dest_vreg = dest.get_base_reg();
      if(vregVal.find(dest_vreg) != vregVal.end()){
        vregVal.erase(dest_vreg);
      }
      if(orig_ins->get_num_operands() == 2 && orig_ins->get_operand(1).is_imm_ival()){
        vregVal[dest_vreg] = orig_ins->get_operand(1).get_imm_ival();
        delete new_ins;
        new_ins = nullptr;
      } else{
        for(int j = 1; j < orig_ins->get_num_operands(); j++){
          Operand op = orig_ins->get_operand(j);
          if(!op.is_memref() && !op.is_non_reg()){
            int op_vreg = op.get_base_reg();
            if(vregVal.find(op_vreg) != vregVal.end()){
              Operand new_op(Operand::IMM_IVAL, vregVal[op_vreg]);
              new_ins->set_operand(new_op, j);
            }
          }
        }
      }
    } else{
      for(int j = 0; j < orig_ins->get_num_operands(); j++){
        Operand op = orig_ins->get_operand(j);
        if(!op.is_memref() && !op.is_non_reg()){
          int op_vreg = op.get_base_reg();
          if(vregVal.find(op_vreg) != vregVal.end()){
            new_ins = orig_ins->duplicate();
            Operand new_op(Operand::IMM_IVAL, vregVal[op_vreg]);
            new_ins->set_operand(new_op, j);
          }

        }
      }
    }
    if(new_ins){
      result_iseq->append(new_ins);
      std::string formatted_ins = formatter.format_instruction(new_ins);
      // printf("\t%s\n", formatted_ins.c_str());
      // puts("above is generated");
      new_ins = nullptr;
    }

  }

  // return std::shared_ptr<InstructionSequence>(orig_bb->duplicate());
  return result_iseq;
}

std::shared_ptr<InstructionSequence>
MyOptimization::dead_store(const InstructionSequence* orig_bb){
  const BasicBlock* orig_bb_as_basic_block = static_cast<const BasicBlock*>(orig_bb);
  std::shared_ptr<InstructionSequence> result_iseq(new InstructionSequence());
  // puts("block separator");
  for(auto i = orig_bb->cbegin(); i != orig_bb->cend(); ++i){
    Instruction* orig_ins = *i;
    bool preserve_instruction = true;

    if(HighLevel::is_def(orig_ins)){
      Operand dest = orig_ins->get_operand(0);

      LiveVregs::FactType live_after = m_live_vregs.get_fact_after_instruction(orig_bb_as_basic_block, orig_ins);
      // printf("fact %s\n", LiveVregs::fact_to_string(live_after).c_str());


      //If a vreg is not alive at the end of the basic block, that means it's not used for the rest of the basic blocks
      if(!live_after.test(dest.get_base_reg()) && dest.get_base_reg() != 0){
        preserve_instruction = false;
      }
    }

    if(preserve_instruction)
      result_iseq->append(orig_ins->duplicate());
  }

  return result_iseq;

}