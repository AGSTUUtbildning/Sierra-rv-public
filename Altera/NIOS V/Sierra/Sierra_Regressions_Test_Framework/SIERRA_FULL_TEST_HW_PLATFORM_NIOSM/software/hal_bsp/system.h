/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'cpu' in SOPC Builder design 'sierra_test_niosvg_m_hw'
 * SOPC Builder design path: E:/Utveklingsprojekt/sierra_test/Sierra_test_NIOS_V_M/sierra_test_niosvg_m_hw.sopcinfo
 *
 * Generated: Sun Mar 15 11:02:11 CET 2026
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "intel_niosv_m"
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_DATA_ADDR_WIDTH 0x20
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HAS_CSR_SUPPORT 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_ICACHE_LINE_SIZE 0
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_ICACHE_SIZE 0
#define ALT_CPU_INST_ADDR_WIDTH 0x20
#define ALT_CPU_INT_MODE 0
#define ALT_CPU_MTIME_OFFSET 0x08040400
#define ALT_CPU_NAME "cpu"
#define ALT_CPU_NIOSV_CORE_VARIANT 1
#define ALT_CPU_NUM_GPR 32
#define ALT_CPU_RESET_ADDR 0x04000000
#define ALT_CPU_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define ALT_CPU_TIMER_DEVICE_TYPE 2


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define ABBOTTSLAKE_CPU_FREQ 50000000u
#define ABBOTTSLAKE_DATA_ADDR_WIDTH 0x20
#define ABBOTTSLAKE_DCACHE_LINE_SIZE 0
#define ABBOTTSLAKE_DCACHE_LINE_SIZE_LOG2 0
#define ABBOTTSLAKE_DCACHE_SIZE 0
#define ABBOTTSLAKE_HAS_CSR_SUPPORT 1
#define ABBOTTSLAKE_HAS_DEBUG_STUB
#define ABBOTTSLAKE_ICACHE_LINE_SIZE 0
#define ABBOTTSLAKE_ICACHE_LINE_SIZE_LOG2 0
#define ABBOTTSLAKE_ICACHE_SIZE 0
#define ABBOTTSLAKE_INST_ADDR_WIDTH 0x20
#define ABBOTTSLAKE_INT_MODE 0
#define ABBOTTSLAKE_MTIME_OFFSET 0x08040400
#define ABBOTTSLAKE_NIOSV_CORE_VARIANT 1
#define ABBOTTSLAKE_NUM_GPR 32
#define ABBOTTSLAKE_RESET_ADDR 0x04000000
#define ABBOTTSLAKE_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define ABBOTTSLAKE_TIMER_DEVICE_TYPE 2


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTPLL
#define __INTEL_NIOSV_M
#define __SIERRA
#define __TIMER_HW_IP


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "MAX 10"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0x80404a0
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0x80404a0
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0x80404a0
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "sierra_test_niosvg_m_hw"
#define ALT_SYS_CLK_TICKS_PER_SEC ALT_CPU_TICKS_PER_SEC
#define ALT_TIMESTAMP_CLK_TIMER_DEVICE_TYPE ALT_CPU_TIMER_DEVICE_TYPE


/*
 * TIMER_HW_IP_0 configuration
 *
 */

#define ALT_MODULE_CLASS_TIMER_HW_IP_0 TIMER_HW_IP
#define TIMER_HW_IP_0_BASE 0x8040480
#define TIMER_HW_IP_0_IRQ -1
#define TIMER_HW_IP_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define TIMER_HW_IP_0_NAME "/dev/TIMER_HW_IP_0"
#define TIMER_HW_IP_0_SPAN 16
#define TIMER_HW_IP_0_TYPE "TIMER_HW_IP"


/*
 * cpu_dm_agent configuration
 *
 */

