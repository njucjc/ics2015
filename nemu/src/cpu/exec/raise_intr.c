#include<setjmp.h>
#include "cpu/reg.h"
#include "../../lib-common/x86-inc/mmu.h"


extern jmp_buf jbuf;

void swaddr_write(swaddr_t, size_t, uint32_t ,uint8_t);
uint32_t lnaddr_read(lnaddr_t, size_t);

typedef union {
	uint32_t buf[2];
	GateDesc gde;
} GDE;

void raise_intr(uint8_t NO) {

	/* save EFLAGS, CS, eip*/
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.EFLAGS_VAL, R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.sreg[R_CS].val, R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip, R_SS);

	/*load Gate Descriper*/

	GDE gde_trans;
	int i;
	for(i = 0; i < 2; i++) 
		gde_trans.buf[i] = lnaddr_read(cpu.IDTR.base + (NO << 3) + (i << 2), 4);

	GateDesc *gde = &gde_trans.gde; 
	cpu.sreg[R_CS].val = gde->segment;//gde_trans.gde.segment;
	uint32_t seg_offset = (gde->offset_31_16 << 16) + (gde->offset_15_0);
	cpu.eip = seg_offset;
	
	/*jump back to cpu_exec()*/
	longjmp(jbuf,1);
}


