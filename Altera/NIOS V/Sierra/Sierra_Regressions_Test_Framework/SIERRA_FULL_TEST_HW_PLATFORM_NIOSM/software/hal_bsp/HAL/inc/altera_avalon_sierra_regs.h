/*!
 * \file       altera_avalon_sierra_regs.h
 * \details    Hardware-specific register access.
 * \author     Lennart Lindh
 * \version    9.4.1
 * \date       2006
 * \history    Modified 2022:
 *             - Added macro for logging time register
 * \copyright  COPYRIGHT (C) 2006 - 2021 AGSTU AB
 *
 *             All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise.
 *             This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering,
 *             decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent
 *             of AGSTU AB is obtained.
 *
 *             Disclaimer:
 *             All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to
 *             implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any
 *             damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of
 *             business information, or any other pecuniary loss) arising out of the use or inability to use information (including software,
 *             designs and files) provided on this site.
 */

#ifndef __ALTERA_AVALON_SIERRA_REGS_H__
#define __ALTERA_AVALON_SIERRA_REGS_H__

#include <altera_avalon_sierra_name.h>
#include <io.h>

// Altera HAL Macros for NIOS
#define M_Sierra_HW_Version_reg             IORD_32DIRECT(SIERRA_RTOS_BASE,0);
#define M_RD_SierraTime_base_reg            IORD_32DIRECT(SIERRA_RTOS_BASE,0x10);
#define M_IOWR_SierraTime_base_reg(data)    IOWR_32DIRECT(SIERRA_RTOS_BASE,0x10, data); // Not yet verified!
#define M_RD_Sierra_statusA_reg             IORD_32DIRECT(SIERRA_RTOS_BASE,0x20);
#define M_RD_Sierra_statusB_reg             IORD_32DIRECT(SIERRA_RTOS_BASE,0x30);
#define M_RD_Sierra_statusB_reg_utan        IORD_32DIRECT(SIERRA_RTOS_BASE,0x30) // utan att acka på IRQ, används i get_next_task
#define IOWR_ALT_SVC_REGISTER(data)         IOWR_32DIRECT(SIERRA_RTOS_BASE,0x40, data);
#define IOWR_ALT_CTRL_REGISTER(data)        IOWR_32DIRECT(SIERRA_RTOS_BASE,0x50, data);
#define M_WD_SW_RESET_REGISTER(data)        IOWR_32DIRECT(SIERRA_RTOS_BASE,0x60, data);
#define M_RD_TIME_LOGGING_REGISTER          IORD_32DIRECT(SIERRA_RTOS_BASE,0x70);

/*
 * Memory mapped reigisters, can perhaps use in the most systems
 * For Native bus sizing
 * 0x80000000 = set bit 32 to bypass cache in NIOS
 */
 
//#include <altera_avalon_sierra_io.h>
//volatile unsigned int *SierraVersion_reg    = (volatile unsigned int *) (0x80000000 | SIERRA_RTOS_BASE);
//volatile unsigned int *SierraTime_base_reg_point  = (volatile unsigned int *) (0x80000000 | (SIERRA_RTOS_BASE+0x10) );
//#define M_RD_Sierra_statusA_reg (volatile int*)(0x80000000|(SIERRA_RTOS_BASE+0x20)) 
//volatile statusA_t    *CPUstatusA_reg       = (volatile statusA_t *)    (0x80000000 | (SIERRA_RTOS_BASE+0x20) );
//volatile statusB_t    *CPUstatusB_reg       = (volatile statusB_t *)    (0x80000000 | (SIERRA_RTOS_BASE+0x30) );
//volatile svc_t        *CPUsvc_request_reg   = (volatile svc_t *)        (0x80000000 | (SIERRA_RTOS_BASE+0x40) );
//volatile ctrl_t       *CPUctrl_reg          = (volatile ctrl_t*)        (0x80000000 | (SIERRA_RTOS_BASE+0x50) );
//volatile unsigned int *CPUctrl_sw_reset_reg  = (volatile unsigned int *) (0x80000000 | (SIERRA_RTOS_BASE+0x60) );
//volatile  int *CPUctrl_sw_reset_reg  = (volatile  int *) (0x80000000 | (SIERRA_RTOS_BASE+0x50) );

#endif /* __ALTERA_AVALON_SIERRA_REGS_H__ */
