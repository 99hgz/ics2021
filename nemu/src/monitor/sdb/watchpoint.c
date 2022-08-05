#include "sdb.h"
#include <isa.h>
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
    printf("breakpoint%d %s value=%u already hit %d time(s)\n",tmp->NO,tmp->cmd,tmp->val,tmp->hit_times);
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
extern CPU_state cpu;
void itrace_display();
int do_wp_check(){
  WP *tmp=head;
  int ret=0;
  while(tmp!=NULL){
    bool suc = true;
    uint32_t val_ = expr(tmp->cmd,&suc);
    if(!suc)assert(0);
    if(val_!=tmp->val){
      ret = -1;
      tmp->hit_times++;
      printf("pc=0x%8x breakpoint %d %s old value=%u current value=%u\n",cpu.pc,tmp->NO,tmp->cmd,tmp->val,val_);
      itrace_display();
    }
    tmp->val=val_;
    tmp = tmp -> next;
  }
  return ret;
}
char itrace_records[20][128];
int itrace_cur = 0;
void itrace_record(char *p){
  memcpy(itrace_records[itrace_cur],p,strlen(p)+1);
  itrace_cur++;
  if(itrace_cur>=20)itrace_cur=0;
}
void itrace_display(){
  int tmp = itrace_cur;
  do{
    if(strlen(itrace_records[tmp])!=0)
      printf("%s\n",itrace_records[tmp]);
    tmp++;
    if(tmp>=20)tmp=0;
  }while(itrace_cur!=tmp);
}