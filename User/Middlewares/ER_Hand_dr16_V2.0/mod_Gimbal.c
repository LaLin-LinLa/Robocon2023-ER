/**
  ******************************************************************************
  * @file    mod_Gimbal.c/h
  * @brief   云台模块
  * @version 2.0
  * @par Change Log：
  * 		Date				Version				Author					Description
	*			23.3.28			V1.0					Gsx							Creator
	*			23.6.2			V2.0					Gsx							定档射速	
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "mod_Gimbal.h"
#include "system_datapool.h"

Gimbal_Control_t gimbal_control = {
	.shoot_control.shoot_pwm1_val = SHOOT_SPEED1_MIN,			//初始化油门1
	.shoot_control.set_pwm1_val =SHOOT_SPEED1_MIN,				//初始化油门1设定
	.shoot_control.shoot_pwm2_val = SHOOT_SPEED2_MIN,			//初始化油门2
	.shoot_control.set_pwm2_val = SHOOT_SPEED2_MIN,				//初始化油门2设定
	.shoot_control.SHOOT_SPEED_1	=	SPEED_1,
	.shoot_control.SHOOT_SPEED_2	=	SPEED_2,
	.shoot_control.SHOOT_SPEED_3	=	SPEED_3,
	.shoot_control.SHOOT_SPEED_4	=	SPEED_4,
	.shoot_control.SHOOT_SPEED_5	=	SPEED_5,
	.shoot_control.SHOOT_SPEED_6	=	SPEED_6,
};

static void Gimbal_Mode_Set(Gimbal_Control_t *gimbal);				//云台模式设置
static void Gimbal_Mode_Update(Gimbal_Control_t *gimbal);			//云台模式更新
static void Gimbal_Control_Input(Gimbal_Control_t *gimbal);		//云台控制输入
static void Gimbal_Motor_Control(Gimbal_Control_t *motor);		//云台电机控制


/*========================================================API函数========================================================*/
/** 
	* @brief 云台初始化函数
 **/
void Gimbal_Init(Gimbal_Control_t *gimbal_init)
{
	/*dr16遥控器初始化*/
	gimbal_init->gimbal_rc_ctrl = get_dr16_control_point();
	/*电机初始化*/
	gimbal_init->shoot_motor	=  get_Shoot_Measure_Point();
	
}
/** 
	* @brief 云台控制函数
 **/
void Gimbal_Loop(Gimbal_Control_t *gimbal_loop)
{
	Gimbal_Mode_Set(gimbal_loop);			
	Gimbal_Mode_Update(gimbal_loop);
	Gimbal_Control_Input(gimbal_loop);
	Gimbal_Motor_Control(gimbal_loop);
}
/*========================================================功能函数========================================================*/
/** 
	* @brief 发射稳压控制
 **/
int  Shoot_voltage_Control( int speed)
{
	double	out;
	if(sys_dat.voltage >= 3.360635838f)
	{
		out = speed * 0.971741f;
	}
	else if(sys_dat.voltage<3.360635838f && sys_dat.voltage>=3.265549133f)
	{
		out = speed * 0.978120f;
	}
	else if(sys_dat.voltage<3.265549133f && sys_dat.voltage>=3.197630058f)
	{
		out = speed * 0.983498f;
	}
	else if(sys_dat.voltage<3.197630058f && sys_dat.voltage>=3.075375723f)
	{
		out = speed * 0.992889f;
	}
	else if(sys_dat.voltage<3.075375723f && sys_dat.voltage>=3.034624277f)
	{
		out = speed * 1.002691f;
	}
	else if(sys_dat.voltage<3.034624277f)
	{
		out = speed * 1.007894f;
	}
	return out;
}
/** 
	* @brief 发射转速微调
 **/
