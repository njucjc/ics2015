#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct expression {

	uint32_t value;		/* original value of expression */
	char e[100];			/* expression */
} EXPR;

typedef struct watchpoint {
	int NO;
	struct expression expr;
	struct watchpoint *next;
	
	/* TODO: Add more members if necessary */


} WP;
void add_wp(char *e);
void free_wp(int num);
void print_wp();
bool check_wp();
#endif
