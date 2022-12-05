#include <cassert>
#include <map>
#include "node.h"
#include "instruction.h"
#include "operand.h"
#include "local_storage_allocation.h"
#include "highlevel.h"
#include "lowlevel.h"
#include "exceptions.h"
#include "lowlevel_codegen.h"
#include "cfg.h"
#include "cfg_transform.h"

namespace{

  // This map has some "obvious" translations of high-level opcodes to
  // low-level opcodes.
  const std::map<HighLevelOpcode, LowLevelOpcode> HL_TO_LL = {
    { HINS_nop, MINS_NOP},
    { HINS_add_b, MINS_ADDB },
    { HINS_add_w, MINS_ADDW },
    { HINS_add_l, MINS_ADDL },
    { HINS_add_q, MINS_ADDQ },
    { HINS_sub_b, MINS_SUBB },
    { HINS_sub_w, MINS_SUBW },
    { HINS_sub_l, MINS_SUBL },
    { HINS_sub_q, MINS_SUBQ },
    { HINS_mul_l, MINS_IMULL },
    { HINS_mul_q, MINS_IMULQ },
    { HINS_mov_b, MINS_MOVB },
    { HINS_mov_w, MINS_MOVW },
    { HINS_mov_l, MINS_MOVL },
    { HINS_mov_q, MINS_MOVQ },
    { HINS_sconv_bw, MINS_MOVSBW },
    { HINS_sconv_bl, MINS_MOVSBL },
    { HINS_sconv_bq, MINS_MOVSBQ },
    { HINS_sconv_wl, MINS_MOVSWL },
    { HINS_sconv_wq, MINS_MOVSWQ },
    { HINS_sconv_lq, MINS_MOVSLQ },
    { HINS_uconv_bw, MINS_MOVZBW },
    { HINS_uconv_bl, MINS_MOVZBL },
    { HINS_uconv_bq, MINS_MOVZBQ },
    { HINS_uconv_wl, MINS_MOVZWL },
    { HINS_uconv_wq, MINS_MOVZWQ },
    { HINS_uconv_lq, MINS_MOVZLQ },
    { HINS_ret, MINS_RET },
    { HINS_jmp, MINS_JMP },
    { HINS_call, MINS_CALL },

    // For comparisons, it is expected that the code generator will first
    // generate a cmpb/cmpw/cmpl/cmpq instruction to compare the operands,
    // and then generate a setXX instruction to put the result of the
    // comparison into the destination operand. These entries indicate
    // the apprpropriate setXX instruction to use.
    { HINS_cmplt_b, MINS_SETL },
    { HINS_cmplt_w, MINS_SETL },
    { HINS_cmplt_l, MINS_SETL },
    { HINS_cmplt_q, MINS_SETL },
    { HINS_cmplte_b, MINS_SETLE },
    { HINS_cmplte_w, MINS_SETLE },
    { HINS_cmplte_l, MINS_SETLE },
    { HINS_cmplte_q, MINS_SETLE },
    { HINS_cmpgt_b, MINS_SETG },
    { HINS_cmpgt_w, MINS_SETG },
    { HINS_cmpgt_l, MINS_SETG },
    { HINS_cmpgt_q, MINS_SETG },
    { HINS_cmpgte_b, MINS_SETGE },
    { HINS_cmpgte_w, MINS_SETGE },
    { HINS_cmpgte_l, MINS_SETGE },
    { HINS_cmpgte_q, MINS_SETGE },
    { HINS_cmpeq_b, MINS_SETE },
    { HINS_cmpeq_w, MINS_SETE },
    { HINS_cmpeq_l, MINS_SETE },
    { HINS_cmpeq_q, MINS_SETE },
    { HINS_cmpneq_b, MINS_SETNE },
    { HINS_cmpneq_w, MINS_SETNE },
    { HINS_cmpneq_l, MINS_SETNE },
    { HINS_cmpneq_q, MINS_SETNE },
  };

}

LowLevelCodeGen::LowLevelCodeGen(bool optimize)
  : m_total_memory_storage(0)
  , m_optimize(optimize){
  highest = 10;
}

LowLevelCodeGen::~LowLevelCodeGen(){
}

