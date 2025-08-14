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
 
#ifndef __UI_H__
#define __UI_H__

#include "Protocol_Judgement.h"
#include "typedef.h"
#include "usart.h"
#include "freeRTOS.h"
#pragma pack(1)                           //��1�ֽڶ���

#define NULL 0
#define M__FALSE 100

/****************************��ʼ��־*********************/
#define UI_SOF 0xA5
/****************************CMD_ID����********************/
#define UI_CMD_Robo_Exchange 0x0301    
/****************************����ID����********************/
#define UI_Data_ID_Del 0x100 
#define UI_Data_ID_Draw1 0x101
#define UI_Data_ID_Draw2 0x102
#define UI_Data_ID_Draw5 0x103
#define UI_Data_ID_Draw7 0x104
#define UI_Data_ID_DrawChar 0x110
/****************************�췽ID********************/
#define UI_Data_RobotID_RHero 1         
#define UI_Data_RobotID_REngineer 2
#define UI_Data_RobotID_RStandard1 3
#define UI_Data_RobotID_RStandard2 4
#define UI_Data_RobotID_RStandard3 5
#define UI_Data_RobotID_RAerial 6
#define UI_Data_RobotID_RSentry 7
#define UI_Data_RobotID_RRadar 9
/****************************����ID********************/
#define UI_Data_RobotID_BHero 101
#define UI_Data_RobotID_BEngineer 102
#define UI_Data_RobotID_BStandard1 103
#define UI_Data_RobotID_BStandard2 104
#define UI_Data_RobotID_BStandard3 105
#define UI_Data_RobotID_BAerial 106
#define UI_Data_RobotID_BSentry 107
#define UI_Data_RobotID_BRadar 109
/**************************�췽�ͻ���ID************************/
#define UI_Data_CilentID_RHero 0x0101
#define UI_Data_CilentID_REngineer 0x0102
#define UI_Data_CilentID_RStandard1 0x0103
#define UI_Data_CilentID_RStandard2 0x0104
#define UI_Data_CilentID_RStandard3 0x0105
#define UI_Data_CilentID_RAerial 0x0106
/***************************�����ͻ���ID***********************/
#define UI_Data_CilentID_BHero 0x0165
#define UI_Data_CilentID_BEngineer 0x0166
#define UI_Data_CilentID_BStandard1 0x0167
#define UI_Data_CilentID_BStandard2 0x0168
#define UI_Data_CilentID_BStandard3 0x0169
#define UI_Data_CilentID_BAerial 0x016A
/***************************ɾ������***************************/
#define UI_Data_Del_NoOperate 0
#define UI_Data_Del_Layer 1
#define UI_Data_Del_ALL 2
/***********************ͼ�β���********************/
#define UI_Graph_Add 1
#define UI_Graph_Change 2
#define UI_Graph_Del 3
/***************************ͼ������********************/
#define UI_Graph_Line 0         //ֱ��
#define UI_Graph_Rectangle 1    //����
#define UI_Graph_Circle 2       //��Բ
#define UI_Graph_Ellipse 3      //��Բ
#define UI_Graph_Arc 4          //Բ��
#define UI_Graph_Float 5        //������
#define UI_Graph_Int 6          //����
#define UI_Graph_Char 7         //�ַ���
/***************************ͼ����ɫ********************/
#define UI_Color_Main 0         //������ɫ
#define UI_Color_Yellow 1
#define UI_Color_Green 2
#define UI_Color_Orange 3
#define UI_Color_Purplish_red 4 //�Ϻ�ɫ
#define UI_Color_Pink 5
#define UI_Color_Cyan 6         //��ɫ
#define UI_Color_Black 7
#define UI_Color_White 8


typedef struct
{
   u8 SOF;                    //��ʼ�ֽ�,�̶�0xA5
   u16 Data_Length;           //֡���ݳ���
   u8 Seq;                    //�����
   u8 CRC8;                   //CRC8У��ֵ
   u16 CMD_ID;                //����ID
} UI_Packhead;             //֡ͷ

typedef struct
{
   u16 Data_ID;               //����ID
   u16 Sender_ID;             //������ID
   u16 Receiver_ID;           //����ID
} UI_Data_Operate;         //��������֡

typedef struct
{
   u8 Delete_Operate;         //ɾ������
   u8 Layer;                  //ɾ��ͼ��
} UI_Data_Delete;          //ɾ��֡

typedef struct
{ 
   u8 graphic_name[3]; 
   u32 operate_tpye:3; 
   u32 graphic_tpye:3; 
   u32 layer:4; 
   u32 color:4; 
   u32 start_angle:9;
   u32 end_angle:9;
   u32 width:10; 
   u32 start_x:11; 
   u32 start_y:11;
   int32_t graph_Float;              //��������
} Float_Data;

