#include "fuzzypid.h"
#include <math.h>

#define NUM_AREA	8

const float E_st[7]   = {-3,-2,-1,0,1,2,3};           //输入e的隶属值
const float Ec_st[7]  = { -3,-2,-1,0,1,2,3 };         //输入de/dt的隶属值
const float Kp_st[7]  = { -3,-2,-1,0,1,2,3 };         //输出增量kp的隶属值
const float Ki_st[7]  = { -3,-2,-1,0,1,2,3 };         //输出增量ki的隶属值
const float Kd_st[7]  = { -3,-2,-1,0,1,2,3 };         //输出增量kd的隶属值
const float Out_st[7] = { -3,-2,-1,0,1,2,3 };				  //pid输出的隶属度

static const int  Kp_rule_list[7][7] = { {PB,PB,PM,PM,PS,ZO,ZO},     //kp规则表
														             {PB,PB,PM,PS,PS,ZO,NS},
														             {PM,PM,PM,PS,ZO,NS,NS},
														             {PM,PM,PS,ZO,NS,NM,NM},
														             {PS,PS,ZO,NS,NS,NM,NM},
														             {PS,ZO,NS,NM,NM,NM,NB},
														             {ZO,ZO,NM,NM,NM,NB,NB} };

static const int  Ki_rule_list[7][7] = { {NB,NB,NM,NM,NS,ZO,ZO},     //ki规则表
																				 {NB,NB,NM,NS,NS,ZO,ZO},
																				 {NB,NM,NS,NS,ZO,PS,PS},
																				 {NM,NM,NS,ZO,PS,PM,PM},
																				 {NM,NS,ZO,PS,PS,PM,PB},
																				 {ZO,ZO,PS,PS,PM,PB,PB},
																				 {ZO,ZO,PS,PM,PM,PB,PB} };

