/*!
 * \file        sierra_info.h
 * \details     Sierra information access functions
 * \author      Lennart Lindh
 * \version     10.03.15
 * \date        2006
 * \history     Modified 2022:
 *              - Function names changed to fit the prefix format
 *              - Added function sierra_printf_version()
 * \copyright   COPYRIGHT (C) AGSTU AB
 *
 *              All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise.
 *              This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering,
 *              decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent
 *              of AGSTU AB is obtained.
 *
 *              Disclaimer:
 *              All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to
 *              implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any
 *              damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of
 *              business information, or any other pecuniary loss) arising out of the use or inability to use information (including software,
 *              designs and files) provided on this site.
 */

#ifndef __SIERRA_INFO_H__
#define __SIERRA_INFO_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! Structure used for dividing the software version number into following sections:
    -MAJOR version when you make incompatible changes,
    -MINOR version when you add functionality in a backwards-compatible manner
    -PATCH version when you make backwards-compatible bug fixes
 */
typedef union {
  struct{
    uint32_t PATCH_SW : 10;
    uint32_t MINOR_SW : 10;
    uint32_t MAJOR_SW : 12;
  } sw_version;
  uint32_t sw_version_int;
} sw_version_union;

//! Logs out (using printf) Sierra hw version from hardware register and software version.
extern void sierra_print_versions(void);

//! Logs out (using printf) Sierra information from hardware register.
extern void sierra_printf_HW_version(void);

//! Logs out (using printf) state and priority information about all created tasks.
extern void sierra_task_info(void);

#ifdef __cplusplus
}
#endif

#endif // __SIERRA_INFO_H__
