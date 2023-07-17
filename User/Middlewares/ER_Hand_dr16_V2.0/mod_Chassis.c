/**
  ******************************************************************************
  * @file    mod_Chassis.c/h
  * @brief   ����ģ��
  * @version 1.0
  * @par Change Log��
  * 		Date				Version				Author					Description
	*			23.3.28			V1.0					Gsx							Creator	
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "mod_Chassis.h"

Chassis_Control_t chassis_control  = {
	.set_Xpos = X_set,
	.set_Ypos = Y_set,
};

static float PID_DRIVER_SPEED[4][5]={20.0f, 0.0f, 0.1f, 16000.0f, 800.0f,			//0
																		 20.0f, 0.0f, 0.1f, 16000.0f, 800.0f,			//1
																		 20.0f, 0.0f, 0.1f, 16000.0f, 800.0f,			//2
																		 20.0f, 0.0f, 0.1f, 16000.0f, 800.0f};		//3

static void Chassis_Mode_Set(Chassis_Control_t *chassis);
static void Chassis_Mode_Update(Chassis_Control_t *chassis);
static void Chassis_Control_Input(Chassis_Control_t *chassis_move_control);
static void Chassis_Motor_Control( Chassis_Control_t *motor);
																		 
/*========================================================API����========================================================*/
/** 
	* @brief ���̳�ʼ������
 **/
void Chassis_Init(Chassis_Control_t *chassis_init)
{
	char i=0;
	/*dr16ң������ʼ��*/
	chassis_init->chassis_rc_ctrl = get_dr16_control_point();	
	for(i=0;i<4;i++)
	{
		Init_PID(&chassis_init->drive_speed_pid[i],PID_MODE_POS, PID_DRIVER_SPEED[i]); //pid��ʼ��
		chassis_init->drive_motor[i].motor_measure = get_Driver_Measure_Point(i);	//�����ʼ��
	}
	
}
/** 
	* @brief ���̿��ƺ���
 **/
void Chassis_Loop(Chassis_Control_t *chassis_loop)
{
	Chassis_Mode_Set(chassis_loop);
	Chassis_Mode_Update(chassis_loop);
	Chassis_Control_Input(chassis_loop);
	Chassis_Motor_Control(chassis_loop);
}
/*========================================================���ܺ���========================================================*/
 // 3��������2
 //	|		 |
 //	1��������0
#define R 10			//�����	
#define SPEED_MAX	8000		//������ٶ�
#define PALSTANCE_MAX	200	//�����ٶ�
/** 
	* @brief ȫ�����ٶȷֽ�(����)
	* @param V_limit				���ٶ�����
	* @param W_limit				���ٶ�����
 **/
void Driver_speed4_Cale(Chassis_Control_t *chassis, int V_limit, int W_limit)
{
	
	//����������
	chassis->vx_set = int_map(int16_buffer(	chassis->chassis_rc_ctrl->rc.ch[2], 
																										RC_DEADLINE), -639, 640, -V_limit, V_limit);
	chassis->vy_set = int_map(int16_buffer(	chassis->chassis_rc_ctrl->rc.ch[3], 
																										RC_DEADLINE), -640, 640, -V_limit, V_limit);
	chassis->vz_set = (int_map(int16_buffer(	chassis->chassis_rc_ctrl->rc.ch[0], 
																										50), -610, 610, -W_limit, W_limit))*R;
	
	
	if(chassis->vx_set!=0)
		chassis->vx_last = RAMP_int32_t(chassis->vx_set, chassis->vx_last, 4);
	else
		chassis->vx_last = 0;
	if(chassis->vy_set!=0)
		chassis->vy_last = RAMP_int32_t(chassis->vy_set, chassis->vy_last, 4);
	else
		chassis->vy_last = 0;
	
//	motor->vx_last = RAMP_int32_t(chassis->vx_set, motor->vx_last, 4);
//	motor->vy_last = RAMP_int32_t(chassis->vy_set, motor->vy_last, 4);
	
	chassis->vz_last = chassis->vz_set;
	
	chassis->drive_motor[0].set_motor_speed = (int32_t)(0.707*(-chassis->vy_last-chassis->vx_last)+chassis->vz_last);
	chassis->drive_motor[1].set_motor_speed = (int32_t)(0.707*(chassis->vy_last-chassis->vx_last)+chassis->vz_last);
	chassis->drive_motor[2].set_motor_speed = (int32_t)(0.707*(-chassis->vy_last+chassis->vx_last)+chassis->vz_last);
	chassis->drive_motor[3].set_motor_speed = (int32_t)(0.707*(chassis->vy_last+chassis->vx_last)+chassis->vz_last);
	
}

