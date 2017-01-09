#include "cpu/exec/template-start.h"

#define instr cli

make_helper(concat(cli_,SUFFIX)) {
		cpu.EFLAGS.IF = 0;
		print_asm("cli");
		return 1;
}

#include "cpu/exec/template-end.h"
