 /**
 * @file Task_OffLineCheck.c
 * @author Cyx
 * @brief ��������ĳ�����ʱ�ɲ����ô�����
 * @version 0.1
 * @date 2024-03-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "Task_OffLineCheck.h"
#include "Shoot.h"
#include "Cloud_Control.h"
#include "Saber_C3.h"
#include "SBUS.h"
#include "DT7.h"
#include "J4310_Motor.h"
#include "BSP_Can.h"
#include "BSP_Test.h"
#include "stm32f4xx_hal.h"
static uint16_t pitch_Frame = 0;
static uint16_t fricL_Frame = 0;
static uint16_t fricR_Frame = 0;
static uint16_t dial_Frame = 0;
static uint32_t Pitch_last_check_time = 0;    // �ϴμ��ʱ��

void Off_Line_Check(void const *argument)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    const TickType_t TimeIncrement = pdMS_TO_TICKS(100); //ÿ100����ǿ�ƽ������ݷ���
    for (;;)
    {
			//Pitch���߼��
			uint32_t Pitch_current_time = HAL_GetTick(); // ��ȡϵͳ��ǰʱ��(ms)
			if (Pitch_current_time - Pitch_last_check_time >= 1000)
			{
				if(pitch_Frame == J4310s_Pitch.InfoUpdateFrame)
				{
					J4310_Fun.J4310_Enable();
				}
				pitch_Frame = J4310s_Pitch.InfoUpdateFrame ;
				Pitch_last_check_time = Pitch_current_time;
			}

				// if(pitch_Frame == J4310s_Pitch.InfoUpdateFrame)
				// {
				// 	J4310s_Pitch.InfoUpdateFlag = 0;
				// 	J4310s_Pitch.InfoUpdateFrame = 0;
				// 	J4310_Fun.J4310_Enable();
				// }
				// pitch_Frame = J4310s_Pitch.InfoUpdateFrame ;
				
			//FricL���߼��
				if(fricL_Frame == M3508_Array[FricL_Wheel].InfoUpdateFrame)
				{
					M3508_Array[FricL_Wheel].InfoUpdateFlag = 0;
					M3508_Array[FricL_Wheel].InfoUpdateFrame = 0 ;
					ControlMes.fric_Flag = 0;
				}
				fricL_Frame = M3508_Array[FricL_Wheel].InfoUpdateFrame ;
				
			//FricR���߼��
				if(fricR_Frame == M3508_Array[FricR_Wheel].InfoUpdateFrame)
				{
					M3508_Array[FricR_Wheel].InfoUpdateFlag = 0;
					M3508_Array[FricR_Wheel].InfoUpdateFrame = 0 ;				
					ControlMes.fric_Flag = 0;
				}
				fricR_Frame = M3508_Array[FricR_Wheel].InfoUpdateFrame ;
				
			//Dial���߼��
				if(dial_Frame == M2006_Array[Dial_Motor].InfoUpdateFrame )
				{
					M2006_Array[Dial_Motor].InfoUpdateFlag = 0;
					M2006_Array[Dial_Motor].InfoUpdateFrame = 0;
					M2006_Array[Dial_Motor].targetSpeed = 0;
				}
				dial_Frame = M2006_Array[Dial_Motor].InfoUpdateFrame ;
				
		
        vTaskDelayUntil(&xLastWakeTime, TimeIncrement);

    }
}
