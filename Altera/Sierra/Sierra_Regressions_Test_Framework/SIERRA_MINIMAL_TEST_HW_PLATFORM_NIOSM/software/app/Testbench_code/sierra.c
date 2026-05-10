

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
#include <altera_avalon_sierra_ker.h>
#include <sierra_info.h>
#include <system.h> // for use of SOPC base-address definitons
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
#include "test_014A_testing_mbox_peek.h"
#include "test_014B_testing_mbox_read.h"
#include "test_setup.h"


// HW beroende tester ska inte kompileras utan korrekt platform. 
#if SIERRA_FULL_TEST_HW_PLATFORM
	#include "test_013_manual_irq_trigger.h"
#endif


// defining the idle task, in Sierra
#define IDLE 0

static void print_test_result(const char* name, int result)
{
	if (result == 0) {
		agstu_print_string(name);
		agstu_print_string(" PASSED\n");
	} else {
		agstu_print_string(name);
		agstu_print_string(" Failed\n");
		assert(0);
	}
}

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

	// Skriva ut HW version.
	Printf_sierra_HW_version();

	while(1){
		print_test_result("Test 1", test_001_testing_task_create());
		print_test_result("Test 2", test_002_testing_task_delete());
		print_test_result("Test 3", test_003_testing_task_block());
		print_test_result("Test 4", test_004_testing_priority());
		print_test_result("Test 5", test_005_testing_task_start());
		print_test_result("Test 6", test_006_testing_sem_take_and_release());
		print_test_result("Test 6B", test_006B_testing_sem_take_and_release());
		print_test_result("Test 7", test_007_testing_flags());
		print_test_result("Test 8", test_008_testing_time_management());
		print_test_result("Test 9", test_009_testing_delay());
		print_test_result("Test 10", test_010_testing_change_prio());
		print_test_result("Test 11", test_011_testing_task_block());
		print_test_result("Test 12", test_012_testing_change_prio_same());
		print_test_result("Test 12B", test_012B_testing_change_prio_same());
		#if SIERRA_FULL_TEST_HW_PLATFORM
			print_test_result("Test 13", test_013_manual_irq_trigger());
			IOWR_32DIRECT(PIO_LOOP_COUNTER_BASE, 0, counter + 1);
		#endif
		print_test_result("Test 14A", test_014A_testing_mbox_peek());
		print_test_result("Test 14B", test_014B_testing_mbox_read());
		counter++;
		agstu_print_string("\nLoop nr ");
		agstu_print_string(agstu_int_to_string(counter));
		agstu_print_char('\n');
		agstu_print_char('\n');
	}
}
/*!-----------------------------------------------------
    Main
-------------------------------------------------------*/

void sierra_main(void)
{
	Sierra_Initiation_HW_and_SW();
	/*********************************************************************
	 * Initialize time base register.
	 * This example     : 50 MHz system-clock
	 * Wanted tick time : 20 ms (50Hz)
	 * Formula gives    : 20 ms x 50 MHx / 1000 => 1000(dec)
	 * ******************************************************************/
	set_timebase(1000);

	//Create the idle task here
	task_create(IDLE, 0, READY_TASK_STATE, idle_task_code, idle_stack, STACK_SIZE);
	// Start the Sierra scheduler
	tsw_on();

	while(1) {
		// Should never end up here...!
		agstu_print_string("* ERROR! SYSTEM FAILED *\n ");
	}
}
