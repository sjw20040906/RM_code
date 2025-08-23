/**
 * @file Dial.h
 * @author Why
 * @brief �����̵���Ŀ�������
 * @version 0.1
 * @date 2023-08-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __DIAL_H
#define __DIAL_H

#include "Shoot.h"
#include "main.h"
#include "cmsis_os.h"

/**
 * @brief  ��������Ŀ���
 * @param
 */
typedef enum
{
	Dial_Off = 0,
	Dial_On,
	Dial_ing,
} Dial_On_Off;

/**
 * @brief  ����ģʽ
 * @param  ����������
 */
typedef enum
{
	Single_Shoot = 0,
	Continuous_Shoot,
} Shoot_Modes;

/* ����һ�����裬�����Ҫת���ĽǶȣ�ӳ��Ϊ360��-8192 */
#define Angle_DialOneBullet_17mm 36864 // 1024*36

/* Dial�����Լ������Ľӿڽṹ�� */
#define Dial_DataGroundInit \
	{                       \
		330,                \
		0,                  \
		0,                  \
		-900,               \
		Single_Shoot,       \
		Dial_Off,           \
	}

#define Dial_FunGroundInit  \
	{                       \
		&Dial_Processing_1, \
		&Dial_Processing_2, \
		&Dial_Processing,   \
		&Dial_Update_Angel, \
		&PID_Clear_Dial,    \
	}

typedef struct Dial_Data_t
{
	// uint8_t  Number_ToBeFired;   //��Ҫ����ĵ�������
	uint16_t Shoot_Interval; // �������
	// uint16_t Shoot_Period;       //������ʱ��
	uint32_t Time_NextShoot; // �´������ʱ�䣬��ϵͳʱ�䳬�����ֵ�������
	// uint32_t Time_StopShoot;     //����ֹͣ�����ʱ��
	uint16_t Bullet_Dialed; // �Ѿ�������ӵ�
	int16_t Speed_Dial;		// �����Ĳ��̵���ٶ�

	Shoot_Modes Shoot_Mode;	 // ����ģʽ��������������
	Dial_On_Off Dial_Switch; // ��������Ŀ���
} Dial_Data_t;

typedef struct Dial_Fun_t
{
	void (*Dial_Processing_1)();
	void (*Dial_Processing_2)();
	void (*Dial_Processing)();
	void (*Dial_Update_Angel)(bool Fric_ReadyOrNot);
	void (*PID_Clear_Dial)(void);

} Dial_Fun_t;

extern Dial_Fun_t Dial_Fun;
extern Dial_Data_t Dial_Data;

#endif /*__DIAL_H*/
