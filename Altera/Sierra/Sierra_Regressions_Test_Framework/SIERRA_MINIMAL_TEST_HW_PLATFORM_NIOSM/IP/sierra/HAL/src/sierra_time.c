/*!
 * \file       sierra_time.c
 * \details    This file contains:
 *             - the sierra service calls for time handling
 * \author     Lennart Lindh
 * \version    10.03.15
 * \date       2006
 * \history    Modified 2022:
 * \           - Added sierra_ prefix for external functions.
 *             - Added probes for the logging interface
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

#include <sierra_io.h>
#include <sierra_ker.h>
#include <sierra_logging.h>

/*!-----------------------------------------------------------------------
  Time management
-------------------------------------------------------------------------*/

//----------------------------------------------------------------------------
void sierra_period_time_init(int per_time)
{
  svc_t svc;
  svc.init_period_time.type =  sierra_init_period_time;
  svc.init_period_time.period =  per_time;
  handle_service_call(&svc);

  // Logs data when a designated time period is set for a particular task
  sierra_logging_full(info_sierra_time_period, sierra_get_current_time(), RUNNING_TASKID, per_time);
}

//----------------------------------------------------------------------------
task_periodic_start_union sierra_await_next_period(void)
{
  // Logs data when a task is suspended until the start of the next time period
  sierra_logging_medium(info_sierra_time_suspended, sierra_get_current_time(), RUNNING_TASKID);

  // Silently disable task switching
  extern void sierra_tsw_off_internal(void);
  sierra_tsw_off_internal();

  svc_t svc;
  svc.wait_for_next_period.type =  sierra_wait_for_next_period;
  const statusA_union statusA = handle_service_call(&svc);
 
  // Get next task ID
  task_periodic_start_union info;
  info.periodic_start_integer = statusA.statusA_t.svc_return;
  NEXT_TASKID = constant_task_mask & info.task_periodic_start_t.task_id;

  // Silently re-enable task switching
  extern void sierra_tsw_on_internal(void);
  sierra_tsw_on_internal();

  taskswitch; // perform manual contextswitch



  // Deadline control result, can be removed if not used by project
  return info;
}

//----------------------------------------------------------------------------
void sierra_delay_task(int delay_time)
{
  svc_t svc;
  svc.delay.type =  sierra_delay;
  svc.delay.nroftick =  delay_time;
  const statusA_union statusA = handle_service_call(&svc);

  // Get next task ID
  NEXT_TASKID = constant_task_mask & statusA.statusA_t.svc_return;

  // Logs data when a task is delayed with a specified number of ticks
  sierra_logging_full(info_sierra_time_delay, sierra_get_current_time(), NEXT_TASKID, delay_time);

  taskswitch; // perform manual contextswitch
}
