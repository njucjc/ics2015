#include "cpu/exec/template-start.h"

#define instr out

void pio_write(ioaddr_t,size_t ,uint32_t);

make_helper(concat(out_,SUFFIX)) {
	pio_write(reg_w(R_DX),1,REG(R_EAX));

	print_asm("out" " %%al,(%%dx)");

	return 1;

}

#include "cpu/exec/template-end.h"

