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
  rtl_li(s, s0, 4);
  rtl_add(s, ddest, &gpc, s0);
  rtl_li(s, s0, id_src1->imm);
  rtl_sextb(s, s0, s0, 20);
  rtl_add(s, s1, &gpc, s0);
  rtl_jr(s, s1);
}