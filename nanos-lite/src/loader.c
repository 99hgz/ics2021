#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_header;
  Elf_Phdr prog_header;
  ramdisk_read(&elf_header, 0, sizeof(Elf_Ehdr));
  if (memcmp(elf_header.e_ident, "\177ELF", 4))
    assert(0);
  Elf32_Off prog_header_offset = elf_header.e_phoff;
  Elf32_Half prog_header_size = elf_header.e_phentsize;
  //printf("%d\n",elf_header.e_phnum);
  uint32_t prog_idx = 0;
  while (prog_idx < elf_header.e_phnum){
    memset(&prog_header, 0, prog_header_size);
    ramdisk_read(&prog_header, prog_header_offset, prog_header_size);
    if (PT_LOAD == prog_header.p_type){
      //printf("loadto %u\n",prog_header.p_vaddr);
      ramdisk_read((void *)prog_header.p_vaddr, prog_header.p_offset, prog_header.p_filesz);
      memset((void *)(prog_header.p_vaddr+prog_header.p_filesz), 0, prog_header.p_memsz-prog_header.p_filesz);
    }
    prog_header_offset += elf_header.e_phentsize;
    prog_idx++;
  }
  return elf_header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

