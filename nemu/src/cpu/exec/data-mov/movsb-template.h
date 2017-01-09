#include "cpu/exec/template-start.h"

#define instr movsb

static void do_execute() {
	int8_t val_t = op_src->val;
	DATA_TYPE_S val = val_t ;
	OPERAND_W(op_dest, val);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
