#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../../lib-common/x86-inc/cpu.h"
#include "../../../lib-common/x86-inc/mmu.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { R_ES, R_CS, R_SS, R_DS };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union  {
		union  {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
	struct {
		uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};

	union {
		struct {
			uint32_t CF:1;
			uint32_t P_C:1;
			uint32_t PF:1;
			uint32_t A_P:1;
			uint32_t AF:1;
			uint32_t Z_A:1;
			uint32_t ZF:1;
			uint32_t SF:1;
			uint32_t TF:1;
			uint32_t IF:1;
			uint32_t DF:1;
			uint32_t OF:1;
			uint32_t IOPL:2;
			uint32_t NT:1;
			uint32_t R_N:1;
			uint32_t RF:1;
			uint32_t VM:1;
			uint32_t REM:14;
		} EFLAGS;
		uint32_t EFLAGS_VAL;
	};	
		
	/*segment register*/
	union {
		struct {
			uint16_t rpl:1;
			uint16_t ti:2;
			uint16_t index:13;
		}; 
		uint16_t val;
	}sreg[ 4];

	/*control register*/
	CR0 cr0;
	CR3 cr3;

	/*GDT register*/
	struct {
		uint16_t limit;
		uint32_t base;
	} GDTR; 

	struct {
		uint16_t limit;
		uint32_t base;
	} IDTR; 

	volatile bool INTR;
	swaddr_t eip;

} CPU_state;

extern CPU_state cpu;
void init_eflags();


static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

static inline int check_sreg_index(int index) {
	assert(index >= 0 && index < 4);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])
#define sreg(index) (cpu.sreg[check_sreg_index(index)].val)

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* sregs[];

#endif
