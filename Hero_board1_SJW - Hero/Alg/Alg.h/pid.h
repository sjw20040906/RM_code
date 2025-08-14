/**
 * @file PID.h
 * @author Miraggio (w1159904119@gmail)
 * @brief
 * @version 0.1
 * @date 2021-03-30
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef ___PID_H
#define ___PID_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kalman_filter.h"
#include "FuzzyPID.h"
#include "math.h"

/**********PID�������ݽӿ�************/

// ����ʽPID����Ļ�׼����
typedef struct incrementalpid_t
{
    float Target;         // �趨Ŀ��ֵ
    float Measured;       // ����ֵ
    float err;            // ����ƫ��ֵ
    float err_last;       // ��һ��ƫ��
    float err_beforeLast; // ���ϴ�ƫ��
    float last_set_point; // ��һ��Ŀ��ֵ
    float error_target;   // ǰ������
    float integral_error; // �������ۻ�
    float Kp;
    float Ki;
    float Kd;
    float kf; // Kp, Ki, Kd��Kf����ϵ��
    float p_out;
    float i_out;
    float d_out;            // ���������ֵ
    float f_out;            // ǰ��f���
    float pwm;              // pwm���
    uint32_t MaxOutput;     // ����޷�
    uint32_t IntegralLimit; // �����޷�
    float (*Incremental_PID)(struct incrementalpid_t *pid_t, float target, float measured);
} incrementalpid_t;

// λ��ʽPID����Ļ�׼����
typedef struct positionpid_t
{
    float Target;         // �趨Ŀ��ֵ
    float Measured;       // ����ֵ
    float err;            // ����ƫ��ֵ
    float err_last;       // ��һ��ƫ��
    float err_change;     // ���仯��
    float last_set_point; // ��һ��Ŀ��ֵ
    float error_target;   // ǰ������
    float integral_error; // �������ۻ�
    float Kp;
    float Ki;
    float Kd; // Kp, Ki, Kd����ϵ��
    float kf;
    float p_out;
    float i_out;
    float d_out;               // ���������ֵ
    float f_out;               // ǰ��f���
    float pwm;                 // pwm���
    float MaxOutput;           // ����޷�
    float Integral_Separation; // ���ַ�����ֵ
    float IntegralLimit;       // �����޷�
    float (*Position_PID)(struct positionpid_t *pid_t, float target, float measured);
} positionpid_t;

// ģ��PID����Ļ�׼����
typedef struct Struct_PID_Manage_Object_Fuzzy
{
    float kp;
    float ki;
    float kd;
    float kf;

    float error;
    float last_error;
    float before_last_error;
    float integral_error; // �������ۻ�
    float error_target;   // ǰ������
    float err_change;     // ���仯��

    float set_point;      // Ŀ��ֵ
    float now_point;      // ��ǰֵ
    float last_set_point; // ��һ��Ŀ��ֵ

    float integral_limit;      // �����޷�
    float output_limit;        // ����޷�
    float Integral_Separation; // ���ַ�����ֵ
    float deadzone;            // ����

    float p_out;  // p���
    float i_out;  // i���
    float d_out;  // d���
    float f_out;  // ǰ��f���
    float output; // �����
} Struct_PID_Manage_Object_Fuzzy;

extern Struct_PID_Manage_Object_Fuzzy M3508s_DialPID;
// extern Struct_PID_Manage_Object_Fuzzy M2006s_FricLPID;
// extern Struct_PID_Manage_Object_Fuzzy M2006s_FricRPID;

extern float Incremental_PID(incrementalpid_t *pid_t, float target, float measured);
extern float Position_PID(positionpid_t *pid_t, float target, float measured);
extern float ClassisTwister_PID(positionpid_t *pid_t, float target, float measured);
extern void Incremental_PIDInit(incrementalpid_t *pid_t, float Kp, float Kd, float Ki, uint32_t MaxOutput, uint32_t IntegralLimit);
extern void Position_PIDInit(positionpid_t *pid_t, float Kp, float Kd, float Ki, float Kf, float MaxOutput, float IntegralLimit, float Integral_Separation);

extern float Position_PID_Pitch(positionpid_t *pid_t, FUZZYPID_Data_t *fuzzy_t, float target, float measured);

extern One_Kalman_t Cloud_YAWODKalman;
extern One_Kalman_t Cloud_PITCHODKalman;

#endif
