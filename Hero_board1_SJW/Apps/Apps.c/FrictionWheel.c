/**
 * @file FrictionWheel.c
 * @author Why
 * @brief ����Ħ���ֵĿ�������
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "FrictionWheel.h"
#include "Dial.h"

/**************�û����ݶ���****************/
void Fric_Processing(void);
void Fric_Judge_ReadyOrNot(void);
void Fric_Set_targetSpeed(void);
void PID_Clear_FricL(void);
void PID_Clear_FricR(void);
float PID_Model4_Update(incrementalpid_t *pid,FUZZYPID_Data_t *PID, float _set_point, float _now_point);


/****************�ӿڶ���******************/
Fric_Fun_t Fric_Fun = Fric_FunGroundInit;
#undef Fric_FunGroundInit
Fric_Data_t Fric_Data = Fric_DataGroundInit;
#undef Fric_DataGroundInit

/**
  * @brief  Ħ���ֿ����ܴ�����
  * @param  void
  * @retval void
  * @attention
  */
void Fric_Processing()
{
	/* �趨Ŀ��ֵ */
	Fric_Set_targetSpeed();

	M3508_Array[FricL_Wheel].targetSpeed = -1*Fric_Data.Required_Speed;
	M3508_Array[FricR_Wheel].targetSpeed = Fric_Data.Required_Speed;
	
	M3508_Array[FricL_Wheel].outCurrent = PID_Model4_Update(&M3508_FricL_Pid, &fuzzy_pid_shoot_l, M3508_Array[FricL_Wheel].targetSpeed, M3508_Array[FricL_Wheel].realSpeed);
	M3508_Array[FricR_Wheel].outCurrent = PID_Model4_Update(&M3508_FricR_Pid, &fuzzy_pid_shoot_r, M3508_Array[FricR_Wheel].targetSpeed, M3508_Array[FricR_Wheel].realSpeed);
	
}

/**
  * @brief  ��Ħ���ֵ��ٶȴﵽtarget����ʱ��ΪĦ�����Ѿ�����
  * @param  void
  * @retval void
  * @attention  �������δ��������ΪH�����arm_abs_q15��arm�����ᱨ��
  */
void Fric_Judge_ReadyOrNot()    
{
//	static q15_t abs_differ[2] = {0};
//	static q15_t abs_factor[2];           //��Ŀ���ٶȵ�0.1Ϊ��
//	static q15_t temp[2];
//	
//	temp[0] = Fric_Data.Required_Speed + M3508_Array[FricL_Wheel].realSpeed;
//	temp[1] = Fric_Data.Required_Speed - M3508_Array[FricR_Wheel].realSpeed;
//	arm_abs_q15(temp, abs_differ, 2);
//	temp[0] = Fric_Data.Required_Speed * 0.1f;
//	temp[1] = Fric_Data.Required_Speed * 0.1f;
//	arm_abs_q15(temp, abs_factor, 2);
//	
//	if(abs_differ[0] < abs_factor[0] && abs_differ[1] < abs_factor[1])
//		Fric_Data.Fric_Ready = Fric_Ready;
//	else Fric_Data.Fric_Ready = Fric_NotReady;
}

/**
  * @brief  �趨Ħ���ֵ�Ŀ���ٶ�
  * @param  void
  * @retval void
  * @attention
  */
void Fric_Set_targetSpeed(void)
{
	
	if(ControlMes.fric_Flag == 0)
	{
		Fric_Data.Required_Speed = 0;
		return;
	}
	else if(ControlMes.fric_Flag == 1)
	{
			Fric_Data.Required_Speed = Fric_SpeedLevel1;		
	}
}

/**
  * @brief  ��Ħ���ֵ�PID����
  * @param  void
  * @retval void
  * @attention//75 0.03 05 //3.0 0.03 0
  */
void PID_Clear_FricL(void)
{
	Incremental_PIDInit(&M3508_FricL_Pid, 3.3f, 0.035f, 0, 8000, 700);
}

/**
  * @brief  ��Ħ���ֵ�PID����
  * @param  void
  * @retval void
  * @attention
  */
void PID_Clear_FricR(void)
{
	Incremental_PIDInit(&M3508_FricR_Pid, 3.3f, 0.035f, 0, 8000, 700);
}

