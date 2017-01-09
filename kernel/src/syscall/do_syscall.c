#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
int fs_open(const char *, int);
int fs_read(int, void *, size_t);
int fs_write(int, void*, size_t);
int fs_lseek(int, int, int);
void fs_close(int);
void serial_printc(char);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

static ssize_t sys_write(int fd, const void *buf, size_t count) {
	if(fd == 1 || fd == 2) {
#ifdef HAS_DEVICE
		int i;
		for(i = 0; i < count; i++)
			serial_printc(*(char *)(buf + i));
#else
		asm volatile (".byte 0xd6" : : "a"(2),"c"(buf),"d"(count));
#endif
		return count;
	}
	else 
		return -1;


}

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk: sys_brk(tf); break;
		case SYS_write: tf->eax = sys_write(tf->ebx,(void *)tf->ecx,tf->edx); break;
		case SYS_open: tf->eax = fs_open((void *)tf->ebx,tf->ecx);break;
		case SYS_read: tf->eax = fs_read(tf->ebx, (void *)tf->ecx, tf->edx);break;
		case SYS_lseek: tf->eax = fs_lseek(tf->ebx, tf->ecx, tf->edx);break;
		case SYS_close: fs_close(tf->ebx);break;
		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

