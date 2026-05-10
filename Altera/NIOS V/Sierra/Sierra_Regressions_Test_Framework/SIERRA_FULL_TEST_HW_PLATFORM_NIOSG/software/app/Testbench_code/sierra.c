

/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  main_test_file.c (sierra.c), varför ändrat namn?

  Company       :  AGSTU AB (www.agstu.com)
  Author	:  Maya Isaksson (isaksson.maya@gmail.com) (2015)
		   Lennart (2026), Change to write only in ideal task, NIOS V version M and G.
  Date Changed  :  20260307 - uppdaterad
  System
  Components    :  Sierra, real-time kernel HW and SW.
  Note          :  For full test a test HW have to be used, in other case skipp the External IRQ tests.

-----------------------------------------------------
  Description:
  This is the main file used for testing the different function calls in the Sierra. 
  For more information see: Sierra Testbench Reference Manual

-------------------------------------------------------

COPYRIGHT (C) 2009, 2010, 2015, 2026 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.

-----------------------------------------------------------------------*/

/* Sierra Driver Includes, hardware base adresses and also C libary */
#include <sierra_ker.h>
#include <sierra_io.h>
#include <sierra_regs.h>
#include <sierra_name.h>
#include <sierra_tcb.h>
#include <sierra_tcb.h>
#include <sierra_logging.h>
#include <system.h> // for use of SOPC base-address definitons
#include <stdio.h>
#include <io.h>
#include <assert.h> //for testing, will print errors on Nios II Console if the conditions aren't met.

//All test files and test_setup.h needs to be included
#include "test_001_testing_task_create.h"
#include "test_002_testing_task_delete.h"
#include "test_003_testing_task_block.h"
#include "test_004_testing_priority.h"
#include "test_005_testing_task_start.h"
#include "test_006_testing_sem_take_and_release.h"
#include "test_006B_testing_sem_take_and_release.h"
#include "test_007_testing_flags.h"
#include "test_008_testing_time_management.h"
#include "test_009_testing_delay.h"
#include "test_010_testing_task_change_prio.h"
#include "test_011_testing_task_block.h"
#include "test_012_testing_task_change_prio_same.h"
#include "test_012B_testing_task_change_prio_same.h"

#include "test_setup.h"
#include <sierra_info.h>


#ifdef ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
	#include <sys/alt_exceptions.h>
	#include <sys/alt_stdio.h>
	#include <inttypes.h>
    // Ifall hårdvaru exception api är igång kan hårdvaru fel hämta. 
    alt_exception_result instr_exception_handler(alt_exception_cause cause,
    alt_u32 epc, alt_u32 tval)
    {
        alt_printf("Instruction exception!\n");
        alt_printf(" * cause: %d\n", cause);
        alt_printf(" * epc: 0x%" PRIx32 "\n", epc);
        alt_printf(" * tval: 0x%" PRIx32 "\n", tval);
        while (1) {};
        return NIOSV_EXCEPTION_RETURN_REISSUE_INST; 
    }
#endif


// HW beroende tester ska inte kompileras utan korrekt platform. 
#if SIERRA_FULL_TEST_HW_PLATFORM
	#include "test_013_manual_irq_trigger.h"
#endif


// defining the idle task, in Sierra
#define IDLE 0

// TASK STACKS

__attribute__ ((aligned (4))) char idle_stack[STACK_SIZE];
__attribute__ ((aligned (4))) char task_1_stack[STACK_SIZE];
__attribute__ ((aligned (4))) char task_2_stack[STACK_SIZE];
__attribute__ ((aligned (4))) char task_3_stack[STACK_SIZE];
__attribute__ ((aligned (4))) char task_4_stack[STACK_SIZE];
__attribute__ ((aligned (4))) char task_5_stack[STACK_SIZE];
__attribute__ ((aligned (4))) char task_6_stack[STACK_SIZE];
__attribute__ ((aligned (4))) char task_7_stack[STACK_SIZE];