#define ALT_MODULE_CLASS_cpu_dm_agent intel_niosv_m
#define CPU_DM_AGENT_BASE 0x8030000
#define CPU_DM_AGENT_CPU_FREQ 50000000u
#define CPU_DM_AGENT_DATA_ADDR_WIDTH 0x20
#define CPU_DM_AGENT_DCACHE_LINE_SIZE 0
#define CPU_DM_AGENT_DCACHE_LINE_SIZE_LOG2 0
#define CPU_DM_AGENT_DCACHE_SIZE 0
#define CPU_DM_AGENT_HAS_CSR_SUPPORT 1
#define CPU_DM_AGENT_HAS_DEBUG_STUB
#define CPU_DM_AGENT_ICACHE_LINE_SIZE 0
#define CPU_DM_AGENT_ICACHE_LINE_SIZE_LOG2 0
#define CPU_DM_AGENT_ICACHE_SIZE 0
#define CPU_DM_AGENT_INST_ADDR_WIDTH 0x20
#define CPU_DM_AGENT_INTERRUPT_CONTROLLER_ID 0
#define CPU_DM_AGENT_INT_MODE 0
#define CPU_DM_AGENT_IRQ -1
#define CPU_DM_AGENT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CPU_DM_AGENT_MTIME_OFFSET 0x08040400
#define CPU_DM_AGENT_NAME "/dev/cpu_dm_agent"
#define CPU_DM_AGENT_NIOSV_CORE_VARIANT 1
#define CPU_DM_AGENT_NUM_GPR 32
#define CPU_DM_AGENT_RESET_ADDR 0x04000000
#define CPU_DM_AGENT_SPAN 65536
#define CPU_DM_AGENT_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define CPU_DM_AGENT_TIMER_DEVICE_TYPE 2
#define CPU_DM_AGENT_TYPE "intel_niosv_m"


/*
 * cpu_timer_sw_agent configuration
 *
 */

#define ALT_MODULE_CLASS_cpu_timer_sw_agent intel_niosv_m
#define CPU_TIMER_SW_AGENT_BASE 0x8040400
#define CPU_TIMER_SW_AGENT_CPU_FREQ 50000000u
#define CPU_TIMER_SW_AGENT_DATA_ADDR_WIDTH 0x20
#define CPU_TIMER_SW_AGENT_DCACHE_LINE_SIZE 0
#define CPU_TIMER_SW_AGENT_DCACHE_LINE_SIZE_LOG2 0
#define CPU_TIMER_SW_AGENT_DCACHE_SIZE 0
#define CPU_TIMER_SW_AGENT_HAS_CSR_SUPPORT 1
#define CPU_TIMER_SW_AGENT_HAS_DEBUG_STUB
#define CPU_TIMER_SW_AGENT_ICACHE_LINE_SIZE 0
#define CPU_TIMER_SW_AGENT_ICACHE_LINE_SIZE_LOG2 0
#define CPU_TIMER_SW_AGENT_ICACHE_SIZE 0
#define CPU_TIMER_SW_AGENT_INST_ADDR_WIDTH 0x20
#define CPU_TIMER_SW_AGENT_INTERRUPT_CONTROLLER_ID 0
#define CPU_TIMER_SW_AGENT_INT_MODE 0
#define CPU_TIMER_SW_AGENT_IRQ -1
#define CPU_TIMER_SW_AGENT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CPU_TIMER_SW_AGENT_MTIME_OFFSET 0x08040400
#define CPU_TIMER_SW_AGENT_NAME "/dev/cpu_timer_sw_agent"
#define CPU_TIMER_SW_AGENT_NIOSV_CORE_VARIANT 1
#define CPU_TIMER_SW_AGENT_NUM_GPR 32
#define CPU_TIMER_SW_AGENT_RESET_ADDR 0x04000000
#define CPU_TIMER_SW_AGENT_SPAN 64
#define CPU_TIMER_SW_AGENT_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define CPU_TIMER_SW_AGENT_TIMER_DEVICE_TYPE 2
#define CPU_TIMER_SW_AGENT_TYPE "intel_niosv_m"


/*
 * hal2 configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK CPU
#define ALT_TIMESTAMP_CLK CPU
#define INTEL_FPGA_DFL_START_ADDRESS 0xffffffffffffffff
#define INTEL_FPGA_USE_DFL_WALKER 0


/*
 * intel_niosv_m_hal_driver configuration
 *
 */

