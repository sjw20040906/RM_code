#include "kalman_filter.h"

/**
 * @author  Cyx
 * һ�׿������˲�������RoboMaster��̳
 * һά�������˲���
 * ʹ��ʱ�ȶ���һ��kalmanָ�룬Ȼ�����Kalman_Create()����һ���˲���
 * ÿ�ζ�ȡ�����������ݺ󼴿ɵ���KalmanFilter()�������ݽ����˲�
 *         һ��ʹ��ʾ��
 *         One_Kalman_t p;               //����һ���������˲����ṹ��
 *         float SersorData;             //��Ҫ�����˲�������
 *         Kalman_Create(&p,20,200);      //��ʼ�����˲�����Q=20 R=200����
 *         while(1)
 *         {
 *            SersorData = sersor();                     //��ȡ����
 *            SersorData = One_Kalman_Filter(&p,SersorData);  //�����ݽ����˲�
 *         }
 *			����ʹ��ʾ��
 *
 *
 */

/**
 * @name   kalman_Create
 * @brief  ����һ���������˲���
 * @param  p:  �˲���
 *         T_Q:ϵͳ����Э����
 *         T_R:��������Э����
 *
 * @retval none
 * @attention
 *	R�̶���QԽ�󣬴���Խ���β���ֵ��Q�������ֻ�ò���ֵ
 *	��֮��QԽС����Խ����ģ��Ԥ��ֵ��QΪ������ֻ��ģ��Ԥ��
 */
void One_Kalman_Create(One_Kalman_t *Kal, float T_Q, float T_R)
{
  Kal->X_Last = (float)0;
  Kal->P_Last = 1;
  Kal->Q = T_Q;
  Kal->R = T_R;
  Kal->A = 1;
  Kal->B = 0;
  Kal->H = 1;
  Kal->X_Mid = Kal->X_Last;
}

/**
 * @name   Kalman_Filter
 * @brief  �������˲���
 * @param  Kal:  �˲���
 *         Data:���˲�����
 * @retval �˲��������
 * @attention
 *	Data(Z(k))��ϵͳ����,������ֵ
 *	X(k|k)�ǿ������˲����ֵ,���������
 *	A=1 B=0 H=1 I=1  W(K)(��������)/V(k)(�۲�/ϵͳ����)�Ǹ�˹������,�����ڲ���ֵ����,���Բ��ù�
 *	�����ǿ�������5�����Ĺ�ʽ
 *	һ��H'��Ϊ������,����Ϊת�þ���
 *	��ʽ(1)    x(k|k-1) = A*X(k-1|k-1)+B*U(k)+W(K)
 *	��ʽ(2)    p(k|k-1) = A*p(k-1|k-1)*A'+Q
 *	��ʽ(3)    kg(k) = p(k|k-1)*H'/(H*p(k|k-1)*H'+R)
 *	��ʽ(4)    x(k|k) = X(k|k-1)+kg(k)*(Z(k)-H*X(k|k-1))
 *	��ʽ(5)    p(k|k) = (I-kg(k)*H)*P(k|k-1)
 */

float One_Kalman_Filter(One_Kalman_t *Kal, float Data)
{
  Kal->X_Mid = Kal->A * Kal->X_Last;                       //(1)���׶ε��������
  Kal->P_Mid = Kal->A * Kal->P_Last + Kal->Q;              //(2)���׶�������Ƶ�Э����
  Kal->Kt = Kal->P_Mid / (Kal->P_Mid + Kal->R);            //(3)�������������(ȡһ��H=1)
  Kal->X_Now = Kal->X_Mid + Kal->Kt * (Data - Kal->X_Mid); //(4)���㵱�׶ε����Ź���
  Kal->P_Now = (1 - Kal->Kt) * Kal->P_Mid;                 //(5)���㵱�׶����Ź��Ƶ�Э����
  Kal->P_Last = Kal->P_Now;                                // ״̬����
  Kal->X_Last = Kal->X_Now;                                // ״̬����
  return Kal->X_Now;                                       // ������Ź���x(k|k)
}
/**
 * ���׿������˲���
 *
 */
/************************************************/

float matrix_value1;
float matrix_value2;
/**
 * @name   kalman_filter_init
 * @brief  ���׿������˲�������
 * @param  F:�˲���
 *         I:����������
 * @retval
 */
