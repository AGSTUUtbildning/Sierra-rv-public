/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_002_testing_task_delete.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson
    Updated       : 2026, Lennart
    Description   : Test 002 – Verify correct task deletion behaviour.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that:

    1. A running task reports state RUNNING.
    2. After calling task_delete(), the task enters DORMANT state.
    3. The next task in priority order runs correctly.
    4. Previously deleted tasks remain in DORMANT state.

    Test Strategy
    -------------
    - Seven tasks are created with descending priorities.
    - Each task verifies that:
        • The previous task is DORMANT (deleted).
        • The current task is RUNNING.
    - The final task marks the test as PASSED.

    Expected state values:
        0 = RUNNING
        3 = DORMANT

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_002_testing_task_delete.h"

#include <sierra_ker.h>
#include <sierra_name.h>
#include <assert.h>
#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

static task_info_t info;                 /* Used to inspect task state */
static volatile int shared_pas_variable = 1;  /* 0=PASS, 1=FAIL */

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

/*
   Each task verifies:
   - Previous task is DORMANT
   - Current task is RUNNING
   - Then deletes itself
*/

static void task_1_code(void)
{
    info = sierra_get_task_info(task_1);
    assert(info.state_info == 0);   /* Must be RUNNING */

    sierra_delete_task();
}

/* ----------------------------------------------------- */

static void task_2_code(void)
{
    info = sierra_get_task_info(task_1);
    assert(info.state_info == 3);   /* task_1 must be DORMANT */

    info = sierra_get_task_info(task_2);
    assert(info.state_info == 0);   /* Must be RUNNING */

    sierra_delete_task();
}

/* ----------------------------------------------------- */

static void task_3_code(void)
{
    info = sierra_get_task_info(task_2);
    assert(info.state_info == 3);

    info = sierra_get_task_info(task_3);
    assert(info.state_info == 0);

    sierra_delete_task();
}

/* ----------------------------------------------------- */

static void task_4_code(void)
{
    info = sierra_get_task_info(task_3);
    assert(info.state_info == 3);

    info = sierra_get_task_info(task_4);
    assert(info.state_info == 0);

    sierra_delete_task();
}

/* ----------------------------------------------------- */

static void task_5_code(void)
{
    info = sierra_get_task_info(task_4);
    assert(info.state_info == 3);

    info = sierra_get_task_info(task_5);
    assert(info.state_info == 0);

    sierra_delete_task();
}

/* ----------------------------------------------------- */

static void task_6_code(void)
{
    info = sierra_get_task_info(task_5);
    assert(info.state_info == 3);

    info = sierra_get_task_info(task_6);
    assert(info.state_info == 0);

    sierra_delete_task();
}

/* ----------------------------------------------------- */

static void task_7_code(void)
{
    info = sierra_get_task_info(task_6);
    assert(info.state_info == 3);

    info = sierra_get_task_info(task_7);
    assert(info.state_info == 0);

    shared_pas_variable = 0;   /* PASS */

    sierra_delete_task();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_002_testing_task_delete(void)
{
    shared_pas_variable = 1;

    /*
       Create all tasks with descending priorities.
       Highest priority runs first.
    */

    sierra_tsw_off();

    sierra_create_task(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
    sierra_create_task(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    sierra_create_task(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
    sierra_create_task(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
    sierra_create_task(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
    sierra_create_task(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
    sierra_create_task(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);

    sierra_tsw_on();

    /* Allow scheduler to execute sequence */
    time_delay(delay_test_constant);

    return shared_pas_variable;   /* 0 = PASS, 1 = FAIL */
}