std::shared_ptr<InstructionSequence> LowLevelCodeGen::generate(const std::shared_ptr<InstructionSequence>& hl_iseq){
  // TODO: if optimizations are enabled, could do analysis/transformation of high-level code
  Node* funcdef_ast = hl_iseq->get_funcdef_ast();

  // cur_hl_iseq is the "current" version of the high-level IR,
  // which could be a transformed version if we are doing optimizations
  std::shared_ptr<InstructionSequence> cur_hl_iseq(hl_iseq);

  if(m_optimize){
    // Create a control-flow graph representation of the high-level code
    HighLevelControlFlowGraphBuilder hl_cfg_builder(cur_hl_iseq);
    std::shared_ptr<ControlFlowGraph> cfg = hl_cfg_builder.build();

    // Do local optimizations
    MyOptimization hl_opts(cfg);
    cfg = hl_opts.transform_cfg();

    // Convert the transformed high-level CFG back to an InstructionSequence
    cur_hl_iseq = cfg->create_instruction_sequence();

    // The function definition AST might have information needed for
    // low-level code generation
    cur_hl_iseq->set_funcdef_ast(funcdef_ast);
  }

  // Translate (possibly transformed) high-level code into low-level code
  std::shared_ptr<InstructionSequence> ll_iseq = translate_hl_to_ll(cur_hl_iseq);

  if(m_optimize){
    // ...could do transformations on the low-level code, possible peephole
    //    optimizations...
  }

  return ll_iseq;
}

std::shared_ptr<InstructionSequence> LowLevelCodeGen::translate_hl_to_ll(const std::shared_ptr<InstructionSequence>& hl_iseq){
  std::shared_ptr<InstructionSequence> ll_iseq(new InstructionSequence());

  // The high-level InstructionSequence will have a pointer to the Node
  // representing the function definition. Useful information could be stored
  // there (for example, about the amount of memory needed for local storage,
  // maximum number of virtual registers used, etc.)
  Node* funcdef_ast = hl_iseq->get_funcdef_ast();
  assert(funcdef_ast != nullptr);

  // It's not a bad idea to store the pointer to the function definition AST
  // in the low-level InstructionSequence as well, in case it's needed by
  // optimization passes.
  ll_iseq->set_funcdef_ast(funcdef_ast);

  // Determine the total number of bytes of memory storage
  // that the function needs. This should include both variables that
  // *must* have storage allocated in memory (e.g., arrays), and also
  // any additional memory that is needed for virtual registers,
  // spilled machine registers, etc.
  m_total_memory_storage = ll_iseq->get_funcdef_ast()->get_symbol()->get_addr(); // FIXME: determine how much memory storage on the stack is needed
  mem_addr = -(m_total_memory_storage + (8 - (m_total_memory_storage % 8)));

  // The function prologue will push %rbp, which should guarantee that the
  // stack pointer (%rsp) will contain an address that is a multiple of 16.
  // If the total memory storage required is not a multiple of 16, add to
  // it so that it is.
  highest = ll_iseq->get_funcdef_ast()->get_symbol()->get_vreg();
  m_total_memory_storage += (highest - 9) * 8;
  if((m_total_memory_storage) % 16 != 0)
    m_total_memory_storage += (16 - (m_total_memory_storage % 16));

  // Iterate through high level instructions
  for(auto i = hl_iseq->cbegin(); i != hl_iseq->cend(); ++i){
    Instruction* hl_ins = *i;

    // If the high-level instruction has a label, define an equivalent
    // label in the low-level instruction sequence
    if(i.has_label())
      ll_iseq->define_label(i.get_label());

    // Translate the high-level instruction into one or more low-level instructions
    translate_instruction(hl_ins, ll_iseq);
  }

  return ll_iseq;
}

namespace{

  // These helper functions are provided to make it easier to handle
  // the way that instructions and operands vary based on operand size
  // ('b'=1 byte, 'w'=2 bytes, 'l'=4 bytes, 'q'=8 bytes.)

  // Check whether hl_opcode matches a range of opcodes, where base
  // is a _b variant opcode. Return true if the hl opcode is any variant
  // of that base.
  bool match_hl(int base, int hl_opcode){
    if(base == HINS_sconv_bw){
      return hl_opcode >= base && hl_opcode < (base + 6);
    }
    return hl_opcode >= base && hl_opcode < (base + 4);
  }

