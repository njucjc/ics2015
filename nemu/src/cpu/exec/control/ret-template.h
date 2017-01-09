#include "cpu/exec/template-start.h"

#define instr ret

/*static void do_execute() {
	cpu.eip = MEM_R(REG(R_ESP));
	REG(R_ESP) += 2;
	cpu.eip &= 0xffff;
	printf("0x%x %d\n",op_src->val,DATA_BYTE);
	REG(R_ESP) += op_src->val;

}*/

make_helper(concat(ret_near_,SUFFIX)) {
	cpu.eip = MEM_R(cpu.esp,R_SS);
	//printf("%x\n",cpu.eip);
#if DATA_BYTE == 2
	cpu.eip &= 0xffff;
#endif
	REG(R_ESP) += DATA_BYTE;
	print_asm("ret");
	return 0;
}


//make_instr_helper(i);
make_helper(concat(ret_imm_,SUFFIX)) {
	/*if(DATA_BYTE != 2)
		printf("In ret have a bug.\n");*/
	uint16_t imm = instr_fetch(eip+1,2);
	cpu.eip = MEM_R(REG(R_ESP),R_SS);
	REG(R_ESP) += 2;
	//cpu.eip &= 0xffff;
	REG(R_ESP) += imm;
	print_asm("ret" " 0x%x",imm);
	return 0;
	
}


#include "cpu/exec/template-end.h"
