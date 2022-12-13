#include <cassert>
#include "cfg.h"
#include "cfg_transform.h"
#include "highlevel_defuse.h"
#include "highlevel.h"
#include <map>
#include "highlevel_formatter.h"
#include "lowlevel_codegen.h"
#include "lowlevel.h"



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
    //10 iterations of constant propagation and constant folding
    for(auto i = 0; i < 10; i++){
      transformed_bb = constant_fold(transformed_bb.get(), orig);
    }
    // transformed_bb = reg_alloc(transformed_bb.get(), orig);
    for(auto i = 0; i < 10; i++){
      transformed_bb = copy_prop(transformed_bb.get(), orig);
    }

    for(auto i = transformed_bb->cbegin(); i != transformed_bb->cend(); i++){
      HighLevelFormatter formatter;
      std::string formatted_ins = formatter.format_instruction(*i);
      // printf("\t%s\n", formatted_ins.c_str());
    }
    // puts("");

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
MyOptimization::constant_fold(const InstructionSequence* orig_bb, BasicBlock* orig){

  std::shared_ptr<InstructionSequence> result_iseq(new InstructionSequence());
  std::unordered_map<int, long> vregVal;
  // idea: unless assigned new value, replace reference to that vreg with the constant val
  for(auto i = orig_bb->cbegin(); i != orig_bb->cend(); ++i){
    Instruction* orig_ins = *i;
    Instruction* new_ins = orig_ins->duplicate();

    // if destination is changed, need to update
    if(orig_ins->get_opcode() == HINS_localaddr){
      Operand dest = orig_ins->get_operand(0);
      int dest_vreg = dest.get_base_reg();
      if(vregVal.find(dest_vreg) != vregVal.end()){
        vregVal.erase(dest_vreg);
      }
    } else if(match_hl(HINS_mov_b, orig_ins->get_opcode())){
      Operand dest = orig_ins->get_operand(0);
      Operand first = orig_ins->get_operand(1);
      int dest_vreg = dest.get_base_reg();
      if(first.is_reg()){
        if(vregVal.find(first.get_base_reg()) != vregVal.end() && !m_live_vregs.get_fact_after_instruction(orig, orig_ins).test(first.get_base_reg())){
          Operand new_op(Operand::IMM_IVAL, vregVal[first.get_base_reg()]);
          new_ins->set_operand(new_op, 1);
        } else{
          vregVal.erase(dest_vreg);
        }
      }
      if(first.is_imm_ival() && !dest.is_memref() && dest.get_base_reg() > 9 && !m_live_vregs.get_fact_at_end_of_block(orig).test(dest.get_base_reg())){
        vregVal[dest_vreg] = first.get_imm_ival();
        delete new_ins;
        new_ins = nullptr;
      }
    } else if(HighLevel::is_def(orig_ins)){
      Operand dest = orig_ins->get_operand(0);
      Operand first = orig_ins->get_operand(1);
      int dest_vreg = dest.get_base_reg();
      if(first.is_reg()){
        if(vregVal.find(first.get_base_reg()) != vregVal.end() && !m_live_vregs.get_fact_after_instruction(orig, orig_ins).test(first.get_base_reg())){
          Operand new_op(Operand::IMM_IVAL, vregVal[first.get_base_reg()]);
          new_ins->set_operand(new_op, 1);
        }
      }
      if(orig_ins->get_num_operands() == 2){
        if(first.is_imm_ival() && !match_hl(HINS_neg_b, orig_ins->get_opcode())){
          vregVal[dest_vreg] = first.get_imm_ival();
          delete new_ins;
          new_ins = nullptr;
        } else{
          vregVal.erase(dest.get_base_reg());
        }
      }
      if(orig_ins->get_num_operands() == 3){
        Operand second = orig_ins->get_operand(2);
        if(second.is_reg()){
          if(vregVal.find(second.get_base_reg()) != vregVal.end() && !m_live_vregs.get_fact_after_instruction(orig, orig_ins).test(second.get_base_reg())){
            Operand new_op(Operand::IMM_IVAL, vregVal[second.get_base_reg()]);
            new_ins->set_operand(new_op, 2);
          }
        }
        if(first.is_imm_ival() && second.is_imm_ival()){
          HighLevelOpcode opcode = (HighLevelOpcode)orig_ins->get_opcode();
          long long first_ival = first.get_imm_ival();
          long long second_ival = second.get_imm_ival();
          if(match_hl(HINS_add_b, opcode)){
            first_ival = first_ival + second_ival;
            vregVal[dest.get_base_reg()] = first_ival;
            delete new_ins;
            new_ins = nullptr;
          } else if(match_hl(HINS_sub_b, opcode)){
            first_ival = first_ival - second_ival;
            vregVal[dest.get_base_reg()] = first_ival;
            delete new_ins;
            new_ins = nullptr;
          } else if(match_hl(HINS_div_b, opcode)){
            first_ival = first_ival / second_ival;
            vregVal[dest.get_base_reg()] = first_ival;
            delete new_ins;
            new_ins = nullptr;
          } else if(match_hl(HINS_mul_b, opcode)){
            first_ival = first_ival * second_ival;
            vregVal[dest.get_base_reg()] = first_ival;
            delete new_ins;
            new_ins = nullptr;
          }
        } else{
          vregVal.erase(dest.get_base_reg());
        }
      }
    }
    if(new_ins){
      result_iseq->append(new_ins);
      new_ins = nullptr;
    }

  }
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
      //If a vreg is not alive at the end of the basic block, that means it's not used for the rest of the basic blocks
      if(!live_after.test(dest.get_base_reg()) && dest.get_base_reg() > 9){
        preserve_instruction = false;
      }
    }

    if(preserve_instruction)
      result_iseq->append(orig_ins->duplicate());
  }

  return result_iseq;
}

