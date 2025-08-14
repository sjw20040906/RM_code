/**
 * @file Shoot.c
 * @author Why
 * @brief �ۺ�Ħ���ֺͲ��̵���Ĵ��������������������١�����������
 * @version 0.1
 * @date 2023-08-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "Shoot.h"


/**************�û����ݶ���****************/
void Shoot_Processing(void);
void Shoot_Update_Status(void);

/****************�ӿڶ���******************/
Shoot_Fun_t Shoot_Fun = Shoot_FunGroundInit;
#undef Shoot_FunGroundInit
Shoot_Data_t Shoot_Data = Shoot_DataGroundInit;
#undef Shoot_DataGroundInit

incrementalpid_t M3508_FricF1_Pid; // ǰ1Ħ���ֵ��pid
incrementalpid_t M3508_FricF2_Pid; // ǰ2Ħ���ֵ��pid
incrementalpid_t M3508_FricF3_Pid; // ǰ3Ħ���ֵ��pid
incrementalpid_t M3508_FricB1_Pid; // ��1Ħ���ֵ��pid
incrementalpid_t M3508_FricB2_Pid; // ��2Ħ���ֵ��pid
incrementalpid_t M3508_FricB3_Pid; // ��3Ħ���ֵ��pid

positionpid_t M3508_DialV_Pid;	  // ���̵���ٶ�pid
incrementalpid_t M3508_DialI_Pid; // ���̵������pid

/**
 * @brief  ����ܴ�������
 * @param  void
 * @retval void
 * @attention
 */
void Shoot_Processing()
{
	Fric_Fun.Fric_Processing();

	Shoot_Update_Status();
	/* �������̵�� */
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
		M3508_Array[Dial_Wheel].targetAngle = M3508_Array[Dial_Wheel].totalAngle;
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
			M3508_Array[Dial_Wheel].targetSpeed = 2000;
		}
		else
		{
			M3508_Array[Dial_Wheel].targetSpeed = 0;
		}
	}
}
