/**
 * @file Task_CanMsg.c
 * @author why
 * @brief
 * @version 0.1
 * @date 2021-03-30
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "Task_CanReceive.h"
#include "BSP_CAN.h"
#include "PowerControl.h"
#include "M3508_Motor.h"
#include "M6020_Motor.h"
#include "Cloud_Control.h"
#include "BSP_BoardCommunication.h"
#include "BSP_Usart.h"
#include "J4310_Motor.h"


int16_t J4310_RxID;
/**
  * @Data   2021-03-28
  * @brief  can1��������
  * @param  void
  * @retval void
  */
void Can1Receives(void const *argument)
{
    Can_Export_Data_t Can_Export_Data;

    uint32_t ID;
    for (;;)
    {
        xQueueReceive(CAN1_ReceiveHandle, &Can_Export_Data, portMAX_DELAY);
        ID = Can_Export_Data.CAN_RxHeader.StdId;

        //����Ħ���ֵ�� ���̵�� ��������
        if (ID >= M3508_READID_START && ID <= M3508_READID_END)
        {
            M3508_FUN.M3508_getInfo(Can_Export_Data);
        }
						
				else if (ID == M2006_DIAL_ID)
        {
            M2006_FUN.M2006_getInfo(Can_Export_Data);
        }
    }
}

/**
  * @Data   2021-03-28
  * @brief  can2��������
  * @param  void
  * @retval void
  */
void Can2Receives(void const *argument)
{
    Can_Export_Data_t Can_Export_Data;
    uint32_t ID;
    for (;;)
    {
        xQueueReceive(CAN2_ReceiveHandle, &Can_Export_Data, portMAX_DELAY);
        ID = Can_Export_Data.CAN_RxHeader.StdId;
			   if (ID == CAN_ID_CHASSIS)
        {
          Board1_FUN.Board1_getGimbalInfo(Can_Export_Data);
        }
				 else if (J4310_RxID == J4310_READID_PITCH )  
        {
					J4310_Fun.J4310_getInfo(Can_Export_Data);
        }

//        else if (ID == PowerFeedback_ID)
//        {
//            PowerControl_Fun.PowerControl_MsgRec(Can_Export_Data);
//        }

    }
}
