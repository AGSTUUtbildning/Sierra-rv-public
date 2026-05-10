/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_014B_testing_mbox_read.c

    Company       : AGSTU AB
    Author        : Ludvig Martin
    Updated       : 2026
    Description   : Test 014B - Verify mailbox read() behaviour.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Sierra mailbox software extension
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that:
    1. A mailbox can be initialized.
    2. A task can send a message to the mailbox.
    3. read() returns the expected payload.
    4. The message is removed after read().
    5. A second read() returns MBOX_EMPTY.

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_014B_testing_mbox_read.h"

#include <altera_avalon_sierra_ker.h>
#include <altera_avalon_sierra_name.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <sierra_extension/sierra_mbox.h>

#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

static volatile int step = 0;
static volatile int shared_pas_variable = 1; /* 0 = PASS, 1 = FAIL */

static sierra_mbox_queue_t mbox;
static __attribute__ ((aligned (4))) char mbox_mem_pool[MBOX_POOL_SIZE];

static const char message[] = "test";

/* -------------------------------------------------------
   Task implementations
------------------------------------------------------- */

static void task_1_code(void)
{

assert(step == 0);
    step = 1;
    sierra_mbox_header_t header;

    header.id = task_1; 
    header.type = MBOX_CHAR_ARR; 
    header.size = sizeof(message); 
    header.data = (void*)message; 

    assert(sierra_mbox_send(&mbox, &header) == MBOX_OK);    // send message to mailbox


    task_start(task_2);
    task_delete();
}

static void task_2_code(void)
{
    sierra_mbox_header_t header;
    char buffer[MBOX_MAX_SIZE];

    assert(step == 1);
    step = 2;

    memset(buffer, 0, sizeof(buffer));
    header.data = buffer; // load buffer into mbox
    assert(sierra_mbox_read(&mbox, &header) == MBOX_OK);    // read from the mailbox
    assert(memcmp(buffer, message, sizeof(message)) == 0);  // check that the message is correct

    memset(buffer, 0, sizeof(buffer));
    header.data = buffer;
    assert(sierra_mbox_read(&mbox, &header) == MBOX_EMPTY); // check if empty

    shared_pas_variable = 0;

    task_delete();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_014B_testing_mbox_read(void)
{

    step = 0;
    shared_pas_variable = 1;


    sierra_mbox_init(&mbox, mbox_mem_pool, MBOX_MAX_MESSAGES, MBOX_MAX_SIZE); // init mailbox

    tsw_off();

    task_create(task_2, 6, BLOCKED_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
    task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);

    tsw_on();

    time_delay(delay_test_constant);

    return shared_pas_variable;
}

/* Back to idle task in main_test_file */
