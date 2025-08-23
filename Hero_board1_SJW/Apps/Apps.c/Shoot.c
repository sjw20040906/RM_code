/**
 * @file Shoot.c
 * @author Why
 * @brief �ۺ�Ħ���ֺͲ��̵���Ĵ������������������١�����������
 * @version 0.1
 * @date 2023-08-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "Shoot.h"
#include "BSP_Can.h"
#include "Cloud_Control.h"
#include "Extern_Handles.h"
#include "BSP_BoardCommunication.h"

/**************�û����ݶ���****************/
void Shoot_Processing(void);
void Shoot_Update_Status(void);

/****************�ӿڶ���******************/
Shoot_Fun_t Shoot_Fun = Shoot_FunGroundInit;
#undef Shoot_FunGroundInit
Shoot_Data_t Shoot_Data = Shoot_DataGroundInit;
#undef Shoot_DataGroundInit

incrementalpid_t M3508_FricL_Pid; // ��Ħ���ֵ��pid
incrementalpid_t M3508_FricR_Pid; // ��Ħ���ֵ��pid
positionpid_t M2006_DialV_Pid;	  // ���̵���ٶ�pid
incrementalpid_t M2006_DialI_Pid; // ���̵������pid

/**
 * @brief  ����ܴ�����
 * @param  void
 * @retval void
 * @attention
 */
void Shoot_Processing()
{
	Fric_Fun.Fric_Processing();

	Shoot_Update_Status();
	/* �����̵�� */
	Dial_Fun.Dial_Processing_2();
}

/**
 * @brief  �����ظ���״̬λ�ĸ���
 * @param  void
 * @retval void
 * @attention
 */
void Shoot_Update_Status()
{
	if (Dial_Data.Shoot_Mode == Continuous_Shoot) // ���õ���ʱ��Ŀ�����ó�ʵ��ֵ
		M2006_Array[Dial_Motor].targetAngle = M2006_Array[Dial_Motor].totalAngle;
	if (Shoot_Data.Shoot_Switch == Shoot_On)
	{
		Dial_Data.Dial_Switch = Dial_On;
	}
	else if (Shoot_Data.Shoot_Switch == Shoot_Off)
	{
		Dial_Data.Dial_Switch = Dial_Off;
	}

	/* ���ò��� */
	if (ControlMes.shoot_state == RC_SW_MID)
	{
		if (ControlMes.redial == 1)
		{
			M2006_Array[Dial_Motor].targetSpeed = 2000;
		}
		else
		{
			M2006_Array[Dial_Motor].targetSpeed = 0;
		}
	}
}
