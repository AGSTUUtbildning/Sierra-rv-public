/*!
 * \file       sierra.c
 * \details    This file contains:
 *             - the Sierra service calls
 *             - Sierra registers
 *             - Sierra content data
 *             - Sierra SW version number
 * \author     Lennart Lindh
 * \version    10.03.15
 * \date       2006
 * \history    Modified 2022:
 *             - Moved SW version number declaration from altera_avalon_sierra_ker.h to this file
 *             - Included sierra_info.h
 *             - Modified sierra_SW_driver_version() to return sw_version_union structure
 *             - Added sierra_ prefix for external functions
 *             - Added sierra_get_current_time function
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

#include <io.h>
#include <system.h>
#include "sierra.h"
#include <sys/alt_irq.h>
#include <sierra_logging.h>

#if SIERRA_LOGGING == 1
  #include <stdio.h>
#else
  #include <sys/alt_log_printf.h>
#endif

/*!----------------------------------------------------------------------------
    Include Sierra driver files specific to Altera
-----------------------------------------------------------------------------*/
#include <sierra_ker.h>
#include <sierra_tcb.h>
#include <sierra_regs.h>
#include <sierra_io.h>
#include <sierra_name.h>
//#include <priv/alt_legacy_irq.h> NIOS V
#include <sierra_info.h>

//! Global variable used in interrupt handling assembler code
volatile unsigned int first_csw = 0;

//NIOS V
void dummy_isr(void* context) {
    // DUMMY. Not used but needed!
}


/*static void init_interrupt()
{
	//NIOS V
	if (alt_ic_isr_register(0, SIERRA_RTOS_IRQ, dummy_isr, NULL, NULL )) printf("error registration Sierra irq");

	  // Aktivera globalt avbrottssystem
	asm volatile("csrsi mstatus, 0x8");
	asm volatile("li t0, 0x00000808");    //# MEIE=1, MTIE=0 (Ingen nios V timer), MSIE=1
	asm volatile("csrs mie, t0");         //# Skriv till mie
}
  */

static void init_interrupt(void)
{

    // Global enable: mstatus.MIE = 1
    asm volatile("csrsi mstatus, 0x8");

    // Enable mask i mie: MEIE (bit11) + MSIE (bit3) = 0x800 + 0x008 = 0x808
    asm volatile("li t0, 0x00000808");
    asm volatile("csrs mie, t0");

        // Registrera ISR för Sierran, behövs inte, eftersom den tas om han i trap_vecktor
    int rc = alt_ic_isr_register(0, SIERRA_RTOS_IRQ, dummy_isr, NULL, NULL);
    if (rc != 0) {
    #if SIERRA_LOGGING == 1
        printf("error registration Sierra irq (rc=%d)\n", rc);
    #else
        ALT_LOG_PRINTF("error registration Sierra irq (rc=%d)\n", rc);
    #endif
    }
}

//! TCB list 
tcb_t TCB_LIST[N_TASKS];

//! Context data for current and next executing task 
volatile uint8_t RUNNING_TASKID = INVALID_TASK_ID;
volatile uint8_t NEXT_TASKID = INVALID_TASK_ID;
volatile tcb_t *current_tcb = 0;

//! SW version number.
#define C_Sierra_SW_driver_version 100315

//----------------------------------------------------------------------------
sw_version_union sierra_SW_driver_version(void)
{
  sw_version_union version;
  version.sw_version.PATCH_SW =  C_Sierra_SW_driver_version%100;
  version.sw_version.MINOR_SW = (C_Sierra_SW_driver_version%10000)/100;
  version.sw_version.MAJOR_SW = (C_Sierra_SW_driver_version)/10000;
  return version;
}
  
//----------------------------------------------------------------------------
version_register_union sierra_HW_version(void)
{
  version_register_union version;
  version.version_register_int = M_Sierra_HW_Version_reg;
  return version;
}

//----------------------------------------------------------------------------
uint32_t sierra_time_base_reg(void)
{
  const uint32_t time_base_reg = M_RD_SierraTime_base_reg;
  return time_base_reg;
}

//----------------------------------------------------------------------------
uint32_t sierra_ticks_to_ms(uint32_t ticks)
{
  const uint32_t time_base = sierra_time_base_reg();
  const uint32_t cpu_freq_mhz = ALT_CPU_FREQ / 1000000;

  if (cpu_freq_mhz == 0) {
    return 0;
  }

  return (uint32_t)(((uint64_t)ticks * time_base) / cpu_freq_mhz);
}

//----------------------------------------------------------------------------
void sierra_set_timebase(uint32_t hex)
{
  // Logs the set time base
  sierra_logging_medium(info_sierra_time_timebase_set, sierra_get_current_time(), hex);
  M_IOWR_SierraTime_base_reg(hex);

  if (hex > SIERRA_MAX_TIMEBASE)
  {
    // Warns in case of abnormal values
    sierra_logging_full(warn_sierra_time_timebase_exc, sierra_get_current_time(), hex, SIERRA_MAX_TIMEBASE);
  }
}

//-----------------------------------------------------------------------------
uint32_t sierra_get_current_time(void)
{
  const uint32_t time_log_reg = M_RD_TIME_LOGGING_REGISTER;
  return time_log_reg;
}

