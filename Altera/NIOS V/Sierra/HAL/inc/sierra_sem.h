/*!
 * \file       sierra_sem.h
 * \details    This section describes the functionality of the semaphore management. The semaphores are used in the system to protect
 *             shared resources and for synchronization of different tasks.
 *
 *             There are 8 binary semaphores available in the Sierra. A semaphore can have a queue of waiting tasks that is as long as
 *             there are tasks in the system. This means that a semaphore can be taken by one task and up to 8 other tasks can be waiting
 *             for it. The queue is arranged by task-id numbers. Task with highest id-number in the queue will run when the semaphore
 *             becomes available.
 *
 *             The Sierra has support for flags for efficient synchronizing of events. The entire synchronizing algorithm is handled by
 *             the hardware kernel. This makes handling of flags very efficient since no valuable CPU time is spent on synchronization.
 *
 *             Flags are very efficient in cases where you, for example, have one or several events handled by some input tasks and there
 *             exist an output task triggered by one or several tasks - see figure below.
 *
 *             \image rtf flags.png "Flag example"
 *
 *             The semantics for the figure is; the output task makes a system call where it will need a combination of flags set to be
 *             able to continue to run. If this combination is not true at the time when the call is performed, the task will be suspended
 *             until the combination becomes true. Later on, task1 runs and sets flag1. In this scenario the output task will not be made
 *             ready at this point, as it asks for an AND operation between flag1 and flag2. After task2 has set flag2, the output task
 *             will be made ready. The output task is scheduled and will start to run when it has the highest priority in the ready queue.
 *
 *             If the Sierra is configured to support 4 flag bits, the flag bits can be used in 2^4-1 (=15) different combinations. 
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

#ifndef __SIERRA_SEM_H__
#define __SIERRA_SEM_H__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------
    Binary semaphore handling
-------------------------------------------------------------------------*/

/*! \brief   Take a semphore, task will become blocked if the semaphore is currently unavailable.
 *  \details \par Description
 *           Makes a task pending (waiting) for a semaphore. If the semaphore is free, the task will continue to execute
 *           immediately. If the semaphore is allocated by another task, the calling task will be suspended and put in a
 *           semaphore waiting queue, until the semaphore becomes free
 *  \note    The queue is arranged in task-id numbers and task with highest id-number in the queue will get the
 *           semaphore when it becomes available.
 *  \param   semid Semaphore ID number.
 *  \par Example
 *  \code
 *  #define SEM1 1
 *  void t1(void)
 *  {
 *    ...
 *    sierra_take_sem(SEM1); // Wait on semaphore 1
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_take_sem(int semID);

/*! \brief   Release a semaphore, tasks waiting for it may now become unblocked.
 *  \details \par Description
 *           Releases a specified semaphore. If there are one or more tasks waiting for the semaphore, the first task in
 *           the semaphore waiting queue will get the semaphore and will be moved to ready state.
 *  \param   semid Semaphore ID number.
 *  \par Example
 *  \code
 *  #define SEM1 1
 *  void t1(void)
 *  {
 *    ...
 *    sierra_release_sem(SEM1); // Release semaphore 1
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_release_sem(int semID);

/*! \brief   Find out what sempahore, if any, a task is waiting for.
 *  \return  sem_info_t structure containing the result from the read request.
 *  \param   taskID Task ID number.
 *  \par Example
 *  \code
 *  void t1(void)
 *  {
 *    ...
 *    const sem_info_t sem = sierra_read_sem(T2); // Read semaphore status of task T2
 *    const int status = sem.status; 
 *    const int semID = sem.semID;
 *    ...
 *  }
 *  \endcode
 */
extern sem_info_t sierra_read_sem(const uint8_t taskID);

/*------------------------------------------------------------------------
    Flags handling
-------------------------------------------------------------------------*/

/*! \brief   Request current task to wait for one or more flags, if any of the flags are currenly unset.
 *  \details \par Description
 *           This call makes a task wait for one or more flags to be set. If the flag(s) are already set, the task will
 *           continue to run. Otherwise it will be suspended until the combination is set.
 *  \param   flag_mask One flagbit, or a combination of flagbits. The four lowest bits are used i.e. values between 1-15
 *           are valid. 0 is not a valid flag value. 
 *  \par Example
 *  \code
 *  #define FLAG_MASK 5 // Flag1 AND Flag3 -> 0101
 *  void t1(void)
 *  {
 *    ...
 *    sierra_await_flag(FLAG_MASK); // Wait for Flag1 and Flag3 to be set
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_await_flag(int flag_mask);

/*! \brief   Set one or more flags, releasing all tasks that may have been waiting for them.
 *  \details \par Description
 *           This call sets one or more flags. If there are any task(s) waiting for the specific combination of flags
 *           that are set during the call, they will be made ready and start to run when they have the highest priority
 *           in the ready queue.
 *           If a task is waiting for a combination of flags and the call only sets one or few of the flags, the waiting
 *           task will not be activated before all flags are set.
 *  \param   flag_mask One flagbit, or a combination of flagbits. The four lowest bits are used i.e. values between 1-15
 *           are valid. 0 is not a valid flag value. 
 *  \par Example
 *  \code 
 *  #define FLAG_MASK 7 // Flag1 AND Flag2 AND Flag3 -> 0111
 *  void t1(void)
 *  {
 *    ...
 *    sierra_set_flag(FLAG_MASK); // Set Flag1, Flag2 and Flag3
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_set_flag(int flag_mask);

/*! \brief   Unset one or more flags.
 *  \details \par Description
 *           This call clears one or more flags. When a flag has been set, it needs to be cleared after a waiting task
 *           has taken care of the event that was waiting for the flag. If there is more than one task using the flag,
 *           it is important to know which one(s) of these tasks that will be permitted to do this call.
 *           Example; there are two tasks waiting for a common flag, but one of the tasks is also waiting for another
 *           flag. When this flag is set, the task that only waits for this flag is made ready and will start to run
 *           when it has the highest priority in the ready queue. However, if the other task still is waiting for the
 *           other flag when this first task has done its job, this first task should not clear the flag as the other 
 *           task still is depending on this flag. In this specific scenario it is the task that is waiting for both
 *           flags that should clear the flag.
 *  \param   flag_mask One flagbit, or a combination of flagbits. The four lowest bits are used i.e. values between 1-15
 *           are valid. 0 is not a valid flag value. 
 *  \par Example
 *  \code
 *  #define FLAG_MASK 7 // Flag1 AND Flag2 AND Flag3 -> 0111
 *  void t1(void)
 *  {
 *    ...
 *    sierra_clear_flag(FLAG_MASK); // Clear Flag1, Flag2 and Flag3
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_clear_flag(int flag_mask);

#ifdef __cplusplus
}
#endif

#endif // __SIERRA_SEM_H__
