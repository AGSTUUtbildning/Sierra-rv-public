/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_008_testing_time_management.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson
    Updated       : 2026, Lennart
    Description   : Test 008 – Verify periodic task start order (time management).

    Test Description
    ----------------
    This test verifies that periodic scheduling starts tasks in the expected
    order according to their configured period times.

    Important:
    - The test does NOT verify exact timing accuracy (ticks/µs).
    - It verifies only that tasks resume in the intended sequence when
      wait_for_next_period() is used.

    Test Strategy
    -------------
    - Tasks are configured with different period times (50..100).
    - Each task calls wait_for_next_period() and then checks the global
      test variable to ensure previous tasks have executed first.
    - task_6 has the shortest period (50) and is expected to run first.
    - task_1 has the longest period (100) and is expected to run last.
    - task_7 acts as an "idle task" for this test and keeps the system
      running until all periodic tasks are finished.

    Expected execution order after first period:
        task_6 -> task_5 -> task_4 -> task_3 -> task_2 -> task_1

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_008_testing_time_management.h"

#include <altera_avalon_sierra_ker.h>
#include <assert.h>

#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

/* Used to verify execution order between periodic tasks */
static volatile int test_variable_008 = 0;

/* 0 = PASS, 1 = FAIL (default FAIL) */
static volatile int shared_pas_variable = 1;

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

static void task_1_code(void)
{
    /* Longest period -> expected to run last */
    init_period_time(100);
    wait_for_next_period();

    assert(test_variable_008 == 5);
    test_variable_008 = 6;   /* Signal test completion to task_7 */

    /* Push periodic timer far into future before deletion */
    init_period_time(0xFFFF);
    task_delete();
}

static void task_2_code(void)
{
    init_period_time(90);
    wait_for_next_period();

    assert(test_variable_008 == 4);
    test_variable_008 = 5;

    init_period_time(0xFFFF);
    task_delete();
}

static void task_3_code(void)
{
    init_period_time(80);
    wait_for_next_period();

    assert(test_variable_008 == 3);
    test_variable_008 = 4;

    init_period_time(0xFFFF);
    task_delete();
}

static void task_4_code(void)
{
    init_period_time(70);
    wait_for_next_period();

    assert(test_variable_008 == 2);
    test_variable_008 = 3;

    init_period_time(0xFFFF);
    task_delete();
}

static void task_5_code(void)
{
    init_period_time(60);
    wait_for_next_period();

    assert(test_variable_008 == 1);
    test_variable_008 = 2;

    init_period_time(0xFFFF);
    task_delete();
}

static void task_6_code(void)
{
    /* Shortest period -> expected to run first */
    init_period_time(50);
    wait_for_next_period();

    /* First periodic task sets the chain */
    test_variable_008 = 1;

    init_period_time(0xFFFF);
    task_delete();
}

/*
   task_7 acts as a simple idle task for this test.
   It prevents returning to main test code until all periodic tasks are done.
*/
static void task_7_code(void)
{
    /* Wait until task_1 signals completion by setting test_variable_008 = 6 */
    while (test_variable_008 != 6)
    {
        /* Busy-wait. Higher priority periodic tasks will preempt this task. */
        asm volatile ("nop");
    }

    task_delete();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_008_testing_time_management(void)
{
    test_variable_008 = 0;
    shared_pas_variable = 1;

    /*
       Create all tasks:
       - task_1..task_6 are periodic tasks with different period times.
       - task_7 is an idle-like task that waits until test completes.
    */
    tsw_off();

    task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
    task_create(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
    task_create(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
    task_create(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
    task_create(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);

    /* Highest priority task created last (optional style) */
    task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);

    tsw_on();

    /* Give enough time for all periods to elapse and tasks to finish */
   time_delay(delay_test_constant);

    shared_pas_variable = 0;  /* PASS if we reach here without assert */

    return shared_pas_variable;
}

/* Back to idle task in main_test_file */
