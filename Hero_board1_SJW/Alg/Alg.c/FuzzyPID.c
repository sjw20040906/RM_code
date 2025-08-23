/**
 * @file FuzzyPID.c
 * @author Why/xyz
 * @brief ����ĺ�������뷨��д��PID�ļ����ڲ�ȥ��ʵ��Fuzzy���ߵ���PID����
 *		  ��Ҫ������ֻ��PID�����ı䷶Χ
 * @version 0.1
 * @date 2023-09-28
 *
 */
 
#include "FuzzyPID.h"


FUZZYPID_Data_t fuzzy_pid_shoot_l={0};
FUZZYPID_Data_t fuzzy_pid_shoot_r={0};
FUZZYPID_Data_t fuzzy_pid_bullet_v={0};
FUZZYPID_Data_t fuzzy_pid_bullet_l={0};
FUZZYPID_Data_t fuzzy_pid_pitch_in={0};
FUZZYPID_Data_t fuzzy_pid_pitch_out={0};
#define NB   -3
#define NM	 -2
#define NS	 -1
#define ZO	 0
#define PS	 1
#define PM	 2
#define PB	 3

/* ����� */
static const float ruleKp[7][7] = {
  PB,	PB,	PM,	PM,	PS,	ZO,	ZO,
    PB,	PB,	PM,	PS,	PS,	ZO,	NS,
    PM,	PM,	PM,	PS,	ZO,	NS,	NS,
    PM,	PM,	PS,	ZO,	NS,	NM,	NM,
    PS,	PS,	ZO,	NS,	NS,	NM,	NM,
    PS,	ZO,	NS,	NM,	NM,	NM,	NB,
    ZO,	ZO,	NM,	NM,	NM,	NB,	NB
};

static const float ruleKi[7][7] = {
   NB,	NB,	NM,	NM,	NS,	ZO,	ZO,
    NB,	NB,	NM,	NS,	NS,	ZO,	ZO,
    NB,	NM,	NS,	NS,	ZO,	PS,	PS,
    NM,	NM,	NS,	ZO,	PS,	PM,	PM,
    NS,	NS,	ZO,	PS,	PS,	PM,	PB,
    ZO,	ZO,	PS,	PS,	PM,	PB,	PB,
    ZO,	ZO,	PS,	PM,	PM,	PB,	PB
};

static const float ruleKd[7][7] = {
  PS,	NS,	NB,	NB,	NB,	NM,	PS,
    PS,	NS,	NB,	NM,	NM,	NS,	ZO,
    ZO,	NS,	NM,	NM,	NS,	NS,	ZO,
    ZO,	NS,	NS,	NS,	NS,	NS,	ZO,
    ZO,	ZO,	ZO,	ZO,	ZO,	ZO,	ZO,
    PB,	NS,	PS,	PS,	PS,	PS,	PB,
    PB,	PM,	PM,	PM,	PS,	PS,	PB
};

FUZZYPID_Data_t FuzzyPID_Pitch = FUZZYPID_Pitch_GroupInit;
#undef FUZZYPID_Pitch_GroupInit


void fuzzy_init(FUZZYPID_Data_t *PID,float _maximum,float _minimum,float _qkp,float _qki,float _qkd)
{	PID->delta_kp=0;
	PID->delta_ki=0;
	PID->delta_kd=0;
	
	PID->error_maximum=_maximum;
	PID->error_minimum=_minimum;
	
	PID->qKp=_qkp;
	PID->qKi=_qki;
	PID->qKd=_qkd;

}


/**
  * @brief  ���������ı仯��ת����-3��3�������ϣ�����e����ec
  * @param  FUZZYPID_Data_t *PID --��Ӧ��FuzzyPID�ṹ��ָ��
			float thisError       --��ε����
			float lastError       --��һ�ε����
			float *qValue         --��E��EC�����������׵�ַָ��
  * @retval None
  */
void Linear_Quantization(FUZZYPID_Data_t *PID, float thisError, float lastError, float *qValue)
{
    float deltaError;
    deltaError = thisError - lastError;   //����ƫ������

    //E��EC������
    qValue[0] = 3.0f * thisError*2 / (PID->error_maximum - PID->error_minimum);
    qValue[1] = 3.0f * deltaError /  (PID->error_maximum - PID->error_minimum);
	if(qValue[0] >=5)
	{qValue[0]=5;
	}
	if(qValue[0]<=-5)
	{qValue[0]=-5;
	}
	if(qValue[1] >=5)
	{qValue[1]=5;
	}
	if(qValue[1]<=-5)
	{qValue[1]=-5;
	}

}
/**
  * @brief  �����ȼ��㺯����ʹ�ô����Ե�����
  * @param  float *ms 		--��Ӧģ���Ӽ���������
			float qv      	--�������ģ����
			int *index      --���ݴ�С�������±�������׵�ַ
  * @retval None
  */