  // For a low-level instruction with 4 size variants, return the correct
  // variant. base_opcode should be the "b" variant, and operand_size
  // should be the operand size in bytes (1, 2, 4, or 8.)
  LowLevelOpcode select_ll_opcode(LowLevelOpcode base_opcode, int operand_size){
    int off;

    switch(operand_size){
      case 1: // 'b' variant
        off = 0; break;
      case 2: // 'w' variant
        off = 1; break;
      case 4: // 'l' variant
        off = 2; break;
      case 8: // 'q' variant
        off = 3; break;
      default:
        assert(false);
        off = 3;
    }

    return LowLevelOpcode(int(base_opcode) + off);
  }
  LowLevelOpcode select_ll_opcode(LowLevelOpcode base_opcode, int first_size, int sec_size){
    int off = 0;
    if(first_size == 1){
      if(sec_size == 2){
        off = 0;
      } else if(sec_size == 4){
        off = 1;
      } else if(sec_size == 8){
        off = 2;
      } else{}
    } else if(first_size == 2){
      if(sec_size == 4){
        off = 3;
      } else if(sec_size == 8){
        off = 4;
      } else{}
    } else if(first_size == 4){
      if(sec_size == 8){
        off = 5;
      } else{}
    } else{}

    return LowLevelOpcode(int(base_opcode) + off);
  }

  // Get the correct Operand::Kind value for a machine register
  // of the specified size (1, 2, 4, or 8 bytes.)
  Operand::Kind select_mreg_kind(int operand_size){
    switch(operand_size){
      case 1:
        return Operand::MREG8;
      case 2:
        return Operand::MREG16;
      case 4:
        return Operand::MREG32;
      case 8:
        return Operand::MREG64;
      default:
        assert(false);
        return Operand::MREG64;
    }
  }

}

