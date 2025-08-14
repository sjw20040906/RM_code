/**
 * @file FeedForward.c
 * @author Why
 * @brief ǰ������,ȡʵ�����İٷ�֮��ʮ
 * @version 0.1
 * @date 2023-08-28
 * @attention һ��Ҫ��PID�������֮�����������������������Ϊ��ֱ�Ӽ��������������
				����ֻ�Ƿ�����ļ�����ߵĶ�������û���õ��ľɰ汾����һ�����ӵĶ���
 *
 * @copyright
 *
 */


#include "FeedForward.h"

/**************�û����ݶ���****************/
void FeedForward_Fric(void);
//void FeedForward_Yaw(void);
void Compensator_Yaw(void);

/****************�ӿڶ���******************/
FeedForward_FUN_t FeedForward_FUN = FeedForward_FunGroundInit;
#undef FeedForward_FunGroundInit

#include "FeedForward.h"

/**
 * @brief   ��Ħ���ַ���ǰ��
 * @param   M3508���ⲿ�ӿ�
 * @retval  void
 */
void FeedForward_Fric()
{
	/* ƽ��Ħ���� */
	if (M3508_Array[Fric_Front_1].realSpeed < 1000)
		M3508_Array[Fric_Front_1].outCurrent +=
			(212 - 0.124f * (float)M3508_Array[Fric_Front_1].realSpeed);
	else
		M3508_Array[Fric_Front_1].outCurrent +=
			(88 - 0.01f * (float)(M3508_Array[Fric_Front_1].realSpeed - 1000));

	if (M3508_Array[Fric_Front_2].realSpeed < 1000)
		M3508_Array[Fric_Front_2].outCurrent +=
			(212 - 0.124f * (float)M3508_Array[Fric_Front_2].realSpeed);
	else
		M3508_Array[Fric_Front_2].outCurrent +=
			(88 - 0.01f * (float)(M3508_Array[Fric_Front_2].realSpeed - 1000));

	if (M3508_Array[Fric_Front_3].realSpeed < 1000)
		M3508_Array[Fric_Front_3].outCurrent +=
			(212 - 0.124f * (float)M3508_Array[Fric_Front_3].realSpeed);
	else
		M3508_Array[Fric_Front_3].outCurrent +=
			(88 - 0.01f * (float)(M3508_Array[Fric_Front_3].realSpeed - 1000));

	if (M3508_Array[Fric_Back_1].realSpeed < 1000)
		M3508_Array[Fric_Back_1].outCurrent +=
			(212 - 0.124f * (float)M3508_Array[Fric_Back_1].realSpeed);
	else
		M3508_Array[Fric_Back_1].outCurrent +=
			(88 - 0.01f * (float)(M3508_Array[Fric_Back_1].realSpeed - 1000));

	if (M3508_Array[Fric_Back_2].realSpeed < 1000)
		M3508_Array[Fric_Back_2].outCurrent +=
			(212 - 0.124f * (float)M3508_Array[Fric_Back_2].realSpeed);
	else
		M3508_Array[Fric_Back_2].outCurrent +=
			(88 - 0.01f * (float)(M3508_Array[Fric_Back_2].realSpeed - 1000));

	if (M3508_Array[Fric_Back_3].realSpeed < 1000)
		M3508_Array[Fric_Back_3].outCurrent +=
			(212 - 0.124f * (float)M3508_Array[Fric_Back_3].realSpeed);
	else
		M3508_Array[Fric_Back_3].outCurrent +=
			(88 - 0.01f * (float)(M3508_Array[Fric_Back_3].realSpeed - 1000));

	/* ƽ�ⵯ���Ħ���ֵľ޴����������ٵ��� */
	M3508_Array[Fric_Front_1].outCurrent += 5000;
	M3508_Array[Fric_Front_2].outCurrent += 5000;
	M3508_Array[Fric_Front_3].outCurrent += 5000;
	M3508_Array[Fric_Back_1].outCurrent += 5000;
	M3508_Array[Fric_Back_2].outCurrent += 5000;
	M3508_Array[Fric_Back_3].outCurrent += 5000;
}

/**
 * @brief   ��Yaw��������ǰ��
 * @param   ��̨����̨������ⲿ�ӿ�
 * @retval  void
 */
// void FeedForward_Yaw()
//{
//	/* Yaw���ǰ�� */
//	M6020s_Yaw.targetSpeed -= Saber_Angle.Z_Vel / 6.0f;  // �ӽǶ�ÿ��ת����תÿ����
// }

/**
 * @brief   ��Yaw���������Ŷ��Ĳ���
 * @param   ��̨����̨������ⲿ�ӿ�
 * @retval  void
 */
void Compensator_Yaw()
{
	/* ��ʱ���� */
	float exp1, exp2, exp3, exp4;
	/* constant for yaw */
	static float Coefficient_2 = Yaw_J * Yaw_L / (Yaw_K * Yaw_K);
	static float Coefficient_1 = (Yaw_J * Yaw_R + Yaw_L * Yaw_b) / (Yaw_K * Yaw_K);
	static float Coefficient_0 = (Yaw_b * Yaw_R) / (Yaw_K * Yaw_K) + 1;
	static float speed_last = 0;
	static float error = 0;
	static float error_last = 0;
	static float eerror = 0;

	/* �����Ƕ��׵���һ�׵�����׵���ֵ,�˴��õ���ֵӦΪ��ֵ����λ���� */
	//	exp4 = Saber_Angle.Z_Vel / 57.29578f; // �������ǽǶ�ÿ�룬ת��Ϊ����
	error = exp4 - speed_last;
	eerror = error - error_last;
	exp3 = Coefficient_2 * eerror;
	exp2 = Coefficient_1 * error;
	exp1 = Coefficient_0 * exp4;
	speed_last = exp4;
	error_last = error;

	M6020s_Yaw.outCurrent += 1000 * (exp1 + exp2 + exp3);
}
