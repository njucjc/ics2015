#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_,SUFFIX)) {
	//MEM_W(REG(R_EDI),REG(R_EAX));
	swaddr_write(cpu.edi,DATA_BYTE,(DATA_TYPE)cpu.eax,R_ES);
	if(cpu.EFLAGS.DF == 0) 
		cpu.edi += DATA_BYTE;
	else
		cpu.edi -= DATA_BYTE;
	print_asm("stos"str(SUFFIX));
	return 1;
}

#include "cpu/exec/template-end.h"
