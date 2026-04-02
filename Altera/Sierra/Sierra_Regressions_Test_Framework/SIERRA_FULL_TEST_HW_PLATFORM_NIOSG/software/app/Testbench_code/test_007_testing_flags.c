
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test_007_testing_flags.c

  Company       :  AGSTU AB (www.agstu.com)
  Author	:  2015,Maya Isaksson (isaksson.maya@gmail.com), 
  Date Changed  :  2026
  Components    :  Sierra, real-time kernel HW and SW.
  Note          :  Förloppet byggs upp stegvis:

Description   :
Task 5 startar först, sätter test_variable=1 och sätter flagga 0001.
Det gör att Task 4 kan fortsätta. Den kontrollerar att ordningen är rätt och avslutas.
Task 6 sätter sedan test_variable=2 och flagga 0010.
Då kan Task 3 fortsätta och verifiera nästa steg.
Task 7 sätter därefter test_variable=3 och flagga 0100.
Då kan Task 2 fortsätta, verifiera ordningen, sätta test_variable=4 och sedan sätta flagga 0111.
Till sist kan Task 1 fortsätta, kontrollera att allt skett i rätt ordning och markera testet som godkänt.


COPYRIGHT (C) 2009, 2010 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.
-----------------------------------------------------------------------*/

#include "test_007_testing_flags.h"
#include <sierra_ker.h>
#include <sierra_io.h>
#include <sierra_regs.h>
#include <sierra_name.h>
#include <sierra_tcb.h>
#include <sierra_tcb.h>
#include <system.h> // for use of SOPC base-address definitons
#include <stdio.h>
#include <io.h>
#include <assert.h> //for testing, will print errors on Nios II Console if the conditions aren't met.
#include "test_setup.h"
/*TEST 007. This will create 7 tasks and make sure the tasks with higher prio waits for the right
 * flags before starting. The order of the tasks starting should be 5,4,6,3,7,2,1.
 *
 *The combination of flags tested are 0001, 0011, 0111, 1111
 */

static int test_variable=0;
static volatile int shared_pas_variable=1;

//----------------------------------------------------- Task code
static void task_1_code()
{
	sierra_await_flag(7);   //wait for flags to be 0111 /* PN 200322 */
	assert(test_variable==4);  //for stopping test if tasks do not start in right order
        shared_pas_variable=0; // 0: passed, 1:faild
	sierra_clear_flag(7);  //clear all flags for future use
	sierra_delete_task();  //deletes the task for next test to use


}
//-----------------------------------------------------

static void task_2_code(void){

	sierra_await_flag(4);  //wait for flags 0100
	assert(test_variable==3); //for stopping test if tasks do not start in right order
	sierra_clear_flag(4);  //clear flags
	test_variable=4; //change test variable
	sierra_set_flag(7);  //set flags 0111 /* PN 200322 */
	sierra_delete_task();  //delete task for future tests

}
//-----------------------------------------------------

static void task_3_code(void){

	sierra_await_flag(2); //wait for flags 0010
	assert(test_variable==2);  //for stopping test if tasks do not start in right order
	sierra_clear_flag(2);  //clear flags
	sierra_delete_task();	//delete task for future tests

}

//-----------------------------------------------------

static void task_4_code(void){

	sierra_await_flag(1);  //wait for flag 0001
	assert(test_variable==1); //to stop the test if tasks start in wrong order
	sierra_clear_flag(1);	//clear flags
	sierra_delete_task();	//delete task
}

//-----------------------------------------------------

static void task_5_code(void){
	test_variable=1;  //set test variable
	sierra_set_flag(1); //set flags 0001
	sierra_delete_task(); //delete task
}
//-----------------------------------------------------

static void task_6_code(void){
	test_variable=2; //set test variable
	sierra_set_flag(2); //set flags to 0010
	sierra_delete_task(); 	//delete task
}
//-----------------------------------------------------

static void task_7_code(void){
	test_variable=3; //set test variable
	sierra_set_flag(4);  //set flags to 0100
	sierra_delete_task(); //delete task for future tests
}
//-----------------------------------------------------

int test_007_testing_flags(){
	//Create all tasks for testing
	sierra_tsw_off();
	sierra_create_task(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
	sierra_create_task(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
	sierra_create_task(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
	sierra_create_task(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
	sierra_create_task(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
	sierra_create_task(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);
	sierra_create_task(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
	sierra_tsw_on();
	time_delay(delay_test_constant);
	return shared_pas_variable;
}
//back to idle task in main_test_file
