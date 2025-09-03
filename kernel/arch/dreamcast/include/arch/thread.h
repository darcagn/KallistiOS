/* KallistiOS ##version##

   kernel/arch/dreamcast/include/arch/thread.h
   Copyright (C) 2001 Megan Potter

*/

#ifndef __ARCH_THREAD_H
#define __ARCH_THREAD_H

/* Scheduler frequency for Dreamcast */
#define ARCH_THD_SCHED_HZ    100

/* Defined in dreamcast/kernel/thdswitch.s */
int arch_thd_block_now(irq_context_t *mycxt);

#endif /* __ARCH_THREAD_H */
