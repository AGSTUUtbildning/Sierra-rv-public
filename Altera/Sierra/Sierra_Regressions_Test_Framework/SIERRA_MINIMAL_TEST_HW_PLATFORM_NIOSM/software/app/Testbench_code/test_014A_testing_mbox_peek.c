/*!----------------------------------------------------------------------
    Sierra Real-Time Kernel Test Code

    Filename      : test_014A_testing_mbox_peek.c

    Company       : AGSTU AB
    Author        : Ludvig Martin
    Updated       : 2026
    Description   : Test 014A - Verify mailbox peek() behaviour.

    System Components:
        - Sierra Real-Time Kernel (HW + SW)
        - Sierra mailbox software extension
        - Nios II / Nios V processor

    Test Description
    ----------------
    This test verifies that:
    1. A mailbox can be initialized.
    2. A task can send a message to the mailbox.
    3. peek() returns the expected payload.
    4. A second peek() returns the same payload again.
    5. peek() does not consume the message.

    Return value:
        0 = PASS
        1 = FAIL
-----------------------------------------------------------------------*/

#include "test_014A_testing_mbox_peek.h"

#include <altera_avalon_sierra_ker.h>
#include <altera_avalon_sierra_name.h>
#include <assert.h>
#include <stdint.h>

#include <sierra_extension/sierra_mbox.h>

#include "test_setup.h"

/* -------------------------------------------------------
   Global test state
------------------------------------------------------- */

static volatile int step = 0;
static volatile int shared_pas_variable = 1; /* 0 = PASS, 1 = FAIL */

static sierra_mbox_queue_t mbox;
static __attribute__ ((aligned (4))) char mbox_mem_pool[MBOX_POOL_SIZE];

static const uint32_t mbox_value = 0x12345678u;

/* -------------------------------------------------------
   Task implementations
------------------------------------------------------- */

static void task_1_code(void)
{
    assert(step == 0);
    step = 1;

    sierra_mbox_header_t header;

   

    header.id = task_1; 
    header.type = MBOX_INTEGER; 
    header.size = sizeof(mbox_value); 
    header.data = (void*)&mbox_value; 

    assert(sierra_mbox_send(&mbox, &header) == MBOX_OK); // send message to mailbox

    

    task_start(task_2);
    task_delete();
}

static void task_2_code(void)
{
    assert(step == 1);

    sierra_mbox_header_t header;
    uint32_t buffer = 0u;

    header.data = &buffer; // load buffer into header for peek 
    assert(sierra_mbox_peek(&mbox, &header) == MBOX_OK); // peek into the mailbox without removing the content
    assert(buffer == mbox_value); // check that the value is correct

    step = 2;

    buffer = 0u;           // reset buffer 
    header.data = &buffer; // load buffer into header for peek again
    assert(sierra_mbox_peek(&mbox, &header) == MBOX_OK); // same value should still be available
    assert(buffer == mbox_value); // check that peeked value is still the same

    shared_pas_variable = 0;

    task_delete();
}

/* -------------------------------------------------------
   Test entry function
------------------------------------------------------- */

int test_014A_testing_mbox_peek(void)
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

