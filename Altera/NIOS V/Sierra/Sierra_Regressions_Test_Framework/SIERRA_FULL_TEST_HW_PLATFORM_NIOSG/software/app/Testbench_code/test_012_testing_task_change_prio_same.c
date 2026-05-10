
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test012_testing_change_prio_same();

  Company       :  AGSTU AB (www.agstu.com)
  Author	:  Lennart Lindh
  Date Changed  :  2016, 2026
  Description   :  System to test functions in Sierra.
Två tasks skapas:
Task 1 startar först eftersom den har högre prioritet. Under körningen ändrar Task 1 prioriteten på Task 2 till en högre prioritet än sin egen genom task_change_prio().
Detta ska omedelbart orsaka ett context switch, vilket gör att Task 2 börjar köras.
Task 2 verifierar att den startade vid rätt tidpunkt och uppdaterar en testvariabel innan den avslutas.
Därefter återgår exekveringen till Task 1, som kontrollerar att taskväxlingen verkligen skedde.

  System
  Components    :  Sierra, real-time kernel HW and SW.

COPYRIGHT (C) AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.
-----------------------------------------------------------------------*/

#include "test_012_testing_task_change_prio_same.h"
#include <sierra_ker.h>
#include <sierra_io.h>
#include <sierra_regs.h>
#include <sierra_name.h>
#include <sierra_tcb.h>
#include <sierra_tcb.h>
#include <system.h> // for use of SOPC base-address definitions
#include <stdio.h>
#include <io.h>
#include <assert.h> //for testing, will print errors on Nios II Console if the conditions aren't met.
#include "test_setup.h"
#include <sierra_info.h> /* PN 200320 */

static volatile int test_priority_012=0;
static volatile int shared_pas_variable=1; // 0: passed, 1:faild

// task_1 have priority 7 and then priority 3
static void task_1_code(void){
	int i;
	assert(test_priority_012==0);

	test_priority_012=1;
	sierra_change_task_prio(task_2, 7); //change task_2 to higher priority then task_1, should cause a task switch and task_2 should start running

	for(i=0; i<500000; i++);
	for(i=0; i<500000; i++);

	assert(test_priority_012==2); // Coming back from task_2
	shared_pas_variable=0; 

	sierra_delete_task();			//deleting the task to free it up for other tests
}
//-----------------------------------------------------

static void task_2_code(void){
	assert(test_priority_012==1);
	test_priority_012=2;
	sierra_delete_task();
}


int test_012_testing_change_prio_same(){
	test_priority_012=0;

	sierra_tsw_off(); //shut down the scheduler to create all new tests
	sierra_create_task(task_1, 6, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
	sierra_create_task(task_2, 3, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
	sierra_tsw_on(); //start the scheduler again

	time_delay(delay_test_constant);
	
	return shared_pas_variable;
}