void LowLevelCodeGen::translate_instruction(Instruction* hl_ins, const std::shared_ptr<InstructionSequence>& ll_iseq){
  HighLevelOpcode hl_opcode = HighLevelOpcode(hl_ins->get_opcode());
  // printf("%s\n", highlevel_opcode_to_str(hl_opcode));
  // single operand
  if(hl_opcode == HINS_enter){
    ll_iseq->append(new Instruction(MINS_PUSHQ, Operand(Operand::MREG64, MREG_RBP)));
    ll_iseq->append(new Instruction(MINS_MOVQ, Operand(Operand::MREG64, MREG_RSP), Operand(Operand::MREG64, MREG_RBP)));
    ll_iseq->append(new Instruction(MINS_SUBQ, Operand(Operand::IMM_IVAL, m_total_memory_storage), Operand(Operand::MREG64, MREG_RSP)));

    return;
  }
  if(hl_opcode == HINS_leave){
    ll_iseq->append(new Instruction(MINS_ADDQ, Operand(Operand::IMM_IVAL, m_total_memory_storage), Operand(Operand::MREG64, MREG_RSP)));
    ll_iseq->append(new Instruction(MINS_POPQ, Operand(Operand::MREG64, MREG_RBP)));

    return;
  }
  if(hl_opcode == HINS_ret){
    ll_iseq->append(new Instruction(MINS_RET));
    return;
  }

  // label
  Operand label = hl_ins->get_operand(0);
  if(hl_opcode == HINS_call){
    ll_iseq->append(new Instruction(MINS_CALL, label));
    return;
  }
  if(hl_opcode == HINS_jmp){
    ll_iseq->append(new Instruction(MINS_JMP, label));
    return;
  }

  // double operand

  int size = highlevel_opcode_get_source_operand_size(hl_opcode);
  Operand first_operand = get_ll_operand(hl_ins->get_operand(0), size, ll_iseq);
  Operand zero(Operand::IMM_IVAL, 0);

  if(hl_opcode == HINS_cjmp_t || hl_opcode == HINS_cjmp_f){
    label = hl_ins->get_operand(1);
    ll_iseq->append(new Instruction(MINS_CMPL, zero, first_operand));
    ll_iseq->append(new Instruction(MINS_JNE - (hl_opcode - HINS_cjmp_t), label));
    return;
  }
  if(hl_opcode == HINS_localaddr){
    int sec_operand = hl_ins->get_operand(1).get_imm_ival();
    Operand r10(Operand::MREG64, MREG_R10); // r10 
    int addr = (sec_operand + 8) + mem_addr;
    // printf("vreg all %d\n", sec_operand);
    Operand rbp(Operand::MREG64_MEM_OFF, MREG_RBP, addr);

    ll_iseq->append(new Instruction(MINS_LEAQ, rbp, r10));
    ll_iseq->append(new Instruction(MINS_MOVQ, r10, first_operand));

    return;
  }
  LowLevelOpcode mov_opcode = select_ll_opcode(MINS_MOVB, size);
  Operand::Kind mreg_kind = select_mreg_kind(size);
  Operand sec_operand = get_ll_operand(hl_ins->get_operand(1), size, ll_iseq);
  Operand r10(mreg_kind, MREG_R10);
  Operand r11(mreg_kind, MREG_R11);
  if(match_hl(HINS_sconv_bw, hl_opcode)){
    LowLevelOpcode first_mov;
    LowLevelOpcode sec_mov;
    int first, sec;
    int dif = hl_opcode - HINS_sconv_bw;
    switch(dif){
      case 0:{
        first = 1;
        sec = 2;
        break;
      }
      case 1:{
        first = 1;
        sec = 4;
        break;
      }
      case 2:{
        first = 1;
        sec = 8;
        break;
      }
      case 3:{
        first = 2;
        sec = 4;
        break;
      }
      case 4:{
        first = 2;
        sec = 8;
        break;
      }
      case 5:{
        first = 4;
        sec = 8;
        break;
      }
      default:{
        assert(false);
      }
    }
    first_mov = select_ll_opcode(MINS_MOVB, first);
    sec_mov = select_ll_opcode(MINS_MOVB, sec);
    Operand first_r10(select_mreg_kind(first), MREG_R10);
    Operand sec_r10(select_mreg_kind(sec), MREG_R10);
    ll_iseq->append(new Instruction(first_mov, sec_operand, first_r10));
    ll_iseq->append(new Instruction(dif + MINS_MOVSBW, first_r10, sec_r10));
    ll_iseq->append(new Instruction(sec_mov, sec_r10, first_operand));
    return;
  }
  if(match_hl(HINS_mov_b, hl_opcode)){

    if(sec_operand.is_memref() && first_operand.is_memref()){
      ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
      sec_operand = r10;
    }
    ll_iseq->append(new Instruction(mov_opcode, sec_operand, first_operand));
    return;
  }
  if(match_hl(HINS_div_b, hl_opcode) || match_hl(HINS_mod_b, hl_opcode)){
    LowLevelOpcode opcode;
    if(hl_opcode == HINS_div_q || hl_opcode == HINS_mod_q){
      opcode = MINS_IDIVQ;
    } else{
      opcode = MINS_IDIVL;
    }
    Operand trd_operand = get_ll_operand(hl_ins->get_operand(2), size, ll_iseq);

    Operand rax(select_mreg_kind(size), MREG_RAX);
    Operand rdx(select_mreg_kind(size), MREG_RDX);
    if(match_hl(HINS_div_b, hl_opcode)){
      rdx = rax;
    }
    ll_iseq->append(new Instruction(mov_opcode, sec_operand, rax));
    ll_iseq->append(new Instruction(MINS_CDQ));
    ll_iseq->append(new Instruction(mov_opcode, trd_operand, r10));
    ll_iseq->append(new Instruction(opcode, r10));

    ll_iseq->append(new Instruction(mov_opcode, rdx, first_operand));

    return;
  }
  if(match_hl(HINS_mul_b, hl_opcode)){
    LowLevelOpcode opcode;
    if(hl_opcode == HINS_mul_q){
      opcode = MINS_IMULQ;
    } else{
      opcode = MINS_IMULL;
    }

    Operand trd_operand = get_ll_operand(hl_ins->get_operand(2), size, ll_iseq);

    if(sec_operand.is_memref() && trd_operand.is_memref()){
      ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
      sec_operand = r10;
    }

    ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
    ll_iseq->append(new Instruction(opcode, trd_operand, r10));
    ll_iseq->append(new Instruction(mov_opcode, r10, first_operand));
    return;
  }
  if(match_hl(HINS_neg_b, hl_opcode)){
    if(sec_operand.is_memref() && first_operand.is_memref()){
      ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
      sec_operand = r10;
    }
    LowLevelOpcode sub = select_ll_opcode(MINS_SUBB, size);
    // ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
    ll_iseq->append(new Instruction(mov_opcode, zero, first_operand));
    ll_iseq->append(new Instruction(sub, sec_operand, first_operand));
    return;
  }
  if(match_hl(HINS_add_b, hl_opcode) || match_hl(HINS_sub_b, hl_opcode)){
    LowLevelOpcode opcode;
    if(match_hl(HINS_add_b, hl_opcode)){
      opcode = MINS_ADDB;
    } else if(match_hl(HINS_sub_b, hl_opcode)){
      opcode = MINS_SUBB;
    }
    opcode = select_ll_opcode(opcode, size);

    Operand trd_operand = get_ll_operand(hl_ins->get_operand(2), size, ll_iseq);

    // if(sec_operand.is_memref() && trd_operand.is_memref()){
    ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
    sec_operand = r10;
    // }
    // if(sec_operand.is_imm_ival() && trd_operand.is_imm_ival()){
    //   ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
    //   sec_operand = r10;
    // }
    ll_iseq->append(new Instruction(opcode, trd_operand, sec_operand));
    if(sec_operand.is_memref() && first_operand.is_memref()){
      ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
      sec_operand = r10;
    }
    ll_iseq->append(new Instruction(mov_opcode, sec_operand, first_operand));
    return;
  }
  if(match_hl(HINS_cmplte_b, hl_opcode) || match_hl(HINS_cmplt_b, hl_opcode) || match_hl(HINS_cmpeq_b, hl_opcode) || match_hl(HINS_cmpgt_b, hl_opcode) || match_hl(HINS_cmpgte_b, hl_opcode)){
    LowLevelOpcode cmp_opcode = select_ll_opcode(MINS_CMPB, size);
    LowLevelOpcode movzb_opcode = select_ll_opcode(MINS_MOVZBW, 1, size);

    Operand trd_operand = get_ll_operand(hl_ins->get_operand(2), size, ll_iseq);

    if(sec_operand.is_memref() && trd_operand.is_memref()){
      ll_iseq->append(new Instruction(mov_opcode, sec_operand, r10));
      sec_operand = r10;
    }
    ll_iseq->append(new Instruction(cmp_opcode, trd_operand, sec_operand));
    //zero byte
    Operand r10b(Operand::MREG8, MREG_R10);
    if(match_hl(HINS_cmplte_b, hl_opcode)){
      ll_iseq->append(new Instruction(MINS_SETLE, r10b));
    } else if(match_hl(HINS_cmplt_b, hl_opcode)){
      ll_iseq->append(new Instruction(MINS_SETL, r10b));
    } else if(match_hl(HINS_cmpeq_b, hl_opcode)){
      ll_iseq->append(new Instruction(MINS_SETE, r10b));
    } else if(match_hl(HINS_cmpgt_b, hl_opcode)){
      ll_iseq->append(new Instruction(MINS_SETG, r10b));
    } else if(match_hl(HINS_cmpgte_b, hl_opcode)){
      ll_iseq->append(new Instruction(MINS_SETGE, r10b));
    }
    ll_iseq->append(new Instruction(movzb_opcode, r10b, r11));
    ll_iseq->append(new Instruction(mov_opcode, r11, first_operand));

    return;
  }

  printf("%s not handled\n", highlevel_opcode_to_str(hl_opcode));
  return;

  // RuntimeError::raise("high level opcode %d not handled" , int(hl_opcode));
}

