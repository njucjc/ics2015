#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute() {
		DATA_TYPE value1 = op_dest->val + op_src->val;
		DATA_TYPE value = value1+cpu.EFLAGS.CF;
		OPERAND_W(op_dest,value);

		SET_ZF(value);
		SET_SF(value);
		SET_ADD_OF(op_dest->val,op_src->val,value1);
		SET_ADC_OF(value1,value);
		SET_ADD_CF(op_dest->val,value);
		SET_PF(value);
		print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)

#if DATA_BYTE == 2 || DATA_BYTE == 4

make_instr_helper(si2rm)

#endif

#include "cpu/exec/template-end.h"
