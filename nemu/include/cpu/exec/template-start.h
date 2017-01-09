#include "cpu/exec/helper.h"

#if DATA_BYTE == 1

#define SUFFIX b
#define DATA_TYPE uint8_t
#define DATA_TYPE_S int8_t

#elif DATA_BYTE == 2

#define SUFFIX w
#define DATA_TYPE uint16_t
#define DATA_TYPE_S int16_t

#elif DATA_BYTE == 4

#define SUFFIX l
#define DATA_TYPE uint32_t
#define DATA_TYPE_S int32_t

#else

#error unknown DATA_BYTE

#endif

#define REG(index) concat(reg_, SUFFIX) (index)
#define REG_NAME(index) concat(regs, SUFFIX) [index]
#define SREG(index) sreg(index)
#define SREG_NAME(index) sregs[index]

#define MEM_R(addr, sreg) swaddr_read(addr, DATA_BYTE, sreg)
#define MEM_W(addr, data, sreg) swaddr_write(addr, DATA_BYTE, data, sreg)

#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)

#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))


#define SET_ZF(n) (((n) == 0)?(cpu.EFLAGS.ZF = 1):(cpu.EFLAGS.ZF = 0))
#define SET_SF(n) (cpu.EFLAGS.SF = MSB(n))
#define SET_ADD_OF(dest,src,val) ((MSB(dest) == MSB(src) && MSB(dest) != MSB(val))?(cpu.EFLAGS.OF = 1):(cpu.EFLAGS.OF = 0))
#define SET_ADC_OF(val1,val2) ((MSB(val1) != MSB(val2))?(cpu.EFLAGS.OF = 1):(cpu.EFLAGS.OF = cpu.EFLAGS.OF))
#define SET_SUB_OF(dest,src,val) (((MSB(dest)==1 && MSB(src)==0 && MSB(val)==0) || (MSB(dest)==0 && MSB(src)==1 && MSB(val)==1))?(cpu.EFLAGS.OF=1):(cpu.EFLAGS.OF=0))
#define SET_ADD_CF(dest,value) ((dest > value)?(cpu.EFLAGS.CF = 1):(cpu.EFLAGS.CF = 0))
#define SET_SUB_CF(dest,src) ((dest < src)?(cpu.EFLAGS.CF = 1):(cpu.EFLAGS.CF = 0))
#define SET_PF(val)  \
	uint8_t least_byte = val & 0xff;\
	least_byte ^= least_byte >> 4;\
	least_byte ^= least_byte >> 2;\
	least_byte ^= least_byte >> 1;\
	bool test_pf = !(least_byte & 0x1);\
	test_pf?(cpu.EFLAGS.PF=1):(cpu.EFLAGS.PF=0);\


