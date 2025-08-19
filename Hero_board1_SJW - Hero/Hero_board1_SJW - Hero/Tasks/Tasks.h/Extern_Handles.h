/**
 * @file Handle.h
 * @author Why (1091537104@qq)
 * @brief 
 * @version 0.1
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _HANDLE_H
#define _HANDLE_H

#include "cmsis_os.h"


extern QueueHandle_t CAN1_ReceiveHandle;
extern QueueHandle_t CAN2_ReceiveHandle;
extern QueueHandle_t CAN_SendHandle;
extern QueueHandle_t Communicate_ReceivefromPCHandle;

#endif /*_HANDLE_H*/
