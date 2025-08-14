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

/* M3508所有电机的数组 */
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
 * @brief  设置M3508电机电流值（id号为1~4）
 * @param  iqx (x:1~4) 对应id号电机的电流值，范围-16384~0~16384
 * @retval None
 */
void M3508_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4, uint8_t *data)
{
    // 数据格式详见说明书P32
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
 * @brief  从CAN报文中获取M3508电机信息
 * @param  RxMessage 	CAN报文接收结构体
 * @retval None
 */
void M3508_getInfo(Can_Export_Data_t RxMessage)
{
    uint32_t StdId;
    StdId = (int32_t)(RxMessage.CAN_RxHeader.StdId - 0x201);
    // 解包数据，数据格式详见C620电调说明书P33
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

    // 帧率统计，数据更新标志位
    M3508_Array[StdId].InfoUpdateFrame++;
    M3508_Array[StdId].InfoUpdateFlag = 1;
}

/**
 * @brief  对电机参数进行初始化
 * @param  None
 * @retval None
 * @attention 当前只针对其初始输出电流进行了清零，以后有更多需求需改写
 */
void M3508_Init(void)
{
    for (int i = 0; i <= 3; i++)
    {
        M3508_Array[i].outCurrent = 0;
    }
}