static void Shoot_Set(Gimbal_Control_t* gimbal, uint8_t speed)
{
	if(gimbal->shoot_control.Key_lock==0 && gimbal->gimbal_rc_ctrl->rc.ch[1] > 400)
	{
		gimbal->shoot_control.set_pwm1_val += speed;
		gimbal->shoot_control.set_pwm2_val += speed;
		if(gimbal->shoot_control.set_pwm1_val > SHOOT_SPEED1_MAX) gimbal->shoot_control.set_pwm1_val = SHOOT_SPEED1_MAX;
		if(gimbal->shoot_control.set_pwm2_val > SHOOT_SPEED2_MAX) gimbal->shoot_control.set_pwm2_val = SHOOT_SPEED2_MAX;
		gimbal->shoot_control.Key_lock=1;
	}
	else if(gimbal->shoot_control.Key_lock==0 && gimbal->gimbal_rc_ctrl->rc.ch[1] < -400)
	{
		gimbal->shoot_control.set_pwm1_val -= speed;
		gimbal->shoot_control.set_pwm2_val -= speed;
		if(gimbal->shoot_control.set_pwm1_val < SHOOT_SPEED1_MIN) gimbal->shoot_control.set_pwm1_val = SHOOT_SPEED1_MIN;
		if(gimbal->shoot_control.set_pwm2_val < SHOOT_SPEED2_MIN) gimbal->shoot_control.set_pwm2_val = SHOOT_SPEED2_MIN;
		gimbal->shoot_control.Key_lock=1;
	}
	else if(gimbal->gimbal_rc_ctrl->rc.ch[1] == 0)
	gimbal->shoot_control.Key_lock=0;
}

/*========================================================任务函数========================================================*/
/** 
	* @brief 云台模式设置
	* @note	 右上				云台自动
	* 			 右中				云台锁死
	* 			 右下				云台设置
 **/
static void Gimbal_Mode_Set(Gimbal_Control_t *gimbal)
{
	if(switch_is_mid(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_L]))					//左中
	{
		if(switch_is_up(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_R]))					//右上	速度3
		{
			gimbal->gimbal_mode = SHOOT_3;
		}
		else if(switch_is_mid(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_R]))		//右中	速度2
		{
			gimbal->gimbal_mode = SHOOT_2;
		}
		else if(switch_is_down(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_R]))	//右下	速度1
		{
			gimbal->gimbal_mode = SHOOT_1;
		}
	}
	else if(switch_is_up(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_L]))			//左上
	{
		if(switch_is_up(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_R]))					//右上
		{
			gimbal->gimbal_mode = SHOOT_6;
		}
		else if(switch_is_mid(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_R]))		//右中	
		{
			gimbal->gimbal_mode = SHOOT_5;
		}
		else if(switch_is_down(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_R]))	//右下	
		{
			gimbal->gimbal_mode = SHOOT_4;	
		}
	}
	else if(switch_is_down(gimbal->gimbal_rc_ctrl->rc.s[ModeChannel_L]))		//左下
	{
		gimbal->gimbal_mode = GIMBAL_OFF;
	}
}

/** 
	* @brief 云台模式更新
 **/
