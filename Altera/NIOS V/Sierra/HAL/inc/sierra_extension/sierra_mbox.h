/*!
 * \file       sierra_mbox.h
 * \details    This file contains:
 *             - Structure, Enum and functions for Mailbox.
 * \author     Isa Magomadov
 * \version    10.03.15
 * \date       2022
 * \history    Modified 2022:
 *             - added initiale Structs, Enums and functions.
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

#ifndef SIERRA_MBOX_INC_SIERRA_MBOX_H_
#define SIERRA_MBOX_INC_SIERRA_MBOX_H_
#define SIERRA_MBOX_INVALID_MESSAGE_ID -1

#include <stdint.h>

// Default types which allows programmer to know what data types message contains
typedef enum
{
  MBOX_INTEGER,
  MBOX_INTEGER_ARR,
  MBOX_FLOAT,
  MBOX_FLOAT_ARR,
  MBOX_DOUBLE,
  MBOX_DOUBLE_ARR,
  MBOX_CHAR,
  MBOX_CHAR_ARR,
  MBOX_COSTUME
} sierra_mbox_types_e;

typedef enum
{
  MBOX_OK,
  MBOX_FULL,
  MBOX_EMPTY,
  MBOX_DATA_TOO_LARGE
} sierra_mbox_res_e;

// message header,
// which contains:
// 		sender id,
// 		size of the data,
// 		type of data,
// 		pointer to data
typedef struct
{
  uint8_t id;
  uint16_t size;
  sierra_mbox_types_e type;
  void *data;
}  sierra_mbox_header_t;


// mailbox queue
typedef struct
{
    uint16_t max_messages;
    uint16_t largest_message_size;
    int32_t front;
    int32_t rear;
    char* base_header_addr;
    char* base_message_addr;
}  sierra_mbox_queue_t;

/*! \brief Calculate size for char array that will be used for Mailbox.
 *  \param[in] max_messages, Maximum messages allowed to be stored in mailbox.
 *  \param[in] largest_size, Largest message data.
 *  \return  uint32_t, that represents required size for mailbox based on inputs.
 */
extern uint32_t sierra_mbox_get_required_size(register const uint16_t amount_of_messages, register const uint16_t largest_message_data);

/*! \brief Initiates mailbox to initial state based on values.
 *  \param[in,out] mbox will be initiated and ready to be used.
 *  \param[in] mem_pool, Mailbox will use this as memory for header and message/data.
 *  \param[in] max_messages, Maximum messages allowed to be stored in mailbox.
 *  \param[in] largest_size, Largest message data allowed.
 *  \return  void.
 * 
 *  \par Example
 *  \code
 * 	#include <sierra_extension/sierra_mbox.h>
 *  #define MAX_MAILBOX_MESSAGES 3
 *  #define MAX_MAILBOX_MESSAGE_SIZE 4
 *  ...
 * 
 *  // create global Mailbox that is accessible for tasks. 
 *  sierra_mbox_queue_t mbox;
 *  ...
 * 
 *  void main()
 *  {
 *    ...
 *    // create memory pool for Mailbox
 *    const uint32_t total_size = sierra_mbox_get_required_size(MAX_MAILBOX_MESSAGES, MAX_MAILBOX_MESSAGE_SIZE);
 *    char* memory_pool = malloc(total_size);
 *    ...
 *    sierra_mbox_init(&mbox, memory_pool, MAX_MAILBOX_MESSAGES, MAX_MAILBOX_MESSAGE_SIZE);
 *    ...
 *  }
 *  \endcode
 */
extern void sierra_mbox_init(sierra_mbox_queue_t* mbox, char* mem_pool, uint16_t max_messages, uint16_t largest_size);

/*! \brief saves message to mailbox.
 *  \param[in] mbox, Mailbox that header and message will be saved at.
 *  \param[in] header, Header and Message to save.
 *  \return  sierra_mbox_res_e, returns MBOX_OK if Mailbox could save the header and message, else it will return one of the other response.
 * 
 *  \par Example
 *  \code
 *  ...
 *  define TASK_2 2
 *  ...
 *  void task_code_2()
 *  {
 *    sierra_mbox_res_e res = MBOX_OK;
 *    ...
 *    // data
 *    const uint32_t my_num = 777;
 * 
 *    // create header
 *    sierra_mbox_header_t header;
 *    header.id = TASK_2; // who is the sender
 *    header.type = MBOX_INTEGER; // what type of data
 *    header.size = sizeof(uint32_t); // size of the data
 *    header.data = &data; // address/location of the data
 * 
 *    res = sierra_mbox_send(&mbox, &header); // save the message to mailbox
 *    
 *    if(res == MBOX_OK)
 * 		...
 * 	  else
 * 		...
 * 
 *    ...
 * 		
 *  }
 *  \endcode
 */
extern sierra_mbox_res_e sierra_mbox_send(register sierra_mbox_queue_t* mbox, register const sierra_mbox_header_t* header);

/*! \brief read message from mailbox and remvoe the message from the mailbox.
 *  \param[in] mbox, Reading from Mailbox.
 *  \param[in] header, where to save read header and message.
 *  \return  sierra_mbox_res_e, returns MBOX_OK if Mailbox could read the header and message, else it will return one of the other response.
 * 
 *  \par Example
 *  \code
 *  void task_code_1()
 *  {
 *    sierra_mbox_res_e res = MBOX_OK;
 *    ...
 *    // data
 *    char buffer[mbox.largest_message_size]; // creates buffer with maximum size of data/message that mailbox can store.
 * 
 *    // create header
 *    sierra_mbox_header_t header;
 *    header.data = buffer; // address/location of the buffer
 * 
 *    res = sierra_mbox_read(&mbox, &header); // save the message from mailbox
 *    
 *    if(res == MBOX_OK)
 *    {
 *      ...
 *      if(header.type == MBOX_INTEGER && header.id == TASK_2)
 *         printf("task 2 sent: %d\n", (*(uint32_t*)buffer));
 *      ...
 *    }
 * 	  else
 * 		...
 * 
 *    ...
 * 		
 *  }
 *  \endcode
 */
extern sierra_mbox_res_e sierra_mbox_read(register sierra_mbox_queue_t* mbox, register sierra_mbox_header_t* header);

/*! \brief read message from mailbox but do not remove the message from mailbox.
 *  \param[in] mbox, Reading from Mailbox.
 *  \param[in] header, where to save read header and message.
 *  \return  sierra_mbox_res_e, returns MBOX_OK if Mailbox could read the header and message, else it will return one of the other response.
 * 
 *  \par Example
 *  \code
 *  void task_code_1()
 *  {
 *    sierra_mbox_res_e res = MBOX_OK;
 *    ...
 *    // data
 *    char buffer[mbox.largest_message_size]; // creates buffer with maximum size of data/message that mailbox can store.
 * 
 *    // create header
 *    sierra_mbox_header_t header;
 *    header.data = buffer; // address/location of the buffer
 * 
 *    res = sierra_mbox_peek(&mbox, &header); // save the message from mailbox
 *    
 *    if(res == MBOX_OK)
 *    {
 *      ...
 *      if(header.type == MBOX_INTEGER && header.id == TASK_2)
 *         printf("task 2 sent: %d\n", (*(uint32_t*)buffer));
 *      ...
 *    }
 * 	  else
 * 		...
 * 
 *    ...
 * 		
 *  }
 *  \endcode
 */
extern sierra_mbox_res_e sierra_mbox_peek(register sierra_mbox_queue_t* mbox, register sierra_mbox_header_t* header);


#endif /* SIERRA_MBOX_INC_SIERRA_MBOX_H_ */