//All tests runs from the idle task. Comment out the ones you don't want to run.
void idle_task_code(void){
	// Kod som har med test platformen som bara körs en gång.

	int counter = 0;

	// Skriva ut SW version!! 
	sierra_printf_HW_version();

	while(1){
	#if !SIERRA_FULL_TEST_HW_PLATFORM
		if (test_001_testing_task_create() == 0)			{printf("Test 1 PASSED\n");} 	else {printf("Test 1 Failed\n");	assert(0);}
		if (test_002_testing_task_delete() == 0)			{printf("Test 2 PASSED\n");} 	else {printf("Test 2 Failed\n");	assert(0);}
		if (test_003_testing_task_block() == 0)				{printf("Test 3 PASSED\n");} 	else {printf("Test 3 Failed\n");	assert(0);}
		if (test_004_testing_priority() == 0)				{printf("Test 4 PASSED\n");} 	else {printf("Test 4 Failed\n");	assert(0);}
		if (test_005_testing_task_start() == 0)				{printf("Test 5 PASSED\n");} 	else {printf("Test 5 Failed\n");	assert(0);}
		if (test_006_testing_sem_take_and_release() == 0)	{printf("Test 6 PASSED\n");} 	else {printf("Test 6 Failed\n");	assert(0);}
		if (test_006B_testing_sem_take_and_release() == 0)	{printf("Test 6B PASSED\n");} 	else {printf("Test 6B Failed\n");	assert(0);}
		if (test_007_testing_flags() == 0)					{printf("Test 7 PASSED\n");} 	else {printf("Test 7 Failed\n");	assert(0);}
		if (test_008_testing_time_management() == 0)		{printf("Test 8 PASSED\n");} 	else {printf("Test 8 Failed\n");	assert(0);}
		if (test_009_testing_delay() == 0)					{printf("Test 9 PASSED\n");} 	else {printf("Test 9 Failed\n");	assert(0);}
		if (test_010_testing_change_prio() == 0)			{printf("Test 10 PASSED\n");} 	else {printf("Test 10 Failed\n");	assert(0);}
		if (test_011_testing_task_block() == 0)				{printf("Test 11 PASSED\n");} 	else {printf("Test 11 Failed\n");	assert(0);}
		if (test_012_testing_change_prio_same() == 0)		{printf("Test 12 PASSED\n");} 	else {printf("Test 12 Failed\n");	assert(0);}
		if (test_012B_testing_change_prio_same() == 0)		{printf("Test 12B PASSED\n");} 	else {printf("Test 12b Failed\n");	assert(0);}
		counter++;
		printf("\nLoop nr %d\n\n", counter);
	#else
			if (test_001_testing_task_create() == 0)			{agstu_print_string("Test 1 PASSED\n");} 	else {agstu_print_string("Test 1 Failed\n");	assert(0);}
			if (test_002_testing_task_delete() == 0)			{agstu_print_string("Test 2 PASSED\n");} 	else {agstu_print_string("Test 2 Failed\n");	assert(0);}
			if (test_003_testing_task_block() == 0)				{agstu_print_string("Test 3 PASSED\n");} 	else {agstu_print_string("Test 3 Failed\n");	assert(0);}
			if (test_004_testing_priority() == 0)				{agstu_print_string("Test 4 PASSED\n");} 	else {agstu_print_string("Test 4 Failed\n");	assert(0);}
			if (test_005_testing_task_start() == 0)				{agstu_print_string("Test 5 PASSED\n");} 	else {agstu_print_string("Test 5 Failed\n");	assert(0);}
			if (test_006_testing_sem_take_and_release() == 0)	{agstu_print_string("Test 6 PASSED\n");} 	else {agstu_print_string("Test 6 Failed\n");	assert(0);}
			if (test_006B_testing_sem_take_and_release() == 0)	{agstu_print_string("Test 6B PASSED\n");} 	else {agstu_print_string("Test 6B Failed\n");	assert(0);} // slow
			if (test_007_testing_flags() == 0)					{agstu_print_string("Test 7 PASSED\n");} 	else {agstu_print_string("Test 7 Failed\n");	assert(0);}
			if (test_008_testing_time_management() == 0)		{agstu_print_string("Test 8 PASSED\n");} 	else {agstu_print_string("Test 8 Failed\n");	assert(0);}
			if (test_009_testing_delay() == 0)					{agstu_print_string("Test 9 PASSED\n");} 	else {agstu_print_string("Test 9 Failed\n");	assert(0);} // slow
			if (test_010_testing_change_prio() == 0)			{agstu_print_string("Test 10 PASSED\n");} 	else {agstu_print_string("Test 10 Failed\n");	assert(0);}
			if (test_011_testing_task_block() == 0)				{agstu_print_string("Test 11 PASSED\n");} 	else {agstu_print_string("Test 11 Failed\n");	assert(0);}
			if (test_012_testing_change_prio_same() == 0)		{agstu_print_string("Test 12 PASSED\n");} 	else {agstu_print_string("Test 12 Failed\n");	assert(0);} // slow
			if (test_012B_testing_change_prio_same() == 0)		{agstu_print_string("Test 12B PASSED\n");} 	else {agstu_print_string("Test 12b Failed\n");	assert(0);} // slow
			if (test_013_manual_irq_trigger() == 0)				{agstu_print_string("Test 13 PASSED\n");} 	else {agstu_print_string("Test 13 Failed\n");	assert(0);} // slow
			counter++;
			IOWR_32DIRECT(PIO_LOOP_COUNTER_BASE, 0, counter);
			sierra_print_log();
			agstu_print_string("\nLoop nr ");agstu_print_string(agstu_int_to_string(counter));agstu_print_char('\n');agstu_print_char('\n');
	#endif
	}
}
/*!-----------------------------------------------------
    Main
-------------------------------------------------------*/

void sierra_main(void)
{

	#ifdef ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
        printf("Register HW exception handler...\n");
        alt_instruction_exception_register (instr_exception_handler);
    #endif

	sierra_initiation_HW_and_SW();
	/*********************************************************************
	 * Initialize time base register.
	 * This example     : 50 MHz system-clock
	 * Wanted tick time : 20 ms (50Hz)
	 * Formula gives    : 20 ms x 50 MHx / 1000 => 1000(dec)
	 * ******************************************************************/
	sierra_set_timebase(1000);

	//Create the idle task here
	sierra_create_task(IDLE, 0, READY_TASK_STATE, idle_task_code, idle_stack, STACK_SIZE);
	// Start the Sierra scheduler
	sierra_tsw_on();

	while(1) {
		// Should never end up here...!
		printf ("* ERROR! SYSTEM FAILED *\n ");
	}
}