#define NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND 1000


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0x80404a0
#define JTAG_UART_IRQ 1
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 8
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * onchip_memoy configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memoy altera_avalon_onchip_memory2
#define ONCHIP_MEMOY_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMOY_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMOY_BASE 0x8020000
#define ONCHIP_MEMOY_CONTENTS_INFO ""
#define ONCHIP_MEMOY_DUAL_PORT 0
#define ONCHIP_MEMOY_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMOY_INIT_CONTENTS_FILE "sierra_test_niosvg_m_hw_onchip_memoy"
#define ONCHIP_MEMOY_INIT_MEM_CONTENT 0
#define ONCHIP_MEMOY_INSTANCE_ID "NONE"
#define ONCHIP_MEMOY_IRQ -1
#define ONCHIP_MEMOY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMOY_NAME "/dev/onchip_memoy"
#define ONCHIP_MEMOY_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMOY_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMOY_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMOY_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMOY_SIZE_MULTIPLE 1
#define ONCHIP_MEMOY_SIZE_VALUE 65536
#define ONCHIP_MEMOY_SPAN 65536
#define ONCHIP_MEMOY_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMOY_WRITABLE 1


/*
 * pio_irq_buttons configuration
 *
 */

#define ALT_MODULE_CLASS_pio_irq_buttons altera_avalon_pio
#define PIO_IRQ_BUTTONS_BASE 0x8040460
#define PIO_IRQ_BUTTONS_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_IRQ_BUTTONS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_IRQ_BUTTONS_CAPTURE 1
#define PIO_IRQ_BUTTONS_DATA_WIDTH 2
#define PIO_IRQ_BUTTONS_DO_TEST_BENCH_WIRING 0
#define PIO_IRQ_BUTTONS_DRIVEN_SIM_VALUE 0
#define PIO_IRQ_BUTTONS_EDGE_TYPE "FALLING"
#define PIO_IRQ_BUTTONS_FREQ 50000000
#define PIO_IRQ_BUTTONS_HAS_IN 1
#define PIO_IRQ_BUTTONS_HAS_OUT 0
#define PIO_IRQ_BUTTONS_HAS_TRI 0
#define PIO_IRQ_BUTTONS_IRQ 2
#define PIO_IRQ_BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID 0
#define PIO_IRQ_BUTTONS_IRQ_TYPE "EDGE"
#define PIO_IRQ_BUTTONS_NAME "/dev/pio_irq_buttons"
#define PIO_IRQ_BUTTONS_RESET_VALUE 0
#define PIO_IRQ_BUTTONS_SPAN 16
#define PIO_IRQ_BUTTONS_TYPE "altera_avalon_pio"


/*
 * pio_loop_counter configuration
 *
 */

#define ALT_MODULE_CLASS_pio_loop_counter altera_avalon_pio
#define PIO_LOOP_COUNTER_BASE 0x8040470
#define PIO_LOOP_COUNTER_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_LOOP_COUNTER_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_LOOP_COUNTER_CAPTURE 0
#define PIO_LOOP_COUNTER_DATA_WIDTH 32
#define PIO_LOOP_COUNTER_DO_TEST_BENCH_WIRING 0
#define PIO_LOOP_COUNTER_DRIVEN_SIM_VALUE 0
#define PIO_LOOP_COUNTER_EDGE_TYPE "NONE"
#define PIO_LOOP_COUNTER_FREQ 50000000
#define PIO_LOOP_COUNTER_HAS_IN 0
#define PIO_LOOP_COUNTER_HAS_OUT 1
#define PIO_LOOP_COUNTER_HAS_TRI 0
#define PIO_LOOP_COUNTER_IRQ -1
#define PIO_LOOP_COUNTER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_LOOP_COUNTER_IRQ_TYPE "NONE"
#define PIO_LOOP_COUNTER_NAME "/dev/pio_loop_counter"
#define PIO_LOOP_COUNTER_RESET_VALUE 0
#define PIO_LOOP_COUNTER_SPAN 16
#define PIO_LOOP_COUNTER_TYPE "altera_avalon_pio"


/*
 * pio_sierra_extriq configuration
 *
 */

