/**
 * @file UI.c
 * @author Cyx (1686143358@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024
 *
 */


#include "UI.h"
unsigned char UI_Seq;                      //�������
u16 Robot_ID;
u16 Cilent_ID;
UI_Graph1_t UI_Graph1;
UI_Graph2_t UI_Graph2;
UI_Graph5_t UI_Graph5;
UI_Graph7_t UI_Graph7;
UI_String_t UI_String1;
UI_String_t UI_String2;
UI_String_t UI_String3;
UI_String_t UI_String4;
UI_String_t UI_String5;
UI_Delete_t UI_Delete;

UI_FUN_t UI_FUN = UI_FUNGroundInit;
#undef UI_FUNGroundInit



/**
 * @brief �����ˡ��ͻ���ID�ж�
 * @return {*}
 */
void ID_Judge(void)
{
	//�����˽�ɫȷ��
	if( ext_game_robot_state.data.robot_id == 1)			//��1
	{
		Robot_ID=UI_Data_RobotID_RHero;
		Cilent_ID=UI_Data_CilentID_RHero;       
	}
	else if( ext_game_robot_state.data.robot_id == 101)	//��1
	{
		Robot_ID=UI_Data_RobotID_BHero;
		Cilent_ID=UI_Data_CilentID_BHero;  
	}
	else if( ext_game_robot_state.data.robot_id == 3)			//��3
	{
		Robot_ID=UI_Data_RobotID_RStandard1;
		Cilent_ID=UI_Data_CilentID_RStandard1;       
	}
	else if( ext_game_robot_state.data.robot_id == 103)	//��3
	{
		Robot_ID=UI_Data_RobotID_BStandard1;
		Cilent_ID=UI_Data_CilentID_BStandard1;  
	}
	else if( ext_game_robot_state.data.robot_id == 4)		//��4
	{
		Robot_ID=UI_Data_RobotID_RStandard2;
		Cilent_ID=UI_Data_CilentID_RStandard2;  
	}
	else if( ext_game_robot_state.data.robot_id == 104)	//��4
	{
		Robot_ID=UI_Data_RobotID_BStandard2;
		Cilent_ID=UI_Data_CilentID_BStandard2;  
	}
	else if( ext_game_robot_state.data.robot_id == 5)		//��4
	{
		Robot_ID=UI_Data_RobotID_RStandard3;
		Cilent_ID=UI_Data_CilentID_RStandard3;  
	}
	else if( ext_game_robot_state.data.robot_id == 105)	//��4
	{
		Robot_ID=UI_Data_RobotID_BStandard3;
		Cilent_ID=UI_Data_CilentID_BStandard3;  
	}
	else if( ext_game_robot_state.data.robot_id == 7)		//��7
	{
		Robot_ID=UI_Data_RobotID_RSentry;
		Cilent_ID=UI_Data_RobotID_RSentry;  
	}
	else if( ext_game_robot_state.data.robot_id == 107)	//��7
	{
		Robot_ID=UI_Data_RobotID_BSentry;
		Cilent_ID=UI_Data_RobotID_BSentry;  
	}
}


/**
 * @brief UI��ʼ��
 * @return {*}
 */
void UI_init(void)
{
	memset(&UI_Graph1, 0, sizeof(UI_Graph1));
   memset(&UI_String3, 0, sizeof(UI_String3));
	memset(&UI_Graph2, 0, sizeof(UI_Graph2));
	memset(&UI_Graph5, 0, sizeof(UI_Graph5));
	memset(&UI_Graph7, 0, sizeof(UI_Graph7));
   memset(&UI_String1, 0, sizeof(UI_String1));
	memset(&UI_String2, 0, sizeof(UI_String2));
	memset(&UI_Delete, 0, sizeof(UI_Delete));
}

/*��ͼ����ϵ

	��Ļ���� * �� = 1920*1280

	����ԭ�㣺��Ļ���½�Ϊ (0,0)

	�����᣺����ԭ��ˮƽ���ң�x���ӣ�����ԭ����ֱ���ϣ�y����
*/

