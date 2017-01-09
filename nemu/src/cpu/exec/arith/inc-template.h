#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);
	DATA_TYPE dest = 1;
	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	SET_ZF(result);
	SET_SF(result);
	SET_PF(result);
	SET_ADD_OF(dest,op_src->val,result);
	SET_ADD_CF(dest,result);
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
