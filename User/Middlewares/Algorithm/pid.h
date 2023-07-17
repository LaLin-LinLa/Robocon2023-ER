#ifndef _PID_H_
#define _PID_H_
#include <stdint.h>
#include <stddef.h> 

enum PID_MODE
{
    PID_MODE_POS = 0,
    PID_MODE_DEL
};

typedef struct
{
    uint8_t mode;
    //PID 三参数
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //最大输出
    float max_iout; //最大积分输出

    float set;
    float fdb;

    float out;
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[3];  //微分项 0最新 1上一次 2上上次
    float error[3]; //误差项 0最新 1上一次 2上上次

} PidTypeDef;

float PID_Calc(PidTypeDef *pid,float ref,float set);
void PID_Clear(PidTypeDef *PID);
void Init_PID(PidTypeDef *pid, uint8_t mode, const float PID[5]);
	
#endif

