#include "cpu/exec/helper.h"

make_helper(pusha) {
	uint32_t temp = cpu.esp;
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.eax,R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.ecx,R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.edx,R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.ebx,R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,temp,R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.ebp,R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.esi,R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.edi,R_SS);
    return 1;
}
