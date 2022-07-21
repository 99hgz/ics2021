#define def_compute_ehelper(name) \
  def_EHelper(name) { \
    rtl_##name(s, ddest, dsrc1, dsrc2); \
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