/**
 * @file Gimbal_Chassis_Pitch_Angle.c
 * @author ZS
 * @brief
 * @version 1.0
 * @date 2024-11-2
 *
 * @copyright 
 *
 */

#include <math.h>  
#include "Saber_C3.h"
#include "BSP_BoardCommunication.h"
#include "Gimbal_Chassis_Pitch_Angle.h"
  
// ����PI����  
#ifndef M_PI  
#define M_PI 3.1415926536 
#endif  
  
// ���ǶȴӶ�ת��Ϊ����  
float degreesToRadians(float degrees) 
{  
    return degrees * M_PI / 180.0f;  
}  
    
float Pitch_Compensation;
float Gimbal_Chassis_offset = 38;   //GM6020�����λ�ú͵���yaw�����֮��ļнǣ�ƫ������

/**
 * @brief         ������̨pitch�Ჹ�����ĺ���
 * @param         ����Saber����Ƕ�
 * @param         ��̨����ڵ��̵�yaw�Ƕ�
 * @retval        ��̨pitch�Ჹ����
 */ 
float calculateGimbalPitchCompensation(IMUData chassisIMUData, float gimbalYawAngle) 
{  
  
    // ���̵�pitch��roll�ǶȽ�Ӱ����̨��ʵ��pitch�Ƕ�  
    // ����̨��yaw����ת�󣬵��̵�pitch��Ӱ����̨��roll���������̨��������ϵ��  
    // �����̵�roll��Ӱ����̨��pitch������Ҫͨ����̨yaw�Ƕȵ���ת�������任��  
  
    // ������ֻ̨��yaw����ת�����ǿ���ʹ�ö�ά��ת��ʽ�����Ƽ���   
  
    // ��̨pitch��Ĳ��������Կ����ǵ���pitch��roll�Ƕ�����̨����ϵ�е�ͶӰ  
    // ���ͶӰ����ͨ����ת���������㣬������������ʹ�ý��Ʒ���  
  
    // ���Ƽ�����̨pitch��Ĳ�����  
    // ������̨��yaw��ת��gimbalYawAngle�����̵�pitch����Ӱ����̨��"��Чroll"  
    // �����̵�roll���ڣ�ͨ����ת��Ӱ����̨��"��Чpitch"  
    // ������������Ҫ������̨����ڵ����pitch���䣬����ֻ��Ҫ���ǵ���pitch��"��ЧӰ��"  
    // ���"��ЧӰ��"����ͨ������pitch����һ������̨yaw�Ƕ���ص�ϵ�������ƣ����ϵ����cos(gimbalYawAngle)��  
    // ͬʱ�����ǻ���Ҫ����һ�����ڵ���roll�����"��Чpitch"�仯������仯����ͨ������roll����sin(gimbalYawAngle)������   
  
    float compensation = chassisIMUData.pitch * cos(gimbalYawAngle) + chassisIMUData.roll * sin(gimbalYawAngle);  
  
    return compensation;   // ���ز�����  
}  
 
/**
 * @brief  ��Saber��pitch�ǵ�ֵ����ControlMes�ṹ���еĶ�Ӧ��
 * @param  None
 * @retval None
 */
void Gimbal_Chassis_Pitch_Translate(void) 
{  
    IMUData chassisIMUData = { .pitch = Saber_Angle.Pitch, .yaw = Saber_Angle.Yaw, .roll = Saber_Angle.RoLL }; // ����Saber����
		/***********����begin************/
		if(chassisIMUData.pitch < 1.0f && chassisIMUData.pitch > -1.0f)
		{
			chassisIMUData.pitch = 0;
		}
		if(chassisIMUData.roll < 1.0f && chassisIMUData.roll > -1.0f)
		{
			chassisIMUData.roll = 0;
		}
		/***********����end************/
		
		chassisIMUData.pitch = degreesToRadians(chassisIMUData.pitch);   //�Ƕ�תΪ����
		chassisIMUData.roll = degreesToRadians(chassisIMUData.roll);
		chassisIMUData.yaw = degreesToRadians(chassisIMUData.yaw);
		
    float gimbalYawAngle = ControlMes.yaw_realAngle / 8192.f * 360.f - Saber_Angle.Yaw - Gimbal_Chassis_offset; // ��̨����ڵ��̵�yaw�Ƕ� 
		gimbalYawAngle = degreesToRadians(gimbalYawAngle);
  
    // ���ú������㲹����  
    Pitch_Compensation = calculateGimbalPitchCompensation(chassisIMUData, gimbalYawAngle);
    ControlMes.Gimbal_Chassis_Pitch_Angle = (int16_t)(Pitch_Compensation / M_PI *8192.f);
    
}


  
  
