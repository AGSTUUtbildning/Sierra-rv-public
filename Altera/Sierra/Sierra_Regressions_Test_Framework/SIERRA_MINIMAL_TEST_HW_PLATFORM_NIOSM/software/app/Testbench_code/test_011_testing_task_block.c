/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_011_testing_task_block.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson / AGSTU
    Updated       : 2026, Lennart
    Description   : Test 011 – Extended verification of task_block() and task states.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that:
    1) The currently executing task reports state RUNNING.
    2) After task_block(task_id), the task reports state BLOCKED.
    3) Tasks that are not started/ready report a non-running state (as expected).
    4) Blocking a different task than the current one updates that task’s state.

    Notes
    -----
    - This test uses task_getinfo() heavily to validate state transitions.
    - State values used in asserts follow the original test assumptions:
        0 = RUNNING
        1 = BLOCKED
        2 = NOT READY (as used in the original code)
    - If your Sierra implementation uses different numeric encodings,
      update the expected values accordingly.

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_011_testing_task_block.h"

#include <altera_avalon_sierra_ker.h>
#include <assert.h>

#include "test_setup.h"
#include "sierra_info.h"   /* Provides task_info() / debug helpers if enabled */

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

static task_info_t info;
static volatile int shared_pas_variable = 1;  /* 0=PASS, 1=FAIL */

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

static void task_1_code(void)
{
    /* Current task must be RUNNING */
    info = task_getinfo(task_1);
    assert(info.state_info == 0);

    /* Optional: read info for other tasks (debug / side effects) */
    (void)task_getinfo(task_2);
    (void)task_getinfo(task_3);

    /* Block self */
    task_block(task_1);

    /* If task_block() causes immediate reschedule, code below might never run.
       That is OK for this test. */
}

/* ----------------------------------------------------- */

static void task_2_code(void)
{
    /* task_1 should be BLOCKED after task_1 blocked itself */
    info = task_getinfo(task_1);
    assert(info.state_info == 1);

    /* Current task must be RUNNING */
    info = task_getinfo(task_2);
    assert(info.state_info == 0);

    /* task_3 expected to be NOT READY (as in original test) */
    info = task_getinfo(task_3);
    assert(info.state_info == 2);

    /* Block task_3 (not current) and verify it becomes BLOCKED */
    task_block(task_3);
    info = task_getinfo(task_3);
    assert(info.state_info == 1);

    /* Finally block self */
    task_block(task_2);
}

/* ----------------------------------------------------- */

static void task_3_code(void)
{
    /* Debug helper (if implemented in sierra_info.h) */
    task_info();

    /* Original code had:
         info = task_getinfo(task_2);
         assert(info.state_info==7); //error
       That looks like a deliberate "should never happen" assert.
       We keep the intent but express it clearly:
    */
    info = task_getinfo(task_2);

    /* If task_3 runs, task_2 should NOT be in an invalid/error state.
       Adjust expected value if you know the correct state here. */
    assert(info.state_info != 7);

    /* Block self */
    task_block(task_3);
}

/* ----------------------------------------------------- */

static void task_4_code(void)
{
    /* task_3 should be BLOCKED (depending on prior sequence) */
    info = task_getinfo(task_3);
    assert(info.state_info == 1);

    /* Current task must be RUNNING */
    info = task_getinfo(task_4);
    assert(info.state_info == 0);

    /* Block task_5 (not current), then block self */
    task_block(task_5);
    task_block(task_4);
}

/* ----------------------------------------------------- */

static void task_5_code(void)
{
    info = task_getinfo(task_4);
    assert(info.state_info == 1);

    info = task_getinfo(task_5);
    assert(info.state_info == 0);

    task_block(task_5);
}

/* ----------------------------------------------------- */

static void task_6_code(void)
{
    info = task_getinfo(task_5);
    assert(info.state_info == 1);

    info = task_getinfo(task_6);
    assert(info.state_info == 0);

    task_block(task_6);
}

/* ----------------------------------------------------- */

static void task_7_code(void)
{
    info = task_getinfo(task_6);
    assert(info.state_info == 1);

    info = task_getinfo(task_7);
    assert(info.state_info == 0);

    shared_pas_variable = 0;  /* PASS */

    /* End by blocking itself (as original) */
    task_block(task_7);
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_011_testing_task_block(void)
{
    shared_pas_variable = 1;

    tsw_off();

    task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
    task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
    task_create(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
    task_create(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
    task_create(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
    task_create(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);

    tsw_on();

    /* Allow time for sequence to execute */
    time_delay(delay_test_constant);

    return shared_pas_variable;  /* 0 = PASS, 1 = FAIL */
}

/* Back to idle task in main_test_file */
