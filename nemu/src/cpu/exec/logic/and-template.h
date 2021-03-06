#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);
	//printf("%x %x %x\n",op_dest->val,op_src->val,result);
	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	SET_ZF(result);
	SET_SF(result);
	SET_PF(result);
	cpu.EFLAGS.CF = 0;                                                      
	cpu.EFLAGS.OF = 0;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
