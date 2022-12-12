#include <cassert>
#include "cfg.h"
#include "cfg_transform.h"
#include "highlevel_defuse.h"
#include "highlevel.h"
#include <unordered_map>
#include "highlevel_formatter.h"
#include "lowlevel_codegen.h"

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
    // transformed_bb = constant_fold(transformed_bb.get());
    transformed_bb = lvn(transformed_bb.get(), orig);
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

class MyHashFunction{
public:
  size_t operator()(const Instruction& i) const{
    auto result = std::hash<int>()(i.get_opcode());
    Operand first = i.get_operand(1);
    Operand second = i.get_operand(2);
    if(first.is_reg()){
      result += std::hash<int>()(first.get_base_reg());
    }
    if(second.is_reg()){
      result += std::hash<int>()(second.get_base_reg());
    }
    if(first.is_imm_ival()){
      unsigned long temp = std::hash<int>()(first.get_imm_ival());
      result += std::hash<unsigned long>()(temp);
    }
    if(second.is_imm_ival()){
      unsigned long temp = std::hash<int>()(second.get_imm_ival());
      result += std::hash<unsigned long>()(temp);
    }
    if(first.is_memref()){
      unsigned long temp = std::hash<int>()(first.get_base_reg());
      temp = std::hash<unsigned long>()(temp);
      result += std::hash<unsigned long>()(temp);
    }
    if(first.is_memref()){
      unsigned long temp = std::hash<int>()(second.get_base_reg());
      temp = std::hash<unsigned long>()(temp);
      result += std::hash<unsigned long>()(temp);
    }
    return result;
  }
};

