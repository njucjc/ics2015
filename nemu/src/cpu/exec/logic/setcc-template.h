#include "cpu/exec/template-start.h"

#define instr set

make_setcc_helper(ne,cpu.EFLAGS.ZF == 0,rm)
make_setcc_helper(e,cpu.EFLAGS.ZF == 1,rm)
make_setcc_helper(le,cpu.EFLAGS.ZF == 1 && cpu.EFLAGS.SF != cpu.EFLAGS.OF,rm)
make_setcc_helper(ge,cpu.EFLAGS.SF == cpu.EFLAGS.OF,rm)
make_setcc_helper(l,cpu.EFLAGS.SF != cpu.EFLAGS.OF,rm)
make_setcc_helper(ns,cpu.EFLAGS.SF == 0,rm)
make_setcc_helper(g,cpu.EFLAGS.ZF == 0 || cpu.EFLAGS.SF == cpu.EFLAGS.OF,rm)
make_setcc_helper(a,cpu.EFLAGS.CF == 0 && cpu.EFLAGS.ZF == 0,rm)
make_setcc_helper(ae,cpu.EFLAGS.CF == 0,rm)
make_setcc_helper(b,cpu.EFLAGS.CF == 1,rm)
make_setcc_helper(be,cpu.EFLAGS.CF == 1 || cpu.EFLAGS.ZF == 1,rm)
make_setcc_helper(o,cpu.EFLAGS.OF == 1,rm)
make_setcc_helper(no,cpu.EFLAGS.OF == 0,rm)
make_setcc_helper(s,cpu.EFLAGS.SF == 1,rm)
make_setcc_helper(p,cpu.EFLAGS.PF == 1,rm)
make_setcc_helper(po,cpu.EFLAGS.PF == 0,rm)


#include "cpu/exec/template-end.h"

