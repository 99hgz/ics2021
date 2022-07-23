#include "../local-include/reg.h"
#include <cpu/ifetch.h>
#include <isa-all-instr.h>

def_all_THelper();

static uint32_t get_instr(Decode *s) {
  return s->isa.instr.val;
}

// decode operand helper
#define def_DopHelper(name) \
  void concat(decode_op_, name) (Decode *s, Operand *op, word_t val, bool flag)

static def_DopHelper(i) {
  op->imm = val;
}
//static word_t zero_null = 0;
static def_DopHelper(r) {
  bool is_write = flag;
  //zero_null = 0;
  static word_t zero_null = 0;
  op->preg = (is_write && val == 0) ? &zero_null : &gpr(val);
}

static def_DHelper(I) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  decode_op_i(s, id_src2, SEXT32(s->isa.instr.i.simm11_0,12), false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}
static def_DHelper(UI) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  decode_op_i(s, id_src2, ZEXT32(s->isa.instr.i.simm11_0,12), false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}
static def_DHelper(DI) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  uint32_t imm = ZEXT32(s->isa.instr.i.simm11_0,12) & 0xff;
  decode_op_i(s, id_src2, imm, false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}

static def_DHelper(U) {
  decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
  decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}

static def_DHelper(S) {
  decode_op_r(s, id_src1, s->isa.instr.s.rs1, false);
  sword_t simm = SEXT32((s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0,12);
  decode_op_i(s, id_src2, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.s.rs2, true);
}

static def_DHelper(J) {
  sword_t simm = SEXT32((s->isa.instr.j.imm10_1 << 1) | (s->isa.instr.j.imm11 << 11) | (s->isa.instr.j.imm19_12 << 12) | (s->isa.instr.j.imm20 << 20),21);
  decode_op_i(s, id_src1, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.j.rd, true);
}

static def_DHelper(R) {
  decode_op_r(s, id_src1, s->isa.instr.r.rs1, false);
  decode_op_r(s, id_src2, s->isa.instr.r.rs2, false);
  decode_op_r(s, id_dest, s->isa.instr.r.rd, true);
}


static def_DHelper(B) {
  decode_op_r(s, id_src1, s->isa.instr.b.rs1, false);
  decode_op_r(s, id_src2, s->isa.instr.b.rs2, false);
  sword_t simm = SEXT32((s->isa.instr.b.imm4_1 << 1) | (s->isa.instr.b.imm11 << 11) | (s->isa.instr.b.imm10_5 << 5) | (s->isa.instr.b.imm12 << 12),12);
  //printf("B-type imm=%d\n",simm);
  decode_op_i(s, id_dest, simm, false);
}


static def_DHelper(CI) {
  sword_t simm = SEXT32((s->isa.instr.ci.imm5_5 << 5) | s->isa.instr.ci.imm4_0,5);
  decode_op_i(s, id_src1, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.ci.rd, false);
}


def_THelper(load) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", lb);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", lh);
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", lw);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", lbu);
  def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", lhu);
  return EXEC_ID_inv;
}

def_THelper(store) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", sb);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", sh);
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", sw);
  return EXEC_ID_inv;
}

def_THelper(comp) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ?0??? ??", addi);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ?0??? ??", xori);
  def_INSTR_TAB("??????? ????? ????? 110 ????? ?0??? ??", ori);
  def_INSTR_TAB("??????? ????? ????? 111 ????? ?0??? ??", andi);
  def_INSTR_TAB("0000000 ????? ????? 001 ????? ?0??? ??", slli);
  def_INSTR_TAB("0000000 ????? ????? 101 ????? ?0??? ??", srli);
  def_INSTR_TAB("0100000 ????? ????? 101 ????? ?0??? ??", srai);
  def_INSTR_TAB("??????? ????? ????? 010 ????? ?0??? ??", slti);
  def_INSTR_TAB("??????? ????? ????? 011 ????? ?0??? ??", sltiu);
  return EXEC_ID_inv;
}

def_THelper(comp1) {
  def_INSTR_TAB("0000000 ????? ????? 000 ????? ?1??? ??", add);
  def_INSTR_TAB("0100000 ????? ????? 000 ????? ?1??? ??", sub);
  def_INSTR_TAB("0000000 ????? ????? 100 ????? ?1??? ??", xor);
  def_INSTR_TAB("0000000 ????? ????? 110 ????? ?1??? ??", or);
  def_INSTR_TAB("0000000 ????? ????? 111 ????? ?1??? ??", and);
  def_INSTR_TAB("0000000 ????? ????? 001 ????? ?1??? ??", sll);
  def_INSTR_TAB("0000000 ????? ????? 101 ????? ?1??? ??", srl);
  def_INSTR_TAB("0100000 ????? ????? 101 ????? ?1??? ??", sra);
  def_INSTR_TAB("0000000 ????? ????? 010 ????? ?1??? ??", slt);
  def_INSTR_TAB("0000000 ????? ????? 011 ????? ?1??? ??", sltu);

  def_INSTR_TAB("0000001 ????? ????? 000 ????? ????? ??", mul);
  def_INSTR_TAB("0000001 ????? ????? 001 ????? ????? ??", mulh);
  def_INSTR_TAB("0000001 ????? ????? 010 ????? ????? ??", mulsu);
  def_INSTR_TAB("0000001 ????? ????? 011 ????? ????? ??", mulu);
  def_INSTR_TAB("0000001 ????? ????? 100 ????? ????? ??", div);
  def_INSTR_TAB("0000001 ????? ????? 101 ????? ????? ??", divu);
  def_INSTR_TAB("0000001 ????? ????? 110 ????? ????? ??", rem);
  def_INSTR_TAB("0000001 ????? ????? 111 ????? ????? ??", remu);
  return EXEC_ID_inv;
}

def_THelper(branch) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", beq);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", bne);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", blt);
  def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", bge);
  def_INSTR_TAB("??????? ????? ????? 110 ????? ????? ??", bltu);
  def_INSTR_TAB("??????? ????? ????? 111 ????? ????? ??", bgeu);
  return EXEC_ID_inv;
}


def_THelper(main) {
  def_INSTR_IDTAB("??????? ????? ????? 011 ????? 00100 11", UI    , comp);
  def_INSTR_IDTAB("0100000 ????? ????? 101 ????? 00100 11", DI    , comp);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00100 11", I     , comp);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01100 11", R     , comp1);

  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11000 11", B     , branch);

  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00000 11", I     , load);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01000 11", S     , store);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01101 11", U     , lui);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00101 11", U     , auipc);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11011 11", J     , jal);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11001 11", I     , jalr);
  def_INSTR_IDTAB("0000000000000000 010 ??????????? 01"   , CI    , li);
  
  def_INSTR_TAB  ("??????? ????? ????? ??? ????? 11010 11",         nemu_trap);
  return table_inv(s);
};

int isa_fetch_decode(Decode *s) {
  s->isa.instr.val = instr_fetch(&s->snpc, 4);
  int idx = table_main(s);
  return idx;
}