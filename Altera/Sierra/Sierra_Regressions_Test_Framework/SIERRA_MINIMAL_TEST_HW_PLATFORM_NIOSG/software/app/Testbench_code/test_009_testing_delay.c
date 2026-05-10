/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_009_testing_delay.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson
    Updated       : 2026, Lennart
    Description   : Test 009 – Verify delay() affects task execution order.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that the Sierra delay() function delays a task long
    enough to change the execution order between periodic tasks.

    Test Strategy
    -------------
    - Two periodic tasks are created:
        • task_1 uses period time 100 (normally would start later).
        • task_2 uses period time 90  (normally would start earlier).
    - task_2 calls delay(20) after wait_for_next_period().
      This postpones task_2 execution so that task_1 runs first.
    - A shared variable (test_variable) is used to verify the order.
    - task_3 acts as an idle-like task, waiting until the test is complete.

    Expected execution order:
        1) task_1 runs first and sets test_variable = 1
        2) task_2 runs after delay() and checks test_variable == 1,
           then sets test_variable = 2
        3) task_3 detects test completion (test_variable == 2) and marks PASS

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_009_testing_delay.h"

#include <altera_avalon_sierra_ker.h>
#include <altera_avalon_sierra_name.h>
#include <assert.h>

#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

static volatile int test_variable = 0;         /* Used to verify execution order */
static volatile int shared_pas_variable = 1;  /* 0=PASS, 1=FAIL */

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

static void task_1_code(void)
{
    /* Without any extra delay, this task would normally start later */
    sierra_period_time_init(100);
    sierra_await_next_period();

    /* Must run before task_2 (because task_2 is delayed) */
    assert(test_variable == 0);

    test_variable = 1;

    sierra_delete_task();
}

static void task_2_code(void)
{
    /* Without delay(), this task would normally start first */
    sierra_period_time_init(90);
    sierra_await_next_period();

    /* Delay this task so task_1 gets to run first */
    sierra_delay_task(20);

    /* Must run after task_1 has set test_variable = 1 */
    assert(test_variable == 1);

    test_variable = 2;

    sierra_delete_task();
}

/*
   task_3 acts as an idle-like task for this test.
   It waits until test_variable == 2, then marks PASS.
*/
static void task_3_code(void)
{
    while (1)
    {
        if (test_variable == 2)
        {
            shared_pas_variable = 0; /* PASS */
            sierra_delete_task();
        }

        /* Optional small NOP loop to reduce tight spinning */
        asm volatile ("nop");
    }
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_009_testing_delay(void)
{
    test_variable = 0;
    shared_pas_variable = 1;

    sierra_tsw_off();

    sierra_create_task(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
    sierra_create_task(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    sierra_create_task(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);

    sierra_tsw_on();

    /* Allow time for the periodic tasks to run and the idle task to finish */
    time_delay(delay_test_constant);

    return shared_pas_variable;  /* 0 = PASS, 1 = FAIL */
}

/* Back to idle task in main_test_file */