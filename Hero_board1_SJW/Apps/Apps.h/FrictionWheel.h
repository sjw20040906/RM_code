/**
 * @file FrictionWheel.h
 * @author Why
 * @brief ����Ħ���ֵĿ�������
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __FRICTIONWHEEL_H
#define __FRICTIONWHEEL_H

#include "Shoot.h"
#include "main.h"
#include "BSP_BoardCommunication.h"

/**
  * @brief  Ħ�����Ƿ�ﵽ�趨ֵ
  * @param  ture��ʾ���Դﵽ
  */
typedef enum
{
	Fric_Ready  = true,
	Fric_NotReady = false,
}Fric_ReadyOrNot;

/**
  * @brief  Ħ���ֵĿ���
  * @param  ture��ʾ��ʼ
  */
typedef enum
{
	Fric_On  = true,
	Fric_Off = false,
}Fric_On_Off;
 
 /* �������ٵõ���Ħ����ת�� */
#define Fric_SpeedLevel1 6200		//23m/s 6200
#define Fric_SpeedLevel2 5000		//

/* Dial���������Լ������Ľӿڽṹ�� */
#define Fric_DataGroundInit   \
    {                         \
		0,                    \
		Fric_NotReady,        \
		Fric_Off,             \
	}

#define Fric_FunGroundInit      \
    {                           \
		&Fric_Processing,       \
		&Fric_Judge_ReadyOrNot, \
		&PID_Clear_FricL,			\
		&PID_Clear_FricR,			\
	}		
		
typedef struct Fric_Data_t
{
	float Required_Speed;    //Ħ������Ҫ���ٶ�	
	bool Fric_Ready;        //Ħ���ּ������
	bool Fric_Switch;       //Ħ���ֿ���
}Fric_Data_t;

typedef struct Fric_Fun_t
{
	void (*Fric_Processing)();
	void (*Fric_Judge_ReadyOrNot)();
	void (*PID_Clear_FricL)(void);
	void (*PID_Clear_FricR)(void);
}Fric_Fun_t;

extern Fric_Fun_t Fric_Fun;
extern Fric_Data_t Fric_Data;

#endif /*__FRICTIONWHEEL_H*/
