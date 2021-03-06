#include "cpu/exec/template-start.h"

#define instr in

uint32_t pio_read(ioaddr_t, size_t);

make_helper(concat(in_,SUFFIX)) {
	REG(R_EAX) = pio_read((reg_w(R_DX)),DATA_BYTE);
	print_asm("in" " (%%dx),%%%s", REG_NAME(R_EAX));
	return 1;

}

#include "cpu/exec/template-end.h"
