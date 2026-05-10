/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_005_testing_task_start.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson
    Updated       : 2026, Lennart
    Description   : Test 005 – Verify task_start() behaviour.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that a task in BLOCKED state can be started using
    task_start(task_id), and that control flow can be transferred through
    a chain of tasks.

    Test Strategy
    -------------
    - Seven tasks are created:
        • task_1 is created READY (runs immediately).
        • task_2 ... task_7 are created BLOCKED (must be started explicitly).
    - Each task starts the next task using task_start(), then deletes itself.
    - If task_7 executes, the test is marked as PASSED.

    Execution flow:
        task_1 -> task_2 -> task_3 -> task_4 -> task_5 -> task_6 -> task_7

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_005_testing_task_start.h"

#include <altera_avalon_sierra_ker.h>
#include "test_setup.h"

/* -------------------------------------------------------
   Global test result
------------------------------------------------------- */

/* 0 = PASS, 1 = FAIL (default FAIL) */
static volatile int shared_pas_variable = 1;

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

static void task_1_code(void)
{
    task_start(task_2);
    task_delete();
}

static void task_2_code(void)
{
    task_start(task_3);
    task_delete();
}

static void task_3_code(void)
{
    task_start(task_4);
    task_delete();
}

static void task_4_code(void)
{
    task_start(task_5);
    task_delete();
}

static void task_5_code(void)
{
    task_start(task_6);
    task_delete();
}

static void task_6_code(void)
{
    task_start(task_7);
    task_delete();
}

static void task_7_code(void)
{
    /* If we reach task_7, the chain worked -> PASS */
    shared_pas_variable = 0;
    task_delete();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_005_testing_task_start(void)
{
    shared_pas_variable = 1;

    /*
       Create all tasks for testing:
       - task_2..task_7 are BLOCKED and must be started explicitly.
       - task_1 is READY and starts the chain.
    */

    tsw_off();

    task_create(task_2, 6, BLOCKED_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    task_create(task_3, 5, BLOCKED_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
    task_create(task_4, 4, BLOCKED_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
    task_create(task_5, 3, BLOCKED_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
    task_create(task_6, 2, BLOCKED_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
    task_create(task_7, 1, BLOCKED_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);

    /* Start point of the chain */
    task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);

    tsw_on();

    /* Allow scheduler to execute test chain */
   time_delay(delay_test_constant);

    return shared_pas_variable;  /* 0 = PASS, 1 = FAIL */
}

/* Back to idle task in main_test_file */
