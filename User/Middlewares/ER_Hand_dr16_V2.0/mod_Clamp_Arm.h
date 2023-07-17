#ifndef _MOD_CLAMP_ARM_H_
#define _MOD_CLAMP_ARM_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "system.h"
#include "drv_dr16.h"

/*气缸口*/
/*龙门吊*/
#define cyl_A(a)						\
				if(a) DC7(1);				\
				else	DC7(0);				\
/*二段机械臂*/
#define cyl_B(a)						\
				if(a) DC5(1);				\
				else	DC5(0);				\
/*内撑*/
#define cyl_C(a)						\
				if(a) DC4(1);				\
				else	DC4(0);				\
/*旋转*/
#define cyl_D(a)						\
				if(a) DC6(1);				\
				else	DC6(0);				\

#define ARM_TIME		550		//机械臂拾取动作间隔时间
#define ROTATE_TIME	800

/*==========拾取模式==========*/
typedef enum
{
	CLAMP_ARM_ON = 0,
	CLAMP_ARM_READY,  //拾取准备
	CLAMP_ARM_START,	//拾取启动
	CLAMP_ARM_STOP,
	CLAMP_ARM_OFF,
} Clamp_Arm_mode_e;
/*==========机械臂状态==========*/
typedef enum
{
	A0 = 0,	//初始化状态
	A1,			//二段机械臂下降
	A2,			//内撑张开
	A3,			//二段机械臂上升，龙门吊上升
	A4,			//笔形气缸旋转
	A5,			//二段机械臂下降
	A6,			//内撑收缩
	A7,			//二段机械臂回收
	A8,			//笔形气缸归位
} Clamp_Arm_sta_e;
/*==========拾取控制==========*/
typedef struct
{
	const DR16_ctrl_t *clamp_arm_rc_ctrl;
	uint8_t clamp_arm_sta;			//机械臂拾取动作状态标志.
	Clamp_Arm_mode_e	clamp_arm_mode;
	Clamp_Arm_mode_e	clamp_arm_mode_last;
	
} Clamp_Arm_Control_t;

/*API*/
void Clamp_Arm_Init(Clamp_Arm_Control_t *clamp_arm_init);
void Clamp_Arm_Loop(Clamp_Arm_Control_t *clamp_arm_loop);
/*功能函数*/
void Clamp_Arm_rest(Clamp_Arm_Control_t *clamp_arm);
void Clamp_Arm_action(uint8_t mode, Clamp_Arm_Control_t *clamp_arm);
/*Handle*/
extern Clamp_Arm_Control_t clamp_arm_control;

#ifdef __cplusplus
}
#endif
#endif

