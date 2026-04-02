/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_013B_manual_irq_trigger.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Tomas Hägg, Lennart
    Updated       : 2026
    Description   : Test 013 – Verify irq_wait(int n) using
                    pio connected to extriq_n port on sierra.

 Description
    -----------
   Test 013 – Verification of the function irq_wait(int n) using a
   manually triggered external interrupt signal. The signal is generated
   through a PIO connected to the extriq_n port of the Sierra kernel.

   Note: This is not a CPU interrupt. The extriq_n input is a hardware
   signal connected directly to the Sierra kernel. When the signal
   goes low (0), Sierra releases the task waiting on irq_wait(n) and
   moves it to the ready queue. When the signal is high (1), the task
   remains in the wait state.

   The function irq_wait(int n) blocks the calling task until a low signal 
   is received on input "n" of the Sierra kernel. When the low signal occurs, 
   Sierra moves the task from the interrupt-wait state to the ready queue, 
   allowing it to be scheduled for execution.

   The Sierra kernel provides two external inputs that can be used 
   with this mechanism. A maximum of two tasks can wait simultaneously, one 
   for each input.

   After the task is released from irq_wait(), it must acknowledge the signal 
   by writing to the PIO to reset it to a non-trigger state. This is necessary 
   by restoring the signal to its inactive (high) state before performing any 
   other operations. If the signal remains low and the task calls irq_wait(n) 
   again, a new interrupt may immediately be triggered, causing the task to 
   wake up again unintentionally.

    Test Description
    ----------------
    This test verifies that a task can correctly wait for and resume
    execution when an external interrupt occurs.

    The function irq_wait(int n) places the calling task in the
    "wait-for-interrupt" state until interrupt n is triggered.
    The interrupt is generated manually by writing to a PIO
    (PIO_SIERRA_EXTRIQ) that is connected to the Sierra extriq_n port.

    If the system is waiting for a specific interrupt and another
    interrupt ID is triggered manually, an exception will occur and
    the program will terminate.

    Strategy
    --------
    - One task is created that waits for interrupt 0.
    - The test program manually generates interrupt 0 using the PIO.
    - Each interrupt wakes the task, which verifies the correct
      execution sequence and then waits for the next interrupt.

    Execution flow:
        1. task_1 waits for interrupt 0 using irq_wait(0)
        2. The test code triggers interrupt 0 through the PIO
        3. task_1 resumes execution and updates the test state
        4. Steps 1–3 are repeated three times

    After the third successful interrupt wake-up, the test
    is considered successful.

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_013_manual_irq_trigger.h"

#include <altera_avalon_sierra_ker.h>
#include <altera_avalon_sierra_name.h>
#include <assert.h>
#include "test_setup.h"
#include <system.h>
#include <stdio.h>

#if SIERRA_FULL_TEST_HW_PLATFORM
/* -------------------------------------------------------
   Test variables
------------------------------------------------------- */

/* 0 = PASS, 1 = FAIL (default FAIL) */
static volatile int shared_pas_variable = 1;

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

static volatile int test_var = 0;

/*
   task_1 has highest priority and is created last.
   It verifies that all other tasks have executed.
*/

// Tasks wait for manual irq.

static void task_1_code(void)
{
    irq_wait(0);
    IOWR_32DIRECT(PIO_SIERRA_EXTRIQ_BASE, 0, 3); // Reset PIO to non-trigger state.
    test_var = 1;

    irq_wait(0);
    IOWR_32DIRECT(PIO_SIERRA_EXTRIQ_BASE, 0, 3); // Reset PIO to non-trigger state.
    test_var = 2;

    irq_wait(0);
    IOWR_32DIRECT(PIO_SIERRA_EXTRIQ_BASE, 0, 3); // Reset PIO to non-trigger state.
   shared_pas_variable = 0; // testen fungerar

    task_delete();
}


/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_013_manual_irq_trigger(void)
{
   shared_pas_variable = 1;
   test_var = 0;

   IOWR_32DIRECT(PIO_SIERRA_EXTRIQ_BASE, 0, 3); // Set PIO to its none trigger state.

   tsw_off();
   task_create(task_1, 3, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
   tsw_on();

   time_delay(delay_test_constant);
   IOWR_32DIRECT(PIO_SIERRA_EXTRIQ_BASE, 0, 0); // Invoke PIO connected to extirq and start task_0

   time_delay(delay_test_constant);
   IOWR_32DIRECT(PIO_SIERRA_EXTRIQ_BASE, 0, 0); // Invoke PIO connected to extirq and start task_0

   time_delay(delay_test_constant);
   IOWR_32DIRECT(PIO_SIERRA_EXTRIQ_BASE, 0, 0); // Invoke PIO connected to extirq and start task_0


   time_delay(delay_test_constant);

   return shared_pas_variable;
}
#endif
