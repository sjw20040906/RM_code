/**
 * @file M3508_Motor.c
 * @author Why
 * @brief
 * @version 0.1
 * @date 2023-08-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "M3508_Motor.h"
#include <stdio.h>

/* M3508���е�������� */
M3508s_t M3508_Array[TotalNum] = {
    [Chassis_Right_Front] = {0},
    [Chassis_Left_Front] = {0},
    [Chassis_Left_Back] = {0},
    [Chassis_Right_Back] = {0},
    [FricL_Wheel] = {0},
    [FricR_Wheel] = {0},
};

void M3508_getInfo(Can_Export_Data_t RxMessage);
void M3508_Friction_getInfo(Can_Export_Data_t RxMessage);
void M3508_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4, uint8_t *data);

M3508_FUN_t M3508_FUN = M3508_FunGroundInit;
#undef M3508_FunGroundInit

/**
 * @brief  ����M3508�������ֵ��id��Ϊ1~4��
 * @param  iqx (x:1~4) ��Ӧid�ŵ���ĵ���ֵ����Χ-16384~0~16384
 * @retval None
 */
void M3508_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4, uint8_t *data)
{
    // ���ݸ�ʽ���˵����P32
    data[0] = iq1 >> 8;
    data[1] = iq1;
    data[2] = iq2 >> 8;
    data[3] = iq2;
    data[4] = iq3 >> 8;
    data[5] = iq3;
    data[6] = iq4 >> 8;
    data[7] = iq4;
}

/**
 * @brief  ��CAN�����л�ȡM3508�����Ϣ
 * @param  RxMessage 	CAN���Ľ��սṹ��
 * @retval None
 */
void M3508_getInfo(Can_Export_Data_t RxMessage)
{
    uint32_t StdId;
    StdId = (int32_t)(RxMessage.CAN_RxHeader.StdId - 0x201);
    // ������ݣ����ݸ�ʽ���C620���˵����P33
    M3508_Array[StdId].realAngle = (uint16_t)(RxMessage.CANx_Export_RxMessage[0] << 8 | RxMessage.CANx_Export_RxMessage[1]);
    M3508_Array[StdId].realSpeed = (int16_t)(RxMessage.CANx_Export_RxMessage[2] << 8 | RxMessage.CANx_Export_RxMessage[3]);
    M3508_Array[StdId].realCurrent = (int16_t)(RxMessage.CANx_Export_RxMessage[4] << 8 | RxMessage.CANx_Export_RxMessage[5]);
    M3508_Array[StdId].temperture = RxMessage.CANx_Export_RxMessage[6];

    if (M3508_Array[StdId].realAngle - M3508_Array[StdId].lastAngle < -6000)
    {
        M3508_Array[StdId].turnCount++;
    }
    else if (M3508_Array[StdId].lastAngle - M3508_Array[StdId].realAngle < -6000)
    {
        M3508_Array[StdId].turnCount--;
    }
    M3508_Array[StdId].totalAngle = M3508_Array[StdId].realAngle + (8192 * M3508_Array[StdId].turnCount);
    M3508_Array[StdId].lastAngle = M3508_Array[StdId].realAngle;

    // ֡��ͳ�ƣ����ݸ��±�־λ
    M3508_Array[StdId].InfoUpdateFrame++;
    M3508_Array[StdId].InfoUpdateFlag = 1;
}

/**
 * @brief  �Ե���������г�ʼ��
 * @param  None
 * @retval None
 * @attention ��ǰֻ������ʼ����������������㣬�Ժ��и����������д
 */
void M3508_Init(void)
{
    for (int i = 0; i <= 3; i++)
    {
        M3508_Array[i].outCurrent = 0;
    }
}
