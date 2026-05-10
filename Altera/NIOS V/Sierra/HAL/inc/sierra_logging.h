/*!
 * \file       sierra_logging.h
 * \details    This file contains:
 *             - logging macros and functions for the Sierra logging interface.
 *             - the user can choose logging level by enabling the desired logging macro.
 *             - the maximum number of characters for each level is 100.
 * \author     Alexander Eriksson & Jens Lind
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

#ifndef __SIERRA_LOGGING_H__
#define __SIERRA_LOGGING_H__

#include <stdio.h>

/*!
 * #SIERRA_LOGGING macro turns on/off the logging interface.
 * The user can additionally add timestamps from sierra_get_current_time() to the log by setting the macro to 2 or higher.
 */
#define SIERRA_LOGGING 0

#if SIERRA_LOGGING > 0

#define SIERRA_MAX_LOG_CHARACTERS 100

/*!
 * #SIERRA_LOG_INFO is the default level for the logging interface.
 * The macro logs events during normal execution of a library.
 * The level provides descriptive data of how the system operates.
 */
#define SIERRA_LOG_INFO(...)\
{\
  char formatMsg[SIERRA_MAX_LOG_CHARACTERS];\
  sprintf(formatMsg, __VA_ARGS__);\
  sierra_log_info(formatMsg);\
}

/*!
 * #SIERRA_LOG_WARN alerts the user about abnormal events and potential problems.
 * The macro logs incidents prior to execution and during runtime.
 * The system will continue to operate as normal even after a warning is generated.
 * A warning will prompt the user to take action before it turns into an error.
 */
#define SIERRA_LOG_WARN(...)\
{\
  char formatMsg[SIERRA_MAX_LOG_CHARACTERS];\
  sprintf(formatMsg, __VA_ARGS__);\
  sierra_log_warn(formatMsg);\
}

/*!
 * #SIERRA_LOG_ERROR alerts the user that the system has encountered an error from which it cannot recover.
 * Error messages notify the user of problems that need to be addressed immediately.
 */
#define SIERRA_LOG_ERROR(...)\
{\
  char formatMsg[SIERRA_MAX_LOG_CHARACTERS];\
  sprintf(formatMsg, __VA_ARGS__);\
  sierra_log_error(formatMsg);\
}

#else

///@{
//! Defines empty macros if SIERRA_LOGGING is undefined
#define SIERRA_LOG_INFO(...)
#define SIERRA_LOG_WARN(...)
#define SIERRA_LOG_ERROR(...)
///@}

#endif // 

//! \brief Prints logging status.
extern void sierra_print_logging_status(void);

/*! \brief   Prints informative messages.
 *  \details \par Description
 *           The function prints data from Sierra functions during normal execution of a library.
 */
extern void sierra_log_info(const char* szMsg);

/*! \brief   Prints warning messages.
 *  \details \par Description
 *           This function prints metadata from functions that encounter abnormal activity.
 */
extern void sierra_log_warn(const char* szMsg);

/*! \brief   Prints error messages.
 *  \details \par Description
 *           This function prints metadata from functions that have encountered an erroneous event.
 */
extern void sierra_log_error(const char* szMsg);

#endif /* __SIERRA_LOGGING_H__ */
