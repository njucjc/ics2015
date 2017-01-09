#include "common.h"
#include "cpu/reg.h"  


#define NR_SDC 4

uint32_t lnaddr_read(hwaddr_t, size_t);

typedef struct {
	uint16_t tag;
	uint32_t buf[2];
	bool valid;
	SegDesc* segdesc;
} SegDescCache_block;

static SegDescCache_block SegDescCache[NR_SDC];

void init_SegDescCache() {
	int i;
	for(i = 0; i < NR_SDC ; i++)
		SegDescCache[i].valid = false;
}

SegDesc* load_seg(uint8_t sreg) {
	if((SegDescCache[sreg].valid == true) && (cpu.sreg[sreg].index == SegDescCache[sreg].tag)) { 
// hit 
		Assert(SegDescCache[sreg].segdesc->present == 1,"Segment fault !");
		uint8_t dpl = SegDescCache[sreg].segdesc->privilege_level;
		Assert(cpu.sreg[R_CS].rpl <= dpl && cpu.sreg[sreg].rpl <= dpl,"Segment fault !");
		Assert(cpu.sreg[sreg].index * 8 < (SegDescCache[sreg].segdesc->limit_19_16 << 16) + SegDescCache[sreg].segdesc->limit_15_0, "Segment fault!");

		return SegDescCache[sreg].segdesc;
	
	}
 	else {
		int i;
		for(i = 0; i < 2; i++) {
			SegDescCache[sreg].buf[i] = lnaddr_read(cpu.GDTR.base + (cpu.sreg[sreg].index << 3)+ (i << 2), 4);
 		}

		SegDescCache[sreg].tag = cpu.sreg[sreg].index;
		SegDescCache[sreg].segdesc = (SegDesc*)SegDescCache[sreg].buf;
		SegDescCache[sreg].valid = true;

	}
	return load_seg(sreg);

}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {

	if(cpu.cr0.protect_enable == 0) 
		return addr; 
 	
	SegDesc *segdesc = load_seg(sreg);

	lnaddr_t lnaddr;
	lnaddr = (segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0 + addr;
	
	Assert(addr == lnaddr,"Segment fault !");
	
	return lnaddr;
}


void init_seg() {

	
	SegDesc *segdesc = (SegDesc*)SegDescCache[R_CS].buf;
	segdesc->present = 1;
	segdesc->base_31_24 = segdesc->base_23_16 = segdesc->base_15_0 = 0;
	segdesc->limit_19_16 = segdesc->limit_15_0 = -1;
	SegDescCache[R_CS].segdesc = segdesc;
	SegDescCache[R_CS].valid = true;
	SegDescCache[R_CS].tag = cpu.sreg[R_CS].index;
}


