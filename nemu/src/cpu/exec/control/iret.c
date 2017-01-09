#include "cpu/exec/helper.h"

#define instr iret

make_helper(iret) {
	cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;

	cpu.sreg[R_CS].val = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;

	cpu.EFLAGS_VAL = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
	//Log(" 0x%x\n",cpu.eip);

	print_asm("iret");
	return 0;

}

