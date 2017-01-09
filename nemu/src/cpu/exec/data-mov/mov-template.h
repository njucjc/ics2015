#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX),R_DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr,R_DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

make_helper(concat(mov_r2cr_,SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	uint8_t creg = m.reg;
	uint8_t reg = m.R_M;
	//uint8_t creg = (instr_fetch(eip+1,1) >> 3)&0x7;
	//uint8_t reg = instr_fetch(eip+1,1)&0x7;
	if(creg == 0) {
		cpu.cr0.val = REG(reg);
		//Log("%x\t%x",m.reg,reg);
	}
	else if(creg == 3){
		cpu.cr3.val = REG(reg);
	}
	else
		assert(0);
	print_asm("mov"str(SUFFIX) " %%%s,%%cr%d",REG_NAME(reg),creg);
	return 2;
}

make_helper(concat(mov_cr2r_,SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	uint8_t creg = m.reg;
	uint8_t reg = m.R_M;
	//uint8_t creg = (instr_fetch(eip+1,1) >> 3)&0x7;
	//uint8_t reg = instr_fetch(eip+1,1)&0x7;
	if(creg == 0) {
		REG(reg) = cpu.cr0.val;
	}
	else if(creg == 3){
		REG(reg) = cpu.cr3.val;
	}
	else
		assert(0);
	print_asm("mov"str(SUFFIX) " %%cr%d,%%%s",creg,REG_NAME(reg));
	return 2;
}

make_helper(concat(mov_rm2sr_,SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);

	assert(m.mod == 3);/*use r mod*/
	SREG(m.reg) = REG(m.R_M) & 0xffff;//data byte is 16.
	print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), SREG_NAME(m.reg));
	return 2;
}



#include "cpu/exec/template-end.h"