//----------------------------------------------------------------------------
void sierra_initiation_HW_and_SW(void)
{
  // SW reset of HW Sierran
  M_WD_SW_RESET_REGISTER(0x15);
  M_WD_SW_RESET_REGISTER(0x00);
  
  // Initialize TCBs
  for (size_t i = 0; i < N_TASKS; ++i)
    TCB_LIST[i].taskID = INVALID_TASK_ID;
  current_tcb = NULL;
  
  // Initiate Sierra HW IRQ for NiosII
  init_interrupt();

  // Prints status of logging
 // sierra_print_logging_status();
}

//----------------------------------------------------------------------------
// Scheduler decide next task to run and perform context switch
void get_next_task(void)
{
  // Ack. on TSW req
  ctrl_union CPU_CTRL_reg;
  CPU_CTRL_reg.ctrl_t.tswoff_req = 0;
  CPU_CTRL_reg.ctrl_t.tsw_req_ack = 1;
  IOWR_ALT_CTRL_REGISTER(CPU_CTRL_reg.ctrl_integer);

  statusB_union status;
  do {
    status.statusB_reg_integer = M_RD_Sierra_statusB_reg;
  } while (status.statusB_t.tsw_req_flag != 0);

  // Get next task ID
  const uint8_t next_taskid = constant_task_mask & status.statusB_t.running_taskID;
  sierra_logging_full(info_sierra_task_preemted_task, sierra_get_current_time(), next_taskid, RUNNING_TASKID);
  RUNNING_TASKID = next_taskid;
  current_tcb = &TCB_LIST[RUNNING_TASKID];
 
  // Nack on TSW req 
  CPU_CTRL_reg.ctrl_t.tsw_req_ack = 0;
  IOWR_ALT_CTRL_REGISTER(CPU_CTRL_reg.ctrl_integer);

  //Kontrollera att IRQ har gått lågt! M_RD_Sierra_statusB_reg_utan är utan ";"
  while ((M_RD_Sierra_statusB_reg_utan & 0x1u) != 0u) {
    /* vänta tills Sierra släpper request/IRQ */
    asm volatile("nop");
  }
/* här är IRQ=0, fortsätt normal hantering */
}

//----------------------------------------------------------------------------
// Internal version which turns task switching on without requesting logging
void sierra_tsw_on_internal(void)
{
  // Update CPU control register
  ctrl_union CPU_CTRL_reg;
  CPU_CTRL_reg.ctrl_t.tswoff_req = 0;
  CPU_CTRL_reg.ctrl_t.tsw_req_ack = 0;
  IOWR_ALT_CTRL_REGISTER(CPU_CTRL_reg.ctrl_integer);
}

//----------------------------------------------------------------------------
void sierra_tsw_on(void)
{
  sierra_tsw_on_internal();

  // Logs data when tsw is turned on
  sierra_logging_short(info_sierra_tsw_switching_on, sierra_get_current_time());
}

//----------------------------------------------------------------------------
// Internal version which turns task switching off without requesting logging
void sierra_tsw_off_internal(void)
{
  // Update CPU control register
  ctrl_union CPU_CTRL_reg;
  CPU_CTRL_reg.ctrl_t.tswoff_req = 1;
  CPU_CTRL_reg.ctrl_t.tsw_req_ack = 0;
  IOWR_ALT_CTRL_REGISTER(CPU_CTRL_reg.ctrl_integer);
}

//----------------------------------------------------------------------------
void sierra_tsw_off(void)
{
  sierra_tsw_off_internal();

  // Logs data when tsw is turned off
  sierra_logging_short(info_sierra_tsw_switching_off, sierra_get_current_time());
}

//----------------------------------------------------------------------------
void get_new_task(void)
{
  // New task to start is fetched from NEXT_TASKID
  RUNNING_TASKID = NEXT_TASKID;
  current_tcb = &TCB_LIST[RUNNING_TASKID];

  // Logs data about the next running task
  sierra_logging_medium(info_sierra_task_next_requested, sierra_get_current_time(), RUNNING_TASKID);
}

//----------------------------------------------------------------------------
statusA_union handle_service_call(const svc_t* pSVC)
{
  // Start service call
  IOWR_ALT_SVC_REGISTER(pSVC->svc_input);

  // ack_wait
  statusA_union status;
  do {
    status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);

  return status;
}

//----------------------------------------------------------------------------
void sierra_await_irq(int IRQ_number)
{
  sierra_tsw_off_internal();

  svc_t svc;
  svc.irq_wait_t.type = sierra_irq_wait;
  svc.irq_wait_t.irq_type = IRQ_number;
  const statusA_union statusA = handle_service_call(&svc);

  // Logs data when an interrupt service task is ready to process to wait for an external interrupt
  sierra_logging_full(info_sierra_irq_task_wait_irq, sierra_get_current_time(), RUNNING_TASKID, IRQ_number);

  NEXT_TASKID = constant_task_mask & statusA.statusA_t.svc_return;
  sierra_tsw_on_internal();
  taskswitch; // perform manual contextswitch
}
