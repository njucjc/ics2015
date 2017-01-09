#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_SCR_PAGE (SCR_SIZE / PAGE_SIZE)

static PTE vptable[NR_PTE] align_to_page;
/* Use the function to get the start address of user page directory. */
inline PDE* get_updir();
inline PDE* get_kpdir();

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	PDE *pde = get_updir();
	PTE *pte = (PTE *)(0xa0 + vptable);
	pde->val = make_pde(va_to_pa(vptable));
	
	uint32_t  pframe_addr = VMEM_ADDR;
	int i;
	for(i = 0; i <= NR_SCR_PAGE; i++) {
		pte->val = make_pte(pframe_addr);
		pte++;
		pframe_addr += PAGE_SIZE;
	}

	/*others*/
	/*PDE *upde = get_updir();
	PDE *kpde = get_kpdir();
	memcpy(upde, kpde, sizeof(PDE));*/
		
	
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

/*void swaddr_write(uint8_t *buf, uint32_t offset, uint32_t len) {
	
	memcpy((void*)offset, buf, len);
}*/

