def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(li) {
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(auipc) {
  rtl_addi(s, ddest, &gpc, id_src1->imm);
}