void Membership_Calc(float *ms, float qv, int8_t *index)
{
    if((qv >= NB) && (qv < NM))
    {
        index[0] = 0;
        index[1] = 1;
        ms[0] = -2-qv; 	
        ms[1] = qv + 3.0f;  	   
    }
    else if((qv >= NM) && (qv < NS))
    {
        index[0] = 1;
        index[1] = 2;
        ms[0] = -1 * qv - 1.0f; 	//y=-1x-1.0
        ms[1] = qv + 2.0f;  	   	//y=0.5x+2.0
    }
    else if((qv >= NS) && (qv < ZO))
    {
        index[0] = 2;
        index[1] = 3;
        ms[0] = -1 * qv;  	   		//y=-1x
        ms[1] = qv + 1.0f; 	   		//y=0.5x+1.0
    }
    else if((qv >= ZO) && (qv < PS))
    {
        index[0] = 3;
        index[1] = 4;
        ms[0] = -1 * qv + 1.0f; 	//y=-1x+1.0
        ms[1] = qv;   	 	   		//y=0.5x
    }
    else if((qv >= PS) && (qv < PM))
    {
        index[0] = 4;
        index[1] = 5;
        ms[0] = -1 * qv + 2.0f; 	//y=-1x+2.0
        ms[1] = qv - 1.0f; 	 	  	//y=0.5x-1.0
    }
    else if((qv >= PM) && (qv <= PB))
    {
        index[0] = 5;
        index[1] = 6;
        ms[0] = -1 * qv + 3.0f; 	//y=-1x+3.0
        ms[1] = qv - 2.0f;  	   	//y=0.5x-2.0
    }
}

/**
  * @brief  ����FuzzyPID�����PID�����ĸı�ֵ
  * @param  FUZZYPID_Data_t *PID --��Ӧ��ģ��PID�ṹ��
			float thisError       --��ε����
			float lastError       --�ϴε����
  * @retval None
  */
void FuzzyComputation (FUZZYPID_Data_t *PID, float thisError, float lastError)
{
 /* pid����ֵ */
    float pidvalue[3]={0};

    /* ���� */
    Linear_Quantization(PID, thisError, lastError,PID->error_map);
    //����e�������Ⱥ����� */
    Membership_Calc(PID->error_membership_degree, PID->error_map[0], PID->error_membership_index);
    /* ����ec�������Ⱥ����� */
  Membership_Calc(PID->d_error_membership_degree, PID->error_map[1], PID->d_error_membership_index);

   pidvalue[0] +=PID->error_membership_degree[0] *PID->d_error_membership_degree[0]*  ruleKp[PID->error_membership_index[0]][PID->d_error_membership_index[0]];
	 pidvalue[0] +=PID->error_membership_degree[0] *PID->d_error_membership_degree[1]*  ruleKp[PID->error_membership_index[0]][PID->d_error_membership_index[1]];
	 pidvalue[0] +=PID->error_membership_degree[1] *PID->d_error_membership_degree[0]*  ruleKp[PID->error_membership_index[1]][PID->d_error_membership_index[0]];
	 pidvalue[0] +=PID->error_membership_degree[1] *PID->d_error_membership_degree[1]*  ruleKp[PID->error_membership_index[1]][PID->d_error_membership_index[1]];
		
	 pidvalue[1] +=PID->error_membership_degree[0] *PID->d_error_membership_degree[0]*  ruleKi[PID->error_membership_index[0]][PID->d_error_membership_index[0]];
	 pidvalue[1] +=PID->error_membership_degree[0] *PID->d_error_membership_degree[1]*  ruleKi[PID->error_membership_index[0]][PID->d_error_membership_index[1]];
	 pidvalue[1] +=PID->error_membership_degree[1] *PID->d_error_membership_degree[0]*  ruleKi[PID->error_membership_index[1]][PID->d_error_membership_index[0]];
	 pidvalue[1] +=PID->error_membership_degree[1] *PID->d_error_membership_degree[1]*  ruleKi[PID->error_membership_index[1]][PID->d_error_membership_index[1]];
		
	 pidvalue[2] +=PID->error_membership_degree[0] *PID->d_error_membership_degree[0]*  ruleKd[PID->error_membership_index[0]][PID->d_error_membership_index[0]];
	 pidvalue[2] +=PID->error_membership_degree[0] *PID->d_error_membership_degree[1]*  ruleKd[PID->error_membership_index[0]][PID->d_error_membership_index[1]];
	 pidvalue[2] +=PID->error_membership_degree[1] *PID->d_error_membership_degree[0]*  ruleKd[PID->error_membership_index[1]][PID->d_error_membership_index[0]];
	 pidvalue[2] +=PID->error_membership_degree[1] *PID->d_error_membership_degree[1]*  ruleKd[PID->error_membership_index[1]][PID->d_error_membership_index[1]];
   
    /* pid�������� */
		//�õ�����pid�ı�ı����������qkp/qki/qkd��������
   PID->delta_kp = pidvalue[0]/3; 
   PID->delta_ki = pidvalue[1]/3; 
   PID->delta_kd = pidvalue[2]/3; 
}