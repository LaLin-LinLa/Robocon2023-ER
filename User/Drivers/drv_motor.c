#include "drv_motor.h"
#include "bsp_can.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

static motor_measure_t  motor_driver[4];
static Shoot_Motor_t	 motor_shoot = {
	.speed1_rpm	=	1100,
	.speed2_rpm = 1100,
};

/**
  * @brief          驱动电机信息读取
  * @param		      ID: 标准帧ID
	* @param		      Data: 电机信息
  * @retval         none
  */
void Driver_motor_DatePrcess(uint32_t ID, uint8_t *Data)
{
	switch(ID)
	{
		case C620_1:
			get_RMmotor_measure(&motor_driver[0],Data);
		break;
		case C620_2:
			get_RMmotor_measure(&motor_driver[1],Data);
		break;
		case C620_3:
			get_RMmotor_measure(&motor_driver[2],Data);
		break;
		case C620_4:
			get_RMmotor_measure(&motor_driver[3],Data);
		break;
		default:
			break;
	}
}

/**
  * @brief         	发射电机信息读取
  * @param		      ID: 标准帧ID
	* @param		      Data: 电机信息
  * @retval         none
  */
void Shoot_motor_DatePrcess(uint32_t ID, uint8_t *Data)
{
	if(ID == Shoot_ID)
	{
		motor_shoot.speed2_rpm = Data[0]<<8 | Data[1];
		motor_shoot.speed1_rpm = Data[2]<<8 | Data[3];
	}
}

/*==========发送驱动电机控制命令函数==========*/
void CAN_CMD_DRIVER(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
		static uint8_t Data[8] = {0};
		Data[0] = iq1 >> 8;
		Data[1] = iq1;
		Data[2] = iq2 >> 8;
		Data[3] = iq2;
		Data[4] = iq3 >> 8;
		Data[5] = iq3;
		Data[6] = iq4 >> 8;
		Data[7] = iq4;
	
		CANx_SendData(&hcan1, CAN_C620_ALL_ID, Data, 0x08);
}
//返回驱动电机变量地址，通过指针方式获取原始数据
const motor_measure_t *get_Driver_Measure_Point(uint8_t i)
{
    return &motor_driver[(i & 0x03)];
}
//返回发射电机变量地址，通过指针方式获取原始数据
const Shoot_Motor_t *get_Shoot_Measure_Point(void)
{
    return &motor_shoot;
}