#define ALT_MODULE_CLASS_pio_sierra_extriq altera_avalon_pio
#define PIO_SIERRA_EXTRIQ_BASE 0x8040440
#define PIO_SIERRA_EXTRIQ_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_SIERRA_EXTRIQ_BIT_MODIFYING_OUTPUT_REGISTER 1
#define PIO_SIERRA_EXTRIQ_CAPTURE 0
#define PIO_SIERRA_EXTRIQ_DATA_WIDTH 2
#define PIO_SIERRA_EXTRIQ_DO_TEST_BENCH_WIRING 0
#define PIO_SIERRA_EXTRIQ_DRIVEN_SIM_VALUE 0
#define PIO_SIERRA_EXTRIQ_EDGE_TYPE "NONE"
#define PIO_SIERRA_EXTRIQ_FREQ 50000000
#define PIO_SIERRA_EXTRIQ_HAS_IN 0
#define PIO_SIERRA_EXTRIQ_HAS_OUT 1
#define PIO_SIERRA_EXTRIQ_HAS_TRI 0
#define PIO_SIERRA_EXTRIQ_IRQ -1
#define PIO_SIERRA_EXTRIQ_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_SIERRA_EXTRIQ_IRQ_TYPE "NONE"
#define PIO_SIERRA_EXTRIQ_NAME "/dev/pio_sierra_extriq"
#define PIO_SIERRA_EXTRIQ_RESET_VALUE 0
#define PIO_SIERRA_EXTRIQ_SPAN 32
#define PIO_SIERRA_EXTRIQ_TYPE "altera_avalon_pio"


/*
 * pll configuration
 *
 */

#define ALT_MODULE_CLASS_pll altpll
#define PLL_BASE 0x8040490
#define PLL_IRQ -1
#define PLL_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PLL_NAME "/dev/pll"
#define PLL_SPAN 16
#define PLL_TYPE "altpll"


/*
 * sdram_controller configuration
 *
 */

#define ALT_MODULE_CLASS_sdram_controller altera_avalon_new_sdram_controller
#define SDRAM_CONTROLLER_BASE 0x4000000
#define SDRAM_CONTROLLER_CAS_LATENCY 3
#define SDRAM_CONTROLLER_CONTENTS_INFO
#define SDRAM_CONTROLLER_INIT_NOP_DELAY 0.0
#define SDRAM_CONTROLLER_INIT_REFRESH_COMMANDS 2
#define SDRAM_CONTROLLER_IRQ -1
#define SDRAM_CONTROLLER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_CONTROLLER_IS_INITIALIZED 1
#define SDRAM_CONTROLLER_NAME "/dev/sdram_controller"
#define SDRAM_CONTROLLER_POWERUP_DELAY 100.0
#define SDRAM_CONTROLLER_REFRESH_PERIOD 7.8125
#define SDRAM_CONTROLLER_REGISTER_DATA_IN 1
#define SDRAM_CONTROLLER_SDRAM_ADDR_WIDTH 0x19
#define SDRAM_CONTROLLER_SDRAM_BANK_WIDTH 2
#define SDRAM_CONTROLLER_SDRAM_COL_WIDTH 10
#define SDRAM_CONTROLLER_SDRAM_DATA_WIDTH 16
#define SDRAM_CONTROLLER_SDRAM_NUM_BANKS 4
#define SDRAM_CONTROLLER_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_CONTROLLER_SDRAM_ROW_WIDTH 13
#define SDRAM_CONTROLLER_SHARED_DATA 0
#define SDRAM_CONTROLLER_SIM_MODEL_BASE 0
#define SDRAM_CONTROLLER_SPAN 67108864
#define SDRAM_CONTROLLER_STARVATION_INDICATOR 0
#define SDRAM_CONTROLLER_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_CONTROLLER_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_CONTROLLER_T_AC 5.4
#define SDRAM_CONTROLLER_T_MRD 3
#define SDRAM_CONTROLLER_T_RCD 15.0
#define SDRAM_CONTROLLER_T_RFC 70.0
#define SDRAM_CONTROLLER_T_RP 15.0
#define SDRAM_CONTROLLER_T_WR 14.0


/*
 * sierra_0 configuration
 *
 */

#define ALT_MODULE_CLASS_sierra_0 sierra
#define SIERRA_0_BASE 0x8040000
#define SIERRA_0_IRQ 0
#define SIERRA_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SIERRA_0_NAME "/dev/sierra_0"
#define SIERRA_0_SPAN 1024
#define SIERRA_0_TYPE "sierra"

#endif /* __SYSTEM_H_ */
