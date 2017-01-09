#include "common.h"
#include "burst.h"
#include "misc.h"
#include "rand.h"


#define L1_COL_WIDTH 6
#define L1_SET_WIDTH 7
#define L1_TAG_WIDTH (27 - L1_COL_WIDTH - L1_SET_WIDTH)

#define L2_COL_WIDTH 6
#define L2_SET_WIDTH 12
#define L2_TAG_WIDTH (27 - L2_COL_WIDTH - L2_SET_WIDTH)



typedef union {
	struct { /*used in L1 cache */
		uint32_t col1	:L1_COL_WIDTH;
		uint32_t set1	:L1_SET_WIDTH;
		uint32_t tag1	:L1_TAG_WIDTH;
	};
	struct { /*used in L2 cache */
		uint32_t  col2	:L2_COL_WIDTH;
		uint32_t  set2	:L2_SET_WIDTH;
		uint32_t tag2	:L2_TAG_WIDTH;
	};
	uint32_t  addr;
} sram_addr;/*sram address decoder*/