/**
 * @brief ����ֱ��
 * @param *image           ���ͼ������
 * @param imagename[3]     ͼ�����ƣ����ڱ�ʾ����
 * @param Graph_Operate    ͼ�β��������ӡ��ı䡢ɾ����
 * @param Graph_Layer      ͼ�㣨0-9��
 * @param Graph_Color      ͼ����ɫ
 * @param Graph_Width      ͼ���߿�
 * @param Start_x,Start_y  ֱ����ʼ����
 * @param End_x,End_y      ֱ�߽�������
 * @return {*}
 */    
void Line_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
   int i;
   for(i=0;i<3&&imagename[i]!=NULL;i++)
   image->graphic_name[2-i]=imagename[i];
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->end_x = End_x;
   image->end_y = End_y;
}

/**
 * @brief ���ƾ���
 * @param *image           ���ͼ������
 * @param imagename[3]     ͼ�����ƣ����ڱ�ʾ����
 * @param Graph_Operate    ͼ�β��������ӡ��ı䡢ɾ����
 * @param Graph_Layer      ͼ�㣨0-9��
 * @param Graph_Color      ͼ����ɫ
 * @param Graph_Width      ͼ���߿�
 * @param Start_x,Start_y  �������Ͻ�����
 * @param End_x,End_y      �������½�����
 * @return {*}
 */         
void Rectangle_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
   int i;
   for(i=0;i<3&&imagename[i]!=NULL;i++)
   image->graphic_name[2-i]=imagename[i];
   image->graphic_tpye = UI_Graph_Rectangle;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->end_x = End_x;
   image->end_y = End_y;
}

/**
 * @brief ������Բ
 * @param *image           ���ͼ������
 * @param imagename[3]     ͼ�����ƣ����ڱ�ʾ����
 * @param Graph_Operate    ͼ�β��������ӡ��ı䡢ɾ����
 * @param Graph_Layer      ͼ�㣨0-9��
 * @param Graph_Color      ͼ����ɫ
 * @param Graph_Width      ͼ���߿�
 * @param Start_x,Start_y  Բ������
 * @param Graph_Radius     Բ�ΰ뾶
 * @return {*}
 */         
void Circle_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 Graph_Radius)
{
   int i;
   for(i=0;i<3&&imagename[i]!=NULL;i++)
   image->graphic_name[2-i]=imagename[i];
   image->graphic_tpye = UI_Graph_Circle;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->radius = Graph_Radius;
}

/**
 * @brief ����Բ��
 * @param *image            ���ͼ������
 * @param imagename[3]      ͼ�����ƣ����ڱ�ʾ����
 * @param Graph_Operate     ͼ�β��������ӡ��ı䡢ɾ����
 * @param Graph_Layer       ͼ�㣨0-9��
 * @param Graph_Color       ͼ����ɫ
 * @param Graph_Width       ͼ���߿�
 * @param Graph_StartAngle,Graph_EndAngle Բ����ʼ����ֹ�Ƕ�
 * @param Start_x,Start_y   Բ������
 * @param x_Length,y_Length x,y�������᳤��������Բ��
 * @return {*}
 */        
void Arc_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_StartAngle,u32 Graph_EndAngle,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 x_Length,u32 y_Length)
{
   int i;
   
   for(i=0;i<3&&imagename[i]!=NULL;i++)
   image->graphic_name[2-i]=imagename[i];
   image->graphic_tpye = UI_Graph_Arc;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->start_angle = Graph_StartAngle;
   image->end_angle = Graph_EndAngle;
   image->end_x = x_Length;
   image->end_y = y_Length;
}

/**
 * @brief ���Ƹ���������
 * @param *image           ���ͼ������
 * @param imagename[3]     ͼ�����ƣ����ڱ�ʾ����
 * @param Graph_Operate    ͼ�β��������ӡ��ı䡢ɾ����
 * @param Graph_Layer      ͼ�㣨0-9��
 * @param Graph_Color      ͼ����ɫ
 * @param Graph_Width      ͼ���߿�
 * @param Graph_Size       �ֺ�
 * @param Graph_Digit      С��λ��
 * @param Start_x,Start_y  ���ݿ�ʼ����
 * @param Graph_Float      ��Ҫ��ʾ�ĸ�����
 * @return {*}
 */      
