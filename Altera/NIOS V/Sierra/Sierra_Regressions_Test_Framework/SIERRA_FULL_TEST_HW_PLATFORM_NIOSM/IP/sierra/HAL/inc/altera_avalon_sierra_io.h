/*!
 * \file       altera_avalon_sierra_io.h
 * \details    This file contains:
 *             - defines used by service call functions
 *             - CPU dependent structures
 *             - task control block structure (tcb)
 *             - structures for service call communication over CPU bus
 *             Encoding of service calls ( 6 bits; 15 downto 10)
 * \author     Lennart Lindh
 * \version    9.4.1
 * \date       2006
 * \copyright  COPYRIGHT (C) 2006 - 2021 AGSTU AB
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

#ifndef __ALTERA_AVALON_SIERRA_IO_H__
#define __ALTERA_AVALON_SIERRA_IO_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

///@{
//! Task requests
#define sierra_task_create            0x0
#define sierra_task_delete            0x1
#define sierra_task_start             0x2
#define sierra_task_block             0x3
#define sierra_task_yield             0x4
#define sierra_task_getinfo           0x5
#define sierra_task_change_prio       0x6
///@}

///@{
//! IRQ handling
#define sierra_irq_init               0x8
#define sierra_irq_wait               0x9
#define sierra_irq_remove             0xA
#define sierra_irq_getinfo            0xB
#define sierra_irq_on_off             0xC
///@}

///@{
//! Task switch requests
#define sierra_delay                  0x10
#define sierra_init_period_time       0x11
#define sierra_read_timeq             0x12
#define sierra_undelay                0x13
#define sierra_wait_for_next_period   0x14
#define sierra_stop_period            0x15
#define sierra_restart_period         0x16
///@}

///@{
//! Flag handling
#define sierra_flag_wait              0x19
#define sierra_flag_set               0x1a
#define sierra_flag_clear             0x1b
///@}

///@{
//! Semaphore handling
#define sierra_sem_take               0x1c
#define sierra_sem_release            0x1d
#define sierra_sem_read               0x18
///@}

///@{
//! Register interface for communication CPU-Sierra - task states
#define BLOCKED_TASK_STATE  0
#define READY_TASK_STATE    1
///@}

//! Status register A
typedef union {
  struct {
    uint32_t svc_ack    :  1;
    uint32_t unused1    :  1;
    uint32_t svc_return : 14;
    uint32_t unused2    : 16;
  } statusA_t;
  uint32_t statusA_reg_integer;
} statusA_union;

//! Status register B
typedef union {
  struct {
    uint32_t tsw_req_flag   :  1;
    uint32_t running_taskID : 16; 
    uint32_t unused1        :  4;
    uint32_t unused2        : 11;
  } statusB_t;
  uint32_t statusB_reg_integer;
} statusB_union;

//! Control Register
typedef union {
  struct {
    uint32_t tsw_req_ack :  1;
    uint32_t tswoff_req  :  1;
    uint32_t unused1     : 14;
    uint32_t unused2     : 16;
  } ctrl_t;
  uint32_t ctrl_integer;
} ctrl_union;

/*! Version register
 *  |    31 - 28    |    27 - 24    |    23 - 20    | 19 - 16 |   15 - 8   | 7 - 0 |
 *  | ------------- | ------------- | ------------- | ------- | ---------- | ----- |
 *  | MAJOR_version | MINOR_version | PATCH_version |    X    | semaphores | tasks |
 */
typedef union {
  struct{
    uint32_t number_of_tasks      : 8;
    uint32_t number_of_semaphores : 8;
    uint32_t state_info           : 4;
    uint32_t PATCH_version        : 4;
    uint32_t MINOR_version        : 4;
    uint32_t MAJOR_version        : 4;
  } version_register;
  uint32_t version_register_int;
} version_register_union;

//! Task info
typedef struct {
  /*! state_info (2 bits):
   *   0 = Running
   *   1 = Blocked
   *   2 = Ready
   *   3 = Dormant
   */
  uint16_t state_info :  2;
  //! priority (3 bits, depend on the version of Sierra): 0 is the lowest priority level and 7 is the highest.
  uint16_t priority   : 14;
} task_info_t;

