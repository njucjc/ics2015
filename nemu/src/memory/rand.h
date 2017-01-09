#include <stdlib.h>
#include <time.h>

#ifndef __RAND_H__
#define __RAND_H__

static inline uint32_t replace_slot(uint32_t nr_slot) { /* random replace */
	srand(time(0));
	return rand()%nr_slot;
}

#endif
