#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE	

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE 

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE 

/*make_helper_v(jmp_rel)
//make_helper_v(jmp_reg)
make_helper_v(jmp_rm0)*/
make_helper_v(jmp_rm)
make_helper_v(jmp_si)
make_helper_v(ljmp)
//make_helper_v(jmp_rel)
//make_helper_v(jmp_i)


