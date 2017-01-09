#include "cpu/exec/template-start.h"

#define instr sti

make_helper(concat(sti_,SUFFIX)) {
	cpu.EFLAGS.IF = 1;
	print_asm("sti");
	return 1;
}

#include "cpu/exec/template-end.h"