void Float_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,float FloatData)
{
   int i;
   
   for(i=0;i<3&&imagename[i]!=NULL;i++)
      image->graphic_name[2-i]=imagename[i];
   image->graphic_tpye = UI_Graph_Float;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->start_angle = Graph_Size;
   image->end_angle = Graph_Digit;
   int32_t IntData = FloatData * 1000;
   image->radius = (IntData & 0x000003ff) >>  0;
   image->end_x = (IntData & 0x001ffc00) >> 10;
   image->end_y = (IntData & 0xffe00000) >> 21;
}

/**
 * @brief �����ַ�������
 * @param *image           ���ͼ������
 * @param imagename[3]     ͼ�����ƣ����ڱ�ʾ����
 * @param Graph_Operate    ͼ�β��������ӡ��ı䡢ɾ����
 * @param Graph_Layer      ͼ�㣨0-9��
 * @param Graph_Color      ͼ����ɫ
 * @param Graph_Width      ͼ���߿�
 * @param Graph_Size       �ֺ�
 * @param Graph_Digit      �ַ�λ��
 * @param Start_x,Start_y  ���ݿ�ʼ����
 * @param *Char_Data      ��Ҫ��ʾ���ַ�����ָ��
 * @return {*}
 */        
void Char_Draw(String_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,char *Char_Data)
{
	int i;
   
	for(i=0;i<3&&imagename[i]!=NULL;i++)
		image->Graph_Control.graphic_name[2-i]=imagename[i];
	image->Graph_Control.graphic_tpye = UI_Graph_Char;
	image->Graph_Control.operate_tpye = Graph_Operate;
	image->Graph_Control.layer = Graph_Layer;
	image->Graph_Control.color = Graph_Color;
   image->Graph_Control.start_angle = Graph_Size;
	image->Graph_Control.end_angle = Graph_Digit;
	image->Graph_Control.width = Graph_Width;
	image->Graph_Control.start_x = Start_x;
	image->Graph_Control.start_y = Start_y;
   
	for(i=0;i<Graph_Digit;i++)
	{
	  image->show_Data[i]=*Char_Data;
	  Char_Data++;
	}
}

/**
 * @brief ����UI���������ַ������ݣ�
 * @param cnt        ͼ�θ�����Ŀǰֻ֧��1��2��5��7��������������㣬��Ҫ����ѧ�⣩
 * @param *Graphs    ��Ҫ���Ƶ�ͼ������
 * @return ��
 */
void UI_PushUp_Graphs(u8 cnt, void *Graphs)
{
   UI_Graph1_t *Graph = (UI_Graph1_t *)Graphs;
   
   Graph->framehead.SOF=UI_SOF;
   Graph->framehead.Data_Length=6+cnt*15;
   Graph->framehead.Seq=UI_Seq;
   Graph->framehead.CRC8=Get_CRC8_Check_Sum_UI((u8 *)Graph,4,0xFF);
   Graph->framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //����ͷ����
   
   switch(cnt)
   {
      case 1:
         Graph->datahead.Data_ID=UI_Data_ID_Draw1;
         break;
      case 2:
         Graph->datahead.Data_ID=UI_Data_ID_Draw2;
         break;
      case 5:
         Graph->datahead.Data_ID=UI_Data_ID_Draw5;
         break;
      case 7:
         Graph->datahead.Data_ID=UI_Data_ID_Draw7;
         break;
      default:
         break;
   }
   Graph->datahead.Sender_ID=Robot_ID;
   Graph->datahead.Receiver_ID=Cilent_ID;                          //����������
   
   if(cnt == 1)
	{
		UI_Graph1_t *Graph1 = (UI_Graph1_t *)Graphs;
		Graph1->CRC16 = Get_CRC16_Check_Sum_UI((u8 *)Graph1, sizeof(UI_Graph1_t) - 2, 0xFFFF);
	}
	else if(cnt == 2)
	{
		UI_Graph2_t *Graph2 = (UI_Graph2_t *)Graphs;
		Graph2->CRC16 = Get_CRC16_Check_Sum_UI((u8 *)Graph2, sizeof(UI_Graph2_t) - 2, 0xFFFF);
	}
	else if(cnt == 5)
	{
		UI_Graph5_t *Graph5 = (UI_Graph5_t *)Graphs;
		Graph5->CRC16 = Get_CRC16_Check_Sum_UI((u8 *)Graph5, sizeof(UI_Graph5_t) - 2, 0xFFFF);
	}
	else if(cnt == 7)
	{
		UI_Graph7_t *Graph7 = (UI_Graph7_t *)Graphs;
		Graph7->CRC16 = Get_CRC16_Check_Sum_UI((u8 *)Graph7, sizeof(UI_Graph7_t) - 2, 0xFFFF);
	}
   
   if(cnt == 1) HAL_UART_Transmit_DMA(&huart6, (u8 *)Graph, sizeof(UI_Graph1_t));
	else if(cnt == 2) HAL_UART_Transmit_DMA(&huart6, (u8 *)Graph, sizeof(UI_Graph2_t));
	else if(cnt == 5) HAL_UART_Transmit_DMA(&huart6, (u8 *)Graph, sizeof(UI_Graph5_t));
	else if(cnt == 7) HAL_UART_Transmit_DMA(&huart6, (u8 *)Graph, sizeof(UI_Graph7_t));
   
   UI_Seq++;                                          //�����+1
}

