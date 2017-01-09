#include "cpu/exec/template-start.h"

#define instr leave

make_helper(concat(leave_ , SUFFIX)){
	(reg_l(R_ESP)) = (reg_l(R_EBP));
	(reg_l(R_EBP))= swaddr_read((reg_l(R_ESP)), 4, R_SS);
	(reg_l(R_ESP)) += 4;
	print_asm("leave");
	return 1;
}
#include "cpu/exec/template-end.h"
