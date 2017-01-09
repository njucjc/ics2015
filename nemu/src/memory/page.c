#include "common.h"
#include "cpu/reg.h"
#include "rand.h"

#define NR_TLB_BLOCK 64


uint32_t hwaddr_read(lnaddr_t ,size_t);

typedef struct {
	bool valid;
	uint32_t tag;
	uint32_t base;
}TLB_block;

static TLB_block TLB[NR_TLB_BLOCK];
static uint32_t cr3_val = 0;
static int block_index = -1;

void init_TLB() {
	cr3_val = cpu.cr3.val;
	block_index = -1;
	int i;
	for(i = 0; i < NR_TLB_BLOCK; i++) 
		TLB[i].valid = false;
}

static hwaddr_t load_page(lnaddr_t lnaddr) {
	
	uint32_t dir = (lnaddr >> 22);
	uint32_t page = ((lnaddr >> 12) & 0x3ff) ;
	
	uint32_t dir_addr = (cpu.cr3.page_directory_base << 12) + (dir << 2);
	PDE pde;
	pde.val = hwaddr_read(dir_addr,4);
	Assert(pde.present == 1,"%x Page fault !",lnaddr);
	
	uint32_t page_addr = (pde.page_frame << 12 ) + (page << 2);

	PTE pte;
	pte.val = hwaddr_read(page_addr,4);

	Assert(pte.present == 1,"%x Page fault !",lnaddr);

	
	if(block_index < NR_TLB_BLOCK)
		block_index ++;
	else {
		block_index = replace_slot(NR_TLB_BLOCK);
	}
	TLB[block_index].base = pte.page_frame << 12;
	TLB[block_index].valid = true;
	TLB[block_index].tag = lnaddr >> 12;
	
	
	return TLB[block_index].base;
}

static hwaddr_t read_TLB(lnaddr_t lnaddr) {
	uint32_t tag = (lnaddr >> 12);
	if(cr3_val != cpu.cr3.val) {
		init_TLB();
		return read_TLB(lnaddr);
	}
	
	int i;
	for( i = 0; i < NR_TLB_BLOCK;i++)
		if(TLB[i].tag == tag && TLB[i].valid) {
			return TLB[i].base;
		}
	return load_page(lnaddr);

}

hwaddr_t page_translate(lnaddr_t lnaddr) {
	//Log("0x%x\n",lnaddr);
	if(cpu.cr0.protect_enable == 0 || cpu.cr0.paging == 0)
		return lnaddr;

	hwaddr_t hwaddr = read_TLB(lnaddr) + (lnaddr & 0xfff);
	return hwaddr;
	
}
