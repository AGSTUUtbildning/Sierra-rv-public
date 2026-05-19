/*!
 * \file       sierra_info.c
 * \details    Sierra information access functions
 * \version    11.00.00
 * \date       2026
 * \history    Modified 2026:
 *             - Removed sierra_backward_compatibility.h
 *             - Changed logging system.
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
#include <sierra_tcb.h>
#include <sierra.h>
#include <stdio.h>

//----------------------------------------------------------------------------
void sierra_print_versions(void)
{

  const sw_version_union info = sierra_SW_driver_version();
  const version_register_union test = sierra_HW_version();


  sierra_default_print("Sierra HW Version %d.%d.%d\n", test.version_register.MAJOR_version,test.version_register.MINOR_version,test.version_register.PATCH_version );
  sierra_default_print("Sierra SW Version %d.%d.%d\n", info.sw_version.MAJOR_SW, info.sw_version.MINOR_SW, info.sw_version.PATCH_SW);
}

//----------------------------------------------------------------------------
void sierra_printf_HW_version(void)
{ 
  const version_register_union test = sierra_HW_version();
  sierra_default_print("Version = %d.%d.%d\n", test.version_register.MAJOR_version, test.version_register.MINOR_version, test.version_register.PATCH_version);
  sierra_default_print("Number of tasks bits = %d\n", test.version_register.number_of_tasks);
  sierra_default_print("Number of semaphore bits = %d\n", test.version_register.number_of_semaphores);
}

//----------------------------------------------------------------------------
void sierra_task_info(void)
{
  task_info_t info;
  const char *task_state[] = {
  "Running",
  "Blocked",
  "Ready",
  "Dormant"
  };
  info = sierra_get_task_info(0);
  sierra_default_print("Idle\n");
  sierra_default_print("  info.state_info = %s\n", task_state[info.state_info]);
  sierra_default_print("  info.priority = %d\n", info.priority); 

  for (size_t i = 1; i < N_TASKS; ++i)
  {
    if (TCB_LIST[i].taskID != INVALID_TASK_ID)
    {
      sierra_default_print("Task %u\n", (unsigned int)i);
      info = sierra_get_task_info(i);
      sierra_default_print("  info.state_info = %s\n", task_state[info.state_info]);
      sierra_default_print("  info.priority = %d\n", info.priority);
    }
  }
}
