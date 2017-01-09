#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	
	int len = concat(decode_rm_, SUFFIX)(cpu.eip + 1);
	REG(R_ESP) -= DATA_BYTE;
	MEM_W(REG(R_ESP),cpu.eip+len+1,R_SS);
	cpu.eip = op_src->val-len-1;
	print_asm_template1();
}

make_helper(concat(call_rel_,SUFFIX)) {
	swaddr_t disp = instr_fetch(eip+1,DATA_BYTE);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp,eip+DATA_BYTE+1,R_SS);
	print_asm("call" " %x",eip+disp+DATA_BYTE+1);
	cpu.eip += disp;
	
	return DATA_BYTE+1;
}


make_instr_helper(rm)	

#include "cpu/exec/template-end.h"
