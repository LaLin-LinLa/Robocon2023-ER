#ifndef __FUZZYPID_H
#define __FUZZYPID_H
#include <stdint.h>
#include <stddef.h> 

#define NB  -3
#define NM  -2
#define NS  -1
#define ZO  0
#define PS  1
#define PM  2
#define PB  3

typedef struct
{
	float  e_grad_mem[2];      //输入e的隶属度
   float  ec_grad_mem[2];     //输入de/dt的隶属度
	char   e_grad_index[2];    //输入e隶属度在规则表的索引
   char   ec_grad_index[2];   //输入de/dt隶属度在规则表的索引
   float  KpgradSums[7];      //输出增量kp总的隶属度
   float  KigradSums[7];      //输出增量ki总的隶属度
   float  KdgradSums[7];      //输出增量kd总的隶属度
   float  qdetail_kp;         //增量kp对应论域中的值
   float  qdetail_ki;         //增量ki对应论域中的值
   float  qdetail_kd;         //增量kd对应论域中的值
   float  qerro;              //输入e对应论域中的值
   float  qerro_c;            //输入de/dt对应论域中的值
   float errosum;             //误差积分  

   float detail_kp;                //输出增量kp
   float detail_ki;                //输出增量ki
   float detail_kd;                //输出增量kd

   float kp;                       //PID参数kp
   float ki;                       //PID参数ki
   float kd;                       //PID参数kd
   float out;                      //PID输出
}FuzyPID_TypeDef;


void FuzyPidTable(float ERR, float ERRC, FuzyPID_TypeDef *pid);
void PIDPos_Set(float pid_p,float pid_i,float pid_d,FuzyPID_TypeDef *fpid);
void PIDAngle_Set(float pid_p,float pid_i,float pid_d,FuzyPID_TypeDef *fpid);
void PIDActvel_Set(float pid_p,float pid_i,float pid_d,FuzyPID_TypeDef *fpid);
void FuzzyPIDcontroller(FuzyPID_TypeDef* fpid, float e_max, float e_min, float ec_max, float ec_min, float kp_max, float kp_min, 
												 float erro, float erro_c,float ki_max,float ki_min,float kd_max,float kd_min,float erro_pre,float errp_ppre);

#endif
    

