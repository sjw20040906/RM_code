/**
 * @file M6020_Motor.h
 * @author Cyx
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright 
 * 
 */
#ifndef __M6020_MOTOR_H
#define __M6020_MOTOR_H

#include "can.h"
#include "typedef.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define M6020_READID_START 0x205 //当ID为1时的报文ID（6020反馈帧从0x205~0x20B）
#define M6020_READID_END 0x206
#define M6020_PITCH_ID 0x208
#define M6020_SENDID 0x1FF //1~4的电机，0x2FF为5~7

#define M6020_CurrentRatio 0f //待测定
#define M6020_MaxOutput 30000 //发送给电机的最大控制值
#define M6020_mAngle 8191     //6020的机械角度最大值0~8191。MachineAngle

#define M6020_mAngleRatio 22.7527f //机械角度与真实角度的比率

#define M6020_getRoundAngle(rAngle) rAngle / M6020_mAngleRatio //机械角度与真实角度的比率

#define M6020_FunGroundInit        \
    {                              \
			&M6020_setVoltage,		  	\
			&M6020_getInfo,		  			\
			&M6020_Reset,          		\
    }

typedef struct
{
    uint16_t realAngle;  //读回来的机械角度
    int32_t realSpeed;   //读回来的速度
    int16_t realCurrent; //读回来的实际转矩电流
    uint8_t temperture;  //读回来的电机温度
    uint16_t lastAngle;  //上次的角度
	
    int32_t targetSpeed; //目标速度
    int32_t targetAngle; //目标角度
	
    int16_t turnCount;   //转过的圈数
    float totalAngle;  //累积总共角度

    int16_t outCurrent; //输出电流

    uint8_t InfoUpdateFlag;   //信息读取更新标志
    uint16_t InfoUpdateFrame; //帧率
    uint8_t OffLineFlag;      //设备离线标志
} M6020s_t;

typedef struct
{
	void (*M6020_setVoltage)(int16_t uq1, int16_t uq2, int16_t uq3, int16_t uq4, uint8_t *data);
    void (*M6020_getInfo)(Can_Export_Data_t RxMessage);
    void (*M6020_Reset)(M6020s_t *m6020);
	
} M6020_Fun_t;

extern M6020s_t M6020s_Yaw;   //ID为1
extern M6020s_t M6020s_Pitch; //2
extern M6020_Fun_t M6020_Fun;

#endif /* __M6020_MOTOR_H */
