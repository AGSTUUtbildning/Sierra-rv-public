/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_004_testing_priority.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson
    Updated       : 2026, Lennart
    Description   : Test 004 – Verify that tasks start in priority order.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that the scheduler starts tasks in the expected
    order based on priority.

    Test Strategy
    -------------
    - Seven tasks are created with descending priority values.
    - A shared counter (test_priority_004) starts at 1.
    - Each task asserts that the counter matches its expected value,
      then increments it.
    - If tasks run in correct order, task_7 marks the test as PASSED.

    Expected execution order:
        task_1 -> task_2 -> task_3 -> task_4 -> task_5 -> task_6 -> task_7

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_004_testing_priority.h"

#include <sierra_ker.h>
#include <assert.h>
#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

static volatile int shared_pas_variable = 1;  /* 0=PASS, 1=FAIL */

/* 
   Counter used to verify execution order.
   Starts at 1 to match expected task order.
*/
static volatile int test_priority_004 = 1;

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

static void task_1_code(void)
{
    assert(test_priority_004 == 1);
    test_priority_004++;
    task_delete();
}

static void task_2_code(void)
{
    assert(test_priority_004 == 2);
    test_priority_004++;
    task_delete();
}

static void task_3_code(void)
{
    assert(test_priority_004 == 3);
    test_priority_004++;
    task_delete();
}

static void task_4_code(void)
{
    assert(test_priority_004 == 4);
    test_priority_004++;
    task_delete();
}

static void task_5_code(void)
{
    assert(test_priority_004 == 5);
    test_priority_004++;
    task_delete();
}

static void task_6_code(void)
{
    assert(test_priority_004 == 6);
    test_priority_004++;
    task_delete();
}

static void task_7_code(void)
{
    assert(test_priority_004 == 7);

    /* PASS */
    shared_pas_variable = 0;

    test_priority_004++;
    task_delete();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_004_testing_priority(void)
{
    shared_pas_variable = 1;
    test_priority_004 = 1;

    /*
       Create tasks with priorities so task_1 is expected to run first
       and task_7 last (depending on Sierra priority semantics).
    */
    tsw_off();

    task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
    task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
    task_create(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
    task_create(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
    task_create(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
    task_create(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);

    tsw_on();

    /* Allow scheduler to execute sequence */
   time_delay(delay_test_constant);

    return shared_pas_variable;  /* 0 = PASS, 1 = FAIL */
}

/* Back to idle task in main_test_file */
