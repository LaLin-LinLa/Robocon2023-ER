#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;  

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  
typedef __I int16_t vsc16; 
typedef __I int8_t vsc8;   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 
typedef __I uint8_t vuc8;

/*BEEP*/
#define BEEP(a)	if (a)	\
				HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)
/*LED*/
#define LED1(a)	if (a)	\
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED2(a)	if (a)	\
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED3(a)	if (a)	\
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)
#define LED4(a)	if (a)	\
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET)
#define LED5(a)	if (a)	\
				HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET)
#define LED7(a)	if (a)	\
				HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_RESET)

/*ЦјИз*/
#define DC1(a)	if (a)	\
				HAL_GPIO_WritePin(DC1_GPIO_Port, DC1_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(DC1_GPIO_Port, DC1_Pin, GPIO_PIN_RESET)
#define DC2(a)	if (a)	\
				HAL_GPIO_WritePin(DC2_GPIO_Port, DC2_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(DC2_GPIO_Port, DC2_Pin, GPIO_PIN_RESET)
#define DC3(a)	if (a)	\
				HAL_GPIO_WritePin(DC3_GPIO_Port, DC3_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(DC3_GPIO_Port, DC3_Pin, GPIO_PIN_RESET)
#define DC4(a)	if (a)	\
				HAL_GPIO_WritePin(DC4_GPIO_Port, DC4_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(DC4_GPIO_Port, DC4_Pin, GPIO_PIN_RESET)
#define DC5(a)	if (a)	\
				HAL_GPIO_WritePin(DC5_GPIO_Port, DC5_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(DC5_GPIO_Port, DC5_Pin, GPIO_PIN_RESET)
#define DC6(a)	if (a)	\
				HAL_GPIO_WritePin(DC6_GPIO_Port, DC6_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(DC6_GPIO_Port, DC6_Pin, GPIO_PIN_RESET)
#define DC7(a)	if (a)	\
				HAL_GPIO_WritePin(DC7_GPIO_Port, DC7_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(DC7_GPIO_Port, DC7_Pin, GPIO_PIN_RESET)
#define DC8(a)	if (a)	\
				HAL_GPIO_WritePin(DC8_GPIO_Port, DC8_Pin, GPIO_PIN_SET);\
					else		\
				HAL_GPIO_WritePin(DC8_GPIO_Port, DC8_Pin, GPIO_PIN_RESET)

extern void delay_ms_nos(uint32_t cnt);
extern void delay_us_nos(uint32_t cnt);					

#ifdef __cplusplus
}
#endif
#endif

