#ifndef _mod_GIMBAL_H_
#define _mod_GIMBAL_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "system.h"
#include "drv_dr16.h"
#include "drv_motor.h"
#include "user_lib.h"
#include "pid.h"

#define TRIGGER_DC	DC8		//��������
	
#define SHOOT_SPEED1_INIT	1000
#define SHOOT_SPEED2_INIT	1000
#define SHOOT_SPEED1_MAX	1940	
#define SHOOT_SPEED1_MIN	1100
#define SHOOT_SPEED2_MAX	1940
#define SHOOT_SPEED2_MIN	1100
	
//��������
//#define	SPEED_1	1370
//#define	SPEED_2	1490
//#define	SPEED_3	1510
//��
#define	SPEED_1	1365
#define	SPEED_2	1485
#define	SPEED_3	1510
#define SPEED_4	1520
#define SPEED_5	1658
#define SPEED_6	1670

//����ع���
//#define	SPEED_1	1330
//#define	SPEED_2	1440
//#define	SPEED_3	1455
//�����
//#define	SPEED_1	1375
//#define	SPEED_2	1490
//#define	SPEED_3	1555
/*==========��̨ģʽ==========*/
typedef enum
{
	GIMBAL_OFF = 0,
	GIMBAL_LOCK,
	GIMBAL_SET,
	SHOOT_1,
	SHOOT_2,
	SHOOT_3,
	SHOOT_4,
	SHOOT_5,
	SHOOT_6,
	SHOOT_Ready,
	SHOOT_Add,
	SHOOT_Minus,
} Gimbal_mode_e;

/*==========��̨����==========*/
typedef struct
{
	const DR16_ctrl_t *gimbal_rc_ctrl;
	struct
	{
		bool					Trigger_sta;		//������־λ
		bool 					Shoot_sta;			//�����־λ
		bool					Key_lock;				
		
		uint32_t			shoot_pwm1_val;	//����1pwmֵ
		uint32_t			set_pwm1_val;		//����1pwm�趨ֵ
		uint32_t			shoot_pwm2_val;	//����2pwmֵ
		uint32_t			set_pwm2_val;		//����2pwm�趨ֵ
		
		uint32_t 			SHOOT_SPEED_1;
		uint32_t 			SHOOT_SPEED_2;
		uint32_t 			SHOOT_SPEED_3;
		uint32_t 			SHOOT_SPEED_4;
		uint32_t 			SHOOT_SPEED_5;
		uint32_t 			SHOOT_SPEED_6;
	}shoot_control;
	
	Gimbal_mode_e			gimbal_mode;
	Gimbal_mode_e			gimbal_mode_last;
	
	const Shoot_Motor_t	*shoot_motor;			//������
} Gimbal_Control_t;

/*API*/
void Gimbal_Init(Gimbal_Control_t *gimbal_init);				//��̨��ʼ��
void Gimbal_Loop(Gimbal_Control_t *gimbal_loop);				//��̨���ƺ���
/*Handle*/
extern Gimbal_Control_t gimbal_control;

#ifdef __cplusplus
}
#endif
#endif

