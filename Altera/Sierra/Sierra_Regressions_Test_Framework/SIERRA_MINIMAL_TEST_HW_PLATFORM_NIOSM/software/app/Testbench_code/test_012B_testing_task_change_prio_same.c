/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_012B_testing_task_change_prio_same.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Lennart Lindh
    Updated       : 2026
    Description   : Test 012B – Verify task_change_prio() using
                    chained priority changes between 6 tasks.

    Test Description
    ----------------
    This test verifies that changing priorities at run-time causes
    immediate rescheduling.

    Strategy
    --------
    - 6 tasks are created.
    - task_1 starts first.
    - Each task raises the priority of the next task.
    - The scheduler must immediately switch to the boosted task.
    - Execution order is verified using a shared step counter.

    Expected execution order:
        task_1 -> task_2 -> task_3 -> task_4 -> task_5 -> task_6

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_012B_testing_task_change_prio_same.h"

#include <sierra_ker.h>
#include <assert.h>
#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

static volatile int step = 0;
static volatile int shared_pas_variable = 1; /* 0=PASS, 1=FAIL */

#define PRIO_TOP 7

/* -------------------------------------------------------
   TASKS
------------------------------------------------------- */

static void task_1_code(void)
{
    assert(step == 0);
    step = 1;

    task_change_prio(task_2, PRIO_TOP);

    /* Resume here after full chain is completed */
    assert(step == 6);

    shared_pas_variable = 0;
    task_delete();
}

static void task_2_code(void)
{
    assert(step == 1);
    step = 2;

    task_change_prio(task_3, PRIO_TOP);
    task_delete();
}

static void task_3_code(void)
{
    assert(step == 2);
    step = 3;

    task_change_prio(task_4, PRIO_TOP);
    task_delete();
}

static void task_4_code(void)
{
    assert(step == 3);
    step = 4;

    task_change_prio(task_5, PRIO_TOP);
    task_delete();
}

static void task_5_code(void)
{
    assert(step == 4);
    step = 5;

    task_change_prio(task_6, PRIO_TOP);
    task_delete();
}

static void task_6_code(void)
{
    assert(step == 5);
    step = 6;

    task_delete();
}

/* -------------------------------------------------------
   Test entry
------------------------------------------------------- */

int test_012B_testing_change_prio_same(void)
{
    step = 0;
    shared_pas_variable = 1;

    tsw_off();

    task_create(task_1, 6, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);

    task_create(task_2, 1, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    task_create(task_3, 1, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
    task_create(task_4, 1, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
    task_create(task_5, 1, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
    task_create(task_6, 1, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);

    tsw_on();

   time_delay(delay_test_constant);

    return shared_pas_variable;
}
