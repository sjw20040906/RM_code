/**
 * @file Dial.c
 * @author Why,ZS,SJW
 * @brief �����̵���Ŀ�������
 * @version 0.1
 * @date 2023-08-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "Dial.h"
#include "FuzzyPID.h"
#include "PID.h"
#include "FeedForward.h"
#include "stm32f4xx_hal.h"

/**************�û����ݶ���****************/
void Dial_Processing(void);
void Dial_Processing_1(void);
void Dial_Processing_2(void);
void Dial_Update_Angel(bool Fric_ReadyOrNot);
void PID_Clear_Dial(void);
float PID_Model4_Update(incrementalpid_t *pid, FUZZYPID_Data_t *PID, float _set_point, float _now_point);

#define CONTINUOUS_ROTATION_SPEED 50.0f //
#define CHECK_INTERVAL          1000    // �Ƕȼ����(ms)
#define REVERSE_DURATION        500    // ��ת����ʱ��(ms)
#define ANGLE_CHANGE_THRESHOLD  20      // �Ƕȱ仯��ֵ(�ж��Ƿ񿨵�)

/**************��̬�������ڿ������****************/
static uint32_t last_check_time = 0;    // �ϴμ��ʱ��
static int32_t last_angle = 0;          // �ϴμ��ʱ�ĽǶ�
static uint8_t is_reversing = 0;        // ��ת״̬���
static uint32_t reverse_start_time = 0; // ��ת��ʼʱ��



/****************�ӿڶ���******************/
Dial_Fun_t Dial_Fun = Dial_FunGroundInit;
#undef Dial_FunGroundInit
Dial_Data_t Dial_Data = Dial_DataGroundInit;
#undef Dial_DataGroundInit
/**
 * @brief  ���̵���ܴ�����
 * @param  void
 * @retval void
 * @attention
 */
void Dial_Processing(void)
{
  if (Dial_Data.Shoot_Mode == Continuous_Shoot && Dial_Data.Dial_Switch == Dial_On)
  {
    M2006_Array[Dial_Motor].targetSpeed = -1000; // Dial_Data.Speed_Dial;

    M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, M2006_Array[Dial_Motor].targetSpeed, M2006_Array[Dial_Motor].realSpeed);

    /********************�����̿�������*****************/

    /*    if (M2006_Array[Dial_Motor].realTorque > 9200 || M2006_Array[Dial_Motor].realTorque < -9200)
        {
          M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, 500, M2006_Array[Dial_Motor].realSpeed);
        }*/
  }
  else
  {
    M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, 0, M2006_Array[Dial_Motor].realSpeed);
  }
}

/**
 * @brief  ���̵���ܴ�����1(�ǶȻ�)
 * @param  void
 * @retval void
 * @attention
 */
void Dial_Processing_1(void)
{

  if (Dial_Data.Shoot_Mode == Continuous_Shoot && Dial_Data.Dial_Switch == Dial_On)
  {
    M2006_Array[Dial_Motor].targetAngle -= 5;

    M2006_Array[Dial_Motor].targetSpeed = Position_PID(&M2006_DialV_Pid, M2006_Array[Dial_Motor].targetAngle, M2006_Array[Dial_Motor].totalAngle);

    M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, M2006_Array[Dial_Motor].targetSpeed, M2006_Array[Dial_Motor].realSpeed);

    /********************�����̿�������*****************/
    if (M2006_Array[Dial_Motor].realTorque > 10000 || M2006_Array[Dial_Motor].realTorque < -10000)
    {
      M2006_Array[Dial_Motor].targetAngle += 1000;

      M2006_Array[Dial_Motor].targetSpeed = Position_PID(&M2006_DialV_Pid, M2006_Array[Dial_Motor].targetAngle, M2006_Array[Dial_Motor].totalAngle);

      M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, M2006_Array[Dial_Motor].targetSpeed, M2006_Array[Dial_Motor].realSpeed);
    }
  }
  else
  {
    M2006_Array[Dial_Motor].targetSpeed = 0;
    M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, 0, M2006_Array[Dial_Motor].realSpeed);
  }
}

/**
 * @brief  ���̵���ܴ��������ٶȻ���
 * @param  void
 * @retval void
 * @attention
 */
void Dial_Processing_2(void)
{
  uint32_t current_time = HAL_GetTick(); // ��ȡϵͳ��ǰʱ��(ms)

  // ������ڷ�ת״̬
  if (is_reversing)
  {
    // ��鷴ת�Ƿ��ѳ����㹻ʱ��
    if (current_time - reverse_start_time >= REVERSE_DURATION)
    {
        // ������ת
        is_reversing = 0;
    }
    else
    {
        // ִ�з�ת�����÷�ת�ٶ�
        M2006_Array[Dial_Motor].targetSpeed = 1000; // Dial_Data.Speed_Dial;
        M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, M2006_Array[Dial_Motor].targetSpeed, M2006_Array[Dial_Motor].realSpeed);
        return; // �˳���������ִ�к����߼�
    }
  }

  if (Dial_Data.Shoot_Mode == Continuous_Shoot && Dial_Data.Dial_Switch == Dial_On)
  {
    M2006_Array[Dial_Motor].targetSpeed = -1000; // Dial_Data.Speed_Dial;

    M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, M2006_Array[Dial_Motor].targetSpeed, M2006_Array[Dial_Motor].realSpeed);

    /********************�����̿�������*****************/
    // ��ʱ���Ƕȱ仯(ÿCHECK_INTERVAL ms���һ��)
    if (current_time - last_check_time >= CHECK_INTERVAL)
    {
      // ����Ƕȱ仯��(ȡ����ֵ)
      int32_t angle_change = abs(M2006_Array[Dial_Motor].totalAngle - last_angle);
            
      // ����Ƕȱ仯С����ֵ���ж�Ϊ����
      if (angle_change < ANGLE_CHANGE_THRESHOLD)
      {
        is_reversing = 1;
        reverse_start_time = current_time;
        // ������ʼ��ת
        M2006_Array[Dial_Motor].targetSpeed = 1000;
        M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, M2006_Array[Dial_Motor].targetSpeed,  M2006_Array[Dial_Motor].realSpeed);
      }
            
      // ���¼���׼ֵ
      last_check_time = current_time;
      last_angle = M2006_Array[Dial_Motor].totalAngle;
    }
  }
  else
  {
    M2006_Array[Dial_Motor].outCurrent = PID_Model4_Update(&M2006_DialI_Pid, &fuzzy_pid_bullet_v, 0, M2006_Array[Dial_Motor].realSpeed);
  }
}

/**
 * @brief  ���²��̵���ĽǶ�ֵ
 * @param  void
 * @retval void
 * @attention
 */
void Dial_Update_Angel(bool Fric_ReadyOrNot)
{
}

/**
 * @brief  Dial���PID���
 * @param  void
 * @retval void
 * @attention
 */
void PID_Clear_Dial(void)
{

  Position_PIDInit(&M2006_DialV_Pid, 0.3f, 0.001f, 0.4, 0.5, 20000, 8000, 700); // ���̵���ٶȻ�
  Incremental_PIDInit(&M2006_DialI_Pid, 12.5f, 0.5f, 0, 10000, 1000);           // ���̵��������
}
