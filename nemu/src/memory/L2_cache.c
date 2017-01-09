#include "cache.h"
//#include <stdlib.h>
//#include <time.h>

#define L2_SLOT_WIDTH 4

#define NR_COL (1 << L2_COL_WIDTH)
#define NR_SLOT (1 << L2_SLOT_WIDTH)
#define NR_SET (1 << L2_SET_WIDTH)

#define HW_MEM_SIZE (1 << 27)

#define COL_MASK (NR_COL - 1)

typedef struct { /* slot of cache */
	uint8_t buf[NR_COL];
	bool valid;
	bool dirty;
	uint32_t tag;
} L2_Slot;

typedef struct { /* set of cache */
	L2_Slot slot[NR_SLOT];
} L2_Set;

typedef struct { /* cache */ 
	L2_Set set[NR_SET];
}L2_Cache;

static L2_Cache cache2;

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_L2_cache() {
	int i,j;
	for(i = 0;i < NR_SET; i++)
		for( j = 0; j < NR_SLOT ;j++)
			cache2.set[i].slot[j].valid = false; /* set cache slot vaild false */
}

static inline void write_back(uint32_t set, uint32_t slot, hwaddr_t addr) { /* write back to dram */
	if(cache2.set[set].slot[slot].dirty) { /*if dirty then write back*/
		uint32_t i;
		for(i = 0; i < NR_COL; i++) 
			dram_write((addr & ~COL_MASK) + i, 1, cache2.set[set].slot[slot].buf[i]);
	}
}

static inline void cache2_miss_do(uint32_t set, uint32_t slot, uint32_t tag, uint32_t addr) {
	cache2.set[set].slot[slot].valid = true;
	cache2.set[set].slot[slot].dirty = false;
	cache2.set[set].slot[slot].tag = tag;
	uint32_t i;
	for(i = 0; i < NR_COL; i++) 
		cache2.set[set].slot[slot].buf[i] = dram_read((addr & ~COL_MASK) + i, 1);
}

static void L2_cache_set_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	/* address decode */
	sram_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col2;
	uint32_t set = temp.set2;
	uint32_t tag = temp.tag2;

	bool full = true;
	bool hit = false;
	
	/* find data in cache */
	uint32_t slot, slot_t = 0;
	for(slot = 0; slot < NR_SLOT; slot++) {
		if(cache2.set[set].slot[slot].valid) {
			if(cache2.set[set].slot[slot].tag == tag) {
				/* hit in cache and start return cache data */
				memcpy(data, cache2.set[set].slot[slot].buf + col, BURST_LEN);
				hit = true;
				break;
			}
		} 
		else {
			full = false;
			slot_t = slot;
		}
	}

	/*not found in cache*/
	if(hit == false) {
		if(full) {
			//srand(time(0));
			//slot_t = rand()%NR_SLOT;
			slot_t = replace_slot(NR_SLOT);  /*random replace */
			write_back(set, slot_t, addr);
		}
		cache2_miss_do(set, slot_t, tag, addr);
		memcpy(data, cache2.set[set].slot[slot_t].buf + col, BURST_LEN);
	}
}

static void L2_cache_set_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	/* address decode */
	sram_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col2;
	uint32_t set = temp.set2;
	uint32_t tag = temp.tag2;

	bool full = true;
	bool hit = false;
	/* find data in cache */
	uint32_t slot, slot_t = 0;
	for(slot = 0; slot < NR_SLOT;slot++) {
		if(cache2.set[set].slot[slot].valid) {
			if(cache2.set[set].slot[slot].tag == tag) {
				hit = true;
				cache2.set[set].slot[slot].dirty = true;
				
				/* hit in cache and start copy data to cache */
				memcpy_with_mask(cache2.set[set].slot[slot].buf + col, data, BURST_LEN, mask);
				break;
			}
		} 
		else {
			full = false;
			slot_t = slot; /*have some empty slot*/
		}
	}

	/*miss in cache2 */
	if(hit == false) {
		if(full) {
			//srand(time(0));
			//slot_t = rand()%NR_SLOT;
			slot_t = replace_slot(NR_SLOT);  /*random replace */
			/* start write back*/
			write_back(set, slot_t, addr);
		}
		cache2_miss_do(set, slot_t, tag, addr);
		memcpy_with_mask(cache2.set[set].slot[slot_t].buf + col, data, BURST_LEN, mask);
	}

}

uint32_t L2_cache_read(hwaddr_t addr, size_t len) { /*reference to dram.c*/
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	L2_cache_set_read(addr, temp);

	if(offset + len > BURST_LEN) {
		L2_cache_set_read(addr + BURST_LEN, temp + BURST_LEN);
	} 
	return unalign_rw(temp + offset, 4);
}

void L2_cache_write(hwaddr_t addr, size_t len, uint32_t data) { /*reference to dram.c*/
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	L2_cache_set_write(addr, temp, mask);

	if(offset + len > BURST_LEN) {
		L2_cache_set_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}