struct myComp{
  bool operator()(const Operand* a, const Operand* b)const{
    return a->use_cnt < b->use_cnt;
  }
};
bool myComp2(std::pair<Operand*, int> a, std::pair<Operand*, int> b){
  return a.first->use_cnt < b.first->use_cnt;
}
std::shared_ptr<InstructionSequence>
MyOptimization::reg_alloc(const InstructionSequence* orig_bb){
  std::shared_ptr<InstructionSequence> result_iseq(new InstructionSequence());
  const BasicBlock* orig = static_cast<const BasicBlock*>(orig_bb);
  // register maps to virtual register(holder)
  std::multimap<Operand*, int, myComp> registers;
  std::map<int, Operand> local_reg;
  for(int i = 12; i < 16; i++){
    Operand* reg = new Operand((Operand::Kind)4, (MachineReg)i);
    registers.emplace(reg, 0);
  }
  LiveVregs::FactType live_after = m_live_vregs.get_fact_at_end_of_block(orig);
  LiveVregs::FactType live_before = m_live_vregs.get_fact_at_beginning_of_block(orig);

  for(auto j = orig_bb->cbegin(); j != orig_bb->cend(); ++j){
    Instruction* orig_ins = *j;
    Instruction* new_ins = orig_ins->duplicate();
    HighLevelOpcode opcode = (HighLevelOpcode)orig_ins->get_opcode();

    for(int i = 0; i < new_ins->get_num_operands(); i++){
      Operand op = new_ins->get_operand(i);
      if(op.has_base_reg() && !live_after.test(op.get_base_reg()) && !live_before.test(op.get_base_reg()) && op.get_base_reg() > 9){
        //unoccupied
        for(auto& p : registers){
          if(p.second == op.get_base_reg()){
            Operand reg = local_reg[op.get_base_reg()];
            new_ins->set_operand(reg, i);
            p.first->use_cnt += 1;
            break;
          } else if(p.second == 0 && opcode > 0 && opcode <= 88){
            int size = highlevel_opcode_get_source_operand_size(opcode);
            auto kind = select_mreg_kind(size);
            auto b_reg = p.first->get_base_reg();
            Operand reg = Operand(kind, b_reg);
            new_ins->set_operand(reg, i);
            p.first->use_cnt += 1;
            p.second = op.get_base_reg();
            local_reg[op.get_base_reg()] = reg;
            break;
          }
        }
      }
    }

    if(new_ins){
      result_iseq->append(new_ins);
      HighLevelFormatter formatter;
      std::string formatted_ins = formatter.format_instruction(new_ins);
      // printf("\t%s\n", formatted_ins.c_str());
      new_ins = nullptr;
    }
  }
  return result_iseq;
}

std::shared_ptr<InstructionSequence>
MyOptimization::copy_prop(const InstructionSequence* orig_bb, BasicBlock* orig){
  const BasicBlock* bb_orig = static_cast<const BasicBlock*>(orig_bb);
  std::shared_ptr<InstructionSequence> result_iseq(new InstructionSequence());
  std::unordered_map<int, Operand> op_map;
  LiveVregs::FactType live_after = m_live_vregs.get_fact_at_end_of_block(orig);
  for(auto j = orig_bb->cbegin(); j != orig_bb->cend(); ++j){
    Instruction* orig_ins = *j;
    Instruction* new_ins = orig_ins->duplicate();
    int total_operand = orig_ins->get_num_operands();
    if(orig_ins->get_num_operands() > 0){
      Operand dest = orig_ins->get_operand(0);
      if(dest.has_base_reg() && op_map.find(dest.get_base_reg()) != op_map.end() && m_live_vregs.get_fact_after_instruction(orig, orig_ins).test(dest.get_base_reg())){
        Operand candidate = op_map[dest.get_base_reg()];
        if(dest.is_memref()){
          new_ins->set_operand(candidate.to_memref(), 0);
        } else{
          new_ins->set_operand(candidate, 0);
        }
      }
      for(int i = 1; i < total_operand; i++){
        Operand op = orig_ins->get_operand(i);
        if(op.has_base_reg()){
          if(match_hl(HINS_mov_b, orig_ins->get_opcode())){
            if(result_iseq->get_length()){

              Instruction* last_ins = result_iseq->get_last_instruction();
              if(last_ins != nullptr && HighLevel::is_def(last_ins)){
                Operand last_dest = last_ins->get_operand(0);
                if(last_dest.is_reg() && op.is_reg() && last_dest.get_base_reg() == op.get_base_reg() && !m_live_vregs.get_fact_after_instruction(bb_orig, orig_ins).test(op.get_base_reg())){
                  last_ins->set_operand(dest, 0);
                  op_map[last_dest.get_base_reg()] = dest;
                  delete new_ins;
                  new_ins = nullptr;
                }
              }
            }
          } else if(op_map.find(op.get_base_reg()) != op_map.end()){
            Operand candidate = op_map[op.get_base_reg()];
            if(op.is_reg() && candidate.is_memref()){
              new_ins->set_operand(candidate, i);
            } else{
              new_ins->set_operand(candidate, i);
            }
          }
        }
      }
    }
    if(new_ins){
      result_iseq->append(new_ins);
      new_ins = nullptr;
    }
  }

  return result_iseq;
}