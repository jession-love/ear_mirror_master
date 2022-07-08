#ifndef __CAMERA_PRO_H
#define __CAMERA_PRO_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

/*Protocol Header*/
#define CPRO_BASIC_MAGIC				0xffeeffee	//this magic is used to get ustorage protocol info, device must support
#define CPRO_CAMERA_MAGIC					0xeeffeeff
#define CPRO_INFO_SIZE  32
#define CPRO_DATA_PORT                   OEMINFO_DEFAULT_BASE_PORT
#define CPRO_DATA_PORT_STR              "10005"

#ifndef BIT
#define BIT(x) (1U << (x))
#endif


enum {
	CPRO_DEVINFO_GET 	= 1,
	CPRO_LIC_GET,
	CPRO_LIC_SET,
	CPRO_OPEN_VIDEO,
	CPRO_CLOSE_VEIDEO,
	CPRO_UPDATE_START,
	CPRO_UPDATE_DATA,
	CPRO_UPDATE_END,
	CPRO_EVENT_NOTIFY,
	CPRO_LED_CONTROL,
	CPRO_NAK_HANDLE,
	CPRO_COMMON_CONTROL, //add by zhangwei 20210714
};


enum{
	CPRO_OK			=        0,	/*successful*/
	CPRO_ERROR      =        1,	/*general error*/
	CPRO_NOSUCHPRO  =        2,	/*protocol not exist*/
};

/*Basic protocol struct*/
/*Used for request protocol magic*/
struct cProBasic{
	int32_t	 	head;	/*protocol magic*/
	uint16_t 	seqNo;
	uint16_t 	cid;
	int8_t 	 	ackNeed;
	int8_t 		reFlag;	
	uint16_t 	len;
}__attribute__((__packed__)); 



//CPRO_DEVINFO_GET
#ifndef CPRO_INFO_SIZE
#define CPRO_INFO_SIZE  32
#endif
#ifndef CPRO_LIC_SIZE
#define CPRO_LIC_SIZE  144
#endif
#define LICENSE_CORRECT_OFFSET		105

enum{
	DEV_TYPE_BK7231U_V1 = 1,
};

struct cProDevInfo{
	uint8_t type;
	uint8_t vendor[CPRO_INFO_SIZE];
	uint8_t product[CPRO_INFO_SIZE];
	uint8_t version[16];	
	uint8_t ssid[CPRO_INFO_SIZE];
	uint8_t mac[6];
	uint8_t gsensor			:1;
	uint8_t isLowPowerOff	:7;
	uint8_t isCharge 		:1;
	uint8_t battery			:7;	
	uint8_t capacity; //add by zhangwei 20210714
	uint8_t workMode; //add by zhangwei 20210728 for c3 work mode;	
	int8_t motorOnOff;		//add by zhangwei 20210729 for ypc c3 motor on off notify
	uint8_t reserved[4];
}__attribute__((__packed__));

//CPRO_LIC_SET
struct cProLic{
	char sn[CPRO_INFO_SIZE];
	char lic[CPRO_LIC_SIZE];
}__attribute__((__packed__));

struct cProWifi{
	char SSID[CPRO_INFO_SIZE];
	char PASSWD[CPRO_INFO_SIZE];
	int8_t channel;
}__attribute__((__packed__));

enum{
	VIDEO_TYPE_V1 = 1,
	VIDEO_TYPE_V2 = 2,
};
struct hVideoUDP{ //header video udp
	uint8_t type;		//video header type
	uint8_t seq;		//sequence for disorder
	uint8_t frameId;	//frame id
	uint8_t isEof;		//eof flag
	uint8_t frameCnt;	//frame count
	uint8_t gsensorSupport;
	uint32_t gsensorData; 	//gsensor data
	uint8_t mac[2];
	int16_t wide;
	int16_t high; 
}__attribute__((__packed__));

enum{
	EVT_TYPE_EAR = 1,
	EVT_TYPE_EAR_V2	= 2,	
};
struct eventSetEar{
	uint8_t type;
	uint8_t isCharge 		:1;
	uint8_t battery			:7;
	uint8_t isSensorOk		:1;
	uint8_t isLowPowerOff	:7;
	uint8_t workMode;		//add by zhangwei 20210728 for ypc c3 mode must > 1
	int8_t motorOnOff;		//add by zhangwei 20210729 for ypc c3 motor on off notify
	uint8_t picbutton  :4;
	uint8_t videobutton :4;
	uint8_t zoomdown :4;
	uint8_t zoomup :4;
	uint8_t mac[2];
	uint8_t reserverd[8];
}__attribute__((__packed__));


enum{
	LED_CAMERA = 1,
};

enum{
	OP_GET = 0,
	OP_SET = 1,
	
	OP_LED_OFF = 0,
	OP_OFF = 0,
	OP_LED_ON = 1,
	OP_ON = 1,
	OP_LED_BLINK = 2,
	OP_LED_BREATH = 3,
	OP_BUTTON = 4,
};

struct cProLed{
	uint8_t led				:4; /*max 15 leds*/
	uint8_t opType			:4; /*OP_GET/OP_SET*/
	uint8_t ledStatus		;	/* on/off/blink/breath*/
	uint8_t value 			; 	/*led light 0-100*/
}__attribute__((__packed__));


enum{
	CAMCAP_LIGHT = BIT(0), //adjust light capacity
	CAMCAP_GSENSOR = BIT(1),
	CAMCAP_MOTOR = BIT(2),
	CAMCAP_ADEFFECT = BIT(3),
	CAMCAP_MODE = BIT(4),
	CAMCAP_MAX,
};

struct cProComCtr{
	uint8_t item;
	uint8_t type			:4; /*OP_GET/OP_SET*/
	uint8_t action			:4;	/* on/off/blink/breath*/	
	int32_t value 			; 	/*value*/
}__attribute__((__packed__));

#ifdef __cplusplus
}
#endif

#endif

