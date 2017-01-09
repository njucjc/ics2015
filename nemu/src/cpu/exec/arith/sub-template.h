#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	
	DATA_TYPE value = op_dest->val - op_src->val;
	OPERAND_W(op_dest,value);

	SET_ZF(value);
	SET_SF(value);
	SET_SUB_CF(op_dest->val,op_src->val);
	SET_SUB_OF(op_dest->val,op_src->val,value);
	SET_PF(value);
	print_asm_template2();	
}

make_instr_helper(r2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2rm)
make_instr_helper(rm2r)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"
