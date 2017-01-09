#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	//printf("%d %d\n",op_src->val ,op_dest->val);
	DATA_TYPE value = (op_src->val) & (op_dest->val);
	//OPERAND_W(op_dest,value);
	cpu.EFLAGS.OF = 0;
	cpu.EFLAGS.CF = 0;
	SET_ZF(value);
	//printf("%d\n",cpu.EFLAGS.ZF);
	SET_PF(value);
	SET_SF(value);

	print_asm_template2();	
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"
