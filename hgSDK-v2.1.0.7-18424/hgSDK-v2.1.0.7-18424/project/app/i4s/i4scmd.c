#include "include.h"
#include "mem_pub.h"
#include "str_pub.h"
#include "uart_pub.h"
#include "rtos_pub.h"
#include <stdlib.h>
#include "param_config.h"
#include "camera_pro.h"
#include "common.h"
#include "i4s.h"

enum{
	MODE_NORMAL = 0,
	MODE_BURN = 1,
	MODE_MAX,
};

enum{
	ERR_OK = 0,
	ERR_BASE = 1,
	ERR_MODE,
	ERR_SETLIC,
	ERR_PARAM,
};

enum{
	S_BL = 0,
	S_DUTCHK = 1,
	S_APP =2 ,
};
typedef struct _modeStatus_t{
	UINT8 bStage	:2; //system start stage
	UINT8 bDUT		:1; //system DUT mode
	UINT8 bReserv	:5;//reserver
}modeStatus_t;

volatile UINT8 curMode = MODE_NORMAL; //0:normal mode   1:burn mode(r/w license)
volatile modeStatus_t i4Mod = {S_BL, 0, 0};
volatile UINT8 i4sDebug = 0;

extern void i4s_printf(const char *fmt, ...);
void i4sSetDebugState(UINT8 state);
UINT8 i4sGetDebugState(void);

#define i4S_DEBUG		i4s_printf
//json return for pc decode
static void i4s_command_mode(UINT8 mode)
{
	if(mode >= MODE_MAX){
		//get mode
		i4S_DEBUG("{\"r\":%d,\"mode\":%d}\r\n", ERR_OK, curMode);
		return;
	}
	curMode = mode;
	i4S_DEBUG("{\"r\":%d,\"mode\":%d}\r\n", ERR_OK, curMode);
}

static void i4s_lic_encode(char *lic)
{
	if(lic == NULL || os_strlen(lic) == 0){
		i4S_DEBUG("\"\"");
		return;
	}
#define TMPLEN 64	
	char tbuf[TMPLEN] = {0};
	char *pos = tbuf, *end = tbuf + TMPLEN;
	int i, ret;
	

	*pos++ = '"';
	for (i = 0; i < os_strlen(lic); i++) {
		if(end - pos <=3){
			*pos = '\0';
			i4S_DEBUG(tbuf);
			pos = tbuf;
		}
		ret = os_snprintf(pos, end - pos, "%02X", (UINT8)lic[i]);
		if (ret < 0){
			end[-1] = '\0';
			i4S_DEBUG("\"\"");
			return ;
		}
		pos += ret;
	}
	*pos++ = '"';
	*pos = '\0';
	i4S_DEBUG(tbuf);
}
//json return for pc decode	
static void i4s_command_getlic(void)
{
	if(curMode != MODE_BURN){
		i4S_DEBUG("{\"r\":%d}\r\n", ERR_MODE);
		return;
	}
	struct cProLic licinfo;
	os_memset(&licinfo, 0, sizeof(struct cProLic));
	//invoke license
	bkapi_get_license(&licinfo);
	i4S_DEBUG("{\"r\":%d, \"sn\":\"%s\", \"lic\":", ERR_OK, licinfo.sn);
	i4s_lic_encode(licinfo.lic);
	i4S_DEBUG("}\r\n");
}

static int ihex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}


