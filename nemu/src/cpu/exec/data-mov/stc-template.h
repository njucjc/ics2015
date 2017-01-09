#include "cpu/exec/template-start.h"

#define instr stc

make_helper(concat(stc_,SUFFIX)) {
	cpu.EFLAGS.CF = 1;
	print_asm("stc");
	return 1;
}

#include "cpu/exec/template-end.h"
