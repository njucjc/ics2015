#include "cpu/exec/template-start.h"

#define instr hlt

make_helper(concat(hlt_,SUFFIX)) {

	assert(cpu.EFLAGS.IF == 1);
	print_asm("hlt");
	while(!cpu.INTR);
	return 1;

}

#include "cpu/exec/template-end.h"
