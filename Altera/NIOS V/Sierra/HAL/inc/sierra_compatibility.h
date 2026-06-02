/*!
 * \file       sierra_backward_compatibility.h
 * \details    This file contains:
 *             - Mapping functions names of previous version for backward compatibility.
 *             - New names written for Sierras internal functions and commented out, if decided eligible in future development.
 * \version    10.03.15
 * \date       2022
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
#ifndef DRIVERS_INC_SIERRA_BACKWARD_COMPATIBILITY_H_
#define DRIVERS_INC_SIERRA_BACKWARD_COMPATIBILITY_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*sierra_func_ptr_print)(const char *, ...);
typedef uint32_t (*sierra_func_ptr_void)(void);
typedef uint32_t (*sierra_func_ptr_uint)(uint32_t);

// Om Systemet är Microblaze V
#if defined(__riscv) && defined(__MICROBLAZE__) 

    #include <xil_printf.h>
    static const sierra_func_ptr_print sierra_default_print = xil_printf;

// Om systemet är ett risc-v är det förmodligen en nios v
#elif defined(__riscv) 

#include <stdio.h>
    static const sierra_func_ptr_print sierra_default_print = printf;

#else
// Other...
#endif

//! Mapping of functions from sierra_info.h with the prefix sierra_
#define Printf_sierra_HW_version(void) sierra_printf_HW_version(void)
#define task_info(void)                sierra_task_info(void)

//! Mapping of functions from sierra_sem.h with the prefix sierra_
#define sem_take(semID)       sierra_take_sem(semID)
#define sem_release(semID)    sierra_release_sem(semID)
#define sem_read(taskID)      sierra_read_sem(taskID) //Not tested.
#define flag_wait(flag_mask)  sierra_await_flag(flag_mask)
#define flag_set(flag_mask)   sierra_set_flag(flag_mask)
#define flag_clear(flag_mask) sierra_clear_flag(flag_mask)

//! Mapping of functions from sierra_tasks.h with the prefix sierra_
#define task_create(taskID, priority, tasktate,taskptr, stackptr, stacksz) sierra_create_task(taskID, priority, tasktate, taskptr, stackptr, stacksz)
#define task_getinfo(taskID)               sierra_get_task_info(taskID)
#define task_start(taskID)                 sierra_start_task(taskID)
#define task_block(taskID)                 sierra_block_task(taskID)
#define task_delete(void)                  sierra_delete_task(void)
#define task_change_prio(taskID, priority) sierra_change_task_prio(taskID, priority)
#define task_yield(void)                   sierra_yield_task(void)

//! Mapping of functions from sierra_time.h with the prefix sierra_
#define init_period_time(per_time) sierra_period_time_init(per_time)
#define wait_for_next_period(void) sierra_await_next_period(void)
#define delay(delay_time)          sierra_delay_task(delay_time)

//! Mapping of functions from sierra.h with the prefix sierra_
#define SierraTime_base_reg(void)         sierra_time_base_reg(void)
#define set_timebase(hex)                 sierra_set_timebase(hex)
#define Sierra_Initiation_HW_and_SW(void) sierra_initiation_HW_and_SW(void)
#define tsw_on(void)                      sierra_tsw_on(void)
#define tsw_off(void)                     sierra_tsw_off(void)
// *Functions for internal use of Sierra*
//#define get_next_task(void)               sierra_get_next_task(void)
//#define get_new_task(void)                sierra_get_new_task(void)
//#define handle_service_call(pSVC)         sierra_handle_service_call(pSVC)

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_INC_SIERRA_BACKWARD_COMPATIBILITY_H_ */
