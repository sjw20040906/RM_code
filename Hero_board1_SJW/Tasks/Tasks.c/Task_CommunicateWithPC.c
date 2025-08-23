#include "Task_CommunicateWithPC.h"
#include "BSP_BoardCommunication.h"
/**
  * @Data   2023.10.09
  * @brief  usb��������
  * @param  void
  * @retval void
  */
void USBCommunicateTask_Receive(void const *argument)
{
	uint8_t UpperCom_Receive_Buffer[UpperCom_MAX_BUF];
	for(;;)
	{
		xQueueReceive(Communicate_ReceivefromPCHandle,UpperCom_Receive_Buffer,portMAX_DELAY);
		UpperCom_Receive_From_Up(UpperCom_Receive_Buffer);
	}
}

/**
  * @Data   2023.10.09
  * @brief  usb��������
  * @param  void
  * @retval void
  */
void USBCommunicateTask_Send(void const *argument)
{
	for(;;)
	{
		UpperCom_Send_To_Up(0xCD);
		vTaskDelay(20);
	}
}
