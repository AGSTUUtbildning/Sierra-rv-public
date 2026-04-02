/*!
 * \file       sierra_ker.h
 * \details    This section describes the functionality of the Interrupt Manager. The interrupts are associated with an
 *             interrupt task, which is scheduled as an ordinary task in the system. External interrupt is connected to
 *             Sierras external IRQ pins. Each IRQ input is level sensitivity and active-high.
 *
 *             The following functions is implemented in hardware:
 *             - Wait for interrupt
 *
 *             If several external interrupts occur simultaneously, the task associated with highest interrupt pins will
 *             be the first one sent to the ready queue.
 *             \image rtf irq.png SW RTK and HW based Sierra solution, two low priority irq.
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

#ifndef __SIERRA_KER_H__
#define __SIERRA_KER_H__

#include <stdint.h>
#include "sierra.h"
#include "sierra_info.h"
#include "sierra_sem.h"
#include "sierra_task.h"
#include "sierra_time.h"
#include <sys/msw_interrupt.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! Number of tasks.
    Maxium number of tasks that will be used depend of the hardware version you have. If less than 16
    tasks will be used, memory space is saved if this variable is set to correct number as each task
    allocates memory for its tcb (about 160 bytes/task, see the configuration of the Sierra you use.
 */
#define N_TASKS 8

//! Mask for getting task id from status return value.
#define constant_task_mask 0x7

//! ID set to tasks prior to creation / initialization.
#define INVALID_TASK_ID 0xff

/*------------------------------------------------------------------------
    DONT MAKE CHANGES BEYOND THIS LINE
------------------------------------------------------------------------*/
#include <sierra_tcb.h>  // Include TCB description
#include <sierra_io.h>   // Include RTU I/O description

//! Thread context (TCB) data
extern tcb_t TCB_LIST[N_TASKS];

//! Context data for task
extern volatile uint8_t RUNNING_TASKID;
extern volatile uint8_t NEXT_TASKID;

//! taskswitch triggered as IRQ
//#define taskswitch  __asm__("trap");

//NIOS V
#define taskswitch alt_niosv_trigger_msw_interrupt();

//! HW-specific taskswitch function
void HW_CTX_SWITCH(void*);

/*!-----------------------------------------------------------------------
    Interrupt handling
-------------------------------------------------------------------------*/

/*! \brief   Waits for interrupt on IRQ level.
 *  \details \par Description
 *           This call used when an interrupt service task is ready to process to wait for an external interrupt. As a
 *           result of this call, the interrupt service task (running task) will be moved to the ‘Wait for interrupt’
 *           state. The Id of task that the CPU should context switch to is in the return data.
 *  \param   IRQ_number Specifies the interrupt level. The range of the interrupt level depends on the version of the
 *                      Sierra.
 *  \par Example
 *  \image rtf irq_example.png Example of two external irq.
 *  \code
 *  void irq_task_code(void)
 *  {
 *    int i=0;
 *    printf("IRQ Task starts\n");
 *    while(1)
 *    {
 *      sierra_await_irq(1); //Wait for external IRQ 1
 *      printf("IRQ 1 start\n");
 *      ...
 *      sierra_await_irq(0); //Wait for external IRQ 0
 *      printf("IRQ 0 start\n");
 *      ...
 *    }
 *  }
 *  \endcode
 */
extern void sierra_await_irq(int IRQ_number);

#ifdef __cplusplus
}
#endif

#endif /* __SIERRA_KER_H__ */
