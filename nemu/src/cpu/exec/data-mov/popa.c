#include "cpu/exec/helper.h"

make_helper(popa) {
	cpu.edi = swaddr_read(cpu.esp,4,R_SS);
	cpu.esp += 4;

	cpu.esi = swaddr_read(cpu.esp,4,R_SS);
	cpu.esp += 4;

	cpu.ebp = swaddr_read(cpu.esp,4,R_SS);
	cpu.esp += 4;

	cpu.esp += 4; /*skip esp */

	cpu.ebx = swaddr_read(cpu.esp,4,R_SS);
	cpu.esp += 4;

	cpu.edx = swaddr_read(cpu.esp,4,R_SS);
	cpu.esp += 4;

	cpu.ecx = swaddr_read(cpu.esp,4,R_SS);
	cpu.esp += 4;

	cpu.eax = swaddr_read(cpu.esp,4,R_SS);
	cpu.esp += 4;
	
	return 1;
}	

