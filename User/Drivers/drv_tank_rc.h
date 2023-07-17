#ifndef _TANK_RC_H_
#define _TANK_RC_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>
	
#define TANK_RC_LENGTH				32
#define CH_VALUE_OFFSET		((uint16_t)127)

#define RC_TO_RM	0x01		//手柄发接收机
#define Rm_TO_RC	0x09		//接收机发手柄
#define PC_TO_RM	0x15		//上位机发接收机
#define RM_To_PC	0x1D		//接收机发上位机

typedef enum 
{
	L_KEY = 0,
	R_KEY,
	R2,
	R1,
	RU,
	RR,
	RD,
	RL,
	ST,
	SE,
	LR,
	LD,
	LL,
	LU,
	L1,
	L2,
} KEY_NUM;


typedef __packed struct
{
	int16_t ch[4];
	char s[16];
	float voltage;
} TankRC_ctrl_t;

uint16_t CRC_Table(uint8_t *ptr, uint16_t len);
void tank_rc_DatePrcess(volatile const uint8_t * sbus_buf);
void Send_to_RC(uint8_t pos);
extern const TankRC_ctrl_t *get_tankrc_control_point(void);

#ifdef __cplusplus
}
#endif
#endif

