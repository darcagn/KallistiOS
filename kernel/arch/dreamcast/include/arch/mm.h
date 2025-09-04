/* KallistiOS ##version##

   arch/dreamcast/include/arch/mm.h
   (c) 2001 Megan Potter

*/

#ifndef __ARCH_MM_H
#define __ARCH_MM_H

#include <stdbool.h>
#include <stdint.h>

/* arch_mem_top defined in startup.S */
extern uint32_t _arch_mem_top;

#define ARCH_MM_MEM_TOP            _arch_mem_top
#define ARCH_MM_PAGE_PHYS_BASE     0x8C010000
#define ARCH_MM_PAGE_SIZE          4096
#define ARCH_MM_PAGE_SIZE_BITS     12
#define ARCH_MM_PAGE_MASK          (ARCH_MM_PAGE_SIZE - 1)
#define ARCH_MM_PAGE_COUNT         ((ARCH_MM_MEM_TOP - ARCH_MM_PAGE_PHYS_BASE) / ARCH_MM_PAGE_SIZE)

int arch_mm_init(void);

void *arch_mm_sbrk(unsigned long increment);

static inline bool arch_mm_valid_address(uintptr_t ptr) {
    return ptr >= ARCH_MM_PAGE_PHYS_BASE && ptr < ARCH_MM_MEM_TOP;
}

/* Symbols provided by the linker script */
extern char _executable_start;
extern char _etext;

static inline bool arch_mm_valid_text_address(uintptr_t ptr) {
    return ptr >= (uintptr_t)&_executable_start && ptr < (uintptr_t)&_etext;
}

#endif  /* __ARCH_MM_H */
