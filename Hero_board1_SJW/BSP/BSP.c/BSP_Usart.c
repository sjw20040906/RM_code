#include "BSP_Usart.h"
#include "SBUS.h"
#include "DT7.h"
//#include "BSP_Test.h"
#include "Extern_Handles.h"
#include "FrictionWheel.h"
//#include "Saber_C3.h"

/***************�û���������****************/
void Send_Motor_Data(M2006s_t *M2006_Array);
void Send_IMU_Data(void);
void Usart_DMA_Receive_Init(void);
//extern fp32 INS_angle[3];
/******************�ӿ�����*****************/
Usart_Fun_t Usart_Fun = Usart_FunGroundInit;
#undef Usart_FunGroundInit
Usart_Data_t Usart_Data = Usart_DataGroundInit;
#undef Usart_DataGroundInit

// /**
//   * @brief  ��ʾ��ǰ�ٶ�����
//   * @param  M2006s_t�ṹ��ָ��
//   * @retval void
//   */
// void Send_Motor_Data(M2006s_t *M2006_Array)
// {
//     uint8_t message[30];

//     sprintf((char *)message, "v1=%d v2=%d v3=%d v4=%d\n\r",
//             M2006_Array[0].realSpeed,
//             M2006_Array[1].realSpeed,
//             M2006_Array[2].realSpeed,
//             M2006_Array[3].realSpeed);
//     HAL_UART_Transmit_DMA(&huart1, (uint8_t *)message,
//                           strlen((const char *)message));
//     //Usart1_Printf("v1=%d v2=%d v3=%d v4=%d\r\n",v1,v2,v3,v4);
// }

// /**
//   * @brief  ����DMA���ճ�ʼ��
//   * @param  void
//   * @retval void
//   */
/*void Usart_DMA_Receive_Init()
 {
     HAL_UARTEx_ReceiveToIdle_DMA(&huart6, Usart_Data.Usart6_Data, Usart_DMA_Idle_Length)
 }*/

/**
  * @brief  ���տ��лص�
  * @param  void
  * @retval void
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	// BaseType_t xHigherPriorityTask;        //FreeROTS�˳��ж�ʱ�ж��Ƿ�Ҫ���������л�
    //�����������USART3,��Ϊң��������
	if(huart->Instance == USART3 )
    {
#if (RemoteControlMethod == TDF)
		{//��ط�ң����
			memcpy(SBUS_RXBuffer,SBUS_Rx_Data,sizeof(SBUS_Rx_Data));
			SBUS_RX_Finish=1;//�ѽ�����һ������
		}
#elif (RemoteControlMethod == DT7)
		//DT7ң����
			DT7_RX_Finish = 1;//�ѽ�����һ������
#endif
		
    }
    //�����������USART2,��Ϊ��λ������
/*	if(huart->Instance == USART2 )
    {
			uint8_t Buf[UpperCom_MAX_BUF];
			HAL_UART_Receive_DMA(&huart3,Buf,sizeof(Buf));		
			
			BaseType_t xHigherPriorityTask;
			xQueueSendToBackFromISR(Communicate_ReceivefromPCHandle, Buf, &xHigherPriorityTask);
			portYIELD_FROM_ISR(xHigherPriorityTask);

    }
	
	//�����������USART1
	if(huart->Instance == USART1)
	{	
		memcpy(Fire_RxBuffer,Fire_Rx_Data,sizeof(Fire_Rx_Data));
		Fire_Rx_Finish = 1;//�ѽ�����һ������
//		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,Fire_Rx_Data,FIRE_MAX_BUF_LEN);
//		
//		int temp;
//		memcpy(&temp, Fire_Rx_Data, 4);
//		Fric_Data.Required_Speed = temp;
//		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,Fire_Rx_Data,FIRE_MAX_BUF_LEN);
	}
	*/
}


	

