#ifndef _RAMP_CONTROL_H_
#define _RAMP_CONTROL_H_
#include "stm32f4xx.h"
typedef struct RampGen_t
{
	int32_t count;
	int32_t XSCALE;
	float out;
	void (*Init)(struct RampGen_t *ramp, int32_t XSCALE);
	float (*Calc)(struct RampGen_t *ramp);
	void (*SetCounter)(struct RampGen_t *ramp, int32_t count);
	void (*ResetCounter)(struct RampGen_t *ramp);
	void (*SetScale)(struct RampGen_t *ramp, int32_t scale);
	uint8_t (*IsOverflow)(struct RampGen_t *ramp);
}RampGen_t;
typedef struct RampGenLoop_t
{
	int32_t count;
	int32_t XSCALE;
	float out;
	int8_t flag;//正负号标志位
}RampGenLoop_t;

#define RAMP_GEN_DAFAULT \
{ \
							0, \
							100, \
							0, \
							&RampInit, \
							&RampCalc, \
							&RampSetCounter, \
							&RampResetCounter, \
							&RampSetScale, \
							&RampIsOverflow, \
						} 

void RampInit(RampGen_t *ramp, int32_t XSCALE);
float RampCalc(RampGen_t *ramp);
void RampSetCounter(struct RampGen_t *ramp, int32_t count);
void RampResetCounter(struct RampGen_t *ramp);
void RampSetScale(struct RampGen_t *ramp, int32_t scale);
uint8_t RampIsOverflow(struct RampGen_t *ramp);

float RampCalcLoop(RampGenLoop_t *ramp);
void RampSetScaleLoop(struct RampGenLoop_t *ramp, int32_t scale);
void RampResetCounterLoop(struct RampGenLoop_t *ramp);
void RampInitLoop(RampGenLoop_t *ramp, int32_t XSCALE);

float RAMP_float( float final, float now, float ramp );				
uint16_t RAMP_uint16( uint16_t final, uint16_t now, uint16_t ramp );
int32_t RAMP_int32_t( int32_t final, int32_t now, int32_t ramp );						
float RampInc_float( float *buffer, float now, float ramp );				
#endif

