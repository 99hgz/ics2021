#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  uint32_t val;
  char cmd[255];
  int hit_times;
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
  if(free_==NULL)assert(0);
  WP *tmp = free_;
  free_=free_->next;
  tmp->next = head;
  head = tmp;
  return head;
}
void free_wp(WP *wp){
  WP *tmp=head;
  while(tmp!=NULL&&tmp->next!=wp)
    tmp=tmp->next;
  if(head==wp)
    head=wp->next;
  else
    tmp->next=wp->next;
  wp->next=free_;
  free_=wp;
}