/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_006B_testing_sem_take_and_release.c

    Company       : AGSTU AB (www.agstu.com)
    Author        : AGSTU
    Updated       : 2026, Lennart
    Description   : Test 006B – Verify correct semaphore take/release
                    behaviour and wake-up order.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that:

    1. Tasks correctly block when calling sem_take().
    2. sem_release() wakes exactly one waiting task.
    3. Tasks resume execution in the expected order.
    4. Multiple semaphores can be chained to control execution flow.

    Test Strategy
    -------------
    - Three tasks (task_1, task_2, task_3) call sem_take() and block.
    - task_1 waits on SEM1, SEM2 and SEM3.
    - task_2 waits on SEM2.
    - task_3 waits on SEM3.
    - A fourth task (task_4) releases SEM1, SEM2 and SEM3 sequentially.
    - A shared state variable (test_failed_variable) ensures that
      tasks resume in the correct order.
    - Each task uses assert() to verify execution order.
    - If all tasks resume in the expected sequence, the test passes.

    Expected execution order:
        1) task_1 resumes after SEM1
        2) task_2 resumes after SEM2
        3) task_3 resumes after SEM3

    Return value:
        0 = PASS
        1 = FAIL

-----------------------------------------------------------------------*/

#include "test_006B_testing_sem_take_and_release.h"

#include <sierra_ker.h>
#include <sierra_name.h>
#include <assert.h>
#include "test_setup.h"

/* ----------------------------------------------------
   Global test state
---------------------------------------------------- */

static volatile int test_failed_variable = 0;
static volatile int result = 1;   /* default FAIL */

/* ----------------------------------------------------
   TASKS
---------------------------------------------------- */

/* Highest priority waiter */
static void task_1_code(void)
{
    sierra_take_sem(SEM1);
    sierra_take_sem(SEM2);
    sierra_take_sem(SEM3);
 
    assert(test_failed_variable == 0);

    test_failed_variable = 1;

    sierra_delete_task();
}

/* Third priority waiter */
static void task_3_code(void)
{
    sierra_take_sem(SEM3);

    /* Must wake third */
    assert(test_failed_variable == 2);

    test_failed_variable = 3;

    sierra_delete_task();
}

/* Second priority waiter */
static void task_2_code(void)
{
    sierra_take_sem(SEM2);

    /* Must wake second */
    assert(test_failed_variable == 1);

    test_failed_variable = 2;

    sierra_delete_task();
}


/* Releaser task */
static void task_4_code(void)
{

    /* Release semaphore three times */

    sierra_release_sem(SEM1);
    time_delay(200000);

    sierra_release_sem(SEM2);
    time_delay(200000);

    sierra_release_sem(SEM3);
    time_delay(200000);

    /* After all wakeups we expect state = 3 */
    assert(test_failed_variable == 3);

    result = 0;   /* PASS */

    sierra_delete_task();
}

/* ----------------------------------------------------
   Test entry
---------------------------------------------------- */

int test_006B_testing_sem_take_and_release(void)
{
    result = 1;
    test_failed_variable = 0;
    sierra_release_sem(SEM1);
    sierra_release_sem(SEM2);
    sierra_release_sem(SEM3);

 //   sem_release(SEM1);  // om den den är tagen från början, så att task 1, 2 och 3 inte kan ta den innan task 4 har släppt den

    sierra_tsw_off();

    sierra_create_task(task_1, 6, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
    sierra_create_task(task_2, 4, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    sierra_create_task(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);

    sierra_create_task(task_4, 3, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);

    sierra_tsw_on();

    time_delay(delay_test_constant);

    return result;  /* 0 = PASS, 1 = FAIL */
}