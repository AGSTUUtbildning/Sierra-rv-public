/*!
 * \file       sierra.h
 * \details    This file contains:
 *             - the Sierra service calls
 * \version    11.0.0
 * \date       2026
 * \history    Modified 2026:
 *             - Removed sierra_backward_compatibility.h
 *             - Changed logging system.
 *             - RISC-V support
 * \copyright  COPYRIGHT (C) AGSTU AB
 *
 *             All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise.
 *             This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering,
 *             decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent
 *             of AGSTU AB is obtained.
 *
 *             Disclaimer:
 *             All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to
 *             implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any
 *             damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of
 *             business information, or any other pecuniary loss) arising out of the use or inability to use information (including software,
 *             designs and files) provided on this site.
 */

#ifndef __SIERRA_H__
#define __SIERRA_H__

#include <sierra_io.h>
#include <sierra_info.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Max time base value is 10000.
#define SIERRA_MAX_TIMEBASE 10000

/*! \brief   Retrieve information about Sierra HW version.
 *  \details \par Description
 *           Sierra HW Version number can be retrieved from Sierra Hardware if you call sierra_HW_version function.
 *           - MAJOR version when you make incompatible changes.
 *           - MINOR version when you add functionality in a backwards-compatible manner.
 *           - PATCH version when you make backwards-compatible bug fixes or formating changes.
 *           - Number of semaphores and number of tasks.
 *  \return  version_register_union struct with version information.
 */
extern version_register_union sierra_HW_version(void);

/*! \brief   Retrieve information about Sierra SW version.
 *  \details \par Description
 *           Sierra SW Version number can be retrieved from sierra.c if you call sierra_SW_driver_version function.
 *           - MAJOR version when you make incompatible changes.
 *           - MINOR version when you add functionality in a backwards-compatible manner.
 *           - PATCH version when you make backwards-compatible bug fixes or formating changes.
 *  \return  sw_version_union struct with version information.
 */
extern sw_version_union sierra_SW_driver_version(void);

/*! \brief  Get the value of the Sierra time_base register.
 *  \return 32-bit integer with time base information.
 */
extern uint32_t sierra_time_base_reg(void);

/*! \brief Converts Sierra hardware tick counts to milliseconds.
 *  \param ticks Sierra time logging register value.
 *  \return Time in milliseconds based on the last sierra_set_timebase() call.
 */
extern uint32_t sierra_ticks_to_ms(uint32_t ticks);

/*! \brief   Sets the internal clock-tick timebase for the Sierra.
 *  \details \par Description
 *           Sets or read the internal clock-tick timebase for the Sierra. This register is used to set-up the
 *           generating of Sierra internal clock tick period for all timing queues in Sierra.
 *           \image rtf time_base.png "Time Base Unit"
 *           Sierra Time Base register value = Time tick (us) * system Frequency (MHz) / 1000
 *           \par Example
 *           How to calculate timebase value based on wanted tick-time:
 *           This example     : 100MHz system-clock
 *           Wanted tick time : 10us
 *           Formula gives    : 10us x 100MHz / 1000 => 1(dec) or 1(hex)
 *           --> sierra_set_timebase(0x1);
 *  \param   hex 32-bit integer with time base information.
 */
extern void sierra_set_timebase(uint32_t hex);

/*! \brief   Returns data from the time logging register.
 *  \details \par Description
 *           Reads the internal clock-tick register in the Sierra
 *  \return  32-bit integer with tick count information.
 */
extern uint32_t sierra_get_current_time(void);

/*! \brief   Initializes all TCB's and CPUctrl_reg, also reset sierra HW.
 *  \details \par Description
 *           Initiate the TCB in soft/hardware and also reset the Sierra hardware. This can always use to make a reset
 *           of SW drivers and HW based sierra. All tasks, etc. kills and also the task switch is disable, also the TCB
 *           will be cleared. After instantiation the task switch is off. This is done in less than 100 system clock for
 *           the standard Sierra IP.
 */
extern void sierra_initiation_HW_and_SW(void);

/*! \brief Enables scheduling and all interrupts.
 *  \see   sierra_tsw_off()
 */
extern void sierra_tsw_on(void);

/*! \brief   Disables scheduling and all interrupts.
 *  \details \par Description
 *           Disables task-switch interrupts in the system. This is useful when a critical section is entered. Anyhow,
 *           this call should be used with restrictions in a real time system as it has effects on how/when tasks can
 *           start to run. If this call is used, try to have the task-switch interrupt off as short time as possible.
 *  \par Example
 *  \code
 *  void t1(void)
 *  {
 *    ...
 *    sierra_tsw_off(); // Entering critical section - Turn off task-switch interrupts
 *    ...
 *    sierra_tsw_on();  // Leaving critical section - Turn on task-switch interrupts
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_tsw_off(void);

//! \brief Retrieves context data on next task (used when the Sierra interrupts for contextswitch).
extern void get_next_task(void);

//! \brief Retrieves context data on next tasks (used when contextswitch is performed manually).
extern void get_new_task(void);

/*! \brief  Handle Sierra service call and wait for ack.
 *  \param  pSVC Service call request data.
 *  \return statusA_union struct with result.
 */
extern statusA_union handle_service_call(const svc_t* pSVC);

#ifdef __cplusplus
}
#endif

#endif // __SIERRA_H__
