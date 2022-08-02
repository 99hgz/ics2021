#define def_compute_ehelper(name) \
  def_EHelper(name) { \
    rtl_##name(s, ddest, dsrc1, dsrc2); \
  }
#define def_compute_ehelperi(name) \
  def_EHelper(name##i) { \
    rtl_##name##i(s, ddest, dsrc1, id_src2->imm); \
  }

def_compute_ehelper(add)
def_compute_ehelper(sub)
def_compute_ehelper(and)
def_compute_ehelper(or)
def_compute_ehelper(xor)
def_compute_ehelper(sll)
def_compute_ehelper(srl)
def_compute_ehelper(sra)

def_compute_ehelperi(add)
def_compute_ehelperi(and)
def_compute_ehelperi(or)
def_compute_ehelperi(xor)
def_compute_ehelperi(sll)
def_compute_ehelperi(srl)
def_compute_ehelperi(sra)

def_EHelper(mul) {
  rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mulh) {
  rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mulsu) {
  rtl_mulsu_hi(s, ddest, dsrc1, dsrc2);
}
def_EHelper(mulu) {
  rtl_mulu_hi(s, ddest, dsrc1, dsrc2);
}
def_EHelper(div) {
  rtl_divs_q(s, ddest, dsrc1, dsrc2);
}
def_EHelper(divu) {
  rtl_divu_q(s, ddest, dsrc1, dsrc2);
}
def_EHelper(rem) {
  rtl_divs_r(s, ddest, dsrc1, dsrc2);
}
def_EHelper(remu) {
  rtl_divu_r(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slt) {
  rtl_li(s, ddest, interpret_relop(RELOP_LT,*dsrc1,*dsrc2));
}
def_EHelper(sltu) {
  rtl_li(s, ddest, interpret_relop(RELOP_LTU,*dsrc1,*dsrc2));
}
def_EHelper(slti) {
  rtl_li(s, ddest, interpret_relop(RELOP_LT,*dsrc1,id_src2->imm));
}
def_EHelper(sltiu) {
  rtl_li(s, ddest, interpret_relop(RELOP_LTU,*dsrc1,id_src2->imm));
}

def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(li) {
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(auipc) {
  rtl_addi(s, ddest, &gpc, id_src1->imm);
}
def_EHelper(jal) {
  rtl_addi(s, ddest, &gpc, 4);
  rtl_addi(s, s0, &gpc, id_src1->imm);
  rtl_jr(s, s0);
}
def_EHelper(jalr) {
  rtl_addi(s, ddest, &gpc, 4);
  rtl_addi(s, s0, dsrc1, id_src2->imm);
  rtl_jr(s, s0);
}
def_EHelper(bne) {
  rtl_addi(s, s0, &gpc, id_dest->imm);
  rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, *s0);
}
def_EHelper(beq) {
  rtl_addi(s, s0, &gpc, id_dest->imm);
  rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, *s0);
}
def_EHelper(blt) {
  rtl_addi(s, s0, &gpc, id_dest->imm);
  rtl_jrelop(s, RELOP_LT, dsrc1, dsrc2, *s0);
}
def_EHelper(bge) {
  rtl_addi(s, s0, &gpc, id_dest->imm);
  rtl_jrelop(s, RELOP_GE, dsrc1, dsrc2, *s0);
}
def_EHelper(bltu) {
  rtl_addi(s, s0, &gpc, id_dest->imm);
  rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, *s0);
}
def_EHelper(bgeu) {
  rtl_addi(s, s0, &gpc, id_dest->imm);
  rtl_jrelop(s, RELOP_GEU, dsrc1, dsrc2, *s0);
}

def_EHelper(csrrw) {
  rtl_addi(s, s0, dsrc2, 0);
  rtl_addi(s, dsrc2, dsrc1, 0);
  rtl_addi(s, ddest, s0, 0);
}
def_EHelper(csrrs) {
  rtl_addi(s, s0, dsrc2, 0);
  rtl_or(s, dsrc2, dsrc1, s0);
  rtl_addi(s, ddest, s0, 0);
}
def_EHelper(csrrc) {
  rtl_addi(s, s0, dsrc2, 0);
  rtl_li(s, dsrc2, (*dsrc2) & (~(*dsrc1)));
  rtl_addi(s, ddest, s0, 0);
}

def_EHelper(csrrwi) {
  rtl_addi(s, s0, dsrc2, 0);
  rtl_li(s, dsrc2, id_src1->imm);
  rtl_addi(s, ddest, s0, 0);
}
def_EHelper(csrrsi) {
  rtl_addi(s, s0, dsrc2, 0);
  rtl_ori(s, dsrc2, s0, id_src1->imm);
  rtl_addi(s, ddest, s0, 0);
}
def_EHelper(csrrci) {
  rtl_addi(s, s0, dsrc2, 0);
  rtl_li(s, dsrc2, (*dsrc2) & (~(id_src1->imm)));
  rtl_addi(s, ddest, s0, 0);
}

def_EHelper(ecall) {
  isa_raise_intr(gpr(17),gpc);
  rtl_j(s, cpu.csr[0x305]._32);
}
def_EHelper(mret) {
  rtl_j(s, cpu.csr[0x341]._32 + 4);
}