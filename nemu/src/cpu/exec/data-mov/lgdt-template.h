#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute() {

	cpu.GDTR.limit = swaddr_read(op_src->addr,2,R_CS);
	cpu.GDTR.base = swaddr_read(op_src->addr+2,4,R_CS);

	print_asm_template1();

}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