static int ihex2byte(const char *hex)
{
	int a, b;

	a = ihex2num(*hex++);
	if (a < 0)
		return -1;
	b = ihex2num(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}

static int ihexstr2bin(const char *hex, u8 *buf, size_t len)
{
	size_t i;
	int a, tlen = len/2;
	const char *ipos = hex;
	u8 *opos = buf;

	if(len % 2){
		return -1;
	}
	for (i = 0; i < tlen; i++) {
		a = ihex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

//json return for pc decode
static void i4s_command_setlic(char *sn, char *lic)
{
	if(curMode != MODE_BURN){
		i4S_DEBUG("{\"r\":%d}\r\n", ERR_MODE);
		return;
	}

	struct cProLic licinfo;
	os_memset(&licinfo, 0, sizeof(struct cProLic));
	os_memcpy(licinfo.sn, sn, CPRO_INFO_SIZE);

	if(ihexstr2bin(lic, licinfo.lic, os_strlen(lic)) < 0){
		i4S_DEBUG("{\"r\":%d}\r\n", ERR_BASE);
		return;
	}
	if(bkapi_set_license(&licinfo) < 0){
		i4S_DEBUG("{\"r\":%d}\r\n", ERR_SETLIC);
		return;
	}
	
	i4S_DEBUG("{\"r\":%d}\r\n", ERR_OK);
	return;	
}

//json return for pc decode
static void i4s_command_fwinfo(void)
{
	oemInfo_OEM baseInfo;
	os_memset(&baseInfo, 0, sizeof(oemInfo_OEM));
	if(i4sCamera_getOEM(&baseInfo, sizeof(oemInfo_OEM), OEMFLAG_OEM) < 0){
		os_memcpy(baseInfo.oemVendor, OEMINFO_DEFAULT_BASE_VENDOR, sizeof(baseInfo.oemVendor));
		os_memcpy(baseInfo.oemProduct, OEMINFO_DEFAULT_BASE_PRODUCT, sizeof(baseInfo.oemProduct));
	}
	i4S_DEBUG("{\"r\":%d,\"vendor\":\"%s\",\"product\":\"%s\",\"ver\":\"%s\"}\r\n", 
		ERR_OK, baseInfo.oemVendor, baseInfo.oemProduct, baseInfo.oemVersion);
}

static void i4s_command_status(void)
{
	if(i4Mod.bStage < S_APP){
		i4S_DEBUG("STARTUPBK\r\n");
		i4Mod.bDUT = 1;
	}else{
		i4S_DEBUG("STARTOVER\r\n");
	}
}

static void i4s_debug_control(UINT8 state)
{
	i4sSetDebugState(state);
}

static void i4s_command_mac(char *pmac)
{
	uint8_t mac[6] = {0};
	int ret = 0;
	if(pmac){
		if(os_strlen(pmac) < 12 || hexstr2bin(pmac, mac, 6) < 0){
			i4S_DEBUG("{\"r\":%d}\r\n", ERR_PARAM);
			return;
		}
		if(wifi_set_mac_address((char *)mac) == 0){
			ret = 1;
		}
	}else{
		wifi_get_mac_address((char *)mac, CONFIG_ROLE_AP);
	}
	i4S_DEBUG("{\"r\":%d,\"mac\":\"%02x%02x%02x%02x%02x%02x\"}\r\n", 
		ret, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);	
}
void i4s_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if(argc < 2){
		i4S_DEBUG("bad parameters\r\n");
		return;
	}
	int logSta = i4sGetDebugState();
	if(logSta){
		i4sSetDebugState(0);
	}
	if(!os_strcmp(argv[1], "mode")){
		if(argc == 2){
			//get mode
			i4s_command_mode(MODE_MAX);
		}else{
			i4s_command_mode(atoi(argv[2]));
		}
	}else if(!os_strcmp(argv[1], "getlic")){
		i4s_command_getlic();
	}else if(!os_strcmp(argv[1], "setlic")){
		if(argc != 4){
			i4S_DEBUG("{\"r\":%d}\r\n", ERR_PARAM);
			if(logSta){
				i4sSetDebugState(logSta);
			}		
			return;
		}
		i4s_command_setlic(argv[2], argv[3]);
	}else if(!os_strcmp(argv[1], "fwinfo")){
		i4s_command_fwinfo();
	}else if(!os_strcmp(argv[1], "status")){
		i4s_command_status();
	}else if(!os_strcmp(argv[1], "debug")){
		if(argc > 2){
			i4s_debug_control((UINT8)atoi(argv[2]));
			return; //just return not restore the old value
		}
	}else if(!os_strcmp(argv[1], "mac")){
		if(argc == 2){
			//get mac
			i4s_command_mac(NULL);
		}else{
			i4s_command_mac(argv[2]);
		}
	}else{
		i4S_DEBUG("bad command\r\n");
	}
	if(logSta){
		i4sSetDebugState(logSta);
	}	
}

#if 0
void i4sBurnModeCheck(UINT16 chkTime)
{
#define PERTIME 	20
	UINT16 tTime = chkTime, useTime = 0;
	if(tTime < 200){
		tTime = 200;
	}
	i4Mod.bStage = S_DUTCHK;
	while(1){
		if(!i4Mod.bDUT){			
			useTime += PERTIME;
		}
		rtos_delay_milliseconds(PERTIME);
		if(useTime >= tTime){
			os_printf("DUT Timeout..\r\n");
			i4Mod.bStage = S_APP;
			return;
		}
	}
}
#else
void i4sBurnModeCheck(UINT16 chkTime)
{
	i4Mod.bStage = S_DUTCHK;
	rtos_delay_milliseconds(chkTime);
}

#endif

UINT8 i4sGetDebugState(void)
{
	return i4sDebug;
}

 void i4sSetDebugState(UINT8 state)
{
	i4sDebug = state;
}

