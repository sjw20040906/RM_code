/**
 * @file Dial.c
 * @author Why,ZS,SJW
 * @brief 拨弹盘处理总函数
 * @version 0.1
 * @date 2023-08-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "Dial.h"


/**************�û����ݶ���****************/
void Dial_Processing(void);
void Dial_Processing_1(void);
void Dial_Processing_2(void);
void PID_Clear_Dial(void);
float PID_Model4_Update(incrementalpid_t *pid, FUZZYPID_Data_t *PID, float _set_point, float _now_point);

#define CONTINUOUS_ROTATION_SPEED 50.0f //
#define CHECK_INTERVAL 1000             // �Ƕȼ����(ms)
#define REVERSE_DURATION 500            // ��ת����ʱ��(ms)
#define ANGLE_CHANGE_THRESHOLD 20       // �Ƕȱ仯��ֵ(�ж��Ƿ񿨵�)

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
 * @brief  ���̵���ܴ�������
 * @param  void
 * @retval void
 * @attention
 */
void Dial_Processing(void)
{
  if (Dial_Data.Shoot_Mode == Continuous_Shoot && Dial_Data.Dial_Switch == Dial_On)
  {
    M3508_Array[Dial_Wheel].targetSpeed = -1000; // Dial_Data.Speed_Dial;

    M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, M3508_Array[Dial_Wheel].targetSpeed, M3508_Array[Dial_Wheel].realSpeed);
  }
  else
  {
    M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, 0, M3508_Array[Dial_Wheel].realSpeed);
  }
}

/**
 * @brief  ���̵���ܴ�������1(�ǶȻ�)
 * @param  void
 * @retval void
 * @attention
 */
void Dial_Processing_1(void)
{

  if (Dial_Data.Shoot_Mode == Continuous_Shoot && Dial_Data.Dial_Switch == Dial_On)
  {
    M3508_Array[Dial_Wheel].targetAngle -= 5;

    M3508_Array[Dial_Wheel].targetSpeed = Position_PID(&M3508_DialV_Pid, M3508_Array[Dial_Wheel].targetAngle, M3508_Array[Dial_Wheel].totalAngle);

    M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, M3508_Array[Dial_Wheel].targetSpeed, M3508_Array[Dial_Wheel].realSpeed);

    /********************�����̿�������*****************/
    if (M3508_Array[Dial_Wheel].realCurrent > 10000 || M3508_Array[Dial_Wheel].realCurrent < -10000)
    {
      M3508_Array[Dial_Wheel].targetAngle += 1000;

      M3508_Array[Dial_Wheel].targetSpeed = Position_PID(&M3508_DialV_Pid, M3508_Array[Dial_Wheel].targetAngle, M3508_Array[Dial_Wheel].totalAngle);

      M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, M3508_Array[Dial_Wheel].targetSpeed, M3508_Array[Dial_Wheel].realSpeed);
    }
  }
  else
  {
    M3508_Array[Dial_Wheel].targetSpeed = 0;
    M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, 0, M3508_Array[Dial_Wheel].realSpeed);
  }
}

/**
 * @brief  ���̵���ܴ����������ٶȻ���
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
      M3508_Array[Dial_Wheel].targetSpeed = 1000; // Dial_Data.Speed_Dial;
      M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, M3508_Array[Dial_Wheel].targetSpeed, M3508_Array[Dial_Wheel].realSpeed);
      return; // �˳���������ִ�к����߼�
    }
  }

  if (Dial_Data.Shoot_Mode == Continuous_Shoot && Dial_Data.Dial_Switch == Dial_On)
  {
    M3508_Array[Dial_Wheel].targetSpeed = -1000; // Dial_Data.Speed_Dial;

    M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, M3508_Array[Dial_Wheel].targetSpeed, M3508_Array[Dial_Wheel].realSpeed);

    /********************�����̿�������*****************/
    // ��ʱ���Ƕȱ仯(ÿCHECK_INTERVAL ms���һ��)
    if (current_time - last_check_time >= CHECK_INTERVAL)
    {
      // ����Ƕȱ仯��(ȡ����ֵ)
      int32_t angle_change = abs(M3508_Array[Dial_Wheel].totalAngle - last_angle);

      // ����Ƕȱ仯С����ֵ���ж�Ϊ����
      if (angle_change < ANGLE_CHANGE_THRESHOLD)
      {
        is_reversing = 1;
        reverse_start_time = current_time;
        // ������ʼ��ת
        M3508_Array[Dial_Wheel].targetSpeed = 1000;
        M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, M3508_Array[Dial_Wheel].targetSpeed, M3508_Array[Dial_Wheel].realSpeed);
      }

      // ���¼���׼ֵ
      last_check_time = current_time;
      last_angle = M3508_Array[Dial_Wheel].totalAngle;
    }
  }
  else
  {
    M3508_Array[Dial_Wheel].outCurrent = PID_Model4_Update(&M3508_DialI_Pid, &fuzzy_pid_bullet_v, 0, M3508_Array[Dial_Wheel].realSpeed);
  }
}

/**
 * @brief  Dial���PID���
 * @param  void
 * @retval void
 * @attention
 */
void PID_Clear_Dial(void)
{

  Position_PIDInit(&M3508_DialV_Pid, 0.3f, 0.001f, 0.4, 0.5, 20000, 8000, 700); // ���̵���ٶȻ�
  Incremental_PIDInit(&M3508_DialI_Pid, 12.5f, 0.5f, 0, 10000, 1000);           // ���̵��������
}
