#include "cpu/exec/template-start.h"
#define instr sbb

static void do_execute() {
	//printf("I am in.\n");
	op_src->val += cpu.EFLAGS.CF;

	DATA_TYPE value = op_dest->val - op_src->val;
	OPERAND_W(op_dest,value);
	
	SET_ZF(value);
	SET_SF(value);
	SET_SUB_CF(op_dest->val,op_src->val);
	SET_SUB_OF(op_dest->val,op_src->val,value);
	SET_PF(value);
	print_asm_template2();	
}

make_instr_helper(rm2r);
make_instr_helper(i2rm);
make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"