/**
 * @brief ����UI�������ַ�����
 * @param *String    ����ַ������ݵı�����һ��ֻ�ܸ���һ���ַ������ݣ�
 * @return ��
 */
void UI_PushUp_String(UI_String_t *String)
{  
   String->framehead.SOF=UI_SOF;
   String->framehead.Data_Length=6+45;
   String->framehead.Seq=UI_Seq;
   String->framehead.CRC8=Get_CRC8_Check_Sum_UI((u8 *)String,4,0xFF);
   String->framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //����ͷ����
   

   String->datahead.Data_ID=UI_Data_ID_DrawChar;
   String->datahead.Sender_ID=Robot_ID;
   String->datahead.Receiver_ID=Cilent_ID;                          //����������
   
   String->CRC16 = Get_CRC16_Check_Sum_UI((u8 *)String, sizeof(UI_String_t) - 2, 0xFFFF);
   
   HAL_UART_Transmit_DMA(&huart6, (u8 *)String, sizeof(UI_String_t));
   
   UI_Seq++;                                          //�����+1
}

/**
 * @brief ɾ������
 * @param *Delete    ���ɾ�������ı���
 * @return ��
 */
void UI_PushUp_Delete(UI_Delete_t *Delete)
{   
   Delete->framehead.SOF=UI_SOF;
   Delete->framehead.Data_Length=6+2;
   Delete->framehead.Seq=UI_Seq;
   Delete->framehead.CRC8=Get_CRC8_Check_Sum_UI((u8 *)Delete,4,0xFF);
   Delete->framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //����ͷ����
   
   Delete->datahead.Data_ID=UI_Data_ID_Del;
   Delete->datahead.Sender_ID=Robot_ID;
   Delete->datahead.Receiver_ID=Cilent_ID;                          //����������
   
   Delete->CRC16 = Get_CRC16_Check_Sum_UI((u8 *)Delete, sizeof(Delete) - 2, 0xFFFF);
   
   HAL_UART_Transmit_DMA(&huart6, (u8 *)Delete, sizeof(UI_Delete_t));
   
   UI_Seq++;                                          //�����+1
}


/*****************************************************CRC8У��ֵ����**********************************************/
const unsigned char CRC8_INIT_UI = 0xff; 

const unsigned char CRC8_TAB_UI[256] = 
{ 
0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41, 
0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 
0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 
0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 
0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07, 
0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a, 
0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 
0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 
0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd, 
0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 
0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 
0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 
0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b, 
0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 
0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 
0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35, 
};

unsigned char Get_CRC8_Check_Sum_UI(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8) 
{ 
   unsigned char ucIndex; 
   while (dwLength--) 
   { 
      ucIndex = ucCRC8^(*pchMessage++); 
      ucCRC8 = CRC8_TAB_UI[ucIndex]; 
   } 
   return(ucCRC8); 
}

u16 CRC_INIT_UI = 0xffff; 

