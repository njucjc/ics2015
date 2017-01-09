#include "cpu/exec/template-start.h"

#define instr movzb

static void do_execute() {
	uint8_t val_t = op_src->val;
	DATA_TYPE val = val_t ;
	OPERAND_W(op_dest, val);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
