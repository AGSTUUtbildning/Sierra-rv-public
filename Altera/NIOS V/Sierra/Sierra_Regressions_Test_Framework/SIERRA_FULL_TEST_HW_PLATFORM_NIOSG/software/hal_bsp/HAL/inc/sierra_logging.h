/*!
 * \file       sierra_logging.h
 * \details    This file contains:
 *             - logging macros and functions for the Sierra logging interface.
 *             - the user can choose logging level by enabling the desired logging macro.
 *             - the maximum number of characters for each level is 100.
 * \version    1.0
 * \date       2026
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

#ifndef __SIERRA_LOGGING_H__
#define __SIERRA_LOGGING_H__

#include <stdio.h>


// Sierra logging = 0 är att logging är avstänkt. Logg funktionerna i koden optimeras bort. 
// Sierra logging = 1 är inte rekomenderat. Med vanlig printf men jtag UART blir väldigt lätt överbelastad. Rekomenderat att använda pooling till jtag UART.
// Sierra logging = 2 är Rekomenderat använder printf gjord för logging och skriver till jtag UART som angivits i BSP för logging. Kan också överbelasta jtag UART bufferten.
// sierra logging = 3 special logging som kräver sierra test platform. Mindre belastning och sparar logg tillfälligt i ett separat minne. 

#define SIERRA_LOGGING 3

#if SIERRA_LOGGING > 0

  // Struct for mlogging message list.
  typedef struct {
      const char* str;
      uint8_t nr_of_vars;
      uint8_t aktive;
  } message_list_struct;

  // List of possible logging messages.
  // When adding a new possible message add its id here.
  typedef enum {
      info_sierra_time_timebase_set = 0,
      warn_sierra_time_timebase_exc,
      info_sierra_task_preemted_task,
      info_sierra_tsw_switching_on,
      info_sierra_tsw_switching_off,
      info_sierra_task_next_requested,
      info_sierra_irq_task_wait_irq,
      info_sierra_svc_task_wait_sem,
      info_sierra_svc_task_take_sem,
      info_sierra_svc_task_releas_sem,
      info_sierra_svc_wait_flag_set,
      info_sierra_svc_task_set_flag,
      info_sierra_svc_task_clear_flag,
      info_sierra_task_create_w_p_s,
      info_sierra_task_started,
      info_sierra_task_blocking,
      info_sierra_task_deleted,
      info_sierra_task_change_prio,
      info_sierra_task_yields,
      info_sierra_time_period,
      info_sierra_time_suspended,
      info_sierra_time_delay,

      log_count // Number of logging messages.
  }log_id;

  // Hardcoded list of messages.
  extern const message_list_struct message_list[]; // prototype see sierra_logging.c

  /*! \brief   Logging funktion. Diffrent logic depending on SIERRA_LOGGING flag.
  *  \details \par Description
  *           SIERRA_LOGGING -> 0: This function is optimized away.
  *           SIERRA_LOGGING -> 1: Will use printf to print the logging message into jtag UART.
  *           SIERRA_LOGGING -> 2: Will use ALT_LOG_PRINTF. Need to set log_port in BSP To work properly.
  *           SIERRA_LOGGING -> 3: Save logg information in a separate memory to be printed out later. 
  *                                Reduce load of logging and cthe logg will be crach protected. 
  *                                Logg need to be printed out later. Max 511 instances. 
  */
  void sierra_logging(uint32_t message, uint32_t time, uint32_t var1, uint32_t var2);

  void sierra_print_log(void);

  #else // If SIERRA_LOGGING is 0. Out optimize sierra_logging and sierra_print_log.
    #define sierra_logging(message, time, var1, var2) ((void) 0)
    #define sierra_print_log() ((void) 0)
  #endif
  
  // Sierra_logging but with no extra vars.
  #define sierra_logging_short(message, time)            (sierra_logging(message, time, 0, 0))
  
  // Sierra_logging with one extra var.
  #define sierra_logging_medium(message, time, var1)     (sierra_logging(message, time, var1, 0))
  
  // Sierra_logging full with two vars.
  #define sierra_logging_full(message, time, var1, var2) (sierra_logging(message, time, var1, var2))

#endif /* __SIERRA_LOGGING_H__ */
