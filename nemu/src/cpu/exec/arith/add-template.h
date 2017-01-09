#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	DATA_TYPE value = op_dest->val + op_src->val;
	OPERAND_W(op_dest,value);

	SET_ZF(value);
	SET_SF(value);
	SET_ADD_OF(op_dest->val,op_src->val,value);
	SET_ADD_CF(op_dest->val,value);
	SET_PF(value);
	print_asm_template2();
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
