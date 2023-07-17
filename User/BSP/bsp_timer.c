#include "bsp_timer.h"

#define TIMx TIM7

volatile uint32_t SystemTimerCnt;
static uint8_t delay_mode = 0;

static void Error_Handler(void);

/**
	* @brief  初始化定时器
	* @param  mode : 选择使用模块定时还是HAL定时
	* @retval None
	*/
void bsp_Timer_Init(uint8_t mode)
{
	delay_mode = mode;
	LL_TIM_EnableIT_UPDATE(TIMx);
	LL_TIM_ClearFlag_UPDATE(TIMx);
	LL_TIM_EnableCounter(TIMx);
}


/**
	* @brief  获取定时器时间
	* @param  None
	* @retval 实时时间
	*/
uint32_t Get_SystemTimer(void)
{
	return TIMx->CNT + SystemTimerCnt * 0xffff;
}

/**
	* @brief  更新定时器时间
	* @not    将此函数加入定时器中断中
	* @param  None
	* @retval None
	*/
void Update_SystemTick(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIMx) == SET)
	{
		LL_TIM_ClearFlag_UPDATE(TIMx);
		SystemTimerCnt++;
	}
}

/**
* @brief  微秒级延时
* @param  cnt : 延时时数
* @retval None
*/
void delay_us_nos(uint32_t cnt)
{
	uint32_t temp = cnt  + microsecond();

	while(temp >= microsecond());
}

/**
* @brief  毫秒级延时
* @param  cnt : 延时时数
* @retval None
*/
void delay_ms_nos(uint32_t cnt)
{
	if(delay_mode)
	{
		uint32_t temp = cnt * 1000 + microsecond();
		while(temp >= microsecond());
	}
	else
		HAL_Delay(cnt);
}


