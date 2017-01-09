#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
//void find_sram(hwaddr_t addr);
//char regs[8][4] = { "eax","ecx","edx","ebx","esp","ebp","esi","edi"};
/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}
	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_p(char *args);
static int cmd_x(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
static int cmd_bt(char *args);
static int cmd_cache(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si","Step one instruction exactly.", cmd_si},
	{ "info","Print the value of registers or the information of watchpoint.",cmd_info},
	{ "p", "Print value of expression EXP.", cmd_p},
	{ "x", "Examine memory:x N EXPR.", cmd_x},
	{ "w", "Set a watchpoint for expression.", cmd_w},
	{ "d", "Delete a watchpoint." ,cmd_d},
	{ "bt", "Print backtrace of all stack frames,or innermost COUNT frame.", cmd_bt},
	{ "cache", "Print cache." ,cmd_cache}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

/*
typedef struct {
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t args[4];
} PartOfStackFrame;

static void create_stackframe_link()
{
	PartOfStackFrame *head = malloc(sizeof(PartOfStackFrame));
	PartOfStackFrame *tail = NULL;
	head->
}
*/
char *get_func_name(swaddr_t eip);

static void print_stackframe(swaddr_t ebp,swaddr_t eip,uint32_t num) {
		swaddr_t prev_ebp = swaddr_read(ebp,4,R_SS);
		swaddr_t ret_addr = swaddr_read(ebp+4,4,R_SS);
		printf("#%d\t%x\t%x\t%x\t%s\t",num,ebp,prev_ebp,ret_addr,get_func_name(eip));
		printf("0x%x\t0x%x\t0x%x\t0x%x\n",swaddr_read(ebp+8,4,R_SS),swaddr_read(ebp+12,4,R_SS),swaddr_read(ebp+16,4,R_SS),swaddr_read(ebp+20,4,R_SS));

		if(prev_ebp == 0)
			return ;
		else 
			return print_stackframe(prev_ebp,ret_addr,num+1);

}

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}
static int cmd_si(char *args)
{
	/*etract the first argument*/
	char *arg = strtok(NULL," ");
	if(arg == NULL) {
                /* no number given */
		cpu_exec(1);
        }
        else
		cpu_exec((unsigned)atoi(arg));
	return 0;
}
static int cmd_info(char *args)
{
	/*get the mode of info*/
	char *arg = strtok(NULL," ");
	int i;
	if(strcmp(arg,"r") == 0) {
	 /*judge the mode of info*/
		for(i = 0;i < 8; i ++)
			printf("%s\t0x%x\t%d\n",regsl[i],cpu.gpr[i]._32,cpu.gpr[i]._32);
	printf("eip = 0x%x\n",cpu.eip);
	printf("CF = %d\n",cpu.EFLAGS.CF);
	printf("OF = %d\n",cpu.EFLAGS.OF);
	printf("SF = %d\n",cpu.EFLAGS.SF);
	printf("ZF = %d\n",cpu.EFLAGS.ZF);
	printf("CR0 = %x\n",cpu.cr0.val);
	printf("CR3 = %x\n",cpu.cr3.val);

	for(i = 0;i < 4;i++)
		printf("%s\t0x%x\t%d\n",sregs[i],cpu.sreg[i].val,cpu.sreg[i].val);
		
	}
	else if(strcmp(arg,"w") == 0) {
		print_wp();
	}
	else
		printf("Unknown command '%s'\n", arg);
	
        return 0;
}

static int cmd_p(char *args)
{
	bool flag = true;
	unsigned value = expr(args,&flag);
	printf("%d\t0x%04x\n",value,value);
	return 0;
}

static int cmd_x(char *args)
{
	char *arg1 = strtok(NULL," ");
	int len = atoi(arg1);
	int i;
	char *arg2 = strtok(NULL," ");
	unsigned addr = expr(arg2,false);
	printf("%x %u\n",addr,addr);
	if( addr >= 0x100000 && len <= HW_MEM_SIZE ) {
		printf("0x%08x:",addr);	
		for(i = 0;i < len ; i ++) {
			printf("0x%02x ",swaddr_read(addr+i,1,R_DS));
		}
		printf("\n");	
	}
	else
		printf("illegal accessing !\n");
	return 0;
}
static int cmd_w(char *args)
{
	add_wp(args);
	return 0;
}
static int cmd_d(char *args)
{
	free_wp(atoi(args));
	return 0;
}
static int cmd_bt(char *args)
{
	printf("Num\tebp\tPrev\tRet\tFunc\targ1\targ2\targ3\targ4\n");
	print_stackframe(cpu.ebp,cpu.eip,0);
	return 0;
}

static int cmd_cache(char *args)
{
	/*char *arg1 = strtok(NULL," ");
	hwaddr_t addr = expr(arg1,false);
	find_sram(addr);*/
	return 0;
}


void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
