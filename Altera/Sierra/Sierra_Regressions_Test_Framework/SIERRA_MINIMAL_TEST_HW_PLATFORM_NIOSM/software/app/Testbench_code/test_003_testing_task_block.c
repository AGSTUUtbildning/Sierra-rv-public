/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_003_testing_task_block.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson
    Updated       : 2026, Lennart
    Description   : Test 003 – Verify correct task blocking behaviour.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that:

    1. The currently executing task reports state RUNNING.
    2. After calling task_block(task_id), the task reports state BLOCKED.
    3. The next task in priority order runs correctly.
    4. Previously blocked tasks remain in state BLOCKED.

    Test Strategy
    -------------
    - Seven tasks are created with descending priorities.
    - Each task checks:
        • The previous task is BLOCKED.
        • The current task is RUNNING.
    - Each task then blocks itself.
    - The final task marks the test as PASSED.

    Expected state values:
        0 = RUNNING
        1 = BLOCKED

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_003_testing_task_block.h"

#include <altera_avalon_sierra_ker.h>
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
   - Previous task is BLOCKED
   - Current task is RUNNING
   Then it blocks itself.
*/

static void task_1_code(void)
{
    info = task_getinfo(task_1);
    assert(info.state_info == 0);  /* Must be RUNNING */

    task_block(task_1);           /* Block itself */
    task_delete();                /* Optional cleanup depending on Sierra semantics */
}

/* ----------------------------------------------------- */

static void task_2_code(void)
{
    info = task_getinfo(task_1);
    assert(info.state_info == 1);  /* task_1 must be BLOCKED */

    info = task_getinfo(task_2);
    assert(info.state_info == 0);  /* Must be RUNNING */

    task_block(task_2);
    task_delete();
}

/* ----------------------------------------------------- */

static void task_3_code(void)
{
    info = task_getinfo(task_2);
    assert(info.state_info == 1);

    info = task_getinfo(task_3);
    assert(info.state_info == 0);

    task_block(task_3);
    task_delete();
}

/* ----------------------------------------------------- */

static void task_4_code(void)
{
    info = task_getinfo(task_3);
    assert(info.state_info == 1);

    info = task_getinfo(task_4);
    assert(info.state_info == 0);

    task_block(task_4);
    task_delete();
}

/* ----------------------------------------------------- */

static void task_5_code(void)
{
    info = task_getinfo(task_4);
    assert(info.state_info == 1);

    info = task_getinfo(task_5);
    assert(info.state_info == 0);

    task_block(task_5);
    task_delete();
}

/* ----------------------------------------------------- */

static void task_6_code(void)
{
    info = task_getinfo(task_5);
    assert(info.state_info == 1);

    info = task_getinfo(task_6);
    assert(info.state_info == 0);

    task_block(task_6);
    task_delete();
}

/* ----------------------------------------------------- */

static void task_7_code(void)
{
    info = task_getinfo(task_6);
    assert(info.state_info == 1);

    info = task_getinfo(task_7);
    assert(info.state_info == 0);

    shared_pas_variable = 0;  /* PASS */

    /* In original code task_7 does not block itself.
       This is OK since the test is finished here. */
    task_delete();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_003_testing_task_block(void)
{
    shared_pas_variable = 1;

    tsw_off();

    /*
       Create tasks with descending priorities.
       Each task will block itself, allowing the next one to run.
    */

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
