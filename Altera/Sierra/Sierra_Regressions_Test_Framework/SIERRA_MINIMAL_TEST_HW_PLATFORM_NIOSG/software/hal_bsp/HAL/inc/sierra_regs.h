/*!
 * \file       sierra_regs.h
 * \details    Hardware-specific register access.
 * \version    11.0.0
 * \date       2026
 * \history    Modified 2026:
 *             - Removed sierra_backward_compatibility.h
 *             - Changed logging system.
 *             - RISC-V support
 * \copyright  COPYRIGHT (C) 2006 - 2026 AGSTU AB
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

#ifndef ___SIERRA_REGS_H__
#define ___SIERRA_REGS_H__

#include <altera_avalon_sierra_name.h>
#include <io.h>

// HAL Macros for NIOS
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

#endif /* __SIERRA_REGS_H__ */
