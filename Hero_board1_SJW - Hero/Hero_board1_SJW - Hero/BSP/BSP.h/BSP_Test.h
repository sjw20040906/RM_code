#ifndef BSP_TEST_H
#define BSP_TEST_H

#include "struct_typedef.h"
#include "main.h"
#include "BSP_Usart.h"
//#include "Omni_Chassis.h"
#include "M6020_Motor.h"

#define FIRE_MAX_BUF_LEN 125 //�궨�壺����ݳ���

//��λ��-->��λ��ָ��궨��
#define SET_PID 0x10
#define SET_Target 0x11
/* ����ͷ�ṹ�� */
typedef __packed struct
{
  uint32_t head;    // ��ͷ
  uint8_t ch;       // ͨ��
  uint32_t len;     // ������
  uint8_t cmd;      // ����
//  uint8_t sum;      // У���
  
}Packet_Head_t;

#define FIRE_MAX_BUF_LEN 125 //�궨�壺����ݳ���

#define FRAME_HEADER     0x59485A53    // ֡ͷ

//��λ��-->��λ��ָ��궨��
#define SET_PID 0x10 //��λ��������λ��PID
#define SET_Target 0x11 //��λ��������λ��Ŀ��ֵ

//��λ��-->��λ��ָ��궨��
#define RETURN_Target 0x01 //��λ��������λ��ͨ����Ŀ��ֵ
#define RETURN_RealValue 0x02 //��λ��������λ��ͨ����ʵ��ֵ
#define RETURN_Cycletime 0x06 //��λ��������λ��ͨ��������

	extern uint8_t Fire_Rx_Data[FIRE_MAX_BUF_LEN];//Ұ�𴮿ڵ������ݻ�����
	extern uint8_t Fire_RxBuffer[FIRE_MAX_BUF_LEN];//Ұ�𴮿ڵ������ݴ洢��
	extern uint16_t Fire_Bx_Len;
	extern uint8_t Fire_Rx_Finish;//Ұ�𴮿�һ�����ݽ�����ɱ�־ 
		
	
	void Fire_Debug_Init(void);//Ұ�𴮿ڵ��Գ�ʼ����
	void Fire_Handle(void);//Ұ�𴮿����ݴ���
	uint8_t Fire_Head_Judge(uint8_t *data);//Ұ�𴮿�֡ͷ�ж�
	uint8_t Fire_Cmd(uint8_t *data);//Ұ��ָ�������ж�
	void Fire_Set_Computer_Value(uint8_t cmd, uint8_t ch, void *data, uint8_t num);//������λ����ֵ
	uint8_t Check_Sum(uint8_t init, uint8_t *ptr, uint8_t len );//����У���
	
#endif
