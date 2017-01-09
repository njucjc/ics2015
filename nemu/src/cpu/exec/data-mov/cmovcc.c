#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmovcc-template.h"
#undef DATA_BYTE	

#define DATA_BYTE 2
#include "cmovcc-template.h"
#undef DATA_BYTE 

#define DATA_BYTE 4
#include "cmovcc-template.h"
#undef DATA_BYTE

make_helper_cc_v(cmov,e,rm2r)
make_helper_cc_v(cmov,ge,rm2r)
make_helper_cc_v(cmov,s,rm2r)
make_helper_cc_v(cmov,ns,rm2r)
make_helper_cc_v(cmov,a,rm2r)
make_helper_cc_v(cmov,ne,rm2r)
make_helper_cc_v(cmov,be,rm2r)
make_helper_cc_v(cmov,o,rm2r)
make_helper_cc_v(cmov,no,rm2r)
make_helper_cc_v(cmov,ae,rm2r)
make_helper_cc_v(cmov,nb,rm2r)
make_helper_cc_v(cmov,p,rm2r)
make_helper_cc_v(cmov,np,rm2r)
make_helper_cc_v(cmov,nge,rm2r)
make_helper_cc_v(cmov,g,rm2r)
make_helper_cc_v(cmov,ng,rm2r)

