#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.csr[0x341]._32=epc;
  cpu.csr[0x342]._32=NO;
  //rtl_j(cpu.csr[0x305]._32);
  return 0;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