static const int  Kd_rule_list[7][7] = { {PS,NS,NB,NB,NB,NM,PS},    //kd规则表
														             {PS,NS,NB,NM,NM,NS,ZO},
														             {ZO,NS,NM,NM,NS,NS,ZO},
														             {ZO,NS,NS,NS,NS,NS,ZO},
														             {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
														             {PB,NS,PS,PS,PS,PS,PB},
														             {PB,PM,PM,PM,PS,PS,PB} };

/**
	* @brief  e与de/dt隶属度计算函数
	* @retval None
	*/
void Get_grad_membership(float erro,float erro_c, FuzyPID_TypeDef* fpid) 
{
	//误差隶属度计算
		if(erro >= E_st[0] && erro <= E_st[6])
		{
			for(char i=0;i<NUM_AREA;i++)
			{
				if (erro >= E_st[i] && erro <= E_st[i + 1])
				{
						fpid->e_grad_mem[0] = -(erro - E_st[i + 1]) / (E_st[i + 1] - E_st[i]);
						fpid->e_grad_mem[1] = 1+(erro - E_st[i + 1]) / (E_st[i + 1] - E_st[i]);
						fpid->e_grad_index[0] = i;
						fpid->e_grad_index[1] = i + 1;
						break;
				}
			}
		}
		else
		{
			if (erro <= E_st[0])
			{
					fpid->e_grad_mem[0] = 1;
					fpid->e_grad_mem[1] = 0;
					fpid->e_grad_index[0] = 0;
					fpid->e_grad_index[1] = -1;
			}
			else if (erro >= E_st[6])
			{
					fpid->e_grad_mem[0] = 1;
					fpid->e_grad_mem[1] = 0;
					fpid->e_grad_index[0] = 6;
					fpid->e_grad_index[1] = -1;
			}
		}
	//误差微分隶属度计算
	if (erro_c > Ec_st[0] && erro_c < Ec_st[6])
    {
        for (int i = 0; i < NUM_AREA - 2; i++)
        {
            if (erro_c >= Ec_st[i] && erro_c <= Ec_st[i + 1])
            {
                fpid->ec_grad_mem[0] = -(erro_c - Ec_st[i + 1]) / (Ec_st[i + 1] - Ec_st[i]);
                fpid->ec_grad_mem[1] = 1 + (erro_c - Ec_st[i + 1]) / (Ec_st[i + 1] - Ec_st[i]);
                fpid->ec_grad_index[0] = i;
                fpid->ec_grad_index[1] = i + 1;
                break;
            }
        }
    }
    else
    {
        if (erro_c <= Ec_st[0])
        {
            fpid->ec_grad_mem[0] = 1;
            fpid->ec_grad_mem[1] = 0;
            fpid->ec_grad_index[0] = 0;
            fpid->ec_grad_index[1] = -1;
        }
        else if (erro_c >= Ec_st[6])
        {
            fpid->ec_grad_mem[0] = 1;
            fpid->ec_grad_mem[1] = 0;
            fpid->ec_grad_index[0] = 6;
            fpid->ec_grad_index[1] = -1;
        }
    }
}

/**
	* @brief  获取输出增量kp,ki,kd的总隶属度
	* @retval None
	*/
void Get_grad_sum(FuzyPID_TypeDef* fpid)
{
	  for (int i = 0; i <= NUM_AREA - 1; i++)
    {
        fpid->KpgradSums[i] = 0;
        fpid->KigradSums[i] = 0;
    		fpid->KdgradSums[i] = 0;
    }
		for (int i=0;i<2;i++)
		{
				if (fpid->e_grad_index[i] == -1)
				{
					continue;
				}
				for (int j = 0; j < 2; j++)
				{
						if (fpid->ec_grad_index[j] != -1)
						{
								int indexKp = Kp_rule_list[fpid->e_grad_index[i]][fpid->ec_grad_index[j]] + 3;
								int indexKi = Ki_rule_list[fpid->e_grad_index[i]][fpid->ec_grad_index[j]] + 3;
								int indexKd = Kd_rule_list[fpid->e_grad_index[i]][fpid->ec_grad_index[j]] + 3;
								//gradSums[index] = gradSums[index] + (e_gradmembership[i] * ec_gradmembership[j])* Kp_rule_list[e_grad_index[i]][ec_grad_index[j]];
								fpid->KpgradSums[indexKp] = fpid->KpgradSums[indexKp] + (fpid->e_grad_mem[i] * fpid->ec_grad_mem[j]);
								fpid->KigradSums[indexKi] = fpid->KigradSums[indexKi] + (fpid->e_grad_mem[i] * fpid->ec_grad_mem[j]);
								fpid->KpgradSums[indexKd] = fpid->KpgradSums[indexKd] + (fpid->e_grad_mem[i] * fpid->ec_grad_mem[j]);
						}
						else
						{
							continue;
						}

				}
		}
}

/**
	* @brief  计算输出增量kp,kd,ki对应论域值
	* @retval None
	*/
void Get_out(FuzyPID_TypeDef* fpid)
{
	for (int i = 0; i < NUM_AREA - 1; i++)
	{
			fpid->qdetail_kp += Kp_st[i] * fpid->KpgradSums[i];
			fpid->qdetail_ki += Ki_st[i] * fpid->KigradSums[i];
			fpid->qdetail_kd += Kd_st[i] * fpid->KdgradSums[i];
	}

}

/**
	* @brief  区间映射函数
	*/
float Quantization(float maximum,float minimum,float x)
{
    float qvalues= 6.0 *(x-minimum)/(maximum - minimum)-3;
    //float qvalues=6.0*()
    return qvalues;
   
    //qvalues[1] = 3.0 * ecerro / (maximum - minimum);
}

/**
	* @brief  反区间映射函数
	*/
float Inverse_quantization(float maximum, float minimum, float qvalues)
{
    float x = (maximum - minimum) *(qvalues + 3)/6 + minimum;
    return x;
}

/**
	* @brief  模糊PID控制实现函数
	* @retval None
	*/
void FuzzyPIDcontroller(FuzyPID_TypeDef* fpid, float e_max, float e_min, float ec_max, float ec_min, float kp_max, float kp_min, 
												 float erro, float erro_c,float ki_max,float ki_min,float kd_max,float kd_min,float erro_pre,float errp_ppre)
{
	fpid->errosum += erro;
	fpid->qerro = Quantization(e_max, e_min, erro);
  fpid->qerro_c = Quantization(ec_max, ec_min, erro_c);
	Get_grad_membership(fpid->qerro, fpid->qerro_c, fpid);
  Get_grad_sum(fpid);
  Get_out(fpid);
	fpid->detail_kp = Inverse_quantization(kp_max, kp_min, fpid->qdetail_kp);
  fpid->detail_ki = Inverse_quantization(ki_max, ki_min, fpid->qdetail_ki);
  fpid->detail_kd = Inverse_quantization(kd_max, kd_min, fpid->qdetail_kd);
	fpid->qdetail_kd = 0;
  fpid->qdetail_ki = 0;
  fpid->qdetail_kp = 0;
	fpid->kp = fpid->kp + fpid->detail_kp;
  fpid->ki = fpid->ki + fpid->detail_ki;
  fpid->kd = fpid->kd + fpid->detail_kd;
	if (fpid->kp < 0) fpid->kp = 0;
	if (fpid->ki < 0) fpid->ki = 0;
	if (fpid->kd < 0) fpid->kd = 0;
  fpid->detail_kp =   fpid->detail_ki = fpid->detail_kd = 0;  
	fpid->out = fpid->kp*(erro - erro_pre) + fpid->ki * erro + fpid->kd * (erro - 2 * erro_pre + errp_ppre);
}