typedef struct
{ 
u8 graphic_name[3]; 
u32 operate_tpye:3; 
u32 graphic_tpye:3; 
u32 layer:4; 
u32 color:4; 
u32 start_angle:9;
u32 end_angle:9;
u32 width:10; 
u32 start_x:11; 
u32 start_y:11;
u32 radius:10; 
u32 end_x:11; 
u32 end_y:11;              //ͼ������
} Graph_Data;


typedef struct
{
   Graph_Data Graph_Control;
   char show_Data[30];
} String_Data;                  //�ַ�������

typedef struct
{
	UI_Packhead framehead;
	UI_Data_Operate datahead;
	Graph_Data imageData[1];
	u16 CRC16;
} UI_Graph1_t;

typedef struct
{
	UI_Packhead framehead;
	UI_Data_Operate datahead;
	Graph_Data imageData[2];
	u16 CRC16;
} UI_Graph2_t;

typedef struct
{
	UI_Packhead framehead;
	UI_Data_Operate datahead;
	Graph_Data imageData[5];
	u16 CRC16;
} UI_Graph5_t;

typedef struct
{
	UI_Packhead framehead;
	UI_Data_Operate datahead;
	Graph_Data imageData[7];
	u16 CRC16;
} UI_Graph7_t;

typedef struct
{ 
	UI_Packhead framehead;
	UI_Data_Operate datahead;
	String_Data String;
	u16 CRC16;
}
UI_String_t;

typedef struct
{
	UI_Packhead framehead;
	UI_Data_Operate datahead;
	UI_Data_Delete Delete;
	u16 CRC16;
} UI_Delete_t;

//extern TaskHandle_t UI_Handle;

void ID_Judge(void);
void UI_init(void);
void UI_PushUp_Graphs(u8 cnt, void *Graphs);
void UI_PushUp_String(UI_String_t *String);
void UI_PushUp_Delete(UI_Delete_t *Delete);
void Line_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y);
unsigned char Get_CRC8_Check_Sum_UI(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
u16 Get_CRC16_Check_Sum_UI(u8 *pchMessage,u32 dwLength,u16 wCRC);
void Circle_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 Graph_Radius);
void Rectangle_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y);
void Float_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,float FloatData);
void Char_Draw(String_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,char *Char_Data);
void Arc_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_StartAngle,u32 Graph_EndAngle,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 x_Length,u32 y_Length);


typedef struct
{
	void (*ID_Judge)(void);
	void (*UI_init)(void);
	void (*UI_PushUp_Graphs)(u8 cnt, void *Graphs);
	void (*UI_PushUp_String)(UI_String_t *String);
	void (*UI_PushUp_Delete)(UI_Delete_t *Delete);
	void (*Line_Draw)(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y);
	unsigned char (*Get_CRC8_Check_Sum_UI)(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
	u16 (*Get_CRC16_Check_Sum_UI)(u8 *pchMessage,u32 dwLength,u16 wCRC);
	void (*Circle_Draw)(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 Graph_Radius);
	void (*Rectangle_Draw)(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y);
	void (*Float_Draw)(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,float FloatData);
	void (*Char_Draw)(String_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,char *Char_Data);
	void (*Arc_Draw)(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_StartAngle,u32 Graph_EndAngle,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 x_Length,u32 y_Length);
		
} UI_FUN_t;


#define UI_FUNGroundInit               \
    {                                     \
			&ID_Judge,			  \
      &UI_init,                      \
			&UI_PushUp_Graphs,			  \
      &UI_PushUp_String,                      \
			&UI_PushUp_Delete,			  \
      &Line_Draw,                      \
			&Get_CRC8_Check_Sum_UI,			  \
      &Get_CRC16_Check_Sum_UI,                      \
			&Circle_Draw,			  \
      &Rectangle_Draw,                      \
			&Float_Draw,			  \
      &Char_Draw,                      \
			&Arc_Draw,			  \
    }



extern UI_Graph1_t UI_Graph1;
extern UI_Graph2_t UI_Graph2;
extern UI_Graph5_t UI_Graph5;
extern UI_Graph7_t UI_Graph7;
extern UI_String_t UI_String1;
extern UI_String_t UI_String2;
extern UI_String_t UI_String3;
extern UI_String_t UI_String4;
extern UI_String_t UI_String5;
extern UI_Delete_t UI_Delete;

extern UI_FUN_t UI_FUN;
		
#pragma pack()                           

#endif
