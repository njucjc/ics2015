#include "cpu/exec/template-start.h"

#define instr movsw

static void do_execute() {
		int16_t val_t = op_src->val;
		int32_t val = val_t;
		OPERAND_W(op_dest, val);
		print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
