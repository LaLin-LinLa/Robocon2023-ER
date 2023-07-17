#ifndef __MOD_CHASSIS_H
#define __MOD_CHASSIS_H
#ifdef __cplusplus
extern "C" {
#endif
#include "ramp_control.h"
#include "math.h"
#include "drv_motor.h"
#include "user_lib.h"
#include "pid.h"
#include "drv_dr16.h"
#include "drv_tank_rc.h"

#ifndef pi
#define pi	3.1415926
#endif

#define X_set				0.0f
#define Y_set				80.0f

/*==========底盘模式==========*/
typedef enum
{
	CHASSIS_OFF = 0,
	CHASSIS_NORMAL,	
	CHASSIS_SLOW,
} Chassis_mode_e;
/*==========底盘控制数据结构体==========*/
typedef struct
{
	const DR16_ctrl_t *chassis_rc_ctrl;
	Chassis_mode_e		chassis_mode;
	Chassis_mode_e		chassis_mode_last;
	
	Driver_Motor_t		drive_motor[4];			//驱动轮电机信息
	PidTypeDef 				drive_speed_pid[4];	//驱动轮速度pid
	PidTypeDef 				chassis_angle_pid;		//底盘角度
	int32_t vx_set, vy_set, vz_set;
	int32_t vx_last,vy_last,vz_last;
	float	yaw;
	float X_pos;	float set_Xpos;
	float	Y_pos;	float	set_Ypos;
} Chassis_Control_t;

/*API*/
void Chassis_Init(Chassis_Control_t *chassis_init);
void Chassis_Loop(Chassis_Control_t *chassis_loop);
/*Handler*/
extern Chassis_Control_t chassis_control;

#ifdef __cplusplus
}
#endif
#endif