/*========================================================������========================================================*/														 
/** 
	* @brief ����ģʽ����
 **/
void Chassis_Mode_Set(Chassis_Control_t *chassis)
{
	if(switch_is_down(chassis->chassis_rc_ctrl->rc.s[ModeChannel_L]))						//����	���Ƶ���
  {
		if(switch_is_down(chassis->chassis_rc_ctrl->rc.s[ModeChannel_R]))					//���£����̹ر�
		{
			chassis->chassis_mode = CHASSIS_OFF;
		}
		else
		{
			if(chassis->chassis_rc_ctrl->rc.ch[4] > 50)
			{
				chassis->chassis_mode = CHASSIS_SLOW;
			}
			else
			{
				chassis->chassis_mode = CHASSIS_NORMAL;
			}
		}
	}
	else
	{
		if(chassis->chassis_rc_ctrl->rc.ch[4] < -50)
		{
			chassis->chassis_mode = CHASSIS_SLOW;
		}
		else
		{
			chassis->chassis_mode = CHASSIS_NORMAL;
		}
	}
	
	
}
/** 
	* @brief ����ģʽ����
 **/
void Chassis_Mode_Update(Chassis_Control_t *chassis)
{
	/*����ģʽ�ı�*/
	if(chassis->chassis_mode_last != chassis->chassis_mode)
	{
		if(chassis->chassis_mode_last == CHASSIS_SLOW && chassis->chassis_mode == CHASSIS_NORMAL)
		{
			chassis->drive_motor[0].set_motor_speed = 0;
			chassis->drive_motor[1].set_motor_speed = 0;
			chassis->drive_motor[2].set_motor_speed = 0;
			chassis->drive_motor[3].set_motor_speed = 0;
		}
	}
}
/** 
	* @brief ���̿�������
 **/
void Chassis_Control_Input(Chassis_Control_t *chassis_move_control)
{
	switch(chassis_move_control->chassis_mode)
	{
		case CHASSIS_OFF:
		{
			chassis_move_control->drive_motor[0].set_motor_speed = 0;
			chassis_move_control->drive_motor[1].set_motor_speed = 0;
			chassis_move_control->drive_motor[2].set_motor_speed = 0;
			chassis_move_control->drive_motor[3].set_motor_speed = 0;
		}
		break;
		case CHASSIS_NORMAL:	
		{
			Driver_speed4_Cale(chassis_move_control, SPEED_MAX, PALSTANCE_MAX);
			
		}
		break;
		case CHASSIS_SLOW:
		{
			Driver_speed4_Cale(chassis_move_control, SPEED_MAX, PALSTANCE_MAX);
			chassis_move_control->drive_motor[0].set_motor_speed /= 5;
			chassis_move_control->drive_motor[1].set_motor_speed /= 5;
			chassis_move_control->drive_motor[2].set_motor_speed /= 5;
			chassis_move_control->drive_motor[3].set_motor_speed /= 5;
		}
		break;
		default:
			break;
	}
}
/*==========���̵������==========*/
void Chassis_Motor_Control( Chassis_Control_t *motor)
{
	char i;
	//pid����
	for(i=0;i<4;i++)
	{	
		//������
		PID_Calc(&motor->drive_speed_pid[i], motor->drive_motor[i].motor_measure->speed_rpm, motor->drive_motor[i].set_motor_speed);
		motor->drive_motor[i].set_current = motor->drive_speed_pid[i].out;
	}
}
