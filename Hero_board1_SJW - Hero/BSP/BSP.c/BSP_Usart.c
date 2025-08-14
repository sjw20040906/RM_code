#include "BSP_Usart.h"


/******************�ӿ�����*****************/
Usart_Data_t Usart_Data = Usart_DataGroundInit;
#undef Usart_DataGroundInit
/**
 * @brief  ��ʾ��ǰ�ٶ�����
 * @param  void
 * @retval void
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	// BaseType_t xHigherPriorityTask;        //FreeROTS�˳��ж�ʱ�ж��Ƿ�Ҫ���������л�
	// �����������USART3,��Ϊң��������
	if (huart->Instance == USART3)
	{
#if (RemoteControlMethod == TDF)
		{ // ��ط�ң����
			memcpy(SBUS_RXBuffer, SBUS_Rx_Data, sizeof(SBUS_Rx_Data));
			SBUS_RX_Finish = 1; // �ѽ�����һ������
		}
#elif (RemoteControlMethod == DT7)
		// DT7ң����
		DT7_RX_Finish = 1; // �ѽ�����һ������
#endif
	}
	// �����������USART2,��Ϊ��λ������
	/*	if(huart->Instance == USART2 )
		{
				uint8_t Buf[UpperCom_MAX_BUF];
				HAL_UART_Receive_DMA(&huart2,Buf,sizeof(Buf));

				BaseType_t xHigherPriorityTask;
				xQueueSendToBackFromISR(Communicate_ReceivefromPCHandle, Buf, &xHigherPriorityTask);
				portYIELD_FROM_ISR(xHigherPriorityTask);

		}*/
}
