#include "BSP_Test.h"
#include "Cloud_Control.h"
#include "M3508_Motor.h"
#include "FrictionWheel.h"
//#include "Mecanum_Chassis.h"
#include "BSP_CAN.h"

uint8_t Fire_Rx_Data[FIRE_MAX_BUF_LEN];
uint8_t Fire_RxBuffer[FIRE_MAX_BUF_LEN];
uint16_t Fire_Bx_Len;
uint8_t Fire_Rx_Finish = 0;

/**
	* @brief ������������ʾ������ʾ����
	*	@param[in]
	*	@retval
	*/
void DebugData_Motor_Speed(M2006s_t *M2006_Array)
 {
	   uint8_t message[30];

    sprintf((char *)message, "%d,%d,%d,%d\r\n",
            M2006_Array[0].realSpeed,
            M2006_Array[1].realSpeed,
            M2006_Array[2].realSpeed,
            M2006_Array[3].realSpeed);
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)message,
                          strlen((const char *)message));
 }
 
/**
  * @brief  Fire���ڵ��Գ�ʼ�������������жϡ�����usart1��DMAģʽ���е���
  * @param  void
  * @retval void
  */
void Fire_Debug_Init()
{
	 HAL_UARTEx_ReceiveToIdle_DMA(&huart1,Fire_Rx_Data,FIRE_MAX_BUF_LEN);
}

/**
  * @brief  Fire����֡ͷ�ж�
  * @param  void
  * @retval void
  */
uint8_t Fire_Head_Judge(uint8_t *data)
{
	uint8_t flag = 0;
	
	if( (data[0] ==  0x53 ) && (data[1] ==  0x5A ) && (data[2] ==  0x48 ) && (data[3] ==  0x59 ) )
		flag = 1;
	return flag;
}

/**
  * @brief  Fireָ�������ж�
  * @param  void
  * @retval void
  */
uint8_t Fire_Cmd(uint8_t *data)
{
	return data[9];//����ָ��
}

/**
  * @brief ʮ������תfloat
  */
static float R4(uint8_t *p) {float r; memcpy(&r, p, 4); return r;}
/**
  * @brief  Fire�������ݴ���
  * @param  void
  * @retval void
  */
void Fire_Handle()
{
	if( Fire_Rx_Finish == 1 )
	{
		Fire_Rx_Finish = 0;
		
		if(Fire_Head_Judge(Fire_RxBuffer))
		{
			uint8_t cmd = Fire_Cmd(Fire_RxBuffer) ;
			uint8_t ID = Fire_RxBuffer[4]-0x01;
			switch(cmd)
			{				
				case SET_PID :
				{
					if(ID == 0)
					{
//						M6020s_PitchIPID.Kp = R4(&Fire_RxBuffer[10]);
//						M6020s_PitchIPID.Ki = R4(&Fire_RxBuffer[14]);
//						M6020s_PitchIPID.Kd = R4(&Fire_RxBuffer[18]);	
						
//						M3508_Chassis_Pid[Chassis_Left_Back].Kp = R4(&Fire_RxBuffer[10]);
//						M3508_Chassis_Pid[Chassis_Left_Back].Ki = R4(&Fire_RxBuffer[14]);
//						M3508_Chassis_Pid[Chassis_Left_Back].Kd = R4(&Fire_RxBuffer[18]);	
					}
					if(ID == 1)
					{
//						M6020s_PitchOPID.Kp = R4(&Fire_RxBuffer[10]);
//						M6020s_PitchOPID.Ki = R4(&Fire_RxBuffer[14]);
//						M6020s_PitchOPID.Kd = R4(&Fire_RxBuffer[18]);	
//						M3508_Chassis_Pid[Chassis_Left_Front].Kp = R4(&Fire_RxBuffer[10]);
//						M3508_Chassis_Pid[Chassis_Left_Front].Ki = R4(&Fire_RxBuffer[14]);
//						M3508_Chassis_Pid[Chassis_Left_Front].Kd = R4(&Fire_RxBuffer[18]);	
					}
					if(ID == 2)
					{
//						M3508_Chassis_Pid[Chassis_Right_Back].Kp = R4(&Fire_RxBuffer[10]);
//						M3508_Chassis_Pid[Chassis_Right_Back].Ki = R4(&Fire_RxBuffer[14]);
//						M3508_Chassis_Pid[Chassis_Right_Back].Kd = R4(&Fire_RxBuffer[18]);	
					}
					if(ID == 3)
					{
//						M3508_Chassis_Pid[Chassis_Right_Front].Kp = R4(&Fire_RxBuffer[10]);
//						M3508_Chassis_Pid[Chassis_Right_Front].Ki = R4(&Fire_RxBuffer[14]);
//						M3508_Chassis_Pid[Chassis_Right_Front].Kd = R4(&Fire_RxBuffer[18]);	
					}
					
					break;
				}
				case SET_Target :
				{
//					if(ID == 0)
//					{
//						M3508_Array[FricL_Wheel].outCurrent=*(int *)&Fire_RxBuffer[10];
//					}
//					else
//					{
//						M3508_Array[FricR_Wheel].outCurrent=*(int *)&Fire_RxBuffer[10];
//					}
						Cloud.Target_Pitch = *(int *)&Fire_RxBuffer[10];
						break;
				}	
			}
		}
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,Fire_Rx_Data,FIRE_MAX_BUF_LEN);//�ٴδ򿪴����ж�
		
	}
	
}


/**
  * @brief ����У���
  * @param ptr����Ҫ���������
  * @param len����Ҫ����ĳ���
  * @retval У���
  */
uint8_t Check_Sum(uint8_t init, uint8_t *ptr, uint8_t len )
{
  uint8_t sum = init;
  
  while(len--)
  {
    sum += *ptr;
    ptr++;
  }
  
  return sum;
}

/**
  * @brief ������λ����ֵ
  * @param cmd������
  * @param ch: ����ͨ��
  * @param data������ָ��
  * @param num����������
  * @retval ��
  */
void Fire_Set_Computer_Value(uint8_t cmd, uint8_t ch, void *data, uint8_t num)
{
  uint8_t sum = 0;    // У���
  num *= 4;           // һ������ 4 ���ֽ�
  
  static Packet_Head_t set_packet;
  
  set_packet.head = FRAME_HEADER;     // ��ͷ 0x59485A53
  set_packet.len  = 0x0B + num;      // ����
  set_packet.ch   = ch;              // ����ͨ��
  set_packet.cmd  = cmd;             // ��������
  
  sum = Check_Sum(0, (uint8_t *)&set_packet, sizeof(set_packet));       // �����ͷУ���
  sum = Check_Sum(sum, (uint8_t *)data, num);                           // �������У���
  
  HAL_UART_Transmit(&huart1, (uint8_t *)&set_packet, sizeof(set_packet), 0xFFFFF);    // ��������ͷ
  HAL_UART_Transmit(&huart1, (uint8_t *)data, num, 0xFFFFF);                          // ���Ͳ���
  HAL_UART_Transmit(&huart1, (uint8_t *)&sum, sizeof(sum), 0xFFFFF);                  // ����У���
}
