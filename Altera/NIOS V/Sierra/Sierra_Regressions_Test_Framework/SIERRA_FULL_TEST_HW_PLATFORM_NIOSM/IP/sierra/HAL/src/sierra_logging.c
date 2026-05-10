/*!
 * \file       sierra_logging.c
 * \details    This file contains:
 *             - the logging framework that retrieves data from each logging macro
 * \author(s)  Alexander Eriksson & Jens Lind
 * \version    1.0
 * \date       2022
 * \copyright  COPYRIGHT (C) AGSTU AB
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

/* Standard Include. */
#include <stdio.h>
#include <stdint.h>
#include <sierra.h>
#include <sys/alt_stdio.h>

/* Include header for logging subsystem. */
#include "sierra_logging.h"

//----------------------------------------------------------------------------
void sierra_print_logging_status(void)
{
#if SIERRA_LOGGING > 1
  printf("Logging interface IS active with timestamps!\n");
#elif SIERRA_LOGGING > 0
  //printf("Logging interface IS active!\n");
#else
  printf("Logging interface is NOT active!\n");
#endif
}

//----------------------------------------------------------------------------
void sierra_log_info(const char* szMsg)
{
#if SIERRA_LOGGING > 1
  // Prints the results with time
  printf("LOG_INFO: %s TIME: %lu \n", szMsg, sierra_get_current_time());
#elif SIERRA_LOGGING > 0
  // Prints the results without time
  printf("LOG_INFO: %s \n", szMsg);
#endif
}

//----------------------------------------------------------------------------
void sierra_log_warn(const char* szMsg)
{
#if SIERRA_LOGGING > 1
  // Prints the results with time
  printf("LOG_WARN: %s TIME: %lu \n", szMsg, sierra_get_current_time());
#elif SIERRA_LOGGING > 0
  // Prints the results without time
  printf("LOG_WARN: %s \n", szMsg);
#endif
}

//----------------------------------------------------------------------------

void sierra_log_error(const char* szMsg)
{
#if SIERRA_LOGGING > 1
  // Prints the results with
  printf("LOG_ERROR: %s TIME: %lu \n", szMsg, sierra_get_current_time());
#elif SIERRA_LOGGING > 0
  // Prints the results without time
  printf("LOG_INFO: %s \n", szMsg);
#endif
}
