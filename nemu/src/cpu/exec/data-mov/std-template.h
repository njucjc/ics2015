#include "cpu/exec/template-start.h"

#define instr std

make_helper(concat(std_,SUFFIX)) {
	cpu.EFLAGS.DF = 1;
	print_asm("std");
	return 1;
}

#include "cpu/exec/template-end.h"
