#include "cache.h"
//#include <stdlib.h>
//#include <time.h>

#define L1_SLOT_WIDTH 3

#define NR_COL (1 << L1_COL_WIDTH) /*L1 cache slot col*/
#define NR_SLOT (1 << L1_SLOT_WIDTH) /* the number of L1 cache set's slot */
#define NR_SET (1 << L1_SET_WIDTH) /* the number of L1 cache set */

#define HW_MEM_SIZE (1 << 27)

#define COL_MASK (NR_COL - 1)

//#define debug

typedef struct { /* slot of cache */
	uint8_t buf[NR_COL];
	bool valid;
	uint32_t tag;
} L1_Slot;

typedef struct { /* set of cache */
	L1_Slot slot[NR_SLOT];
} L1_Set;

typedef struct { /* cache */
	L1_Set set[NR_SET];
}L1_Cache;

static L1_Cache cache1;

//uint32_t dram_read(hwaddr_t, size_t);
uint32_t L2_cache_read(hwaddr_t, size_t); /* cache1 read miss do */

void init_L1_cache() { /*initialize cache */
	int i,j;
	for(i = 0;i < NR_SET; i++)
		for( j = 0; j < NR_SLOT ;j++)
			cache1.set[i].slot[j].valid = false; /* set cache slot vaild false */
}

static void cache1_miss_do(uint32_t set, uint32_t slot, uint32_t tag, uint32_t addr) {
	
	cache1.set[set].slot[slot].valid = true;
	cache1.set[set].slot[slot].tag = tag;
	int i;
	for(i = 0; i < NR_COL;i++) { 
			cache1.set[set].slot[slot].buf[i] = L2_cache_read((addr & ~COL_MASK) + i, 1);
		}
}
static void L1_cache_set_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);
	
	/* address decode */
	sram_addr temp; /*declare in cache.h*/
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col1; /*use mode one */
	uint32_t set = temp.set1;
	uint32_t tag = temp.tag1;

	bool full = true;
	bool hit = false;

	uint32_t slot, slot_t = 0;/*slot_t point to usable cache slot*/
	for(slot = 0; slot < NR_SLOT; slot++) {
		if(cache1.set[set].slot[slot].valid) {
			if(cache1.set[set].slot[slot].tag == tag) { /* hit in cache and start return cache data */
				memcpy(data, cache1.set[set].slot[slot].buf + col, BURST_LEN);
				hit = true;
				break;
			}
		} 
		else {
			full = false; /*have som empty slot*/
			slot_t = slot;
		}
	}
#ifdef debug
	Log(" slot = %d, slot_t = %d\n",slot,slot_t);
#endif
	/* miss in L1 cache */
	if(hit == false) {
		if(full) {
			 slot_t = replace_slot(NR_SLOT); /*random replace */
		}
		cache1_miss_do(set, slot_t, tag, addr); /* read L2 cache */
		memcpy(data, cache1.set[set].slot[slot_t].buf + col, BURST_LEN);
	}
}

static void L1_cache_set_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	/* address decode */
	sram_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col1;
	uint32_t set = temp.set1;
	uint32_t tag = temp.tag1; 
	
	/* find data in cache */
	uint32_t slot;
	for(slot = 0; slot < NR_SLOT; slot++) {
		if(cache1.set[set].slot[slot].valid && cache1.set[set].slot[slot].tag == tag) {
			/* hit in cache and start copy data to cache */
			memcpy_with_mask(cache1.set[set].slot[slot].buf + col, data, BURST_LEN, mask); 
			break;
		}
	}
	/* if miss write L1_cache (do that in memory.c */
}

uint32_t L1_cache_read(hwaddr_t addr, size_t len) { /*reference to dram.c*/
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	L1_cache_set_read(addr, temp);

	if(offset + len > BURST_LEN) {
		L1_cache_set_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	return unalign_rw(temp + offset, 4);
}

void L1_cache_write(hwaddr_t addr, size_t len, uint32_t data) { /*reference to dram.c*/
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	L1_cache_set_write(addr, temp, mask);

	if(offset + len > BURST_LEN) {
		L1_cache_set_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}
