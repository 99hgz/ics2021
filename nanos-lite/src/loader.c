#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

int fs_open(const char *pathname, int flags, int mode);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
size_t fs_read(int fd, void *buf, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_header;
  Elf_Phdr prog_header;
  int fd = fs_open(filename,0,0);
  fs_read(fd, &elf_header, sizeof(Elf_Ehdr));
  if (memcmp(elf_header.e_ident, "\177ELF", 4))
    assert(0);
  Elf32_Off prog_header_offset = elf_header.e_phoff;
  Elf32_Half prog_header_size = elf_header.e_phentsize;
  //printf("%d\n",elf_header.e_phnum);
  uint32_t prog_idx = 0;
  while (prog_idx < elf_header.e_phnum){
    memset(&prog_header, 0, prog_header_size);
    fs_lseek(fd, prog_header_offset, SEEK_SET);
    fs_read(fd, &prog_header, prog_header_size);
    if (PT_LOAD == prog_header.p_type){
      //printf("loadto %u\n",prog_header.p_vaddr);
      fs_lseek(fd, prog_header.p_offset, SEEK_SET);
      fs_read(fd, (void *)prog_header.p_vaddr, prog_header.p_filesz);
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

