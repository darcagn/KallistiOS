/* KallistiOS ##version##

   kernel/arch/dreamcast/include/dc/hw.h
   Copyright (C) 2001 Megan Potter
   Copyright (C) 2023 Eric Fradella

*/

/** \file    dc/hw.h
    \brief   Dreamcast hardware definitions and functions.
    \ingroup arch

    This file has various Dreamcast platform specific functions in it.

    \author Megan Potter
*/

#ifndef __DC_HW_H
#define __DC_HW_H

#include <kos/cdefs.h>
__BEGIN_DECLS

/** \defgroup hw_memsizes           Memory Capacity
    \brief                          Console memory sizes
    \ingroup                        arch

    These are the various memory sizes, in bytes, that can be returned by the
    HW_MEMSIZE macro.

    @{
*/
#define HW_MEM_16           16777216   /**< \brief 16M retail Dreamcast */
#define HW_MEM_32           33554432   /**< \brief 32M NAOMI/modded Dreamcast */
/** @} */

/** \brief   Determine how much memory is installed in current machine.
    \ingroup arch

    \return The total size of system memory in bytes.
*/
#define HW_MEMSIZE (_arch_mem_top - 0x8c000000)

/** \brief   Use this macro to easily determine if system has 32MB of RAM.
    \ingroup arch

    \return Non-zero if console has 32MB of RAM, zero otherwise
*/
#define HW_DBL_MEM (_arch_mem_top - 0x8d000000)

/** \brief   Initialize bare-bones hardware systems.
    \ingroup arch

    This will be done automatically for you on start by the default arch_main(),
    so you shouldn't have to deal with this yourself.

    \retval 0               On success (no error conditions defined).
*/
int hardware_sys_init(void);

/** \brief   Initialize some peripheral systems.
    \ingroup arch

    This will be done automatically for you on start by the default arch_main(),
    so you shouldn't have to deal with this yourself.

    \retval 0               On success (no error conditions defined).
*/
int hardware_periph_init(void);

/** \brief   Shut down hardware that was initted.
    \ingroup arch

    This function will shut down anything initted with hardware_sys_init() and
    hardware_periph_init(). This will be done for you automatically by the
    various exit points, so you shouldn't have to do this yourself.
*/
void hardware_shutdown(void);

/** \defgroup hw_consoles           Console Types
    \brief                          Byte values returned by hardware_sys_mode()
    \ingroup  arch

    These are the various console types that can be returned by the
    hardware_sys_mode() function.

    @{
*/
#define HW_TYPE_RETAIL      0x0     /**< \brief A retail Dreamcast. */
#define HW_TYPE_SET5        0x9     /**< \brief A Set5.xx devkit. */
/** @} */

/** \defgroup hw_regions            Region Codes
    \brief                          Values returned by hardware_sys_mode();
    \ingroup  arch

    These are the various region codes that can be returned by the
    hardware_sys_mode() function.

    \note
    A retail Dreamcast will always return 0 for the region code.
    You must read the region of a retail device from the flashrom.

    \see    fr_region
    \see    flashrom_get_region()

    @{
*/
#define HW_REGION_UNKNOWN   0x0     /**< \brief Unknown region. */
#define HW_REGION_ASIA      0x1     /**< \brief Japan/Asia (NTSC) */
#define HW_REGION_US        0x4     /**< \brief North America */
#define HW_REGION_EUROPE    0xC     /**< \brief Europe (PAL) */
/** @} */

/** \brief   Retrieve the system mode of the console in use.
    \ingroup arch

    This function retrieves the system mode register of the console that is in
    use. This register details the actual system type in use (and in some system
    types the region of the device).

    \param  region          On return, the region code (one of the
                            \ref hw_regions) of the device if the console type
                            allows reading it through the system mode register
                            -- otherwise, you must retrieve the region from the
                            flashrom.
    \return                 The console type (one of the \ref hw_consoles).
*/
int hardware_sys_mode(int *region);

__END_DECLS

#endif  /* __DC_HW_H */
