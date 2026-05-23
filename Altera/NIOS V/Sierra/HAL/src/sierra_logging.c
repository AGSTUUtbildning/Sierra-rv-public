/*!
 * \file       sierra_logging.c
 * \details    This file contains:
 *             - the logging framework that retrieves data from each logging macro
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

/* Standard Include. */
#include <stdio.h>
#include <stdint.h>
#include <sierra.h>

/* Include header for logging subsystem. */
#include <sierra_logging.h>

#if SIERRA_LOGGING > 0

  sierra_func_ptr_print sierra_logg_defined_print = sierra_default_print;
  sierra_func_ptr_void sierra_logg_defined_timer = sierra_get_current_time;
  sierra_func_ptr_uint sierra_time_tick_converter = sierra_ticks_to_ms;

  // For every log_id there is a message.
  // .str -> Full string with format specifiers.
  // .nr_of_vars -> how many format specifiers for this message.
  // .aktive -> If 1 then it will be printed. If 0 it will be ignored.
  const message_list_struct message_list[log_count] = {
      [info_sierra_time_timebase_set]     = {.str = "info  -> SIERRA_TIME -> Timebase is set to %lu.\n",                             .nr_of_vars = 1, .aktive = 1}, 
      [warn_sierra_time_timebase_exc]     = {.str = "warn  -> SIERRA_TIME -> Timebase value %lu exceeds %d!\n",                      .nr_of_vars = 2, .aktive = 1}, 
      [info_sierra_task_preemted_task]    = {.str = "info  -> SIERRA_TASK -> Task %d preempted task %d.\n",                          .nr_of_vars = 2, .aktive = 1},
      [info_sierra_tsw_switching_on]      = {.str = "info  -> SIERRA_TSW  -> Task switching was turned on.\n",                       .nr_of_vars = 0, .aktive = 1},
      [info_sierra_tsw_switching_off]     = {.str = "info  -> SIERRA_TSW  -> Task switching was turned off.\n",                      .nr_of_vars = 0, .aktive = 1},
      [info_sierra_task_next_requested]   = {.str = "info  -> SIERRA_TASK -> Next task requested\n",                                 .nr_of_vars = 1, .aktive = 1},
      [info_sierra_irq_task_wait_irq]     = {.str = "info  -> SIERRA_IRQ  -> Task %d will now wait for IRQ %d.\n",                   .nr_of_vars = 2, .aktive = 1},
      [info_sierra_svc_task_wait_sem]     = {.str = "info  -> SIERRA_SVC  -> Task %d waits for semaphore %d.\n",                     .nr_of_vars = 2, .aktive = 1},
      [info_sierra_svc_task_take_sem]     = {.str = "info  -> SIERRA_SVC  -> Task %d takes semaphore %d.\n",                         .nr_of_vars = 2, .aktive = 1},
      [info_sierra_svc_task_releas_sem]   = {.str = "info  -> SIERRA_SVC  -> Task %d releasing semaphore %d.\n",                     .nr_of_vars = 2, .aktive = 1},
      [info_sierra_svc_wait_flag_set]     = {.str = "info  -> SIERRA_SVC  -> Task %d waits for flag(s) 0x%04X to be set.\n",         .nr_of_vars = 2, .aktive = 1},
      [info_sierra_svc_task_set_flag]     = {.str = "info  -> SIERRA_SVC  -> Task %d setting flag(s) 0x%04X.\n",                     .nr_of_vars = 2, .aktive = 1},
      [info_sierra_svc_task_clear_flag]   = {.str = "info  -> SIERRA_SVC  -> Task %d clearing flag(s) 0x%04X.\n",                    .nr_of_vars = 2, .aktive = 1},
      [info_sierra_task_create_w_p_s]     = {.str = "info  -> SIERRA_TASK -> Task %d was created with priority %d.\n",               .nr_of_vars = 2, .aktive = 1},
      [info_sierra_task_started]          = {.str = "info  -> SIERRA_TASK -> Task %d was started.\n",                                .nr_of_vars = 1, .aktive = 1},
      [info_sierra_task_blocking]         = {.str = "info  -> SIERRA_TASK -> Task %d blocking task %d.\n",                           .nr_of_vars = 2, .aktive = 1},
      [info_sierra_task_deleted]          = {.str = "info  -> SIERRA_TASK -> Task %d is being deleted.\n",                           .nr_of_vars = 1, .aktive = 1},
      [info_sierra_task_change_prio]      = {.str = "info  -> SIERRA_TASK -> Task %d changed priority to %d.\n",                     .nr_of_vars = 2, .aktive = 1},
      [info_sierra_task_yields]           = {.str = "info  -> SIERRA_TASK -> Task %d yields.\n",                                     .nr_of_vars = 1, .aktive = 1},
      [info_sierra_time_period]           = {.str = "info  -> SIERRA_TIME -> Task %d has time period %d.\n",                         .nr_of_vars = 2, .aktive = 1},
      [info_sierra_time_suspended]        = {.str = "info  -> SIERRA_TIME -> Task %d is suspended until next time period.\n",        .nr_of_vars = 1, .aktive = 1},
      [info_sierra_time_delay]            = {.str = "info  -> SIERRA_TIME -> Task %d will be delayed with %d ticks.\n",              .nr_of_vars = 2, .aktive = 1},
  };

  #if SIERRA_LOGGING == 3 // This mode is for advance use.

  

  // Data structure for logging message.
  typedef struct {
      uint32_t time_stamp;
      uint32_t message_code;
      uint32_t var1;
      uint32_t var2;
  } log_data;

  // Compiler helping definitions for log_memory. Fill memory size with datastructures and entries.
  #define HEADER_AND_PADDING   ((2 * sizeof(uint32_t) + (LOG_ALIGNMENT - 1)) & ~(LOG_ALIGNMENT - 1))
  #define NR_LOG_ENTRY            ((LOG_MEMORY_SIZE - HEADER_AND_PADDING) / sizeof(log_data))

  // Data structure for a cirkular memmory buffer.
  // This wil fill a 8kb memory.
  typedef struct {
      uint32_t head; // Last index in the buffer.
      uint32_t tail; // First index in the buffer.

      log_data log_entry[NR_LOG_ENTRY]; // max 511 of messages in the bufferts.
  } log_memory;

  // log_memory Is a separate on-chip memory or a memory region named log_memory.
  log_memory sierra_log __attribute__((section(logging_3_memmory_name)));

  // Save_log data into log_memory.
  static void save_log(uint32_t message, uint32_t time, uint32_t var1, uint32_t var2) {
      sierra_log.log_entry[sierra_log.head].message_code = message;
      sierra_log.log_entry[sierra_log.head].time_stamp   = time;
      sierra_log.log_entry[sierra_log.head].var1         = var1;
      sierra_log.log_entry[sierra_log.head].var2         = var2;

      sierra_log.head = (sierra_log.head + 1) & NR_LOG_ENTRY; // Uppdatera head
  }

  // Write all buffered messages into log_port.
  static void read_log(void) {
      sierra_logg_defined_print("\nPrint log:\n");
      while(sierra_log.tail != sierra_log.head) {
          uint32_t index = sierra_log.tail;
          if (message_list[sierra_log.log_entry[index].message_code].aktive == 1){
            sierra_logg_defined_print("Time: %lu ms : ", (unsigned long)sierra_log.log_entry[index].time_stamp);
            uint32_t nr_vars = message_list[sierra_log.log_entry[index].message_code].nr_of_vars;
            if(nr_vars == 0)
              sierra_logg_defined_print(message_list[sierra_log.log_entry[index].message_code].str);
            else if(nr_vars == 1)
              sierra_logg_defined_print(message_list[sierra_log.log_entry[index].message_code].str, sierra_log.log_entry[index].var1);
            else if(nr_vars == 2)
              sierra_logg_defined_print(message_list[sierra_log.log_entry[index].message_code].str, sierra_log.log_entry[index].var1, sierra_log.log_entry[index].var2);
          }
          sierra_log.tail = (sierra_log.tail + 1) & NR_LOG_ENTRY; // Uppdatera tail. 
      }
  }
  #endif

  //----------------------------------------------------------------------------

  // Log message depending on SIERRA_LOGGING mode.
  void sierra_logging(uint32_t message, uint32_t var1, uint32_t var2)
  {
  #if SIERRA_LOGGING == 3 // Do not use. In development.
    save_log(message, sierra_time_tick_converter( sierra_logg_defined_timer() ), var1, var2); // Save inte logging buffert.

  #elif SIERRA_LOGGING > 0 // When logging is on.
    #if SIERRA_LOGGING > 1 // Timestamps on
      sierra_logg_defined_print("Time: %lu ms : ", (unsigned long)sierra_time_tick_converter( sierra_logg_defined_timer() ));
    #endif
    
    if(message_list[message].nr_of_vars == 0)
      sierra_logg_defined_print("%s", message_list[message].str);
    else if(message_list[message].nr_of_vars == 1)
      sierra_logg_defined_print(message_list[message].str, var1);
    else if(message_list[message].nr_of_vars == 2)
      sierra_logg_defined_print(message_list[message].str, var1, var2);

  #else
    ;
  #endif
  }

  // If there are logg data in the buffert. Print it all out. 
  void sierra_print_log(void){
    #if SIERRA_LOGGING > 2
      read_log();
    #else
      ;
    #endif
  }
#endif
