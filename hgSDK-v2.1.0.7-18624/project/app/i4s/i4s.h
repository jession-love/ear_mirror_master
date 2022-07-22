#ifndef _I4S_H
#define _I4S_H
#include "camera_pro.h"
#include "i4sgsensor.h"
//debug setting
#define APP_I4SC_DEBUG              1
#if APP_I4SC_DEBUG
#define I4SC_PRT                os_printf
#define I4SC_WARN               os_printf
#define I4SC_FATAL              os_printf
#else
#define I4SC_PRT                _os_printf
#define I4SC_WARN               _os_printf
#define I4SC_FATAL              _os_printf
#endif
#define FLASH_WR_BUF_MAX   1024
//程杰到此一游
enum{
	OP_WIFI_CONECTED = 1, /*wifi connected*/
	OP_WIFI_DISCONECTED = 2, /*wifi disconnected*/
	OP_APP_CONECTED = 3, /*app connect*/
	OP_APP_DISCONECTED = 4, /*app disconnect*/
	OP_FIRM_UPGRADEING = 5,/*firmware upgrade*/
	OP_FIRM_FAILED = 6, /*firmware upgrade failed*/
	OP_FIRM_BEGIN = 7, /*firmware upgrade begin*/
	OP_FIRM_UPGRADEOK = 8, 

	OP_INFTMODE = 13, /*into factory mode*/
	OP_GETWKMODE = 14, /*get current work mode[normal/factory/blackhead-->0/1/2]*/
	OP_INNMMODE = 15, /*into normal mode*/	
	OP_INBCMODE = 16, /*into blackhead check mode*/
	OP_GETMOTORSTAT = 17, /*get motor status on/off*/
	OP_NOTIFYUSETIME = 18, /*notify function, use time*/
	OP_INBLUEMODE = 19,	/*into blue mode*/
	OP_ISRECORDING = 20, /*is recroding*/
	OP_GETBATTERYINFO = 21, /*get battery information; bit0-bit6: battery level 0-100; bit7: charge status; bit8:will poweroff*/
	OP_STATUS_BUTTONT1 = 22, /*get button status, 1: pressed*/
	OP_GETMOTORLEVEL = 23,
	OP_STATUS_BTNMEDIA = 24, /*media button bit0: photo; bit1:video; bit2:zoom in; bit3:zoom out*/
	OP_OPEN_CAMERA_LED	= 25,
	OP_CLOSE_CAMERA_LED = 26,
	OP_GET_PIC_VIDEO_BUTTON = 27,
	OP_GET_ZOOM_STA			 = 28,
};
typedef int (*op_cb)(int);

enum{
	COM_GET_MOTOR = 1,
	COM_SET_MOTOR = 2,
	COM_APP_BFGROUND = 3,
	COM_APP_GET_EFFECT = 4,	
	COM_APP_SET_EFFECT = 5,
};
enum{
	OEMFLAG_AP = 1 << 0,
	OEMFLAG_NET = 1<< 1,
	OEMFLAG_OEM = 1<< 2,
	OEMFLAG_ALL = (1<<3)-1,
};
typedef struct _oeminfo_ap_t{
	uint8_t ap_prefix[27];/*32-mac 4 byte - \0*/
	uint8_t ap_pasword[65];
	uint8_t ap_channel;
}__attribute__((__packed__)) oeminfo_ap_t;

typedef struct _oeminfo_net_t{
	uint8_t net_ip[16];
	uint8_t net_mask[16];
	uint8_t net_gateway[16];
}__attribute__((__packed__)) oeminfo_net_t;

typedef struct _oeminfo_base_t{
	uint8_t vendor[32];
	uint8_t product[32];
	uint8_t version[32];
	uint16_t port;
	uint8_t capacity; //bit0:gsensor; bit1-bit7 reserved
}__attribute__((__packed__)) oeminfo_base_t;

#define OEMINFO_DEFAULT_AP_PREFIX 		"Suear-"
#define OEMINFO_DEFAULT_AP_PASSWORD		""
#define OEMINFO_DEFAULT_AP_CAHNNEL		1
#define OEMINFO_DEFAULT_NET_IP			"192.168.1.1"
#define OEMINFO_DEFAULT_NET_MASK		"255.255.255.0"
#define OEMINFO_DEFAULT_NET_GW			"192.168.1.1"
#define OEMINFO_DEFAULT_BASE_VENDOR		"YIPINCHENG"
#define OEMINFO_DEFAULT_BASE_PRODUCT	"ICB-4U01-X6-L" //for ear
#define OEMINFO_DEFAULT_BASE_VERSION	"8.000.006"
#define OEMINFO_DEFAULT_BASE_PORT		10005
#define OEMINFO_HWVERSION				"YPC-TW806-WIFI-V1.0"


int i4s_pre_init(void *cfg);
int i4s_camera_init(void);
void i4s_camera_uninit(void);
int i4s_get_oem_info(void *oem, int len, uint8_t flag);
void i4s_set_oem_info(void *oem, uint8_t flag);

int i4s_handle_event(int event);

//global api used for others
void i4s_op_callback_register(op_cb callback);
void i4s_handle_event_charge(int event);//event=1 charging; event=0 not charge
void i4s_update_gensor_data(uint32_t value);

void i4s_common_callback_register(int (*callback)(uint8_t *, uint16_t *));
void i4s_set_capacity(uint8_t wCap);
void i4s_auto_generate_license(uint8_t autoGen);
void set_battey_fun(float battey1);

void i4sGensorDataUpdate(uint32_t value);

int bkapi_upgrade_data(uint16_t seq_num, char *page, UINT32 len);
int bkapi_upgrade_init(uint16_t seq_num);

//int dp_sendto(int event);

#endif
