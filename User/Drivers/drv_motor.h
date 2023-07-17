#ifndef _DRV_MOTOR_H_
#define _DRV_MOTOR_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "system.h"

/*==========rm电机数据读取==========*/
#define get_RMmotor_measure(ptr, Data)                                        \
{                                                                             \
		if((ptr)->ecd-(ptr)->last_ecd>4096) 																			\
		{(ptr)->round_cnt-- ;}                               											\
		else if((ptr)->ecd-(ptr)->last_ecd<-4096 ) 																\
		{(ptr)->round_cnt ++ ;}											 															\
		(ptr)->last_ecd = (ptr)->ecd;                                             \
		(ptr)->ecd = (uint16_t)(Data[0] << 8 | Data[1]);           								\
		(ptr)->speed_rpm = (uint16_t)(Data[2] << 8 | Data[3]);     								\
		(ptr)->given_current = (uint16_t)(Data[4] << 8 | Data[5]); 								\
		(ptr)->temperate = Data[6];                                              	\
		(ptr)->total_ecd=(ptr)->round_cnt*8191+(ptr)->ecd;                        \
}	


/*==========电机标准帧ID==========*/
typedef enum
{
	Null_id											= 0x000,
	
	CAN_C610_ALL_ID        			= 0x200,
	C610_1               				= 0x201,
	C610_2							 				= 0x202,
	C610_3               				= 0x203,
	C610_4							 				= 0x204,
	
	CAN_C620_ALL_ID        			= 0x200,
	C620_1               				= 0x201,
	C620_2							 				= 0x202,
	C620_3               				= 0x203,
	C620_4							 				= 0x204,
	
	Shoot_ID										= 0x220,
	
} can_msg_id_e;

/*==========RM电机信息==========*/
typedef struct
{
	vu16 	ecd;
	vs16 	last_ecd;
	vs16 	total_ecd;
	vs16 	speed_rpm;
	vs16 	given_current;
	vu8 	temperate;
	vs32	round_cnt;
} motor_measure_t;

/*==========驱动电机内部信息==========*/
typedef struct
{
	const motor_measure_t *motor_measure;
	int16_t set_current;
	int32_t real_speed;
	int32_t set_motor_speed;
} Driver_Motor_t;
/*==========发射电机内部信息==========*/
typedef struct
{
	int32_t	speed1_rpm;
	int32_t	speed2_rpm;
} Shoot_Motor_t;


void Driver_motor_DatePrcess(uint32_t ID, uint8_t *Data);
void Shoot_motor_DatePrcess(uint32_t ID, uint8_t *Data);
void CAN_CMD_DRIVER(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
extern const motor_measure_t *get_Driver_Measure_Point(uint8_t i);
extern const Shoot_Motor_t *get_Shoot_Measure_Point(void);

#ifdef __cplusplus
}
#endif
#endif

