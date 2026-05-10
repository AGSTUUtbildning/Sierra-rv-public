/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_001_testing_task_create.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : Maya Isaksson
    Updated       : 2026, Lennart
    Description   : Test 001 – Verify correct creation and execution of tasks.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that all tasks are successfully created and executed.

    Strategy:
    - Seven tasks are created.
    - Each task sets its own test variable to 1 when it runs.
    - task_1 is created LAST and has highest priority.
    - When task_1 runs, it verifies that all other tasks have already executed.
    - If all assertions pass, the test is marked as PASSED.

    Return value:
        0 = PASS
        1 = FAIL

-----------------------------------------------------------------------*/

#include "test_001_testing_task_create.h"

#include <altera_avalon_sierra_ker.h>
#include <altera_avalon_sierra_name.h>
#include <assert.h>
#include "test_setup.h"

/* -------------------------------------------------------
   Test variables
------------------------------------------------------- */

/* 
   Volatile ensures correct behaviour in:
   - Multi-core systems
   - Optimized builds
*/
static volatile int create_task_1_test = 0;
static volatile int create_task_2_test = 0;
static volatile int create_task_3_test = 0;
static volatile int create_task_4_test = 0;
static volatile int create_task_5_test = 0;
static volatile int create_task_6_test = 0;
static volatile int create_task_7_test = 0;

/* 0 = PASS, 1 = FAIL (default FAIL) */
static volatile int shared_pas_variable = 1;

/* -------------------------------------------------------
   TASK IMPLEMENTATIONS
------------------------------------------------------- */

/*
   task_1 has highest priority and is created last.
   It verifies that all other tasks have executed.
*/
static void task_1_code(void)
{
    create_task_1_test = 1;

    /* Verify all tasks have executed */
    assert(create_task_2_test == 1);
    assert(create_task_3_test == 1);
    assert(create_task_4_test == 1);
    assert(create_task_5_test == 1);
    assert(create_task_6_test == 1);
    assert(create_task_7_test == 1);

    shared_pas_variable = 0;   /* PASS */

    sierra_delete_task();
}

/* Remaining tasks simply mark execution */

static void task_2_code(void)
{
    create_task_2_test = 1;
    sierra_delete_task();
}

static void task_3_code(void)
{
    create_task_3_test = 1;
    sierra_delete_task();
}

static void task_4_code(void)
{
    create_task_4_test = 1;
    sierra_delete_task();
}

static void task_5_code(void)
{
    create_task_5_test = 1;
    sierra_delete_task();
}

static void task_6_code(void)
{
    create_task_6_test = 1;
    sierra_delete_task();
}

static void task_7_code(void)
{
    create_task_7_test = 1;
    sierra_delete_task();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_001_testing_task_create(void)
{
    shared_pas_variable = 1;

    /*
       Create tasks in reverse priority order.
       task_1 is created last and has highest priority,
       ensuring it runs after all others.
    */

    sierra_create_task(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    sierra_create_task(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
    sierra_create_task(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
    sierra_create_task(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
    sierra_create_task(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
    sierra_create_task(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);

    /* Highest priority task created last */
    sierra_create_task(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);

    return shared_pas_variable;
}

/* Back to idle task in main_test_file */