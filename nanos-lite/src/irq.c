#include <common.h>
void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {
  /*printf("pc=%u cause=%u status=%u",c->mepc,c->mcause,c->mcause);
  for(int i=0;i<32;i++)
    printf("r[%d]=%u ",i,c->gpr[i]);
  printf("\n");*/
  switch (e.event) {
    case EVENT_YIELD: printf("Event yield\n");break;
    case EVENT_SYSCALL: do_syscall(c);break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
