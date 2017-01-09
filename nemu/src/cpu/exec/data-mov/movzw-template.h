#include "cpu/exec/template-start.h"

#define instr movzw

static void do_execute() {
	uint16_t val_t = op_src->val;
	uint32_t val = val_t;
	OPERAND_W(op_dest, val);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