//! Task periodic start
typedef union {
  struct {
    uint32_t deadline_control :  1;
    uint32_t task_id          : 16;
    uint32_t unused           : 15;
  } task_periodic_start_t;
  uint32_t periodic_start_integer;
} task_periodic_start_union;

//! Semaphore info
typedef struct {
  uint32_t status  :  1;
  uint32_t semID   : 12;
  uint32_t unused  : 19;
} sem_info_t;

//! Service call argument type definitions for 32-bit register
typedef union {

  //! Task create request
  struct {
    uint32_t taskID   : 12;
    uint32_t priority : 12;
    uint32_t state    :  1;
    uint32_t unused   :  1;
    uint32_t type     :  6;
  } task_create;

  //! Task delete request
  struct {
    uint32_t unused : 26;
    uint32_t type   :  6;
  } task_delete;

  //! Task start request
  struct {
    uint32_t taskID : 12; 
    uint32_t unused : 14;
    uint32_t type   :  6;
  } task_start;

  //! Task block request
  struct {
    uint32_t taskID : 12; 
    uint32_t unused : 14;
    uint32_t type   :  6;
  } task_block;

  //! Task yield request
  struct {
    uint32_t unused : 26;
    uint32_t type   :  6;
  } task_yield;

  //! Change task priority request
  struct {
    uint32_t taskID   : 12;
    uint32_t priority : 12;
    uint32_t type     :  8;
  } task_change_prio;

  //! Task information request
  struct {
    uint32_t taskID : 12;
    uint32_t unused : 14;
    uint32_t type   :  6;
  } task_getinfo;

  //! Interrupt management
  struct {
    uint32_t irq_type :  2;
    uint32_t unused   : 24;
    uint32_t type     :  6;
  } irq_wait_t;

  //! Time management
  struct {
    uint32_t nroftick : 26;
    uint32_t type     :  6;
  } delay;

  //! Time managment
  struct {
    uint32_t taskID :  4; 
    uint32_t unused : 22;
    uint32_t type   :  6;
  } undelay;

  //! Time period change request
  struct {
    uint32_t period : 16; 
    uint32_t unused : 10;
    uint32_t type   :  6;
  } init_period_time;

  //! Time information request
  struct {
    uint32_t taskID :  4; 
    uint32_t unused : 22;
    uint32_t type   :  6;
  } read_timeq;

  //! Wait for next time period request
  struct {
    uint32_t unused : 26;
    uint32_t type   :  6;
  } wait_for_next_period;

  //! Request cancel of wait
  struct {
    uint32_t taskID :  4; 
    uint32_t unused : 22;
    uint32_t type   :  6;
  } stop_period;

  //! Request restart of time period
  struct {
    uint32_t taskID :  4; 
    uint32_t unused : 22;
    uint32_t type   :  6;
  } restart_period;

  //! Request increment of a semaphore count
  struct {
    uint32_t semid  : 12;
    uint32_t unused : 14;
    uint32_t type   :  6;
  } take_sem;


  //! Request decrement of a semphore count
  struct {
    uint32_t semid  : 12;
    uint32_t unused : 14;
    uint32_t type   :  6;
  } release_sem;

  //! Request information about semaphore a task is waiting for
  struct {
    uint32_t taskID : 12;
    uint32_t unused : 14;
    uint32_t type   :  6;
  } read_sem;


  //! Wait until flag has been set
  struct {
    uint32_t flag_mask : 12;
    uint32_t unused    : 14;
    uint32_t type      :  6;
  } flag_wait;


  //! Request flag to be set
  struct {
    uint32_t flag_mask : 12;
    uint32_t unused    : 14;
    uint32_t type      :  6;
  } flag_set;

  //! Request flag to be unset
  struct {
    uint32_t flag_mask : 12;
    uint32_t unused    : 14;
    uint32_t type      :  6;
  } flag_clear;

  uint32_t svc_input;
} svc_t;

#ifdef __cplusplus
}
#endif

#endif /* __ALTERA_AVALON_SIERRA_IO_H__ */

