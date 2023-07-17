/**
  ******************************************************************************
  * @file    system_communication.c/h
  * @brief   系统驱动层文件
  * @version 1.0
  * @par Change Log：
  * 		Date				Version				Author					Description
	*			22.6.1			V1.0					Gsx							Creator
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/	
#include "system_driver.h"
#include "system_datapool.h"
#include "system_os.h"

#include "bsp_usart.h"
#include "bsp_timer.h"
#include "bsp_can.h"

#include "drv_miniPC.h"
#include "drv_dr16.h"
#include "drv_bluetooth.h"
#include "drv_ps2.h"
#include "drv_tank_rc.h"

/*遥控解包任务*/
void RC_Task(void *p_arg);
TaskHandle_t RC_Task_Handler;
/*ps2遥控数据解包任务*/
TaskHandle_t PS2_Task_Handler;
void PS2_Task(void *p_arg);
#define PS2_TASK_TIM	1

static void dr16_call_back(uint8_t *buf, uint16_t len);
//static void tankrc_call_back(uint8_t *buf, uint16_t len);

/**
	* @brief  系统硬件层初始化
	* @retval None
	*/
void System_Driver_Init(void)
{
  /*BSP初始化*/
	bsp_Timer_Init(1);
	bsp_uart4_Init(Uart4_Rx_Buff[0], Uart4_Rx_Buff[1], UART4_RX_BUFFER_SIZE, dr16_call_back);
	//bsp_usart6_Init(Usart6_Rx_Buff[0], Usart6_Rx_Buff[1], USART6_RX_BUFFER_SIZE, tankrc_call_back);
	LL_TIM_EnableAllOutputs(TIM5);
	LL_TIM_EnableCounter(TIM5);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH2);
	LL_TIM_OC_SetCompareCH1(TIM5, 1000);
	LL_TIM_OC_SetCompareCH2(TIM5, 1000);
	HAL_IWDG_Init(&hiwdg);

}
/**
	* @brief  系统硬件层循环
	* @retval None
	*/
void System_Driver_Loop(void)
{
  /*串口遥控任务*/
	xTaskCreate(RC_Task, "rc_task", Small_Stack_Size, NULL, PrioritySuperHigh, &RC_Task_Handler); 
	/*ps2遥控任务*/
	//xTaskCreate(PS2_Task, "ps2_task", Small_Stack_Size, NULL, PriorityHigh, &PS2_Task_Handler); 
	
}
/*=========================================================中断回调函数=========================================================*/
/**
	* @brief  dr16串口中断回调函数
	* @retval None
	*/
static void dr16_call_back(uint8_t *buf, uint16_t len)
{
	static BaseType_t* DR16_TaskWoken;
	static USART_COB	DR16_Port = {0};
	if(len == RC_FRAME_LENGTH)
	{
			DR16_Port.port_num = 4;
			DR16_Port.address = buf;
			xTaskNotifyFromISR(RC_Task_Handler, (uint32_t) &DR16_Port, eSetValueWithOverwrite, DR16_TaskWoken);
	}
}
/**
	* @brief  tank_rc串口中断回调函数
	* @retval None
	*/
//static void tankrc_call_back(uint8_t *buf, uint16_t len)
//{
//	static BaseType_t* TankRC_TaskWoken;
//	static USART_COB	TankRC_Port = {0};
//	if(len <= TANK_RC_LENGTH && len == buf[3])
//	{
//		if((buf[0]<<8|buf[1]) == 0xAAAA)
//		{
//			if((buf[len-1]<<8|buf[len-2]) == CRC_Table(buf, len-2))
//			{
//				TankRC_Port.port_num = 6;
//				TankRC_Port.address = buf;
//				xTaskNotifyFromISR(RC_Task_Handler, (uint32_t) &TankRC_Port, eSetValueWithOverwrite, TankRC_TaskWoken);
//			}
//		}
//	}
//}


/*=========================================================任务函数=========================================================*/
/** 
	* @brief 遥控解包任务 
 **/
void RC_Task(void *p_arg)
{
	static USART_COB* RCPort;
	while(1)
	{
		if(xTaskNotifyWait(0x00000000, 0xFFFFFFFF, (uint32_t *) &RCPort, portMAX_DELAY) == pdTRUE)
		{
			if(RCPort->port_num == 4)
			{
				HAL_IWDG_Refresh(&hiwdg);							//喂狗
				dr16_DatePrcess(RCPort->address);
			}
			if(RCPort->port_num == 6)
			{
				tank_rc_DatePrcess(RCPort->address);
			}
		}
	}
}

/** 
	* @brief PS2遥控解包任务 
 **/
void PS2_Task(void *p_arg)
{
	TickType_t xLastWakeTime_t = xTaskGetTickCount();
	while(1)
	{
		PS2_DateDatePrcess();
		vTaskDelayUntil(&xLastWakeTime_t,PS2_TASK_TIM);
	}
		
}
