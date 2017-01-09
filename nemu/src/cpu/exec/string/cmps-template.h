#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat(cmps_,SUFFIX)) {
	DATA_TYPE src = swaddr_read(cpu.esi,DATA_BYTE,R_DS);//MEM);
	DATA_TYPE dest = swaddr_read(cpu.edi,DATA_BYTE,R_ES);//MEM_R(REG(R_EDI));
	DATA_TYPE value = src-dest;
	if(cpu.EFLAGS.DF == 0) {
		cpu.edi += DATA_BYTE;
		cpu.esi += DATA_BYTE;
	}
	else {
		cpu.edi -= DATA_BYTE;
		cpu.esi -= DATA_BYTE;
	}

	SET_ZF(value);
	SET_SF(value);
	SET_PF(value);
	SET_SUB_OF(src,dest,value);
	SET_SUB_CF(src,dest);
	print_asm("cmps"str(SUFFIX));
	return 1;
}

#include "cpu/exec/template-end.h"