// ������ת���󣨻���ZYXŷ���ǣ�������Z��תyaw������Y��תpitch�������X��תroll��  
//RotationMatrix createRotationMatrixFromEulerAngles(float yaw, float pitch, float roll) 
//{  
//    yaw = degreesToRadians(yaw);  
//    pitch = degreesToRadians(pitch);  
//    roll = degreesToRadians(roll);  
//  
//    RotationMatrix R;  
//    R.row1.x = cos(yaw) * cos(roll) - sin(yaw) * sin(pitch) * sin(roll);  
//    R.row1.y = sin(yaw) * cos(roll) + cos(yaw) * sin(pitch) * sin(roll);  
//    R.row1.z = -sin(roll) * cos(pitch);  
//  
//    R.row2.x = -cos(yaw) * sin(roll) - sin(yaw) * sin(pitch) * cos(roll);  
//    R.row2.y = -sin(yaw) * sin(roll) + cos(yaw) * sin(pitch) * cos(roll);  
//    R.row2.z = cos(roll) * cos(pitch);  
//  
//    R.row3.x = sin(yaw) * cos(pitch);  
//    R.row3.y = cos(yaw) * cos(pitch);  
//    R.row3.z = sin(pitch);  
//  
//    return R;  
//}  
//  
//// ������̨pitch�Ჹ�����ĺ���  
//float calculateGimbalPitchCompensation(RotationMatrix chassisRotationMatrix, float gimbalYawAngle, Vector3 chassisPitchVector) 
//{  
//    // gimbalYawAngle����̨����ڵ��̵�yaw��ת�Ƕȣ��Զ�Ϊ��λ��  
//    // chassisPitchVector�ǵ���pitch���ڵ�������ϵ�еı�ʾ��ͨ��Ϊ{0, 1, 0}��  
//  
//    gimbalYawAngle = degreesToRadians(gimbalYawAngle);  
//  
//    // ������ʾ��̨����ڵ�����ת����ת���󣨽���yaw����ת��  
//    RotationMatrix gimbalYawRotationMatrix = {  
//        {cos(gimbalYawAngle), 0, sin(gimbalYawAngle)},  
//        {0, 1, 0},  
//        {-sin(gimbalYawAngle), 0, cos(gimbalYawAngle)}  
//    };  
//  
//    // �����̵���ת��������̨��yaw��ת������ˣ��õ���̨����������ϵ�е���ת����  
//    RotationMatrix gimbalWorldRotationMatrix = {0};  
//    for (int i = 0; i < 3; i++) {  
//        for (int j = 0; j < 3; j++) {  
//            gimbalWorldRotationMatrix.row1.x += gimbalYawRotationMatrix.row1.x * chassisRotationMatrix.row1.x +  
//                                               gimbalYawRotationMatrix.row1.y * chassisRotationMatrix.row2.x +  
//                                               gimbalYawRotationMatrix.row1.z * chassisRotationMatrix.row3.x;  
//            if (i == 0) {  
//                gimbalWorldRotationMatrix.row1.y += gimbalYawRotationMatrix.row1.x * chassisRotationMatrix.row1.y +  
//                                                   gimbalYawRotationMatrix.row1.y * chassisRotationMatrix.row2.y +  
//                                                   gimbalYawRotationMatrix.row1.z * chassisRotationMatrix.row3.y;  
//                gimbalWorldRotationMatrix.row1.z += gimbalYawRotationMatrix.row1.x * chassisRotationMatrix.row1.z +  
//                                                   gimbalYawRotationMatrix.row1.y * chassisRotationMatrix.row2.z +  
//                                                   gimbalYawRotationMatrix.row1.z * chassisRotationMatrix.row3.z;  
//            } else if (i == 1) {  
//                gimbalWorldRotationMatrix.row2.x = gimbalYawRotationMatrix.row2.x * chassisRotationMatrix.row1.x +  
//                                                  gimbalYawRotationMatrix.row2.y * chassisRotationMatrix.row2.x +  
//                                                  gimbalYawRotationMatrix.row2.z * chassisRotationMatrix.row3.x;  
//                gimbalWorldRotationMatrix.row2.y = gimbalYawRotationMatrix.row2.x * chassisRotationMatrix.row1.y +  
//                                                  gimbalYawRotationMatrix.row2.y * chassisRotationMatrix.row2.y +  
//                                                  gimbalYawRotationMatrix.row2.z * chassisRotationMatrix.row3.y;  
//                gimbalWorldRotationMatrix.row2.z = gimbalYawRotationMatrix.row2.x * chassisRotationMatrix.row1.z +  
//                                                  gimbalYawRotationMatrix.row2.y * chassisRotationMatrix.row2.z +  
//                                                  gimbalYawRotationMatrix.row2.z * chassisRotationMatrix.row3.z;  
//            } else if (i == 2) {  
//                gimbalWorldRotationMatrix.row3.x = gimbalYawRotationMatrix.row3.x * chassisRotationMatrix.row1.x +  
//                                                  gimbalYawRotationMatrix.row3.y * chassisRotationMatrix.row2.x +  
//                                                  gimbalYawRotationMatrix.row3.z * chassisRotationMatrix.row3.x;  
//                gimbalWorldRotationMatrix.row3.y = gimbalYawRotationMatrix.row3.x * chassisRotationMatrix.row1.y +  
//                                                  gimbalYawRotationMatrix.row3.y * chassisRotationMatrix.row2.y +  
//                                                  gimbalYawRotationMatrix.row3.z * chassisRotationMatrix.row3.y;  
//                gimbalWorldRotationMatrix.row3.z = gimbalYawRotationMatrix.row3.x * chassisRotationMatrix.row1.z +  
//                                                  gimbalYawRotationMatrix.row3.y * chassisRotationMatrix.row2.z +  
//                                                  gimbalYawRotationMatrix.row3.z * chassisRotationMatrix.row3.z;  
//            }  
//        }  
//    }  
//  
//    // ������̨����������ϵ����Ҫ���ֵ�pitch����������ͨ��Ϊ{0, 1, 0}��  
//    Vector3 desiredPitchVector = {0, 1, 0};  
//  
//    // �����̵�pitch������ת������������ϵ��  
//    Vector3 chassisPitchVectorInWorld = {  
//        chassisPitchVector.x * gimbalWorldRotationMatrix.row1.x + chassisPitchVector.y * gimbalWorldRotationMatrix.row2.x + chassisPitchVector.z * gimbalWorldRotationMatrix.row3.x,  
//        chassisPitchVector.x * gimbalWorldRotationMatrix.row1.y + chassisPitchVector.y * gimbalWorldRotationMatrix.row2.y + chassisPitchVector.z * gimbalWorldRotationMatrix.row3.y,  
//        chassisPitchVector.x * gimbalWorldRotationMatrix.row1.z + chassisPitchVector.y * gimbalWorldRotationMatrix.row2.z + chassisPitchVector.z * gimbalWorldRotationMatrix.row3.z  
//    };  
//  
//    // ���㲹������������pitch������������ϵ�е�z��������ʾ��б�̶ȣ����෴��  
//    // ��Ϊ������Ҫ������̨����ڵ����ˮƽ��������Ҫ�������̵���б  
//    float compensation = -chassisPitchVectorInWorld.z;  
//  
//    return compensation;  
//}  
//  

