#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_,*tail,*free_tail;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;
	free_tail = &wp_list[NR_WP - 1];
	head = NULL;
	tail = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

WP * new_wp(char *e)
{
	//printf("I am in new_wp\n");
	//printf("%s\n",e);
	WP *cur = free_;
	bool flag = true;
	if(cur == NULL) {
		printf("watchpoint pool is empty.\n");
		return NULL;
	}
	else {
		cur->expr.value = expr(e,&flag);
		//printf("%d\n",cur->expr.value);
		strcpy(cur->expr.e,e);
		//printf("%s\n",cur->expr.e);
		free_ = free_->next;
		return cur;
	}
}

void free_wp(int num)
{
	//printf("%d\n",num);
	if( head == NULL) {
		printf("No watchpoints.\n");
		return ;
	}
	else if( head->NO == num) {
		if(free_ == NULL) {
			free_ = head;
			head = head ->next;
			free_tail = free_;
			free_tail->next = NULL;
		}
		else {
			free_tail->next = head;
			free_tail = head;
			head = head -> next;
			free_tail -> next = NULL;
		}
		return ;
	}
	WP *prev = head;
	WP *cur = head -> next;
	while(cur) {
		if(cur->NO != num) {
			prev = cur;
			cur = cur -> next;
		}
		else 
			break;	
	}
	//printf("%d\n",prev->next->NO);
	//printf("%d\n",cur->NO);
	if(cur == NULL) 
		printf("Not a breakpoint numbered %d\n",num);
	else {
		if(free_ == NULL) {
			prev->next = cur ->next;
			free_ = cur;
			free_tail=free_;
			free_tail->next = NULL;
		}
		else {
		//printf("%d\n",free_tail ->next->NO);
        	prev->next = cur ->next;
		free_tail->next = cur;
		free_tail = cur;
		free_tail->next = NULL;
		}
		if(cur == tail)
			tail = prev;
	}
}

void add_wp(char *e)
{
	//printf("%s\n",e);
	WP *p = new_wp(e);
	if(p != NULL)
		p -> next = NULL;
	else 
		return ;
	if(head == NULL) {
		head = p;
		tail = head;
	}
	else {
		tail ->next = p;
		tail = p;
	}
}

void print_wp()
{
	if(head)
		printf("Num\tType\twhat\n");
	else
		printf("Not any watchpoint.\n");
	WP *h;
	for(h = head;h != NULL;h = h ->next)
		printf("%d\twatchpoint\t%s\n",h->NO,h->expr.e);
}

bool check_wp()
{
	bool wp_flag = true;
	bool expr_flag = true;
	WP *h;
	for(h = head; h != NULL;h = h -> next) {
		uint32_t val = expr(h->expr.e,&expr_flag);
		if(h->expr.value != val) {
		   printf("watchpoint %d:%s\n\nOld value = 0x%x\nNew value = 0x%x\n",h->NO,h->expr.e,h->expr.value,val);
		   wp_flag = false;	
		}
	}
	return wp_flag;
}
