/*!
 * \file       sierra_sem.c
 * \details    This file contains:
 *             - Semaphore handling
 *             - Flag handling
 * \author     Lennart Lindh
 * \version    10.03.15
 * \date       2006
 * \history    Modified 2022:
 *             - Added sierra_ prefix for external functions.
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
 
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_ker.h>
#include <stdio.h>
#include <sierra_logging.h>

#include "sierra_sem.h"

/*!-----------------------------------------------------------------------
  Semaphore management
-------------------------------------------------------------------------*/

//----------------------------------------------------------------------------
void sierra_take_sem(int semID)
{
  svc_t svc;
  svc.take_sem.type = sierra_sem_take;
  svc.take_sem.semid = semID;
  const statusA_union status = handle_service_call(&svc);

  // Check service call status to see if task becomes blocked
  const uint32_t retval = status.statusA_t.svc_return & 0x3f;
  if (0 != (retval & 0x1))
  {
  	sierra_logging_full(info_sierra_svc_task_wait_sem, sierra_get_current_time(), RUNNING_TASKID, semID);
    NEXT_TASKID = constant_task_mask & (retval >> 1);
    taskswitch; // perform manual contextswitch
  }

  sierra_logging_full(info_sierra_svc_task_take_sem, sierra_get_current_time(), RUNNING_TASKID, semID);
}

//----------------------------------------------------------------------------
void sierra_release_sem(int semID)
{
  sierra_logging_full(info_sierra_svc_task_releas_sem, sierra_get_current_time(), RUNNING_TASKID, semID);

  svc_t svc;
  svc.release_sem.type = sierra_sem_release;
  svc.release_sem.semid = semID;
  handle_service_call(&svc);
}

//----------------------------------------------------------------------------
sem_info_t sierra_read_sem(const uint8_t taskID)
{
  svc_t svc;
  svc.read_sem.type = sierra_sem_read;
  svc.read_sem.taskID = taskID;
  const statusA_union status = handle_service_call(&svc);

  // Extract the info data from service call status
  sem_info_t info;
  info.status = status.statusA_t.svc_return & 0x1;
  info.semID = status.statusA_t.svc_return >> 1;
  return info;
}

/*!-----------------------------------------------------------------------
  Flags
-------------------------------------------------------------------------*/

//----------------------------------------------------------------------------
void sierra_await_flag(int flag_mask)
{
  svc_t svc;
  svc.flag_wait.type = sierra_flag_wait;
  svc.flag_wait.flag_mask = flag_mask;
  const statusA_union status = handle_service_call(&svc);

  // Check service call status to see if task becomes blocked
  const uint32_t retval = status.statusA_t.svc_return & 0x3f;
  if (0 != (retval & 0x1))
  {
    sierra_logging_full(info_sierra_svc_wait_flag_set, sierra_get_current_time(), RUNNING_TASKID, flag_mask);
    NEXT_TASKID = constant_task_mask & (retval >> 1);
    taskswitch; // perform manual contextswitch
  }
}

//----------------------------------------------------------------------------
void sierra_set_flag(int flag_mask)
{
  sierra_logging_full(info_sierra_svc_task_set_flag, sierra_get_current_time(), RUNNING_TASKID, flag_mask);

  svc_t svc;
  svc.flag_set.type = sierra_flag_set;
  svc.flag_set.flag_mask = flag_mask;
  handle_service_call(&svc);
}

//----------------------------------------------------------------------------
void sierra_clear_flag(int flag_mask)
{
  sierra_logging_full(info_sierra_svc_task_clear_flag, sierra_get_current_time(), RUNNING_TASKID, flag_mask);

  svc_t svc;
  svc.flag_clear.type = sierra_flag_clear;
  svc.flag_clear.flag_mask = flag_mask;
  handle_service_call(&svc);
}
