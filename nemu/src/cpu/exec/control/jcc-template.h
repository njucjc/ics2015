#include "cpu/exec/template-start.h"

#define instr j

make_jcc_helper(a,cpu.EFLAGS.CF == 0 && cpu.EFLAGS.ZF == 0,si)
make_jcc_helper(be,cpu.EFLAGS.ZF == 1 || cpu.EFLAGS.CF == 1,si)
make_jcc_helper(e,cpu.EFLAGS.ZF == 1,si)
make_jcc_helper(g,cpu.EFLAGS.ZF == 0 && cpu.EFLAGS.SF == cpu.EFLAGS.OF,si)
make_jcc_helper(ge,cpu.EFLAGS.SF == cpu.EFLAGS.OF || cpu.EFLAGS.ZF == 1,si)
make_jcc_helper(l,cpu.EFLAGS.SF != cpu.EFLAGS.OF && cpu.EFLAGS.ZF == 0,si)
make_jcc_helper(le,cpu.EFLAGS.ZF == 1 || (cpu.EFLAGS.SF != cpu.EFLAGS.OF),si)
make_jcc_helper(ne,cpu.EFLAGS.ZF == 0,si)
make_jcc_helper(ns,cpu.EFLAGS.SF == 0,si)
make_jcc_helper(s,cpu.EFLAGS.SF == 1,si)
make_jcc_helper(ae,cpu.EFLAGS.CF == 0,si)
make_jcc_helper(nae,cpu.EFLAGS.CF == 1,si)
make_jcc_helper(o,cpu.EFLAGS.OF == 1,si)
make_jcc_helper(no,cpu.EFLAGS.OF == 0,si) 
make_jcc_helper(p, cpu.EFLAGS.PF == 1,si)
make_jcc_helper(po, cpu.EFLAGS.PF == 0,si)

#include "cpu/exec/template-end.h"
