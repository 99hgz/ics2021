#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
  word_t tmp = paddr_read(addr, len);
  //printf("read memory address = 0x%08x data = %d = %u = %08x\n",addr,tmp,tmp,tmp);
  return tmp;
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  paddr_write(addr, len, data);
  //printf("write memory address = 0x%08x data = %d = %u = %08x len=%d\n",addr,data,data,data,len);
}
