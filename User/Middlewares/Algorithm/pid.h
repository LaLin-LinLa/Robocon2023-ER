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
    //PID ������
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //������
    float max_iout; //���������

    float set;
    float fdb;

    float out;
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[3];  //΢���� 0���� 1��һ�� 2���ϴ�
    float error[3]; //����� 0���� 1��һ�� 2���ϴ�

} PidTypeDef;

float PID_Calc(PidTypeDef *pid,float ref,float set);
void PID_Clear(PidTypeDef *PID);
void Init_PID(PidTypeDef *pid, uint8_t mode, const float PID[5]);
	
#endif

