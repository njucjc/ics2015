#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	int len;
	if(op_src->type == OP_TYPE_IMM) {
		len = concat(decode_si_, SUFFIX)(cpu.eip + 1);
		cpu.eip += op_src->val;
		snprintf(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + len + 1);
	} 
	else {
		len = concat(decode_rm_, SUFFIX)(cpu.eip + 1);
		cpu.eip = op_src->val - len - 1;
	}
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)


make_helper(concat(jmp_rel_,SUFFIX)){
	DATA_TYPE_S disp = instr_fetch(eip + 1, DATA_BYTE);
	cpu.eip = eip + disp;
	print_asm(str(instr) " %x", cpu.eip + DATA_BYTE + 1);
	return DATA_BYTE + 1;
}
make_helper(concat(ljmp_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	uint16_t sreg = instr_fetch(eip + 5, 2);
	cpu.sreg[R_CS].val = sreg;
	cpu.eip = addr;
	print_asm("ljmp" str(SUFFIX) " 0x%x", addr);
	return 0;
}




#include "cpu/exec/template-end.h"
