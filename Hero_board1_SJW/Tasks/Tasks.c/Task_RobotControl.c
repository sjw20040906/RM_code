/**
 * @file Task_RobotControl.c
 * @author rm_control_team
 * @brief
 * @version 0.1
 * @date 2023-08-30
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "Task_RobotControl.h"
#include "Shoot.h"
#include "Cloud_Control.h"
#include "Saber_C3.h"
#include "SBUS.h"
#include "DT7.h"

#include "BSP_Can.h"
#include "BSP_Test.h"
void Robot_Control(void const *argument)
{

    portTickType xLastWakeTime;                        // ��¼�����ϴλ��ѵ�ʱ��㣬���ھ�ȷ���ڿ��ơ�
    xLastWakeTime = xTaskGetTickCount();               // ��ȡ��ǰϵͳʱ�Ӽ���
    const TickType_t TimeIncrement = pdMS_TO_TICKS(2); // ÿ3����ǿ�ƽ������ݷ���
    for (;;)
    {

        Cloud_FUN.Remote_Change(); // ����С����
        Cloud_FUN.Cloud_Sport_Out();
        Shoot_Fun.Shoot_Processing();

        /****************���͵�������***************/
        uint8_t data1[8], data2[8];

        M3508_FUN.M3508_setCurrent(M3508_Array[FricL_Wheel].outCurrent, M3508_Array[FricR_Wheel].outCurrent, M2006_Array[Dial_Motor].outCurrent, M2006_Array[Dial_Motor].outCurrent, data1);

        J4310_Fun.J4310_setParameter(J4310s_Pitch.outPosition, J4310s_Pitch.outSpeed, J4310s_Pitch.outKp, J4310s_Pitch.outKd, J4310s_Pitch.outTorque, data2);

        Can_Fun.CAN_SendData(CAN_SendHandle, &hcan1, CAN_ID_STD, M3508_SENDID_Fric_Dial, data1);

        Can_Fun.CAN_SendData(CAN_SendHandle, &hcan2, CAN_ID_STD, J4310_SENDID_Pitch, data2);
        /****************���͵������� end***************/
        vTaskDelayUntil(&xLastWakeTime, TimeIncrement);
    }
}