/*
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
    std::string formatted_ins = formatter.format_instruction(new_ins);
    // printf("\t%s\n", formatted_ins.c_str());

    // if destination is changed, need to update
    if(orig_ins->get_opcode() == HINS_localaddr){
      Operand dest = orig_ins->get_operand(0);
      int dest_vreg = dest.get_base_reg();
      if(vregVal.find(dest_vreg) != vregVal.end()){
        vregVal.erase(dest_vreg);
      }
    } else if(HighLevel::is_def(orig_ins)){
      loop_check(1, new_ins, orig_ins, vregVal);

      Operand dest = orig_ins->get_operand(0);
      int dest_vreg = dest.get_base_reg();
      if(vregVal.find(dest_vreg) != vregVal.end()){
        vregVal.erase(dest_vreg);
      }
      if(orig_ins->get_num_operands() == 2 && orig_ins->get_operand(1).is_imm_ival()){
        vregVal[dest_vreg] = orig_ins->get_operand(1).get_imm_ival();
        delete new_ins;
        new_ins = nullptr;
      }
      // else{
      //   printf("has %d\n", vregVal.find(11) != vregVal.end());

      // }
    } else{
      loop_check(0, new_ins, orig_ins, vregVal);
    }
    if(new_ins){
      result_iseq->append(new_ins);
      std::string formatted_ins = formatter.format_instruction(new_ins);
      // printf("\t%s\n", formatted_ins.c_str());
      new_ins = nullptr;
    }

  }

  // return std::shared_ptr<InstructionSequence>(orig_bb->duplicate());
  return result_iseq;
}
*/
static long val = 0;
std::shared_ptr<InstructionSequence>
MyOptimization::lvn(const InstructionSequence* orig_bb, const BasicBlock* orig){
  std::shared_ptr<InstructionSequence> result_iseq(new InstructionSequence());
  //hash the instruction
  // std::unordered_map<Instruction, Operand, MyHashFunction> val_map;
  // bool ignore_sconv = false;
  val_to_ival.clear();
  LiveVregs::FactType live_after = m_live_vregs.get_fact_at_end_of_block(orig);
  for(auto i = orig_bb->cbegin(); i != orig_bb->cend(); ++i){
    Instruction* orig_ins = *i;
    Instruction* new_ins = orig_ins->duplicate();
    Operand first, second;
    HighLevelOpcode opcode = (HighLevelOpcode)orig_ins->get_opcode();

    HighLevelFormatter formatter;
    std::string formatted_ins = formatter.format_instruction(new_ins);
    // printf("\t%s\n", formatted_ins.c_str());

    // can ignore moving constant or register to register, but not memory
    if(match_hl(HINS_mov_b, opcode)){
      first = orig_ins->get_operand(0);
      second = orig_ins->get_operand(1);
      recursive_find(second);
      new_ins->set_operand(second, 1);
      if(first.is_reg()){
        val_to_ival[first.get_base_reg()] = second;
        if(second.is_reg()){
          if(orig_ins->get_operand(1).is_memref()){
            new_ins->set_operand(second.to_memref(), 1);
          } else{
            if(first.get_base_reg() > 9 && !live_after.test(first.get_base_reg())){
              delete new_ins;
              new_ins = nullptr;
            }
          }
        }
        // cannot ignore argument assignment
        else if(first.get_base_reg() > 9 && !live_after.test(first.get_base_reg())){
          delete new_ins;
          new_ins = nullptr;
        }
        // can ignore if both registers are the same
        else if(second.is_reg() && first.get_base_reg() == second.get_base_reg()){
          delete new_ins;
          new_ins = nullptr;
        }
      } else if(first.is_memref()){
        if(val_to_ival.find(first.get_base_reg()) != val_to_ival.end()){
          Operand candidate = val_to_ival[first.get_base_reg()];
          if(candidate.is_reg()){
            candidate = candidate.to_memref();
            new_ins->set_operand(candidate, 0);
          }
          val_to_ival[first.get_base_reg()] = candidate;
        }
      }
    } else if(HighLevel::is_def(orig_ins)){
      int dest_reg = orig_ins->get_operand(0).get_base_reg();
      int foldable = 0;
      for(int i = 1; i < orig_ins->get_num_operands(); i++){
        second = orig_ins->get_operand(i);
        if(second.is_imm_ival()){
          foldable++;
        }
        if(second.is_reg() && !m_live_vregs.get_fact_at_end_of_block(orig).test(second.get_base_reg())){
          recursive_find(second);
          if(second.is_imm_ival()){
            foldable++;
          }
          new_ins->set_operand(second, i);
        }
      }

      val_to_ival.erase(orig_ins->get_operand(0).get_base_reg());
      // if all operands are constants for 2 operand operations, constant fold
      opcode = is_basic_operation(opcode);
      if(foldable == 2 && opcode){
        first = new_ins->get_operand(1);
        second = new_ins->get_operand(2);
        assert(first.is_imm_ival() && second.is_imm_ival());
        int first_ival = first.get_imm_ival();
        int second_ival = second.get_imm_ival();
        if(opcode == HINS_add_b){
          first_ival = first_ival + second_ival;
        } else if(opcode == HINS_sub_b){
          first_ival = first_ival - second_ival;
        } else if(opcode == HINS_div_b){
          first_ival = first_ival / second_ival;
        } else if(opcode == HINS_mul_b){
          first_ival = first_ival * second_ival;
        }
        val_to_ival[dest_reg] = Operand(Operand::IMM_IVAL, first_ival);
        delete new_ins;
        new_ins = nullptr;
      }
    } else if(orig_ins->get_opcode() == HINS_localaddr){
      first = orig_ins->get_operand(0);
      second = orig_ins->get_operand(1);

    } else if(orig_ins->get_opcode() >= HINS_sconv_bw && orig_ins->get_opcode() <= HINS_sconv_lq){
      first = orig_ins->get_operand(0);
      second = orig_ins->get_operand(1);
      recursive_find(second);
      if(second.is_imm_ival()){
        val_to_ival[first.get_base_reg()] = second;
        delete new_ins;
        new_ins = nullptr;
      }
    }


    if(new_ins){
      result_iseq->append(new_ins);
      std::string formatted_ins = formatter.format_instruction(new_ins);
      // printf("\t%s\n", formatted_ins.c_str());
      new_ins = nullptr;
    }

  }
  // puts("");

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

void MyOptimization::loop_check(int i, Instruction*& new_ins, Instruction*& orig_ins, std::unordered_map<int, long>& vregVal){
  for(int j = i; j < orig_ins->get_num_operands(); j++){
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
long MyOptimization::set_val(std::map<long, long>& m, Operand o){
  if(m.find(o.get_base_reg()) != m.end()){
    return m[o.get_base_reg()];
  }
  m[o.get_base_reg()] = val;
  return val++;
}
void MyOptimization::recursive_find(Operand& o){
  if(o.is_imm_ival() || o.get_base_reg() < 10 || val_to_ival.find(o.get_base_reg()) == val_to_ival.end()){
    return;
  }
  Operand key = val_to_ival[o.get_base_reg()];
  if(key.has_base_reg() && o.get_base_reg() == key.get_base_reg()){
    return;
  }
  o = key;
  recursive_find(o);
}
HighLevelOpcode MyOptimization::is_basic_operation(HighLevelOpcode opcode){
  if(match_hl(HINS_add_b, opcode)){
    return HINS_add_b;
  } else if(match_hl(HINS_sub_b, opcode)){
    return HINS_sub_b;
  } else if(match_hl(HINS_mul_b, opcode)){
    return HINS_mul_b;
  } else if(match_hl(HINS_div_b, opcode)){
    return HINS_div_b;
  } else{
    return HINS_nop;
  }
}