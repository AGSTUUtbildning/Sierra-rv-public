/*!
 * \file       sierra_task.h
 * \details    This section describes the task handling services provided by the scheduler in the Sierra. The difference
 *             between Sierra and other RTOS kernels is that all scheduling is performed by a hardware piece instead of
 *             software. The only software is the driver that communicates with the hardware kernel. The following task
 *             management functions are implemented in the Sierra hardware kernel:
 *
 *             Dynamic creation of tasks (task_create)
 *             Starting of tasks (task_block)
 *             Yield (task_yield)
 *             Get task status (task_getinfo)
 *             Task switch off and on (tsw_on and tsw_off)
 *             Change task priority
 *
 *             Ready queue is organized in two ways (scheduling algorithm):
 *             - Priority driven (lowest priority is 0)
 *             - Same priority is sorted in ID number order, from low to high.
 *             - Preemption
 *
 *             Idle task must be created with task id 0 and lowest priority (0).
 *
 * \author     Lennart Lindh
 * \version    10.03.15
 * \date       2006
 * \history    Modified 2022:
 *             - Added sierra_ prefix for external functions
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

#ifndef __SIERRA_TASK_H__
#define __SIERRA_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif
 
/*! \brief   Create a new task and initialize its TCB.
 *  \details \par Description
 *           Creates a task with a unique task id. The task will be initialized to a state (blocked or ready) as
 *           specified in the argument. It is possible to create new tasks dynamically during system execution. Idle
 *           task has to be created and have task id 0 and lowest priority (0).
 *  \param   taskID Specifies the ID of the task (range depend on the version of Sierra). An idle task must be created
 *                  and this task must have taskID = 0.
 *  \param   priority Specifies the priority of the task. The range is dependent on the version), where 0 is the
 *                    lowest-priority level. Lowest ID number (0) is reserved only for the idle task.
 *  \param   taskstate 0 = task is initialized to the blocked state (BLOCKED_TASK_STATE),
 *                     1 = task is initialized to the ready state (READY_TASK_STATE).
 *  \param   taskptr Pointer to code start for the task.
 *  \param   stackptr Pointer to beginning of task's stack area.
 *  \param   stacksz Size of stack area (in bytes).
 *  \par Example
 *  \code
 *  #define T1 1
 *  #define READY 1
 *  #define PRIO1 1
 *  #define STACK1_SZ 200
 *  char stack1[STACK1_SZ];
 *
 *  void t1(void)
 *  {
 *    // task-specific code
 *  }
 *
 *  void init(void)
 *  {
 *    ...
 *    sierra_create_task(T1, PRIO1, READY, t1, stack1, STACK1_SZ);
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_create_task(const uint8_t taskID, const int priority, int tasktate, void (*taskptr)(void), void *stackptr, int stacksz);

/*! \brief  Retrieves information of a task.
 *  \param  taskID Specifies the ID of the task (range depend on the version of Sierra)
 *  \return Datas tructure of type task_info_t.
 *  \see    task_info_t
 *  \par Example
 *  \code
 *  printf("Idle\n");
 *  const task_info_t info = sierra_get_task_info(IDLE);
 *  printf(" info.state_info = %d\n", info.state_info);
 *  printf(" info.priority = %d\n", info.priority);
 *  \endcode 
 */
extern task_info_t sierra_get_task_info(const uint8_t taskID);

/*! \brief   Sets state of task to READY.
 *  \details \par Description
 *           Starts a task that is currently placed in blocked state ("un block" the task). Starting a task means that the
 *           task is sent into the ready state (see section 2.4., Scheduler) and does not mean that the task starts to
 *           execute immediately. The task will be moved from blocked state to ready state. 
 *  \param   taskID Specifies the ID of the task (range depend on the version of Sierra).
 *  \par Example
 *  \code
 *  void t1(void)
 *  {
 *    ...
 *    sierra_start_task(T2); // T1 starts T2
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_start_task(const uint8_t taskID);

/*! \brief Sets state of task to BLOCKED.
 *  \details \par Description
 *           Blocks the currently running task. The task will be moved from running state into blocked state. It is not
 *           allowed to block idle task.
 *  \param   taskID Specifies the ID of the task (range depend on the version of Sierra).
 *  \par Example
 *  \code
 *  void t1(void)
 *  {
 *    ...
 *    if (++i == 10)
 *    {
 *      sierra_block_task(T2); // T1 blocks T2 every 10th update
 *      i = 0;
 *    }
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_block_task(const uint8_t taskID);

/*! \brief Move running task to dormant and it has to be created again.
 *  \details \par Description
 *           Delete the currently running task. The task will be moved from the system and the task id number will be
 *           free to be used again. Most be created again to start. It is not allowed to perform this call from the
 *           idle task.
 *  \par Example
 *  \code
 *  void t1(void)
 *  {
 *    ...
 *    if (++i == 10)
 *    {
 *      sierra_delete_task(); // Remove T1 from system after 10 iterations
 *    }
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_delete_task(void);

/*! \brief Change priority on a task.
 *  \details \par Description
 *           This call changes a task's priority to a specified priority. It is not allowed to change idle task
 *           priority.
 *  \param   taskID Specifies the ID of the task (range depend on the version of Sierra).
 *  \param   priority Specifies the priority of the task. The range is dependent on the version), where 0 is the lowest
 *           priority level. Lowest priority is reserved for the idle task.
 *  \par Example
 *  \code
 *  void t1(void)
 *  {
 *    ...
 *    sierra_change_task_prio(T2, 5); // T1 changes T2's priority to 5
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_change_task_prio(const uint8_t taskID, const int priority);

//! \brief Yields to a task of same priority, if one exists. (NOTE: not fully tested yet!)
extern void sierra_yield_task(void);

#ifdef __cplusplus
}
#endif

#endif // __SIERRA_TASK_H__
