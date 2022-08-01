#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.csr[0x341]._32=epc;
  cpu.csr[0x342]._32=NO;
  cpu.csr[0x300]._32=0x1800;
  printf("pc=%u cause=%u status=%u",cpu.csr[0x341]._32,cpu.csr[0x342]._32,cpu.csr[0x300]._32);
  for(int i=0;i<32;i++)
    printf("%u ",cpu.gpr[i]._32);
  printf("\n");
  //rtl_j(cpu.csr[0x305]._32);
  return 0;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
