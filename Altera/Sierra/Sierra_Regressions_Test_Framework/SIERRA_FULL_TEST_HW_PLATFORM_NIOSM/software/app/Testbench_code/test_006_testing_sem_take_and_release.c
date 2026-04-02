/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel test code for Nios II / Nios V

    Filename      : test_006_testing_sem_take_and_release.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson (isaksson.maya@gmail.com)
    Updated       : 2026, Lennart
    Description   : Test 006 – Verify semaphore take/release ordering in Sierra.

    System
    Components    : Sierra real-time kernel (HW + SW)

    Test Description
    ----------------
    This test verifies that tasks calling sem_take() are correctly blocked
    until the corresponding semaphore is released using sem_release().

    Execution sequence:

      1) Task 1 starts and blocks on SEM1 and SEM2.
      2) Task 2 starts and blocks on SEM1 and SEM2.
      3) Task 3 releases SEM1.
      4) Task 4 releases SEM2.
      5) Task 2 verifies it resumed only after both semaphores were released.
      6) Task 5 marks the test as PASSED.

    If execution order is incorrect, assert() will trigger.

    Return value:
        0 = PASS
        1 = FAIL

  COPYRIGHT (C) AGSTU AB
  All rights reserved.
-----------------------------------------------------------------------*/

#include "test_006_testing_sem_take_and_release.h"

#include <altera_avalon_sierra_ker.h>
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_regs.h>
#include <altera_avalon_sierra_name.h>
#include <altera_avalon_sierra_tcb.h>

#include <system.h>
#include <stdio.h>
#include <io.h>
#include <assert.h>

#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

static volatile int test_failed_variable = 0;
static volatile int shared_pas_variable  = 1;  /* 0=PASS, 1=FAIL */

/* -------------------------------------------------------
   Task implementations
------------------------------------------------------- */

static void task_1_code(void)
{
    /* Task 1 must run first */
    assert(test_failed_variable == 0);

    test_failed_variable = 1;

    /* Must block until released */
    sem_take(SEM1);
    sem_take(SEM2);

    task_delete();
}

static void task_2_code(void)
{
    /* Task 2 must run after Task 1 */
    assert(test_failed_variable == 1);

    test_failed_variable = 2;

    /* Must block until released */
    sem_take(SEM1);
    sem_take(SEM2);

    /* Must only continue after Task 4 has set state=4 */
    assert(test_failed_variable == 4);

    task_delete();
}

static void task_3_code(void)
{
    /* Task 3 must run after Task 2 */
    assert(test_failed_variable == 2);

    test_failed_variable = 3;

    /* Release SEM1 to allow waiting tasks to proceed (first stage) */
    sem_release(SEM1);

 //   test_delay(1000000);

    task_delete();
}

static void task_4_code(void)
{
    /* Task 4 must run after Task 3 */
    assert(test_failed_variable == 3);

    test_failed_variable = 4;

    /* Release SEM2 to allow waiting tasks to proceed (second stage) */
    sem_release(SEM2);

    task_delete();
}

static void task_5_code(void)
{
    /* Final check */
    assert(test_failed_variable == 4);

    /* PASS */
    shared_pas_variable = 0;

    task_delete();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_006_testing_sem_take_and_release(void)
{
    test_failed_variable = 0;
    shared_pas_variable  = 1;

    tsw_off();

    task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
    task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
    task_create(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
    task_create(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);

    tsw_on();

    /* Let scheduler run the test sequence */
   time_delay(delay_test_constant);

    return (int)shared_pas_variable;
}

/* Back to idle task in main_test_file */