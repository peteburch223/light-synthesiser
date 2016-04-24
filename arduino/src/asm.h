/*
 * Global register variables.
 */
#ifdef __ASSEMBLER__

/* Assembler-only stuff */

#else  /* !ASSEMBLER */

/* C-only stuff */

#include <stdint.h>

extern "C" uint8_t led(uint8_t);
extern "C" uint8_t asminit(uint8_t);

#endif /* ASSEMBLER */
