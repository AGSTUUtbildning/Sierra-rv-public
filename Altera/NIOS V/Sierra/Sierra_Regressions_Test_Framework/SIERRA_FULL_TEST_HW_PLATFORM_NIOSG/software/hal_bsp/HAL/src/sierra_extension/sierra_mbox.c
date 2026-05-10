/*!
 * \file       sierra_mbox.c
 * \details    This file contains:
 *             - Functions for Mailbox.
 * \author     Isa Magomadov
 * \version    10.03.15
 * \date       2022
 * \history    Modified 2022:
 *             - added initiale functions.
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

#include <string.h>
#include <sierra_extension/sierra_mbox.h>
#include <sierra.h>

uint32_t sierra_mbox_get_required_size(register const uint16_t amount_of_messages, register const uint16_t largest_message_data)
{
  const uint32_t total_size_for_header = sizeof(sierra_mbox_header_t) * amount_of_messages; // calculate how mamny bytes is needed for all header.
  const uint32_t total_size_for_message = largest_message_data * amount_of_messages; // calculate how mamny bytes is needed for all message.

  return (total_size_for_header + total_size_for_message);
}

void sierra_mbox_init(sierra_mbox_queue_t* mbox, char* mem_pool, uint16_t max_messages, uint16_t largest_size)
{
  mbox->front = SIERRA_MBOX_INVALID_MESSAGE_ID;
  mbox->rear = SIERRA_MBOX_INVALID_MESSAGE_ID;
  mbox->base_header_addr = mem_pool;
  mbox->base_message_addr = mem_pool + (sizeof(sierra_mbox_header_t)*max_messages);
  mbox->max_messages = max_messages;
  mbox->largest_message_size = largest_size;
}

sierra_mbox_res_e sierra_mbox_send(register sierra_mbox_queue_t* mbox, register const sierra_mbox_header_t* header)
{
  // turn off sierra
  // We don't want another task to write/read to/from mailbox at the same time.
  // Because it will be uncertain what will be happen,
  // Example issue: Two tasks writing/reading at the same index on mailbox, Because other task has not updated index yet.
  sierra_tsw_off();

  const int size_of_the_header = sizeof(sierra_mbox_header_t);

  // return if message is larger than expected
  // memory pool <--- user data buffer
  // user data buffer should be less or equal to memory pool message/data size
  if(mbox->largest_message_size < header->size)
  {
    sierra_tsw_on();
    return MBOX_DATA_TOO_LARGE;
  }

  // check if queue is full
  if(((mbox->rear + 1) == mbox->front) || (mbox->front == 0 && mbox->rear == mbox->max_messages - 1))
  {
    sierra_tsw_on();
    return MBOX_FULL;
  }

  // if this is the first insert to the mailbox then, set front to 0 and rear to -1
  if(mbox->front == SIERRA_MBOX_INVALID_MESSAGE_ID)
  {
    mbox->front = 0;
    mbox->rear = SIERRA_MBOX_INVALID_MESSAGE_ID;
  }

  // this is slower
  //mbox->_rear = (mbox->_rear + 1) % mbox->_max_messages; // count up or reset

  if((++mbox->rear) == mbox->max_messages)
    mbox->rear = 0;

  // save header into mailbox at address [base_header_address] + [last_index] * [size_of_header]
  *((sierra_mbox_header_t*) (mbox->base_header_addr + (mbox->rear * size_of_the_header))) = *header;
  //memcpy(mbox->_base_header_addr + (mbox->_rear * sizeof(sierra_mbox_header_t)), header, sizeof(sierra_mbox_header_t)); // save header message

  memcpy(mbox->base_message_addr + (mbox->rear * mbox->largest_message_size), header->data, header->size); // save data message

  // turn on sierra
  sierra_tsw_on();

  return MBOX_OK;
}

sierra_mbox_res_e sierra_mbox_read(register sierra_mbox_queue_t* mbox, register sierra_mbox_header_t* header)
{
  // turn off sierra
  // We don't want another task to write/read to/from mailbox at the same time.
  // Because it will be uncertain what will be happen,
  // Example issue: Two tasks writing/reading at the same index on mailbox, Because other task has not updated index yet.
  sierra_tsw_off();

  const int size_of_the_header = sizeof(sierra_mbox_header_t);

  // Remember data buffer as the header will get overwritten
  char* pMsgData = header->data;

  if(mbox->front == SIERRA_MBOX_INVALID_MESSAGE_ID)
  {
    sierra_tsw_on();
    return MBOX_EMPTY;
  }

  // read header from mailbox at address [base_header_address] + [current_index] * [size_of_header]
  *header = *((sierra_mbox_header_t *) (mbox->base_header_addr + (mbox->front * size_of_the_header)));

  //memcpy(header, mbox->_base_header_addr + (mbox->_front * size_of_the_header), size_of_the_header); // get header
  //memcpy(header, mbox->_base_header_addr + (mbox->_front * sizeof(sierra_mbox_header_t)), sizeof(sierra_mbox_header_t)); // get header

  header->data = pMsgData; // point to buffer that user specified, after getting header.

  memcpy(header->data, mbox->base_message_addr + (mbox->front * mbox->largest_message_size), header->size); // copy message to buffer

  // if front and rear pointing to the same address, then we have reached end of messages
  if(mbox->front == mbox->rear)
      mbox->front = SIERRA_MBOX_INVALID_MESSAGE_ID;
  else if((++mbox->front) == mbox->max_messages)
      mbox->front = 0;

  // turn on sierra
  sierra_tsw_on();

  return MBOX_OK;
}


sierra_mbox_res_e sierra_mbox_peek(register sierra_mbox_queue_t* mbox, register sierra_mbox_header_t* header)
{
  // turn off sierra
  // We don't want another task to write/read to/from mailbox at the same time.
  // Because it will be uncertain what will be happen,
  // Example issue: Two tasks writing/reading at the same index on mailbox, Because other task has not updated index yet.
  sierra_tsw_off();

  const int size_of_the_header = sizeof(sierra_mbox_header_t);

  // Remember data buffer as the header will get overwritten
  char* pMsgData = header->data;

  if(mbox->front == SIERRA_MBOX_INVALID_MESSAGE_ID)
  {
    sierra_tsw_on();
    return MBOX_EMPTY;
  }

  // read header from mailbox at address [base_header_address] + [current_index] * [size_of_header]
  *header = *((sierra_mbox_header_t *) (mbox->base_header_addr + (mbox->front * size_of_the_header)));
  //memcpy(header, mbox->_base_header_addr + (mbox->_front * sizeof(sierra_mbox_header_t)), sizeof(sierra_mbox_header_t)); // get header

  header->data = pMsgData; // point to buffer that user specified, after getting header.

  memcpy(header->data, mbox->base_message_addr + (mbox->front * mbox->largest_message_size), header->size); // copy message to buffer

  // turn on sierra
  sierra_tsw_on();

  return MBOX_OK;
}
