/**
 * @file Cloud_control.c
 * @author Cyx��ZS, SJW
 * @brief
 * @version 2.5
 * @date 2023-08-15
 *
 * @copyright
 *
 */

#include "Cloud_Control.h"
#include "FeedForward.h"
#include "M6020_Motor.h"
#include "M3508_Motor.h"
#include "BSP_Can.h"
#include "Extern_Handles.h"
#include "Saber_C3.h"
#include "FuzzyPID.h"
Cloud_t Cloud;

//���°�װ��������ô���ʱ��Ҫ���²�����Щֵ��toalAngle�����������˶���

/****************Pithch��λ*****************/
const float Cloud_Pitch_Min = -15;    
const float Cloud_Pitch_Max = 10;
const float Pitch_Angle_Init = -15; 
const float Cloud_Pitch_Center = 0;
const float Cloud_Pitch_Derta =Cloud_Pitch_Center-Cloud_Pitch_Min ;
/****************Pitch��λ  End*****************/




float Cloud_Target_Aim_Flag;
float Cloud_Init_Angle;
extern M6020s_t *M6020_Array[2]; //��Ӧ�����ID����Ϊ������+1
extern Saber_Angle_t Saber_Angle;
float Pitch_Torque = 3.f;     //��̨����Ť��
float Pitch_v = 2;
float Pitch_Kp =10.5;
float Pitch_Kd = 2;
float Pitch_RC_Sen = 0.0003;
int16_t Cloud_Aim_Pitch_Flag;
int16_t Cloud_Manual_Pitch_Flag;
int Aim_Flag = 0;

/******************�������˲��ṹ�崴��*********/
One_Kalman_t Cloud_PitchMotorAngle_Error_Kalman;// Pitch�����Ƕ�����Kalman�˲���
One_Kalman_t Cloud_PitchCurrent_Kalman;//Pitch�������Kalman�˲���
/******************�������˲��ṹ�崴�� end*********/

void Cloud_Init(void);
void Cloud_Pitch_Angle_Set(void);
void Cloud_Sport_Out(void);
void PID_Clear_Pitch(void);
void Remote_Change(void);
/***************����ӿڶ���***************/
Cloud_FUN_t Cloud_FUN = Cloud_FUNGroundInit;
#undef Cloud_FUNGroundInit


/**
 * @brief  ��̨��ʼ�������ò�������λ��̨
 * @param  None
 * @retval None
 */
void Cloud_Init(void)
{
    //��������ʱ�̵Ļ�е�Ƕ�
	Cloud_Manual_Pitch_Flag = Cloud.Target_Pitch;
//	Cloud.Target_Pitch = Pitch_Angle_Init;
//	Cloud.Pitch_Raw = Cloud_Pitch_Init ;
//	Cloud_Init_Angle = Cloud_Pitch_Init;
	Cloud.AutoAim_Pitch = 0;

  One_Kalman_Create(&Cloud_PitchMotorAngle_Error_Kalman, 1.5, 40);
  One_Kalman_Create(&Cloud_PITCHODKalman, 1, 10);
  One_Kalman_Create(&Cloud_PitchCurrent_Kalman, 6,10 );
	ControlMes.change_Flag = 0;
	ControlMes.shoot_Speed = 2;
	ControlMes.fric_Flag = 0;
	ControlMes.redial =0;
}


/**
  * @brief  ��ͨ�˲�
  * @param  float float float �˲�������һ���˲����ֵ���˲�ϵ��
  * @retval void
  * @attention
  */
float low_pass_filter(float current_value, float prev_value, float alpha) 
{
    return alpha * current_value + (1 - alpha) * prev_value;
}


/**
  * @brief  J4310_Pitch����Ƕȵ��������������������
  * @param  void
  * @retval void
  * @attention
  */
void Cloud_Pitch_Angle_Set(void)
{
    /****************************��̨pitchJ4310���******************************/
    /******************************ң������ֵ����******************************/
		static float Delta_Pitch =0;
		if (ControlMes.AutoAimFlag==1)
		{
			Delta_Pitch += (float)ControlMes.pitch_velocity * Pitch_RC_Sen;
			Cloud.Target_Pitch = Pitch_Angle_Init + Cloud.AutoAim_Pitch + Delta_Pitch;
			Aim_Flag = 1;
		}
			
		else
		{
			Delta_Pitch += (float)ControlMes.pitch_velocity * Pitch_RC_Sen;
			if(Aim_Flag == 0)
			{
				 Cloud.Target_Pitch = Delta_Pitch + Pitch_Angle_Init;
			}
			else if(Aim_Flag == 1)
			{
				 Cloud.Target_Pitch = Delta_Pitch + Pitch_Angle_Init;
			}

		}
	  
    /**********Pitch��λ**********/
    if (Cloud.Target_Pitch > Cloud_Pitch_Max)
    {
        Cloud.Target_Pitch = Cloud_Pitch_Max ;
    }
    else if (Cloud.Target_Pitch < Cloud_Pitch_Min)
    {
        Cloud.Target_Pitch = Cloud_Pitch_Min ;
    }

    /**************************Pitch�������ƣ�����J4310���MITģʽ��������ֵ*****************/
		J4310s_Pitch.outKp = Pitch_Kp;
		J4310s_Pitch.outKd = Pitch_Kd;
		J4310s_Pitch.outSpeed = Pitch_v;
		J4310s_Pitch.outTorque = Pitch_Torque;
		//One_Kalman_Filter(&Cloud_PitchCurrent_Kalman, Cloud.Target_Pitch);
    J4310s_Pitch.outPosition = Cloud.Target_Pitch;


}


/**
  * @brief  J4310������
  * @param  void
  * @retval void
  * @attention
  */
void Cloud_Sport_Out(void)
{
	Cloud_Pitch_Angle_Set();
}

/**
  * @brief  ����С����
  * @param  void
  * @retval void
  * @attention
  */

void Remote_Change(void)
{
	static int change_State = 0 ;
	static int change_Remote = 0 ;
	if(ControlMes.change_Flag == 1)
	{
		if(change_State == 0)
		{
			change_Remote += 3;
			ControlMes.z_rotation_velocity += 3;
			if(change_Remote >= 300)
			{
				change_State = 1;
			}
		}
		else if(change_State == 1)
		{
			change_Remote -= 3;
			ControlMes.z_rotation_velocity -= 3;
			if(change_Remote <= -300)
			{
				change_State = 0;
			}
		}
	}
	else
	{
		ControlMes.z_rotation_velocity -= change_Remote;
		change_Remote = 0;
	}
}

