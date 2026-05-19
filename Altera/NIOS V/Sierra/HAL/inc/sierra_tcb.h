/*!
 * \file       sierra_tcb.h
 * \details    Task Control Block (TCB) definition for C usage
 * \version    11.0.0
 * \date       2026
 * \history    Modified 2026:
 *             - Removed sierra_backward_compatibility.h
 *             - Changed logging system.
 *             - RISC-V support
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

#ifndef __SIERRA_TCB_H__
#define __SIERRA_TCB_H__

#include <stdint.h>
#include "sierra_compatibility.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{

  //NIOS V
  uint32_t *sp_reg;
  void (*mepc_reg);
  uint32_t initialized;
  uint32_t taskID;
  uint32_t priority;
} tcb_t;

#ifdef __cplusplus
}
#endif

#endif /* __SIERRA_TCB_H__ */