//void Gimbal_Chassis_Pitch_Translate(void) 
//{  
//    float chassisYaw = Saber_Angle.Yaw; // ����yaw�Ƕȣ�ͨ�����Ժ��ԣ���Ϊ����ֻ����pitch��roll����̨pitch��Ӱ�죩  
//    float chassisPitch = Saber_Angle.Pitch ; // ����pitch�Ƕ�  
//    float chassisRoll = Saber_Angle.RoLL; // ����roll�Ƕ�  
//    float gimbalYawAngle = ControlMes.yaw_realAngle /8192.f *360.f - Saber_Angle.Yaw ; // ��̨����ڵ��̵�yaw��ת�Ƕ�  
//  
//    // ����������ת����  
//    RotationMatrix chassisRotationMatrix = createRotationMatrixFromEulerAngles(chassisYaw, chassisPitch, chassisRoll);  
//  
//    // ����pitch���ڵ�������ϵ�еı�ʾ  
//    Vector3 chassisPitchVector = {0, 1, 0};
//  
//    // ���ú������㲹����  
//    Pitch_Compensation = calculateGimbalPitchCompensation(chassisRotationMatrix, gimbalYawAngle, chassisPitchVector) + 1.f;  
//    ControlMes.Gimbal_Chassis_Pitch_Angle = (int16_t)(Pitch_Compensation / M_PI *8192.f);
//    
//}