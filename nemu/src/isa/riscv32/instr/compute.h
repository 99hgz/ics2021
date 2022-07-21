#define def_compute_ehelper(name) \
  def_EHelper(name) { \
    rtl_##name(s, ddest, dsrc1, dsrc2); \
  }

def_compute_ehelper(add)
def_compute_ehelper(sub)
def_compute_ehelper(and)
def_compute_ehelper(or)
def_compute_ehelper(xor)
def_compute_ehelper(sll)
def_compute_ehelper(srl)
def_compute_ehelper(sra)

def_EHelper(slt) {
  rtl_li(s, ddest, interpret_relop(RELOP_LTU,*dsrc1,*dsrc2));
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
def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
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