// TODO: implement other private member functions
Operand LowLevelCodeGen::get_ll_operand(Operand hl_opcode, int size, const std::shared_ptr<InstructionSequence>& ll_iseq){
  if(hl_opcode.is_imm_ival() || hl_opcode.is_label() || hl_opcode.is_imm_label()){
    return hl_opcode;
  }

  if(hl_opcode.get_kind() == Operand::Kind::VREG){
    int base = hl_opcode.get_base_reg();
    if(base >= 10){
      // base -= 10;
      base = highest - base;
      Operand op(Operand::MREG64_MEM_OFF, MREG_RBP, mem_addr - base * 8);
      return op;
    } else if(base == 1){
      return Operand(select_mreg_kind(size), MREG_RDI);
    } else if(base == 2){
      return Operand(select_mreg_kind(size), MREG_RSI);
    } else{
      return Operand(select_mreg_kind(size), MREG_RAX);
    }
  }
  if(hl_opcode.get_kind() == Operand::VREG_MEM){
    int base = hl_opcode.get_base_reg();
    if(base >= 10){
      base = highest - base;
      // base -= 10;
    }
    Operand op(Operand::MREG64_MEM_OFF, MREG_RBP, mem_addr - base * 8);
    // printf("memadd = %d\n", mem_addr);
    Operand r11(Operand::MREG64, MREG_R11);
    ll_iseq->append(new Instruction(MINS_MOVQ, op, r11));
    return r11.to_memref();
  }
  // printf("hlopcode, %d", hl_opcode.get_kind());
  return Operand(Operand::MREG64_MEM_OFF, MREG_RBP, (10000 * 8) - m_total_memory_storage);
}

/*

alias back='cd ~/compilers/assign04-yulun/compiler_code_gen/'; alias test4='cd ~/compilers/fall2022-tests/assign04/'; alias test5='cd ~/compilers/fall2022-tests/assign05/'; alias get='back; make clean; git pull origin main; make clean; make depend; make -j;'; export ASSIGN04_DIR=~/compilers/assign04-yulun/compiler_code_gen/;export ASSIGN05_DIR=~/compilers/assign04-yulun/compiler_code_gen/;
alias try='./run_all.rb -o';

*/