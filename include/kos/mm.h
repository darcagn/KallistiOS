/* KallistiOS ##version##

   include/kos/mm.h
   Copyright (C) 2003 Megan Potter
   Copyright (C) 2012 Lawrence Sebald

*/

/** \file   kos/mm.h
    \brief  Memory management.
    \ingroup memmgmt

    This file contains functionality related to the managed region of memory.

    \author Megan Potter
*/

#ifndef __KOS_MM_H
#define __KOS_MM_H

#include <kos/cdefs.h>
__BEGIN_DECLS

#include <arch/mm.h>

/** \brief  Top of managed memory region. */
#define MM_MEM_TOP         ARCH_MM_MEM_TOP

/** \brief  Page size for managed memory region. */
#define MM_PAGE_SIZE       ARCH_MM_PAGE_SIZE

/** \brief  Bits for page size. */
#define MM_PAGE_SIZE_BITS  ARCH_MM_PAGE_SIZE_BITS

/** \brief  Mask for page offset. */
#define MM_PAGE_MASK       ARCH_MM_PAGE_MASK

/** \brief  Base address of available physical pages. */
#define MM_PAGE_PHYS_BASE  ARCH_MM_PAGE_PHYS_BASE

/** \brief  Page count for managed memory region. */
#define MM_PAGE_COUNT      ARCH_MM_PAGE_COUNT

/* These are in mm.c */
/** \brief   Initialize the memory management system.
    \ingroup arch

    \retval 0               On success (no error conditions defined).
*/
static inline int mm_init(void) {
    return arch_mm_init();
};

/** \brief   Request more core memory from the system.
    \ingroup arch

    \param  increment       The number of bytes requested.
    \return                 A pointer to the memory.
    \note                   This function will panic if no memory is available.
*/
static inline void *mm_sbrk(unsigned long increment) {
    return arch_mm_sbrk(increment);
};

/** \brief   Returns true if the passed address is likely to be valid. Doesn't
             have to be exact, just a sort of general idea.
    \ingroup arch

    \return                 Whether the address is valid or not for normal
                            memory access.
*/
static inline bool mm_valid_address(uintptr_t ptr) {
    return arch_mm_valid_address(ptr);
}

/** \brief   Returns true if the passed address is in the text section of your
             program.
    \ingroup arch

    \return                 Whether the address is valid or not for text
                            memory access.
*/
static inline bool mm_valid_text_address(uintptr_t ptr) {
    return arch_mm_valid_text_address(ptr);
}

__END_DECLS

#endif  /* __KOS_MM_H */