static void Gimbal_Mode_Update(Gimbal_Control_t *gimbal)
{
	/*云台模式改变*/
	if(gimbal->gimbal_mode_last != gimbal->gimbal_mode)
	{
		if(gimbal->gimbal_mode == SHOOT_1)
		{
			gimbal->shoot_control.Shoot_sta = 1;
			gimbal->shoot_control.Trigger_sta = 1;
			gimbal->shoot_control.set_pwm1_val = gimbal->shoot_control.set_pwm2_val = gimbal->shoot_control.SHOOT_SPEED_1;
		}
		else if(gimbal->gimbal_mode == SHOOT_2)
		{
			gimbal->shoot_control.Shoot_sta = 1;
			gimbal->shoot_control.Trigger_sta = 1;
			gimbal->shoot_control.set_pwm1_val = gimbal->shoot_control.set_pwm2_val = gimbal->shoot_control.SHOOT_SPEED_2;
		}
		else if(gimbal->gimbal_mode == SHOOT_3)
		{
			gimbal->shoot_control.Shoot_sta = 1;
			gimbal->shoot_control.Trigger_sta = 1;
			gimbal->shoot_control.set_pwm1_val = gimbal->shoot_control.set_pwm2_val = gimbal->shoot_control.SHOOT_SPEED_3;
		}
		else if(gimbal->gimbal_mode == SHOOT_4)
		{
			gimbal->shoot_control.Shoot_sta = 1;
			gimbal->shoot_control.Trigger_sta = 1;
			gimbal->shoot_control.set_pwm1_val = gimbal->shoot_control.set_pwm2_val = gimbal->shoot_control.SHOOT_SPEED_4;
		}
		else if(gimbal->gimbal_mode == SHOOT_5)
		{
			gimbal->shoot_control.Shoot_sta = 1;
			gimbal->shoot_control.Trigger_sta = 1;
			gimbal->shoot_control.set_pwm1_val = gimbal->shoot_control.set_pwm2_val = gimbal->shoot_control.SHOOT_SPEED_5;
		}
		else if(gimbal->gimbal_mode == SHOOT_6)
		{
			gimbal->shoot_control.Shoot_sta = 1;
			gimbal->shoot_control.Trigger_sta = 1;
			gimbal->shoot_control.set_pwm1_val = gimbal->shoot_control.set_pwm2_val = gimbal->shoot_control.SHOOT_SPEED_6;
		}
		
	}
	gimbal->gimbal_mode_last = gimbal->gimbal_mode;
}

/** 
	* @brief 云台控制输入
 **/
static void Gimbal_Control_Input(Gimbal_Control_t *gimbal)
{
	switch(gimbal->gimbal_mode)
	{
		case GIMBAL_OFF:
		{
			gimbal->shoot_control.Key_lock=0;
			gimbal->shoot_control.Shoot_sta = 0;		//关闭发射
			gimbal->shoot_control.Trigger_sta = 0; 	//关闭拨弹
			gimbal->shoot_control.set_pwm1_val = gimbal->shoot_control.set_pwm2_val = 1100;
		}
		break;
		default:
		{
			Shoot_Set(gimbal, 5);
		}
		break;
	}
	
	//gimbal->shoot_control.shoot_pwm1_val = gimbal->shoot_control.set_pwm1_val;
	//gimbal->shoot_control.shoot_pwm2_val = gimbal->shoot_control.set_pwm2_val;
	gimbal->shoot_control.shoot_pwm1_val = Shoot_voltage_Control((double)gimbal->shoot_control.set_pwm1_val);
	gimbal->shoot_control.shoot_pwm2_val = Shoot_voltage_Control((double)gimbal->shoot_control.set_pwm2_val);
}

/** 
	* @brief 云台电机控制
 **/
static void Gimbal_Motor_Control(Gimbal_Control_t *motor)
{	
	
	/*发射电机*/
	if(motor->shoot_control.Shoot_sta)
	{
		LL_TIM_OC_SetCompareCH1(TIM5, motor->shoot_control.shoot_pwm1_val);
		LL_TIM_OC_SetCompareCH2(TIM5, motor->shoot_control.shoot_pwm2_val);
	}
	else
	{
		motor->shoot_control.shoot_pwm1_val = SHOOT_SPEED1_MIN;
		motor->shoot_control.shoot_pwm2_val = SHOOT_SPEED2_MIN;
		LL_TIM_OC_SetCompareCH1(TIM5, SHOOT_SPEED1_INIT);
		LL_TIM_OC_SetCompareCH2(TIM5, SHOOT_SPEED2_INIT);
	}
	/*气缸*/
	if(motor->shoot_control.Trigger_sta)
	{
		if(motor->gimbal_rc_ctrl->rc.ch[4] > 600)
		{
			TRIGGER_DC(0);
		}
		else
		{
			TRIGGER_DC(1);
		}
	}
	else
	{
		TRIGGER_DC(1);
	}
	
	
}

