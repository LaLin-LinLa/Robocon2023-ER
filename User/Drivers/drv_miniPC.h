#ifndef __DRV_MINIPC_H
#define __DRV_MINIPC_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

#define MINIPC_FRAME_LENGTH	12u
#define MINIPC_HEAD							0x55
#define MINIPC_TAIL							0xAA
/*miniPC接收数据包*/
typedef __packed struct
{
	uint8_t head;
	float pitch[4];
	float yaw[4];
	float distance;
	uint8_t shoot_flag;
	uint8_t end;
} miniPC_rx_Pack_t;
/*miniPC发送数据包*/
typedef __packed struct
{
	char 	head;							//1byte		帧头0x5A
	char 	Speed1[2];				//2byte		发射速度1
	char 	Speed2[2];				//2byte		发射速度2
	float	Yaw;							//4byte		自身旋转角度
	float	voltage;					//4byte		主控电压
	char 	end;							//1byte		帧尾0xA5
	
//	float data[8];
//	uint8_t	tail[4];
	
} miniPC_tx_Pack_t;
/*发送上位机共用体*/
typedef __packed union
{
	miniPC_tx_Pack_t	txpack;
	unsigned char PcDataArray[sizeof(miniPC_tx_Pack_t)];
} miniPC_tx_Union_t;
/*接收上位机共用体*/
typedef __packed union
{
	miniPC_rx_Pack_t	rxpack;
	unsigned char PcDataArray[sizeof(miniPC_rx_Pack_t)];
} miniPC_rx_Union_t;

void miniPC_DatePrcess(volatile const uint8_t * buf);
void miniPC_DateSend(miniPC_tx_Union_t *miniPC_tx);
extern const miniPC_rx_Pack_t *get_miniPC_point(void);

#ifdef __cplusplus
}
#endif
#endif
