#include "cpu/exec/template-start.h"

#define instr cmov

make_cmovcc_helper(e,cpu.EFLAGS.ZF == 1,rm2r)
make_cmovcc_helper(ge,cpu.EFLAGS.SF == cpu.EFLAGS.OF,rm2r)
make_cmovcc_helper(s,cpu.EFLAGS.SF == 1,rm2r)
make_cmovcc_helper(ns,cpu.EFLAGS.SF == 0,rm2r)
make_cmovcc_helper(a,cpu.EFLAGS.CF == 0 && cpu.EFLAGS.ZF == 0,rm2r)
make_cmovcc_helper(ne,cpu.EFLAGS.ZF == 0, rm2r)
make_cmovcc_helper(be,cpu.EFLAGS.CF == 1 || cpu.EFLAGS.ZF == 1 , rm2r)
make_cmovcc_helper(o,cpu.EFLAGS.OF == 1,rm2r)
make_cmovcc_helper(no,cpu.EFLAGS.OF == 0,rm2r)
make_cmovcc_helper(ae,cpu.EFLAGS.CF == 1,rm2r)
make_cmovcc_helper(nb,cpu.EFLAGS.CF == 0,rm2r)
make_cmovcc_helper(p,cpu.EFLAGS.PF == 1,rm2r)
make_cmovcc_helper(np,cpu.EFLAGS.PF == 0,rm2r)
make_cmovcc_helper(nge,cpu.EFLAGS.SF != cpu.EFLAGS.OF,rm2r)
make_cmovcc_helper(g,cpu.EFLAGS.ZF == 0 && cpu.EFLAGS.SF == cpu.EFLAGS.OF,rm2r)
make_cmovcc_helper(ng,cpu.EFLAGS.ZF == 1 || cpu.EFLAGS.SF != cpu.EFLAGS.OF,rm2r)

#include "cpu/exec/template-end.h"
