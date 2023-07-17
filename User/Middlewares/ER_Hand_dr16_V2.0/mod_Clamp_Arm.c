/**
  ******************************************************************************
  * @file    Clamp_Arm.c/h
  * @brief   夹爪拾取模块
  * @version 2.0
  * @par Change Log：
  * 		Date				Version				Author					Description
	*			23.3.28			V1.0					Gsx							Creator
	*			23.6.2			V2.0					Gsx							修改相关API以适配二代框架	
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "mod_Clamp_Arm.h"
#include "system_os.h"

Clamp_Arm_Control_t clamp_arm_control = {0};

static void Clamp_Arm_Mode_Set(Clamp_Arm_Control_t *clamp_arm);
static void Clamp_Arm_Mode_Update(Clamp_Arm_Control_t *clamp_arm);	
static void Clamp_Arm_Control_Input(Clamp_Arm_Control_t *clamp_arm);

/*========================================================API函数========================================================*/
/** 
	* @brief 夹爪拾取结构初始化函数
 **/
void Clamp_Arm_Init(Clamp_Arm_Control_t *clamp_arm_init)
{
	clamp_arm_init->clamp_arm_rc_ctrl = get_dr16_control_point();		//dr16遥控器初始化
	Clamp_Arm_action(A0,clamp_arm_init);								//拾取气缸复位
}
/** 
	* @brief 夹爪拾取控制函数
 **/
void Clamp_Arm_Loop(Clamp_Arm_Control_t *clamp_arm_loop)
{
	Clamp_Arm_Mode_Set(clamp_arm_loop);
	Clamp_Arm_Mode_Update(clamp_arm_loop);
	Clamp_Arm_Control_Input(clamp_arm_loop);
}
/*========================================================功能函数========================================================*/
int arm_limit(int value, int min, int max)
{
	if(value<min)
		value = min;
	if(value>max)
		value = max;
	return value;
}
/** 
	* @brief 	机械臂拾取动作
	* @param	mode	动作步骤
 **/
void Clamp_Arm_action(uint8_t mode, Clamp_Arm_Control_t *clamp_arm)
{
	switch(mode)
  {
		/*夹爪拾取结构初始化*/
  	case A0:
		{
			cyl_A(0);
			cyl_B(0);
			cyl_C(0);
			cyl_D(0);
		}
  	break;
		/*动作1：二段机械臂下降*/
		case A1:
		{
			cyl_B(1);
		}
  	break;
		/*动作2：内撑张开*/
		case A2:
		{
			cyl_C(1);
		}
  	break;
		/*动作3：二段机械臂上升，龙门吊上升*/
		case A3:
		{
			cyl_A(1);
			cyl_B(0);
		}
  	break;
		/*动作4：笔形气缸旋转*/
		case A4:
		{
			cyl_D(1);
		}
  	break;
		/*动作5：二段机械臂下降*/
		case A5:
		{
			cyl_B(1);
		}
  	break;
		/*动作6：内撑收缩*/
		case A6:
		{
			cyl_C(0);
		}
  	break;
		/*动作7：二段机械臂回收*/
		case A7:
		{
			cyl_B(0);
		}
  	break;
		/*动作8: 笔形气缸归位*/
		case A8:
		{
			cyl_D(0);
		}
  	break;
  	default:
  		break;
  }
	clamp_arm->clamp_arm_sta = mode;
}
/** 
	* @brief 	机械臂动作复位
 **/
void Clamp_Arm_rest(Clamp_Arm_Control_t *clamp_arm)
{
		switch (clamp_arm->clamp_arm_sta)
		{
			case A0:
			break;
			case A1:
			{
				cyl_B(0);
			}
			break;
			case A2:
			{
				cyl_C(0);
				vTaskDelay(ARM_TIME);
				cyl_B(0);
			}
			break;
			case A3:
			{
				cyl_A(0);
				vTaskDelay(ARM_TIME);
				cyl_C(0);
			}
			break;
			case A4:
			{
				cyl_D(0);
				vTaskDelay(ROTATE_TIME);
				cyl_A(0);
				vTaskDelay(ARM_TIME);
				cyl_C(0);
			}
			break;
			case A5:
			{
				cyl_B(0);
				vTaskDelay(ARM_TIME);
				cyl_D(0);
				vTaskDelay(ROTATE_TIME);
				cyl_A(0);
				vTaskDelay(ARM_TIME);
				cyl_C(0);
			}
			break;
			case A6:
			{
				cyl_B(0);
				vTaskDelay(ARM_TIME);
				cyl_D(0);
				vTaskDelay(ROTATE_TIME);
				cyl_A(0);
			}
			break;
			case A7:
			{
				cyl_D(0);
				vTaskDelay(ROTATE_TIME);
				cyl_A(0);
			}
			case A8:
			{
				cyl_A(0);
			}
			default:
				break;
		}
		Clamp_Arm_action(A0,clamp_arm);
}
/*========================================================任务函数========================================================*/
/** 
	* @brief 夹爪拾取模式设置
 **/
