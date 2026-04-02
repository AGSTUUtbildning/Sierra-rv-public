/* -------------------------------------------------------
   Sierra Test Setup File
   Defines task stacks, task IDs and semaphore IDs.
------------------------------------------------------- */

#ifndef TEST_SETUP_H_
#define TEST_SETUP_H_

/* --------------------------------------------------------------------
   Test platform configuration

   SIERRA_FULL_TEST_HW_PLATFORM = 1
   A dedicated Sierra test hardware platform is available. This hardware
   is specifically designed for the regression test environment and
   supports all test cases in the framework.

   SIERRA_FULL_TEST_HW_PLATFORM = 0
   The tests are executed on a user-defined hardware platform. In this
   case, only a subset of the test cases can be executed because some
   tests require the dedicated Sierra test hardware.
   -------------------------------------------------------------------- */

#define SIERRA_FULL_TEST_HW_PLATFORM 1


/* -------------------------------------------------------
   Task stack size
------------------------------------------------------- */
#define STACK_SIZE 4096

/* -------------------------------------------------------
   Task stacks (defined in test_setup.c)
------------------------------------------------------- */
extern char idle_stack[];
extern char task_1_stack[];
extern char task_2_stack[];
extern char task_3_stack[];
extern char task_4_stack[];
extern char task_5_stack[];
extern char task_6_stack[];
extern char task_7_stack[];

/* -------------------------------------------------------
   Task IDs (Sierra)
------------------------------------------------------- */
#define task_1 1
#define task_2 2
#define task_3 3
#define task_4 4
#define task_5 5
#define task_6 6
#define task_7 7

/* -------------------------------------------------------
   Semaphore IDs
------------------------------------------------------- */
#define SEM1 0
#define SEM2 1
#define SEM3 2
#define SEM4 3
#define SEM5 4
#define SEM6 5
#define SEM7 6
#define SEM8 7

/* -------------------------------------------------------
   Flag IDs (Måste kolla extra)
------------------------------------------------------- */
#define FLAG1 0
#define FLAG2 1
#define FLAG3 2
#define FLAG4 3
#define FLAG5 4
#define FLAG6 5
#define FLAG7 6
#define FLAG8 7

// time_delay for delay before return from the tests
void time_delay(volatile int loops);
#define delay_test_constant 10 
// Adjust this value to increase or decrease in all tests before return

#if SIERRA_FULL_TEST_HW_PLATFORM
   #include <system.h>
   #include <stdio.h>
   #include <io.h>
   char* agstu_int_to_string(int nr);
   void agstu_print_char(char c);
   void agstu_print_string(char* str);
#endif

#endif /* TEST_SETUP_H_ */
