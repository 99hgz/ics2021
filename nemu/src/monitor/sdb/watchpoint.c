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

WP* new_(){
  if(free_==NULL)assert(0);
  WP *tmp = free_;
  free_=free_->next;
  tmp->next = head;
  head = tmp;
  return head;
}
void new_wp(char *args){
  WP *tmp = new_();
  strcpy(tmp->cmd,args);
  bool suc = false;
  tmp->val = expr(tmp->cmd,&suc);
  if(!suc)assert(0);
  tmp->hit_times = 0;
}
void show_wps(){
  WP *tmp = head;
  while(tmp != NULL){
    printf("breakpoint%d %s value=%d already hit %d time(s)",tmp->NO,tmp->cmd,tmp->val,tmp->hit_times);
    tmp = tmp->next;
  }
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
void delete_wp(int no){
  WP *tmp = head;
  while(tmp != NULL){
    if(tmp->NO==no){
      free_wp(tmp);
      Log("deleted breakpoint %d",no);
      break;
    }
    tmp = tmp -> next;
  }
}

int do_wp_check(){
  WP *tmp=head;
  int ret=0;
  while(tmp!=NULL){
    bool suc = true;
    uint32_t val_ = expr(tmp->cmd,&suc);
    if(!suc)assert(0);
    if(val_!=tmp->val){
      ret = -1;
      printf("breakpoint %d old value=%d current value=%d",tmp->NO,tmp->val,val_);
    }
    tmp->val=val_;
    tmp = tmp -> next;
  }
  return ret;
}