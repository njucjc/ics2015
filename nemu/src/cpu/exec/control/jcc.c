#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE	

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE 

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

make_helper_cc_v(j,a,si)
make_helper_cc_v(j,ae,si)
make_helper_cc_v(j,be,si)
make_helper_cc_v(j,e,si)
make_helper_cc_v(j,g,si)
make_helper_cc_v(j,ge,si)
make_helper_cc_v(j,l,si)
make_helper_cc_v(j,le,si)
make_helper_cc_v(j,ne,si)
make_helper_cc_v(j,ns,si)
make_helper_cc_v(j,s,si)
make_helper_cc_v(j,nae,si)
make_helper_cc_v(j,o,si)
make_helper_cc_v(j,no,si)
make_helper_cc_v(j,p,si)
make_helper_cc_v(j,po,si)


