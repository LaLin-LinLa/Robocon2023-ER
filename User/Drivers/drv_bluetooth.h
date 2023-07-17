#ifndef __DRV_BLUETOOTH_H
#define __DRV_BLUETOOTH_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

#define BT_FRAME_LENGTH			4u
#define BT_HEAD							0xAA
#define BT_TAIL							0xBB

/*BT接收数据包*/
typedef __packed struct
{
	uint8_t head;
	uint16_t 	x;
	uint16_t 	y;
	float			yaw;
	uint8_t end;
} BT_rx_Pack_t;
/*BT发送数据包*/
typedef __packed struct
{
	char 	head;
	int		pwm_value[2];
	char	check;
	char 	end;
} BT_tx_Pack_t;
/*发送BT共用体*/
typedef __packed union
{
	BT_tx_Pack_t	txpack;
	unsigned char PcDataArray[sizeof(BT_tx_Pack_t)];
} BT_tx_Union_t;
/*接收BT共用体*/
typedef __packed union
{
	BT_rx_Pack_t	rxpack;
	unsigned char PcDataArray[sizeof(BT_rx_Pack_t)];
} BT_rx_Union_t;

void BT_DatePrcess(volatile const uint8_t * buf);
void BT_DateSend(BT_tx_Union_t *BT_tx);
extern const BT_rx_Pack_t *get_BT_point(void);

#ifdef __cplusplus
}
#endif
#endif
