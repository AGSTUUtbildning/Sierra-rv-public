/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_010_testing_task_change_prio.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Lennart Lindh
    Updated       : 2026, Lennart
    Description   : Test 010 – Verify task_change_prio() updates execution order.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that changing task priorities at run-time affects
    the scheduling/execution order as expected.

    Test Strategy
    -------------
    - Seven tasks are created with initial priorities so that the expected
      execution order is:
          task_1 -> task_2 -> task_3 -> task_4 -> task_5 -> task_6 -> task_7

    - task_1 runs first and changes the priorities of the remaining tasks,
      creating a new intended order:
          task_1 -> task_3 -> task_2 -> task_6 -> task_5 -> task_4 -> task_7

    - A shared counter (test_priority_010) tracks execution order.
      Each task asserts that the counter matches the expected value.

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_010_testing_task_change_prio.h"

#include <altera_avalon_sierra_ker.h>
#include <assert.h>

#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

/* Counter used to verify execution order */
static volatile int test_priority_010 = 0;

/* 0 = PASS, 1 = FAIL (default FAIL) */
static volatile int shared_pas_variable = 1;

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

/*
   task_1 runs first and modifies priorities of other tasks to enforce
   a new execution order:
       task_1 -> task_3 -> task_2 -> task_6 -> task_5 -> task_4 -> task_7
*/
static void task_1_code(void)
{
    assert(test_priority_010 == 0);
    test_priority_010++;

    /* Change priorities (example mapping):
       - Higher number = higher priority (assumed, consistent with other tests)
       - After this, task_3 should run before task_2, etc.
    */
    task_change_prio(task_3, 6);
    task_change_prio(task_2, 5);
    task_change_prio(task_6, 4);
    task_change_prio(task_5, 3);
    task_change_prio(task_4, 2);
    task_change_prio(task_7, 1);

    task_delete();
}

static void task_3_code(void)
{
    /* Expected second */
    assert(test_priority_010 == 1);
    test_priority_010++;
    task_delete();
}

static void task_2_code(void)
{
    /* Expected third */
    assert(test_priority_010 == 2);
    test_priority_010++;
    task_delete();
}

static void task_6_code(void)
{
    /* Expected fourth */
    assert(test_priority_010 == 3);
    test_priority_010++;
    task_delete();
}

static void task_5_code(void)
{
    /* Expected fifth */
    assert(test_priority_010 == 4);
    test_priority_010++;
    task_delete();
}

static void task_4_code(void)
{
    /* Expected sixth */
    assert(test_priority_010 == 5);
    test_priority_010++;
    task_delete();
}

static void task_7_code(void)
{
    /* Expected last */
    assert(test_priority_010 == 6);
    test_priority_010++;

    shared_pas_variable = 0;  /* PASS */
    task_delete();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_010_testing_change_prio(void)
{
    test_priority_010 = 0;
    shared_pas_variable = 1;

    /*
       Create tasks with initial priorities.
       task_1 has highest priority so it runs first and changes priorities.
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

    /* Allow scheduler to execute test sequence */
   time_delay(delay_test_constant);

    return shared_pas_variable;  /* 0 = PASS, 1 = FAIL */
}
