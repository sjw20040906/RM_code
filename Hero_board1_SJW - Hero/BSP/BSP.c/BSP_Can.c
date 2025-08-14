/**
 * @file BSP_Can.c
 * @author Why (1091537104@qq)
 * @brief Init of CAN
 * @version 0.1
 * @date 2023-08-07
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "BSP_Can.h"


/*******************************�û����ݶ���************************************/
void CAN_IT_Init(CAN_HandleTypeDef *hcanx, uint8_t Can_type);
void CAN_SendData(QueueHandle_t CANx_Handle, CAN_HandleTypeDef *CANx, uint8_t id_type, uint32_t id, uint8_t data[8]);

Can_Data_t Can_Data[2] = Can_DataGroundInit;
#undef Can_DataGroundInit
/***********************************�ӿڸ�ֵ************************************/
Can_Fun_t Can_Fun = Can_FunGroundInit;
#undef Can_FunGroundInit
/*******************************************************************************/

// CANͨ�ŷ��ͻ�����
uint8_t CAN1_0x1ff_Tx_Data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t CAN1_0x200_Tx_Data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t CAN1_0x2ff_Tx_Data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t CAN2_0x1ff_Tx_Data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t CAN2_0x200_Tx_Data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t CAN2_0x2ff_Tx_Data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/**
 * @Data   2023-08-07
 * @brief  CANɸѡ����ʼ��,�����ǻ��������ID��
 * @param  CAN_FilterTypeDef *CAN_Filter, CAN_HandleTypeDef *hcanx
 * @retval void
 */
static void CAN_FILTER_Init(CAN_FilterTypeDef *CAN_Filter, CAN_HandleTypeDef *hcanx)
{
  CAN_Filter->FilterFIFOAssignment = CAN_RX_FIFO0; // ɸѡ����������FIFO0
  CAN_Filter->FilterBank = 0;                      // ɸѡ����0
  CAN_Filter->SlaveStartFilterBank = 0;
  CAN_Filter->FilterMode = CAN_FILTERMODE_IDMASK;  // ������ID����ģʽ
  CAN_Filter->FilterScale = CAN_FILTERSCALE_32BIT; // ɸѡ��λ��Ϊ����32λ��
  CAN_Filter->FilterActivation = ENABLE;           // ʹ��ɸѡ��
  // ʹ��ɸѡ�������ձ�־�����ݽ��бȶ�ɸѡ����չID�������µľ����������ǵĻ��������FIFO0//
  CAN_Filter->FilterIdHigh = 0x0000;     // Ҫɸѡ��ID��λ
  CAN_Filter->FilterIdLow = 0x0000;      // Ҫɸѡ��ID��λ
  CAN_Filter->FilterMaskIdHigh = 0x0000; // ɸѡ����16λÿλ����ƥ��
  CAN_Filter->FilterMaskIdLow = 0x0000;  // ɸѡ����16λÿλ����ƥ��
  if (HAL_CAN_ConfigFilter(hcanx, CAN_Filter) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @Data    2021-08-07
 * @brief   canx�ж�����
 * @param   CAN_HandleTypeDef *hcanx, uint8_t Can_type
 * @retval  void
 */
void CAN_IT_Init(CAN_HandleTypeDef *hcanx, uint8_t Can_type)
{
  uint8_t Canx_type = Can_type - 1;

  /*ʹ���˲���*/
  CAN_FILTER_Init(&Can_Data[Canx_type].CAN_FilterTypedef.CAN_Filter, hcanx);
  /*����CAN*/
  HAL_CAN_Start(hcanx);
  /*ʹ��CAN��IT�ж�*/
  // HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);   //����CAN�����ж�
  //__HAL_CAN_ENABLE_IT(hcanx, CAN_IT_RX_FIFO0_MSG_PENDING); //  CAN_IT_FMP0
  HAL_CAN_ActivateNotification(hcanx, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/**
 * @brief  CAN��������
 * @param  CANx	    CAN���
 * 		id_type 	id���� CAN_ID_STD�� CAN_ID_EXT
 *			id			id��
 * 		data[8]		8������
 * @retval None
 */
void CAN_SendData(QueueHandle_t CANx_Handle, CAN_HandleTypeDef *CANx, uint8_t id_type, uint32_t id, uint8_t data[8])
{
  Can_Send_Data_t Can_Send_Data;

  Can_Send_Data.Canx = CANx;
  if (id_type == CAN_ID_STD)
  {
    Can_Send_Data.CAN_TxHeader.StdId = id;
  }
  else
  {
    Can_Send_Data.CAN_TxHeader.ExtId = id; // ID��
  }

  Can_Send_Data.CAN_TxHeader.IDE = id_type;      // ID����
  Can_Send_Data.CAN_TxHeader.RTR = CAN_RTR_DATA; // ���͵�Ϊ����
  Can_Send_Data.CAN_TxHeader.DLC = 0x08;         // ���ݳ���Ϊ8�ֽ�
  Can_Send_Data.CAN_TxHeader.TransmitGlobalTime = DISABLE;

  memcpy(Can_Send_Data.CANx_Send_RxMessage, data, sizeof(uint8_t[8]));

  xQueueSend(CANx_Handle, &Can_Send_Data, 0);
}

/**
 * @brief  CAN���������жϺ���
 * @param  CANx 			CAN���
 * 		    id_type 	id���� CAN_ID_STD�� CAN_ID_EXT
 *			    id				id��
 * 		    data[8]		8������
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  static uint8_t Can_Type = 0; // ����Can1��Can2
  static Can_Export_Data_t Can_Export_Data[2];
  BaseType_t xHigherPriorityTask;
  /*�ж�����һ��Can�ڴ��ص���Ϣ*/
  if (hcan->Instance == CAN1)
    Can_Type = 0;
  else
    Can_Type = 1;

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &Can_Data[Can_Type].CAN_RxTypedef.CANx_RxHeader, Can_Data[Can_Type].CAN_RxTypedef.CAN_RxMessage);
  Can_Export_Data[Can_Type].CAN_RxHeader = Can_Data[Can_Type].CAN_RxTypedef.CANx_RxHeader;
  memcpy(&Can_Export_Data[Can_Type].CANx_Export_RxMessage, Can_Data[Can_Type].CAN_RxTypedef.CAN_RxMessage, sizeof(uint8_t[8]));

  /*�ѽ������ݷ������ն���	*/
  if (!Can_Type)
    xQueueSendToBackFromISR(CAN1_ReceiveHandle, &Can_Export_Data[Can_Type], &xHigherPriorityTask);
  else
    xQueueSendToBackFromISR(CAN2_ReceiveHandle, &Can_Export_Data[Can_Type], &xHigherPriorityTask);
  portYIELD_FROM_ISR(xHigherPriorityTask);
}