const u16 wCRC_Table_UI[256] = 
{ 
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876, 
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5, 
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974, 
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738, 
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7, 
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9, 
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/* 
** Descriptions: CRC16 checksum function 
** Input: Data to check,Stream length, initialized checksum 
** Output: CRC checksum 
*/ 
u16 Get_CRC16_Check_Sum_UI(u8 *pchMessage,u32 dwLength,u16 wCRC) 
{ 
   u8 chData; 
   if (pchMessage == NULL) 
   { 
      return 0xFFFF; 
   } 
   while(dwLength--) 
   { 
      chData = *pchMessage++;
      (wCRC) = ((u16)(wCRC) >> 8) ^ wCRC_Table_UI[((u16)(wCRC) ^ (u16)(chData)) & 
      0x00ff]; 
   } 
   return wCRC; 
}




//void Task_UI(void* pvParament)
//{
//   //����UI_PushUp_Counter�ж϶��Ǻܹ��������
//   //����Ϊ����������ִ��һ�Σ�ֻ�ᷢ��һ��UI����
//   static u16 UI_PushUp_Counter = 261;
//   float Capacitance_X = 0;

//   //����ϵͳ��ʼ��
//   vTaskDelay(300);

//   TickType_t last_time = xTaskGetTickCount();

//   for(;;)
//   {
//      vTaskDelayUntil(&last_time, pdMS_TO_TICKS(10));      //��ʱ�������棬��Ϊ�˷���һ�����ݣ�������ʱ����ֹ����

//      UI_PushUp_Counter++;
//		 
//      //ID�ж�
//      if(UI_PushUp_Counter % 1000 == 0)
//      {
//         ID_Judge();
//      }

//      //���UI��ʾ���������Կ����ǲ���ռ���˹ٷ�UI��λ�õ��µģ�����λ����һ��

//      //��̬UI------------------------------------------------------------------
//      //�����߶���
//      if(UI_PushUp_Counter % 301 == 0)
//		{
//			Line_Draw(&UI_Graph5.imageData[0], "001", UI_Graph_Add, 0, UI_Color_Green, 2,  840,  460,  1080,  460);
//			Line_Draw(&UI_Graph5.imageData[1], "002", UI_Graph_Add, 0, UI_Color_Green, 2,  855,  420,  1065,  420);
//			Line_Draw(&UI_Graph5.imageData[2], "003", UI_Graph_Add, 0, UI_Color_Green, 2,  870,  380,  1050,  380);
//			Line_Draw(&UI_Graph5.imageData[3], "004", UI_Graph_Add, 0, UI_Color_Green, 2,  895,  340,  1035,  340);
//			Line_Draw(&UI_Graph5.imageData[4], "005", UI_Graph_Add, 0, UI_Color_Green, 2,  960,  480,  960,  320);
//			UI_PushUp_Graphs(5, &UI_Graph5);
//			continue;            //����ѭ���ʼ
//		}

//      //������η���
//      if(UI_PushUp_Counter % 311 == 0)
//		{
//			Rectangle_Draw(&UI_Graph1.imageData[0], "006", UI_Graph_Add, 0, UI_Color_Green, 3,  860,  800,  1060,  790);
//			UI_PushUp_Graphs(1, &UI_Graph1);
//			continue;
//		}

//      //��̬��ʼUI---------------------------------------------------------
//      if(UI_PushUp_Counter % 321 == 0)
//		{
//         //���ݵ���
//         Float_Draw(&UI_Graph2.imageData[0],"201", UI_Graph_Add, 2, UI_Color_White, 24, 2, 4, 915, 830, 24.00);
//			Line_Draw(&UI_Graph2.imageData[1], "202", UI_Graph_Add, 2, UI_Color_Green, 10, 860, 800, 1060, 795);
//			UI_PushUp_Graphs(2, &UI_Graph2);
//			continue;
//		}

//      if(UI_PushUp_Counter % 331 == 0)
//		{
//         //Ħ���֡�����
//			Char_Draw(&UI_String1.String, "203", UI_Graph_Add, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tOFF\n\nSer:\tOFF\n\nFU:\tOFF   ");
//			UI_PushUp_String(&UI_String1);
//			continue;
//		}

//      if(UI_PushUp_Counter % 341 == 0)
//		{
//         //С���ݡ�����
//			Char_Draw(&UI_String2.String, "204", UI_Graph_Add, 2, UI_Color_Purplish_red, 18, 20-1, 3, 285, 650, "\nXTL:\tOFF\n\nAIM:\tOFF");
//			UI_PushUp_String(&UI_String2);
//			continue;
//		}

//      //��̬����UI-----------------------------------------------------------------------------
//      //�ַ�����(Ħ���֡�����)
//      if(UI_PushUp_Counter % 21 == 0)
//		{
//         if(UI_Flag.Shoot_UI_Flag==ON && State_Flag.Shoot_Fusillade_Flag==ON && State_Flag.Servo_Flag==ON && State_Flag.FU_Flag==ON)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tON|Fus\n\nSer\tON\n\nFU\tON  ");
//         }
//         else if(UI_Flag.Shoot_UI_Flag==ON && State_Flag.Shoot_Fusillade_Flag==ON && State_Flag.Servo_Flag==OFF && State_Flag.FU_Flag==ON)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tON|Fus\n\nSer:\tOFF\n\nFU:\tON ");
//         }
//         else if(UI_Flag.Shoot_UI_Flag==ON && State_Flag.Shoot_Fusillade_Flag==OFF && State_Flag.Servo_Flag==OFF && State_Flag.FU_Flag==ON)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tON|Sin\n\nSer:\tOFF\n\nFU:\tON ");
//         }		
//         else if(UI_Flag.Shoot_UI_Flag==ON && State_Flag.Shoot_Fusillade_Flag==OFF && State_Flag.Servo_Flag==ON && State_Flag.FU_Flag==ON)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tON|Sin\n\nSer:\tON\n\nFU:\tON  ");
//         }
//         else if(UI_Flag.Shoot_UI_Flag==OFF && State_Flag.Servo_Flag==ON && State_Flag.FU_Flag==ON)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tOFF\n\nSer:\tON\n\nFU:\tON     ");
//         }         
//         else if(UI_Flag.Shoot_UI_Flag==OFF && State_Flag.Servo_Flag==OFF && State_Flag.FU_Flag==ON)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tOFF\n\nSer:\tOFF\n\nFU:\tON    ");
//         } 
//         else if(UI_Flag.Shoot_UI_Flag==ON && State_Flag.Shoot_Fusillade_Flag==ON && State_Flag.Servo_Flag==ON && State_Flag.FU_Flag==OFF)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tON|Fus\n\nSer:\tON\n\nFU:\tOFF ");
//         }
//         else if(UI_Flag.Shoot_UI_Flag==ON && State_Flag.Shoot_Fusillade_Flag==ON && State_Flag.Servo_Flag==OFF && State_Flag.FU_Flag==OFF)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tON|Fus\n\nSer:\tOFF\n\nFU:\tOFF");
//         }
//         else if(UI_Flag.Shoot_UI_Flag==ON && State_Flag.Shoot_Fusillade_Flag==OFF && State_Flag.Servo_Flag==OFF && State_Flag.FU_Flag==OFF)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tON|Sin\n\nSer:\tOFF\n\nFU:\tOFF");
//         }		
//         else if(UI_Flag.Shoot_UI_Flag==ON && State_Flag.Shoot_Fusillade_Flag==OFF && State_Flag.Servo_Flag==ON && State_Flag.FU_Flag==OFF)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tON|Sin\n\nSer:\tON\n\nFU:\tOFF ");
//         }
//         else if(UI_Flag.Shoot_UI_Flag==OFF && State_Flag.Servo_Flag==ON && State_Flag.FU_Flag==OFF)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tOFF\n\nSer:\tON\n\nFU:\tOFF    ");
//         }         
//         else if(UI_Flag.Shoot_UI_Flag==OFF && State_Flag.Servo_Flag==OFF && State_Flag.FU_Flag==OFF)
//         {
//            Char_Draw(&UI_String1.String, "203", UI_Graph_Change, 2, UI_Color_Green, 18, 31-1, 3, 285, 650, "Fri:\tOFF\n\nSer:\tOFF\n\nFU:\tOFF   ");
//         } 
//			UI_PushUp_String(&UI_String1);
//			continue;
//		}
//      // �ַ�����(С���ݡ�����)
//      if(UI_PushUp_Counter % 29 == 0)
//		{
//         if(State_Flag.XTL_Flag==ON && UI_Flag.Aim_UI_Flag==ON && State_Flag.Aim_Flag==ON)
//         {
//            Char_Draw(&UI_String2.String, "204", UI_Graph_Change, 2, UI_Color_Purplish_red, 18, 23-1, 3, 285, 650, "\nXTL:\tON\n\nAIM:\tON|ON  ");
//         }
//         else if(State_Flag.XTL_Flag==ON && UI_Flag.Aim_UI_Flag==ON && State_Flag.Aim_Flag==OFF)
//         {
//            Char_Draw(&UI_String2.String, "204", UI_Graph_Change, 2, UI_Color_Purplish_red, 18, 23-1, 3, 285, 650, "\nXTL:\tON\n\nAIM:\tON|OFF ");
//         }
//         else if(State_Flag.XTL_Flag==ON && UI_Flag.Aim_UI_Flag==OFF)
//         {
//            Char_Draw(&UI_String2.String, "204", UI_Graph_Change, 2, UI_Color_Purplish_red, 18, 23-1, 3, 285, 650, "\nXTL:\tON\n\nAIM:\tOFF    ");
//         }
//         if(State_Flag.XTL_Flag==OFF && UI_Flag.Aim_UI_Flag==ON && State_Flag.Aim_Flag==ON)
//         {
//            Char_Draw(&UI_String2.String, "204", UI_Graph_Change, 2, UI_Color_Purplish_red, 18, 23-1, 3, 285, 650, "\nXTL:\tOFF\n\nAIM:\tON|ON ");
//         }
//         else if(State_Flag.XTL_Flag==OFF && UI_Flag.Aim_UI_Flag==ON && State_Flag.Aim_Flag==OFF)
//         {
//            Char_Draw(&UI_String2.String, "204", UI_Graph_Change, 2, UI_Color_Purplish_red, 18, 23-1, 3, 285, 650, "\nXTL:\tOFF\n\nAIM:\tON|OFF");
//         }
//         else if(State_Flag.XTL_Flag==OFF && UI_Flag.Aim_UI_Flag==OFF)
//         {
//            Char_Draw(&UI_String2.String, "204", UI_Graph_Change, 2, UI_Color_Purplish_red, 18, 23-1, 3, 285, 650, "\nXTL:\tOFF\n\nAIM:\tOFF   ");
//         }
//			UI_PushUp_String(&UI_String2);
//			continue;
//		}

//      // �ַ�����(������ʾ)
//      if(UI_PushUp_Counter % 37 == 0)
//		{
//         if(State_Flag.Shoot_Stuck_Flag==ON)
//         {
//            Char_Draw(&UI_String3.String, "205", UI_Graph_Add, 2, UI_Color_Purplish_red, 18, 5-1, 3, 940, 900, "STUCK");
//         }
//         else
//         {
//            Char_Draw(&UI_String3.String, "205", UI_Graph_Del, 2, UI_Color_Purplish_red, 18, 5-1, 3, 940, 900, "STUCK");
//         }
//			UI_PushUp_String(&UI_String3);
//			continue;
//		}


//      //ͼ�θ���(����)
//		if(UI_PushUp_Counter % 10 == 0)
//		{
//			//������������ʾ
//         Float_Draw(&UI_Graph2.imageData[0], "201", UI_Graph_Change, 2, UI_Color_White, 24, 2, 4, 915, 830, Capacitor_Voltage);
//			
//			//�������ͼ�α�ʾ			
//         Capacitance_X  = 860+200*((Capacitor_Voltage*Capacitor_Voltage)-100)/(24*24-100);

//			if(18 < Capacitor_Voltage && Capacitor_Voltage <= 24) 
//            Line_Draw(&UI_Graph2.imageData[1], "202", UI_Graph_Change, 2, UI_Color_Green , 10, 860, 795, (u32)Capacitance_X, 795);
//			else if(15 < Capacitor_Voltage && Capacitor_Voltage <=  18) 
//            Line_Draw(&UI_Graph2.imageData[1], "202", UI_Graph_Change, 2, UI_Color_Yellow, 10, 860, 795, (u32)Capacitance_X, 795);
//			else if(0  < Capacitor_Voltage && Capacitor_Voltage <=  15) 
//            Line_Draw(&UI_Graph2.imageData[1], "202", UI_Graph_Change, 2, UI_Color_Purplish_red, 10, 860, 795, (u32)Capacitance_X, 795);
//			
//			UI_PushUp_Graphs(2, &UI_Graph2);
//			continue;
//		}
//		
//   }
//}