void Two_Kalman_Filter_Init(Two_Kalman_Filter_t *F, Two_Kalman_Filter_Init_t *I)
{
  //  mat_init(&F->xhat, 2, 1, (float *)I->xhat_data);
  //  mat_init(&F->xhatminus, 2, 1, (float *)I->xhatminus_data);
  //  mat_init(&F->z, 2, 1, (float *)I->z_data);
  //  mat_init(&F->A, 2, 2, (float *)I->A_data);
  //  mat_init(&F->H, 2, 2, (float *)I->H_data);
  //  mat_init(&F->Q, 2, 2, (float *)I->Q_data);
  //  mat_init(&F->R, 2, 2, (float *)I->R_data);
  //  mat_init(&F->P, 2, 2, (float *)I->P_data);
  //  mat_init(&F->Pminus, 2, 2, (float *)I->Pminus_data);
  //  mat_init(&F->K, 2, 2, (float *)I->K_data);
  //  mat_init(&F->AT, 2, 2, (float *)I->AT_data);
  //  mat_trans(&F->A, &F->AT);
  //  mat_init(&F->HT, 2, 2, (float *)I->HT_data);
  //  mat_trans(&F->H, &F->HT);
  //  //  matrix_value2 = F->A.pData[1];
}

// xhatminus==x(k|k-1)  xhat==X(k-1|k-1)
// Pminus==p(k|k-1)     P==p(k-1|k-1)    AT==A'
// HT==H'   K==kg(k)    I=1
//

/**
 *@param �����������ṹ��
 *@param �Ƕ�
 *@param �ٶ�
 */

/*********************************************/
float *Two_Kalman_Filter_Calc(Two_Kalman_Filter_t *F, float signal1, float signal2)
{
  //  float TEMP_data[4] = {0, 0, 0, 0};
  //  float TEMP_data21[2] = {0, 0};
  //  mat TEMP, TEMP21;

  //  mat_init(&TEMP, 2, 2, (float *)TEMP_data);     //
  //  mat_init(&TEMP21, 2, 1, (float *)TEMP_data21); //

  //  F->z.pData[0] = signal1; //z(k)
  //  F->z.pData[1] = signal2; //z(k)

  //  //1. xhat'(k)= A xhat(k-1)
  //  mat_mult(&F->A, &F->xhat, &F->xhatminus); //  x(k|k-1) = A*X(k-1|k-1)+B*U(k)+W(K)

  //  //2. P'(k) = A P(k-1) AT + Q
  //  mat_mult(&F->A, &F->P, &F->Pminus);  //   p(k|k-1) = A*p(k-1|k-1)*A'+Q
  //  mat_mult(&F->Pminus, &F->AT, &TEMP); //  p(k|k-1) = A*p(k-1|k-1)*A'+Q
  //  mat_add(&TEMP, &F->Q, &F->Pminus);   //    p(k|k-1) = A*p(k-1|k-1)*A'+Q

  //  //3. K(k) = P'(k) HT / (H P'(k) HT + R)
  //  mat_mult(&F->H, &F->Pminus, &F->K); //  kg(k) = p(k|k-1)*H'/(H*p(k|k-1)*H'+R)
  //  mat_mult(&F->K, &F->HT, &TEMP);     //      kg(k) = p(k|k-1)*H'/(H*p(k|k-1)*H'+R)
  //  mat_add(&TEMP, &F->R, &F->K);       //        kg(k) = p(k|k-1)*H'/(H*p(k|k-1)*H'+R)

  //  mat_inv(&F->K, &F->P);               //
  //  mat_mult(&F->Pminus, &F->HT, &TEMP); //
  //  mat_mult(&TEMP, &F->P, &F->K);       //

  //  //4. xhat(k) = xhat'(k) + K(k) (z(k) - H xhat'(k))
  //  mat_mult(&F->H, &F->xhatminus, &TEMP21);   //      x(k|k) = X(k|k-1)+kg(k)*(Z(k)-H*X(k|k-1))
  //  mat_sub(&F->z, &TEMP21, &F->xhat);         //            x(k|k) = X(k|k-1)+kg(k)*(Z(k)-H*X(k|k-1))
  //  mat_mult(&F->K, &F->xhat, &TEMP21);        //           x(k|k) = X(k|k-1)+kg(k)*(Z(k)-H*X(k|k-1))
  //  mat_add(&F->xhatminus, &TEMP21, &F->xhat); //    x(k|k) = X(k|k-1)+kg(k)*(Z(k)-H*X(k|k-1))

  //  //5. P(k) = (1-K(k)H)P'(k)
  //  mat_mult(&F->K, &F->H, &F->P); //            p(k|k) = (I-kg(k)*H)*P(k|k-1)
  //  mat_sub(&F->Q, &F->P, &TEMP);  //
  //  mat_mult(&TEMP, &F->Pminus, &F->P);

  //  matrix_value1 = F->xhat.pData[0];
  //  matrix_value2 = F->xhat.pData[1];

  //  F->filtered_value[0] = F->xhat.pData[0];
  //  F->filtered_value[1] = F->xhat.pData[1];
  //  return F->filtered_value;
}
