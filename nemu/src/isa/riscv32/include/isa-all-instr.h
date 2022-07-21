#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(add) f(sub) f(xor) f(or) f(and) f(sll) f(srl) f(sra) f(slt)\
 f(lui) f(lw) f(sw) f(inv) f(nemu_trap) f(li) f(auipc) f(addi) f(jal) f(jalr) f(bne) f(beq) f(blt) f(bge)

def_all_EXEC_ID();
