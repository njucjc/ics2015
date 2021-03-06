#include "common.h"


uint32_t L1_cache_read(hwaddr_t, size_t);
void L1_cache_write(hwaddr_t, size_t, uint32_t);
void L2_cache_write(hwaddr_t, size_t, uint32_t);

lnaddr_t seg_translate(swaddr_t, size_t, uint8_t);
hwaddr_t page_translate(lnaddr_t);

uint32_t mmio_read(hwaddr_t, size_t,int );
void mmio_write(hwaddr_t, size_t, uint32_t, int);

uint32_t is_mmio(hwaddr_t);

static void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	L1_cache_write(addr, len, data);
	L2_cache_write(addr, len, data);
}

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int map_no = is_mmio(addr);
	return (map_no == -1) ? (L1_cache_read(addr, len) & (~0u >> ((4 - len) << 3))):
				    (mmio_read(addr,len,map_no)  & (~0u >> ((4 - len) << 3)));

}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int map_no = is_mmio(addr);
	return (map_no == -1) ? (cache_write(addr, len, data)):(mmio_write(addr, len, data, map_no));
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {

	hwaddr_t hwaddr = page_translate(addr);
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_t hwaddr = page_translate(addr);
	hwaddr_write(hwaddr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif

	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
 	
	lnaddr_write(lnaddr, len, data);
}

