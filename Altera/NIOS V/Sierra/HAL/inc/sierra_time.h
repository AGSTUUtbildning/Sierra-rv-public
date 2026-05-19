/*!
 * \file       sierra_time.h
 * \details    This section describes the functionality of the time management controller. The following functions are
 *             implemented:
 *
 *             delay
 *             init_period_time
 *             wait_for_next_period delay
 *
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

#ifndef _SIERRA_TIME_H_
#define _SIERRA_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sierra_compatibility.h"

/*! \brief   Inititialize period time for current task.
 *  \details \par Description
 *           Initialize the period time for the calling task. This function must be performed before the use of the
 *           function sierra_await_next_period(). See the version of Sierra for the max value. Possible to use deadline
 *           control, to detect starvation.
 *  \param   per_time Specifies the period time, in number of ticks, for calling task.
 */
extern void sierra_period_time_init(int per_time);

/*! \brief   Let current task wait for next period.
 *  \details \par Description
 *           Suspends a periodic task until the start of next period time. If you miss a periodic start, Sierra will
 *           skip this period, not to disturbed the other tasks and also report the miss to the periodic task. The
 *           deadline is the same as the period time.
 *
 *           To use deadline control cost no extra execution or response time to manage.
 *           \image rtf deadline.png "Periodic start with deadline control"
 *  \return  task_periodic_start_union structure for deadline control
 *  \par Example
 *  \code
 *  void t1(void)
 *  {
 *    ...
 *    const task_periodic_start_union test = sierra_await_next_period();
 *    if (test.task_periodic_start_t.deadline_control)
 *      printf("deadline miss for T1");
 *    ...
 *  }
 *  \endcode
 */
extern task_periodic_start_union sierra_await_next_period(void);

/*! \brief   Delay current task.
 *  \details \par Description
 *                Blocks the calling task specified number of ticks. The task will be placed in the blocked state until
 *                the timer expires or an undelay call is performed on the task. When the timer expires, or if the
 *                undelay call is performed, the task is placed in the ready state.
 *  \param   delay_time Specifies the number of ticks to delay the task. Max value depend on the version of Sierra.
 *  \par Example
 *  \code
 *  void t1(void)
 *  {
 *    ...
 *    sierra_delay_task(10); // T1 is blocked for 10 ticks
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_delay_task(int delay_time);

#ifdef __cplusplus
}
#endif

#endif // _SIERRA_TIME_H_
