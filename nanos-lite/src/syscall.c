#include <common.h>
#include "syscall.h"

int _write(int fd , const char * buf , size_t count ){
  int cnt=0;
  if(fd==1 || fd==2){
    while((++cnt)<=count){
      putch(*buf);
      buf++;
    }
    return count;
  }else{
    return -1;
  }
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  printf("do_syscall %d args=%d %d %d\n",a[0],a[1],a[2],a[3]);
  switch (a[0]) {
    case SYS_yield:yield();c->GPRx=0;break;
    case SYS_brk:c->GPRx=0;break;
    case SYS_exit:halt(0);break;
    case SYS_write:c->GPRx=_write(a[1],(char *)a[2],a[3]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
