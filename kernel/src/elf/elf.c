#include "common.h"
#include "memory.h"
//#include "nemu/include/memory/memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0



#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)
//#define SIZE (0xa0000 - 1)

//void swaddr_write(uint8_t *,uint32_t, uint32_t);

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];
	uint8_t load_buf[STACK_SIZE];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
	//HIT_GOOD_TRAP;
#endif

	elf = (void*)buf;

	// TODO: fix the magic number with the correct one 
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	//HIT_GOOD_TRAP;
	nemu_assert(*p_magic == elf_magic);
	
	
	//Load each program segment 
	//panic("please implement me");
	
	ph = (void *)(buf + elf->e_phoff);
	int i;
	for(i = 0;i < elf->e_phnum ;i ++ ) {
		
		if(ph[i].p_type == PT_LOAD) {
#ifdef HAS_DEVICE
	ide_read(load_buf, ELF_OFFSET_IN_DISK + ph[i].p_offset, ph[i].p_filesz);
#else
	ramdisk_read(load_buf, ELF_OFFSET_IN_DISK + ph[i].p_offset, ph[i].p_filesz);
#endif


			uint32_t ph_addr = ph[i].p_vaddr;
#ifdef IA32_PAGE
			ph_addr = mm_malloc(ph[i].p_vaddr,ph[i].p_memsz);
#endif

			memcpy((void*)ph_addr,(void*)load_buf,ph[i].p_filesz);
			
		       
			memset((uint8_t*)(ph_addr+ph[i].p_filesz),0,ph[i].p_memsz-ph[i].p_filesz);
			
#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
