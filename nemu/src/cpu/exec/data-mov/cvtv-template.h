#include "cpu/exec/template-start.h"

#define instr cvtv

make_helper(concat(cvtv_,SUFFIX)) {
	DATA_TYPE_S temp = -1;
	if(MSB(REG(R_EAX))==1)
		REG(R_EDX) = temp;
	else
		REG(R_EDX) = 0;
#if DATA_BYTE == 2
	print_asm("cwtl");
#else
	print_asm("cltd");
#endif
	return 1;
}

#include "cpu/exec/template-end.h"
