/*!
 * \file       sierra_task.c
 * \details    This file contains:
 *             - the Sierra service calls for task handling
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

#include <sierra_io.h>
#include <sierra_ker.h>
#include <sierra_regs.h>
#include <sierra_tcb.h>
#include <stdio.h>
#include <sierra_logging.h>



__attribute__((noreturn)) void task_exit_stub(void) {
  while (1);  // eller: alt_niosv_trigger_exit();
}



void sierra_create_task(const uint8_t taskID,
                        const int priority,
                        int taskstate,
                        void (*taskptr)(void),
                        void *stackptr,
                        int stacksz)
{

  tcb_t *newTask = &(TCB_LIST[taskID]);

  // === Initiera stack ===
  #define CONTEXT_SIZE_WORDS 30
  uint32_t *stack_top = (uint32_t *)stackptr + stacksz / sizeof(uint32_t) - CONTEXT_SIZE_WORDS;

  // Fyll trap-kontexten på stacken (samma ordning som i trap_vector)
  stack_top[0]  = 0;  // ra
  stack_top[1]  = 0;  // gp
  stack_top[2]  = 0;  // tp
  stack_top[3]  = 0;  // t0
  stack_top[4]  = 0;  // t1
  stack_top[5]  = 0;  // t2
  stack_top[6]  = (uint32_t) stack_top;  // s0 (fp)
  stack_top[7]  = 0;  // s1
  stack_top[8]  = 0;  // a0
  stack_top[9]  = 0;  // a1
  stack_top[10] = 0;  // a2
  stack_top[11] = 0;  // a3
  stack_top[12] = 0;  // a4
  stack_top[13] = 0;  // a5
  stack_top[14] = 0;  // a6
  stack_top[15] = 0;  // a7
  stack_top[16] = 0;  // s2
  stack_top[17] = 0;  // s3
  stack_top[18] = 0;  // s4
  stack_top[19] = 0;  // s5
  stack_top[20] = 0;  // s6
  stack_top[21] = 0;  // s7
  stack_top[22] = 0;  // s8
  stack_top[23] = 0;  // s9
  stack_top[24] = 0;  // s10
  stack_top[25] = 0;  // s11
  stack_top[26] = 0;  // t3
  stack_top[27] = 0;  // t4
  stack_top[28] = 0;  // t5
  stack_top[29] = 0;  // t6

  // === Fyll TCB ===
  newTask->sp_reg = stack_top;
  newTask->mepc_reg = taskptr;
  newTask->initialized = 0;

  newTask->taskID = taskID;
  newTask->priority = priority;

  // === Systemtjänst: registrera task ===
  svc_t svc;
  svc.task_create.type = sierra_task_create;
  svc.task_create.state = taskstate;
  svc.task_create.priority = priority;
  svc.task_create.taskID = taskID;
  handle_service_call(&svc);

  // === Loggning ===
  sierra_logging_full(info_sierra_task_create_w_p_s, sierra_get_current_time(), taskID, priority);
}




//----------------------------------------------------------------------------
task_info_t sierra_get_task_info(const uint8_t taskID)
{
  svc_t svc;
  svc.task_getinfo.type =  sierra_task_getinfo;
  svc.task_getinfo.taskID = taskID;
  const statusA_union status = handle_service_call(&svc);

  // Extract the info data from service call status
  task_info_t info;
  info.state_info = status.statusA_t.svc_return & 0b11;
  info.priority = status.statusA_t.svc_return >> 2;
  return info;
}

//----------------------------------------------------------------------------
void sierra_start_task(const uint8_t taskID)
{
  svc_t svc;
  svc.task_start.type =  sierra_task_start;
  svc.task_start.taskID = taskID;
  handle_service_call(&svc);

  // Logs data when a task is started and placed in a ready state
  sierra_logging_medium(info_sierra_task_started, sierra_get_current_time(), taskID);
}

//----------------------------------------------------------------------------
void sierra_block_task(const uint8_t taskID)
{
  // Logs data when the running task is placed in a blocked state
  sierra_logging_full(info_sierra_task_blocking, sierra_get_current_time(), RUNNING_TASKID, taskID);

  svc_t svc;
  svc.task_block.type =  sierra_task_block;
  svc.task_block.taskID = taskID;
  const statusA_union statusA = handle_service_call(&svc);

  // Check service call status to see if task becomes blocked
  const uint32_t retval = statusA.statusA_t.svc_return & 0x3f;
  if (0 != (retval & 0x1))
  {
    NEXT_TASKID = constant_task_mask & (retval >> 1);
    taskswitch; // perform manual context switch
  }
}

//----------------------------------------------------------------------------
void sierra_delete_task(void)
{
  // Logs data when a task is deleted
  sierra_logging_medium(info_sierra_task_deleted, sierra_get_current_time(), RUNNING_TASKID);

  svc_t svc;
  svc.wait_for_next_period.type =  sierra_task_delete;
  const statusA_union statusA = handle_service_call(&svc);

  NEXT_TASKID = constant_task_mask & statusA.statusA_t.svc_return;

  taskswitch; // perform manual context switch
}

//----------------------------------------------------------------------------
void sierra_change_task_prio(const uint8_t taskID, const int priority)
{

  svc_t svc;
  svc.task_start.type =  sierra_task_change_prio;
  svc.task_create.priority = priority;
  svc.task_start.taskID = taskID;
  handle_service_call(&svc);

  // Logs data when a task's priority is changed
  sierra_logging_full(info_sierra_task_change_prio, sierra_get_current_time(), taskID, priority);
}

//----------------------------------------------------------------------------
void sierra_yield_task(void)
{

  svc_t svc;
  svc.task_yield.type = sierra_task_yield;
  handle_service_call(&svc);

  statusB_union statusB;
  statusB.statusB_reg_integer = M_RD_Sierra_statusB_reg;

  NEXT_TASKID = constant_task_mask & statusB.statusB_t.running_taskID;

  // Logs data when a task yields
  sierra_logging_medium(info_sierra_task_yields, sierra_get_current_time(), RUNNING_TASKID);

  taskswitch; // perform manual context switch
}
