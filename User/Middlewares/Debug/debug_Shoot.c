#include "debug_Shoot.h"

int 	D_shoot_set_speed = 1100;
char 	D_shoot_Trigger	=0;

void debug_Shoot_Init(void)
{
	
}

void debug_Shoot_Loop(void)
{
	LL_TIM_OC_SetCompareCH1(TIM5, D_shoot_set_speed);
	LL_TIM_OC_SetCompareCH2(TIM5, D_shoot_set_speed);
	if(D_shoot_Trigger)
	{
		DC8(0);
	}
	else
	{
		DC8(1);
	}
}
