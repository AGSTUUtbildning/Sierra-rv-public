/*------------------
-- Company: TEIS AB
-- Engineer: 
--
-- Create Date: 2026-02-24
-- History:
-- Design Name: sierra_test
-- Target Devices: ALTERA MAX 10
-- Tool versions:
-- Nios IIe Software Build Tools for Eclipse (Quartus Prime 18.1)
-- Description:
-- Test-program for Sierra.
--
------------------*/

#include <alt_types.h>
#include <system.h>
#include "Testbench_code/sierra.h"
#include "Testbench_code/test_setup.h"
#include <inttypes.h>
#include <sys/alt_exceptions.h>
#ifdef ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
    // Ifall hårdvaru exception api är igång kan hårdvaru fel hämta. 
    alt_exception_result instr_exception_handler(alt_exception_cause cause,
    alt_u32 epc, alt_u32 tval)
    {
        (void)cause;
        (void)epc;
        (void)tval;
        agstu_print_string("Instruction exception!\n");
        while (1) {};
        return NIOSV_EXCEPTION_RETURN_REISSUE_INST; 
    }
#endif

int main() {
	    #ifdef ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
	        agstu_print_string("Register Hardware exception handler...\n");
	        alt_instruction_exception_register (instr_exception_handler);
	    #endif

	    agstu_print_string("Testbench for sierra start...\n\n");

	    sierra_main();

    return 0;
}
