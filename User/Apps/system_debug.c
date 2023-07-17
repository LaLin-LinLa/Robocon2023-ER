/**
  ******************************************************************************
  * @file    system_debug.c/h
  * @brief   系统调试
	*					 创建的单个任务专门用于调试
  * @version 1.0
  * @par Change Log：
  * 		Date				Version				Author					Description
	*			22.12.12		V1.0					Gsx							Creator
  ******************************************************************************
  * @attention
	*	本模块需要..\Middlewares\Debug内文件支持
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "system_debug.h"
#include "system_os.h"

#include "debug_Shoot.h"

/*任务句柄*/
TaskHandle_t TestTask_Handler;
/*任务函数*/
void Test_Task(void *p_arg);
/*任务时数*/
#define TEST_TASK_TIM	1

void System_Debug_Init(void)
{
	
}

/**
	*	@brief 测试任务初始化
 **/
void System_Debug_Loop(void)
{
	
	xTaskCreate((TaskFunction_t)Test_Task,             
						 (const char*    )"test_task",           
						 (uint16_t       )Small_Stack_Size,        
						 (void*          )NULL,                  
						 (UBaseType_t    )PriorityHigh,        
						 (TaskHandle_t*  )&TestTask_Handler);   
	
}

/**
	*	@brief 测试任务
 **/
void Test_Task(void *p_arg)
{
	TickType_t xLastWakeTime_t = xTaskGetTickCount();
	while(1)
	{
		debug_Shoot_Loop();
		vTaskDelayUntil(&xLastWakeTime_t,TEST_TASK_TIM);
	}
	
}

