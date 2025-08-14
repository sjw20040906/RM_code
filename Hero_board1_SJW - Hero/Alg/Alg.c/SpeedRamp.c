/**
 * @file SpeedRamp.c
 * @author Miraggio (w1159904119@gmail)
 * @brief
 * @version 0.1
 * @date 2021-03-30
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "SpeedRamp.h"


/**
 * @brief  б�º�������
 * @param	б�º����ṹ��
 * @retval None
 */
int16_t SpeedRampCalc(SpeedRamp_t *SpeedRamp)
{

	SpeedRamp->count += SpeedRamp->rate;
	if (SpeedRamp->count < SpeedRamp->mincount)
	{
		SpeedRamp->count = SpeedRamp->mincount;
	}
	if (SpeedRamp->count > SpeedRamp->maxcount)
	{
		SpeedRamp->count = SpeedRamp->maxcount;
	}
	return SpeedRamp->count;
}

/**
 * @brief  б�¼���ֵ����
 * @param	б�º����ṹ��
 * @retval None
 */
void CountReset(SpeedRamp_t *SpeedRamp)
{

	if (fabs(SpeedRamp->count) < fabs(SpeedRamp->rate))
	{
		SpeedRamp->count = 0;
	}
	else
	{

		SpeedRamp->count -= SpeedRamp->count * 0.2f;
	}
}

/**
 * @brief  б�º���,ʹĿ�����ֵ������������ֵ
 * @param  �����������,��ǰ���,�仯�ٶ�(Խ��Խ��)
 * @retval ��ǰ���
 * @attention
 */
float RAMP_float(float final, float now, float ramp)
{
	float buffer = 0;

	buffer = final - now;

	if (buffer > 0)
	{
		if (buffer > ramp)
		{
			now += ramp;
		}
		else
		{
			now += buffer;
		}
	}
	else
	{
		if (buffer < -ramp)
		{
			now += -ramp;
		}
		else
		{
			now += buffer;
		}
	}

	return now;
}