static void Clamp_Arm_Mode_Set(Clamp_Arm_Control_t *clamp_arm)
{
	if(switch_is_down(clamp_arm->clamp_arm_rc_ctrl->rc.s[ModeChannel_L]))					//左下
	{
		if(switch_is_mid(clamp_arm->clamp_arm_rc_ctrl->rc.s[ModeChannel_R]))			//右中	拾取准备
		{
			clamp_arm->clamp_arm_mode = CLAMP_ARM_READY;
		}
		else if(switch_is_up(clamp_arm->clamp_arm_rc_ctrl->rc.s[ModeChannel_R]))	//右上	拾取启动
		{
			clamp_arm->clamp_arm_mode = CLAMP_ARM_START;
		}
		
		if(clamp_arm->clamp_arm_rc_ctrl->rc.ch[4] < -100)				//拨轮上拨，拾取复位
		{
			clamp_arm->clamp_arm_mode = CLAMP_ARM_OFF;
		}
		
	}
	else 
		clamp_arm->clamp_arm_mode = CLAMP_ARM_STOP;
}

/** 
	* @brief 夹爪拾取模式更新
 **/
static void Clamp_Arm_Mode_Update(Clamp_Arm_Control_t *clamp_arm)
{
	/*拾取模式改变*/
	if(clamp_arm->clamp_arm_mode_last != clamp_arm->clamp_arm_mode)
	{
		/*CLAMP_ARM_READY(拾取准备)	-> CLAMP_ARM_START(拾取启动)*/
		if(clamp_arm->clamp_arm_mode_last == CLAMP_ARM_READY && clamp_arm->clamp_arm_mode == CLAMP_ARM_START)
		{
				switch (clamp_arm->clamp_arm_sta)
				{
					case A0:
							Clamp_Arm_action(A1,clamp_arm);
					break;
					case A2:
						Clamp_Arm_action(A3,clamp_arm);
					break;
					case A4:
						Clamp_Arm_action(A5,clamp_arm);
					break;
					case A6:
						Clamp_Arm_action(A7,clamp_arm);
					break;
					case A8:
						Clamp_Arm_action(A0,clamp_arm);
					break;
					default:
						Clamp_Arm_action(A0,clamp_arm);
					break;
				}
		}
		/*CLAMP_ARM_START(拾取启动)	-> CLAMP_ARM_READY(拾取准备)*/
		else if(clamp_arm->clamp_arm_mode_last == CLAMP_ARM_START && clamp_arm->clamp_arm_mode == CLAMP_ARM_READY)
		{
				switch (clamp_arm->clamp_arm_sta)
				{
					case A1:
						Clamp_Arm_action(A2,clamp_arm);
					break;
					case A3:
						Clamp_Arm_action(A4,clamp_arm);			
					break;
					case A5:
						Clamp_Arm_action(A6,clamp_arm);
					break;
					case A7:
						Clamp_Arm_action(A8,clamp_arm);
					break;
					default:
						Clamp_Arm_action(A0,clamp_arm);
					break;
				}
		}
		clamp_arm->clamp_arm_mode_last = clamp_arm->clamp_arm_mode;	//更新拾取模式
	}
}

/** 
	* @brief 拾取控制输入
 **/
static void Clamp_Arm_Control_Input(Clamp_Arm_Control_t *clamp_arm)
{
	if(clamp_arm->clamp_arm_mode == CLAMP_ARM_OFF)
	{
		Clamp_Arm_rest(clamp_arm);
	}
	
}

