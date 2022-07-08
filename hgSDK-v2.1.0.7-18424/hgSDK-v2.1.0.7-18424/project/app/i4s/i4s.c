/*system include*/
#include "sys_config.h"
#include "typesdef.h"
#include "list.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/sys.h"
#include "lwip/ip_addr.h"
#include "lwip/tcpip.h"
#include "osal/string.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "osal/task.h"
#include "osal/sleep.h"
#include "osal/timer.h"
#include "osal/work.h"
#include "lib/umac/umac.h"
#include "hal/dvp.h"
#include "lib/video/dvp/jpeg/jpg.h"
#include "hal/jpeg.h"
#include "syscfg.h"

/*user include*/
#include "camera_pro.h"
#include "i4s.h"
#include "i4s_port.h"

/*MICRO*/
#define CPRO_HDR_SZIE 	(sizeof(struct cProBasic))

#define I4S_BUF_SIZE		1500

/*Task Status*/
enum{
	TASK_STOP = 0,
	TASK_RUN,
	TASK_QUIT,
};


/*Video status*/
enum{
	VIDEO_STOP = 0,
	VIDEO_START ,
};
	
typedef struct tvideo_hdr_param{
    uint8_t* ptk_ptr;
    uint32_t ptklen;	
    uint8_t frame_id;
    uint32_t is_eof;
    uint32_t frame_cnt;
}TV_HDR_PARAM_ST, *TV_HDR_PARAM_PTR;

typedef struct _vcapacity_t{
	struct{
		uint8_t cap_gsensor 	:1;
		uint8_t cap_reserved	:7;
	}cap_bits;
	
	uint8_t capacity;
}vcapacity_t;

typedef struct vtconf_t{
	uint8_t version; // video transfer protocol version
	uint8_t hdsize; //sub header size
}vtcfg;

typedef struct _con_t{
	volatile uint8_t status; //connect or disconnect	
	int sockfd;	/*socket fd*/
	struct sockaddr_in addr;	
}con_t;

typedef struct _video_t{
	void *thread; /*thread task id*/
	struct os_mutex mutex;
	con_t connections;
}video_t; /*video releative setting*/

typedef struct _proto_t{
	int cmd_sockfd;	/*socket fd*/
	int pic_sockfd;	
	uint8_t buffer[I4S_BUF_SIZE];
	uint16_t port;	/*listen port*/
	void *thread; /*thread task id*/
}proto_t; /*protocol handle struct*/

typedef struct _event_t{
	uint8_t wifi_connected	:1;
	uint8_t have_addr		:1;	
	int sockfd;
	uint16_t port;	/* port*/
	struct in_addr addr;
	void *thread; /*thread task id*/
}event_t;

typedef struct _lic_option_t{
	uint8_t lic_auto_gen;	
	uint8_t lic_done; //generate done or read flash done
	struct cProLic lic;
}lic_option_t;

typedef struct _oem_info_t{
	uint8_t flags;
	oeminfo_ap_t ap_info;
	oeminfo_net_t net_info;
	oeminfo_base_t base_info;
}oem_info_t;

typedef struct _config_t{
	vcapacity_t capacity;
	uint8_t mac[6];
	lic_option_t license; 
	oem_info_t oem;
}config_t;

typedef struct _video_trans_t{
	volatile uint8_t status; //RUN STOP	
	config_t	config;
	proto_t		protocol;
	video_t		video;
	event_t  	events;
}video_trans_t;

typedef struct _callback_t{
	
}callback_t;
//Golbal Context
video_trans_t wifi_camera;
#define WIFI_RUN_STATUS		(wifi_camera.status)

volatile uint32_t gSensor = 8;
volatile uint16_t lseqNo = 0;

extern volatile struct list_head *usr_p;			//应用的节点指针
extern volatile struct list_head free_tab;			//空闲列表，存放空间节点
extern struct dvp_device *dvp_test;

//callback
op_cb op_callback; /*operation callback*/
int (*led_callback)(struct cProLed*)  = NULL;
int (*common_callback)(uint8_t *, uint16_t *)  = NULL;

static int battey = 100;

/***********************************************************************************/
/*				             FUNCTION DECLARATION								   */
/*																				   */
/***********************************************************************************/
uint8_t i4s_get_capacity(void);
static int app_deal_close_video(void);

/***********************************************************************************/
/*				             STATIC CAMERA FUNCTION								   */
/*																				   */
/***********************************************************************************/
static inline void _get_mac_from_cache(uint8_t *mac)
{
	os_memcpy(mac, wifi_camera.config.mac, 6);
}

static inline void _set_mac_to_cache(uint8_t *mac)
{
	os_memcpy(wifi_camera.config.mac, mac, 6);
}

static int app_deal_get_license(uint8_t *payload, int *paysize)
{
	int ret = 0;

	if(!payload || !paysize){
		return 1;
	}

	struct cProLic *lic = (struct cProLic *)(payload);
	/*Write License to Flash*/
	if(wifi_camera.config.license.lic_done == 1){
		os_memcpy(lic, &(wifi_camera.config.license.lic), sizeof(struct cProLic));
		I4SC_FATAL("GetLicense From Cache\r\n");
		ret = 0;
	}else{
		if(wifi_camera.config.license.lic_auto_gen){
			ret = sys_port_get_license(lic);
		}else{
			ret = sys_port_get_license(lic);
		}
		wifi_camera.config.license.lic_done = 1;
	}
	if(ret < 0){
		I4SC_FATAL("Get License Failed\r\n");
	}else{
		I4SC_FATAL("OK=>SN:%s!\r\n", lic->sn);
	}
	*paysize = sizeof(struct cProLic);
	return 0;
}

static int app_deal_set_license(uint8_t *payload, int paysize)
{
	if(!payload || !paysize){
		return -1;
	}
	int lic_len = sizeof(struct cProLic);
	if(paysize != lic_len){
		I4SC_FATAL("len not match\r\n");
		return 1;
	}

	struct cProLic *lic = (struct cProLic *)(payload);
	
	os_memcpy(&(wifi_camera.config.license.lic), lic, sizeof(struct cProLic));
	wifi_camera.config.license.lic_done = 1;

	/*Write License to Flash*/
	if(sys_port_set_license(lic) < 0){
		I4SC_FATAL("Set License Failed\r\n");
		return 2;
	}else{
		I4SC_FATAL("OK=>SN:%s!\r\n", lic->sn);
	}

	return 0;
}

static int app_deal_event_addr(struct sockaddr_in *addr)
{
	event_t *evt_ctx = &(wifi_camera.events);
	
	if(evt_ctx->wifi_connected == 0){
		I4SC_FATAL("Force Set OP_WIFI_CONECTED\r\n");
		i4s_handle_event(OP_WIFI_CONECTED);
	}
	os_memcpy(&(evt_ctx->addr), &(addr->sin_addr), sizeof(struct in_addr));
	evt_ctx->have_addr = 1;
	
	return 0;
}

static int app_deal_get_devinfo(uint8_t *payload, int *paysize)
{
	if(!payload){
		return 1;
	}

	struct cProDevInfo *info = (struct cProDevInfo *)(payload);

	info->type = DEV_TYPE_BK7231U_V1;
	
	oeminfo_base_t base_info;
	if(i4s_get_oem_info(&base_info, sizeof(oeminfo_base_t), OEMFLAG_OEM) < 0){
		os_memcpy(base_info.vendor, OEMINFO_DEFAULT_BASE_VENDOR, sizeof(base_info.vendor));
		os_memcpy(base_info.product, OEMINFO_DEFAULT_BASE_PRODUCT, sizeof(base_info.product));
	}
	os_memcpy(info->vendor, base_info.vendor, CPRO_INFO_SIZE);
	os_memcpy(info->product, base_info.product, CPRO_INFO_SIZE);
	os_memcpy(info->version, base_info.version, sizeof(info->version));

	//wifi info
	struct cProWifi wifi_info;
	os_memset(&wifi_info, 0, sizeof(struct cProWifi));
	sys_port_get_apinfo(&wifi_info);
	os_memcpy(info->ssid, wifi_info.SSID, CPRO_INFO_SIZE);
	//mac
	sys_port_get_mac(info->mac);
	_set_mac_to_cache(info->mac);
	info->gsensor = !!(i4s_get_capacity()&CAMCAP_GSENSOR);
	info->capacity = i4s_get_capacity();
	
	int16_t batterinfo = i4s_handle_event(OP_GETBATTERYINFO);
	info->battery = (batterinfo&0x7F);
	info->isCharge = ((batterinfo&0x80) ?1:0);
	info->isLowPowerOff = ((batterinfo&0x100)?1:0);

	if(common_callback){		
		struct cProComCtr comCtr;
		uint16_t comLen = sizeof(struct cProComCtr);
		os_memset(&comCtr, 0, comLen);
		comCtr.item = CAMCAP_MODE;
		comCtr.type = OP_GET;
		common_callback((uint8_t *)&comCtr, &comLen);
		info->workMode = (comCtr.value&0xFF);
		I4SC_PRT("Work Mode:%d\r\n", comCtr.value);
		//get motor on off status		
		os_memset(&comCtr, 0, comLen);
		comCtr.item = CAMCAP_MOTOR;
		comCtr.type = OP_GET;
		comCtr.action = OP_BUTTON;
		common_callback((uint8_t *)&comCtr, &comLen);
		info->motorOnOff = (comCtr.value&0xFF);
		I4SC_PRT("Motor Status:%d\r\n", comCtr.value);			
	}

	*paysize = sizeof(struct cProDevInfo);
	
	return 0;
}

static int app_deal_open_video(int sockfd, struct sockaddr_in *addr, uint8_t *payload)
{
	uint16_t port = 0;
	struct jpg_device *jpeg_dev;
	
	if(!addr){
		return 1;
	}

	app_deal_close_video();
	
	__disable_irq();
	wifi_camera.video.connections.sockfd = sockfd;
	os_memcpy(&(wifi_camera.video.connections.addr), addr, sizeof(struct sockaddr_in));
	if(payload){
		os_memcpy(&port, payload, sizeof(uint16_t));
		wifi_camera.video.connections.addr.sin_port = htons(port);
	}
	wifi_camera.video.connections.status = VIDEO_START;
	__enable_irq();
	I4SC_FATAL("pic port:%d\r\n", htons(wifi_camera.video.connections.addr.sin_port));	

	jpeg_dev = (struct jpg_device *)dev_get(HG_JPG_DEVID);	
	jpg_open(jpeg_dev);
	dvp_open(dvp_test);

	return 0;
}

static int app_deal_close_video(void)
{
	struct jpg_device *jpeg_dev;

	jpeg_dev = (struct jpg_device *)dev_get(HG_JPG_DEVID);	
	jpg_close(jpeg_dev);
	dvp_close(dvp_test);
	
	wifi_camera.video.connections.status = VIDEO_STOP;
	
	os_sleep_ms(30);
	
	return 0;
}

static int app_deal_control_led(uint8_t *payload, int *paysize)
{
	struct cProLed *ledArg = (struct cProLed *)(payload);
	if(led_callback){
		I4SC_FATAL("Led control:%d-%d-%d-%d\r\n", ledArg->led, ledArg->opType,
				ledArg->ledStatus, ledArg->value);
		return led_callback(ledArg);
	}
	I4SC_FATAL("led callback empty\r\n");
	return 3;
}

static int app_deal_common_control(uint8_t *payload, uint16_t *paysize)
{
	if(common_callback){
		I4SC_FATAL("common callback paysize:%d\r\n", *paysize);
		return common_callback(payload, paysize);
	}
	I4SC_FATAL("common callback empty\r\n");
	return 3;
}

static int app_deal_upgrade(uint16_t cid, uint8_t *payload, int paysize)
{
	if(!payload || !paysize){
		I4SC_FATAL("parameter error2\r\n");
		return 0;
	}

	if(cid == CPRO_UPDATE_START){
		i4s_handle_event(OP_FIRM_BEGIN);
		return  0;
	}else if(cid == CPRO_UPDATE_DATA){		
		i4s_handle_event(OP_FIRM_UPGRADEING);
		return 0;
	}else if(cid == CPRO_UPDATE_END){		
		i4s_handle_event(OP_FIRM_UPGRADEOK);
	}

	return 0;
}

static int app_deal_nak_handle(uint8_t *payload, int paysize)
{
	if(!payload || !paysize){
		return 0;
	}
	
	I4SC_FATAL("Not support NAK\r\n");
	return 3;
}

static int i4s_cmd_protocol_handle(int sock, uint8_t *buffer, int buf_size)
{
	int rev_len, snd_len;
	uint16_t tmp_len;
	socklen_t addr_len;
	struct sockaddr_in cli_addr;
	struct cProBasic *prohdr;

	if(sock == -1){
		I4SC_FATAL("parameter error\r\n");
		return 0;
	}
	
	addr_len = sizeof(struct sockaddr);
	rev_len = recvfrom(sock, buffer, buf_size, 0, (struct sockaddr *)&cli_addr, &addr_len);
	if(rev_len <= 0){
		I4SC_FATAL("recv failed\r\n");
		return 0;
	}
	I4SC_FATAL("recv=%d cli_addr=0x%x port=%d addr_len=%d\r\n", 
		rev_len, cli_addr.sin_addr.s_addr, cli_addr.sin_port, addr_len);

	prohdr = (struct cProBasic *)buffer;
	if(prohdr->head != CPRO_BASIC_MAGIC){
		I4SC_FATAL("bad magic\r\n");
		return 0;
	}
	I4SC_FATAL("Recv SEQ:%d CMD->ID:%d CMDLEN:%d RECVLEN:%d\r\n",
			prohdr->seqNo, prohdr->cid, prohdr->len, rev_len);

	switch(prohdr->cid){
		//设置license，无license设备不能用
		case CPRO_LIC_SET:
			prohdr->reFlag = app_deal_set_license(buffer+CPRO_HDR_SZIE, rev_len-CPRO_HDR_SZIE);
			prohdr->len = 0;
			snd_len = prohdr->len + CPRO_HDR_SZIE;
			break;
		//获取lincense
		case CPRO_LIC_GET:
			prohdr->reFlag = app_deal_get_license(buffer+CPRO_HDR_SZIE, &rev_len);
			prohdr->len = rev_len;
			snd_len = prohdr->len + CPRO_HDR_SZIE;
			break;			
		case CPRO_DEVINFO_GET:			
			prohdr->reFlag = app_deal_get_devinfo(buffer+CPRO_HDR_SZIE, &rev_len);
			prohdr->len = rev_len;
			snd_len = prohdr->len + CPRO_HDR_SZIE;
			app_deal_event_addr(&cli_addr);
			break;
			//下面三个是OTA的 CPRO_UPDATE_START: CPRO_UPDATE_DATA: CPRO_UPDATE_END:
		case CPRO_UPDATE_START:
		case CPRO_UPDATE_DATA:
		case CPRO_UPDATE_END:
			prohdr->reFlag = app_deal_upgrade(prohdr->cid, buffer, rev_len);
			if(prohdr->cid == CPRO_UPDATE_START){
				prohdr->len = sizeof(uint16_t);
			}else{
				prohdr->len = 0;
			}
			snd_len = prohdr->len + CPRO_HDR_SZIE;		
			break;
			//LED控制
		case CPRO_LED_CONTROL:
			prohdr->reFlag = app_deal_control_led(buffer+CPRO_HDR_SZIE, &rev_len);
			snd_len = prohdr->len + CPRO_HDR_SZIE;
			break;
		case CPRO_NAK_HANDLE:
			prohdr->reFlag = app_deal_nak_handle(buffer+CPRO_HDR_SZIE, rev_len-CPRO_HDR_SZIE);
			snd_len = CPRO_HDR_SZIE;
			prohdr->len = 0;
			break;
			//命令的协议
		case CPRO_COMMON_CONTROL:
			tmp_len = prohdr->len;
			prohdr->reFlag = app_deal_common_control(buffer+CPRO_HDR_SZIE, &tmp_len);
			snd_len = prohdr->len + CPRO_HDR_SZIE;
			break;			
		default:
			I4SC_FATAL("Unsupport CMD->ID:%d CMDLEN:%d\r\n", prohdr->cid, prohdr->len);
			prohdr->reFlag = CPRO_NOSUCHPRO;
			snd_len = rev_len;
	};
	
	return sendto(sock, buffer, snd_len, 0, (struct sockaddr *)&cli_addr, addr_len);
}

static int i4s_pic_protocol_handle(int sock, uint8_t *buffer, int buf_size)
{
	//以下是摄像头的协议
	int rev_len, snd_len;
	socklen_t addr_len;
	struct sockaddr_in cli_addr;
	struct cProBasic *prohdr;

	if(sock == -1){
		I4SC_FATAL("parameter error\r\n");
		return 0;
	}
	
	addr_len = sizeof(struct sockaddr);
	rev_len = recvfrom(sock, buffer, buf_size, 0, (struct sockaddr *)&cli_addr, &addr_len);
	if(rev_len <= 0){
		I4SC_FATAL("recv failed\r\n");
		return 0;
	}

	prohdr = (struct cProBasic *)buffer;
	if(prohdr->head != CPRO_BASIC_MAGIC){
		I4SC_FATAL("bad magic\r\n");
		return 0;
	}
	
	switch(prohdr->cid){
		case CPRO_OPEN_VIDEO:
			if(wifi_camera.events.have_addr == 0){
				I4SC_FATAL("Force Set addr!!!\r\n");
				app_deal_event_addr(&cli_addr);
			}
			prohdr->reFlag = app_deal_open_video(sock, &cli_addr, buffer+CPRO_HDR_SZIE);
			prohdr->len = 0;
			snd_len = prohdr->len + CPRO_HDR_SZIE;
			break;
		case CPRO_CLOSE_VEIDEO:
			prohdr->reFlag = app_deal_close_video();
			prohdr->len = 0;
			snd_len = prohdr->len + CPRO_HDR_SZIE;
			break;
		case CPRO_NAK_HANDLE:
			prohdr->reFlag = app_deal_nak_handle(buffer+CPRO_HDR_SZIE, rev_len);
			snd_len = CPRO_HDR_SZIE;
			prohdr->len = 0;
			break;			
		default:
			I4SC_FATAL("Unsupport CMD->ID:%d CMDLEN:%d\r\n", prohdr->cid, prohdr->len);
			prohdr->reFlag = CPRO_NOSUCHPRO;
			snd_len = rev_len;
	}
	
	return sendto(sock, buffer, snd_len, 0, (struct sockaddr *)&cli_addr, addr_len);
}

static void _proto_task(void *arg)
{
	proto_t	*pctxd = (proto_t*)arg;
    fd_set rdset;
    struct timeval tv = {0, 200000};
	int ret, maxfd;

	if(!pctxd){
		I4SC_FATAL("protoCtx_task null\r\n");
		return;
	}
	while(WIFI_RUN_STATUS == TASK_RUN){
        FD_ZERO(&rdset);
        FD_SET(pctxd->cmd_sockfd, &rdset);	
        FD_SET(pctxd->pic_sockfd, &rdset);
		tv.tv_sec = 0;
		tv.tv_usec = 200000; /*200ms*/
		maxfd = max(pctxd->cmd_sockfd, pctxd->pic_sockfd);
		
        ret = select(maxfd+1, &rdset, NULL, NULL, &tv);
        if (ret < 0) {
			I4SC_FATAL("select failed:%d\r\n", ret);
            break;
        }else if(ret == 0) {
			continue;
        }
		if(FD_ISSET(pctxd->cmd_sockfd, &rdset)){
			/*handle request*/
			i4s_cmd_protocol_handle(pctxd->cmd_sockfd, pctxd->buffer, sizeof(pctxd->buffer));
		}
		if(FD_ISSET(pctxd->pic_sockfd, &rdset)){
			/*handle request*/
			i4s_pic_protocol_handle(pctxd->pic_sockfd, pctxd->buffer, sizeof(pctxd->buffer));
		}
	}

	I4SC_PRT("protoCtx Task Quit\r\n");	
}

static void _proto_uninit(proto_t	*pctxd)
{
	if(!pctxd){
		return ;
	}

	close(pctxd->cmd_sockfd);
	close(pctxd->pic_sockfd);	

	
	csi_kernel_task_del(pctxd->thread);
	os_memset(pctxd, 0, sizeof(proto_t));
	
	I4SC_PRT("protoCtx Quit\r\n");
}

static int _proto_init(proto_t	*pctxd, uint16_t port)
{
	int ret;
	struct sockaddr_in addr;

	if(!pctxd){
		return 0;
	}
	os_memset(pctxd, 0, sizeof(proto_t));
	pctxd->port = port;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(pctxd->port);
	/*init cmd socket*/
    pctxd->cmd_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (pctxd->cmd_sockfd  == -1) {
		I4SC_PRT("create cmd udp failed\r\n");
		goto EXIT;
	}
	I4SC_PRT("csock:%d\r\n", pctxd->cmd_sockfd);
	/*bind socket*/
    ret = bind(pctxd->cmd_sockfd, (struct sockaddr *)&addr, sizeof (struct sockaddr));
	if(ret){
		I4SC_PRT("bind udp failed\r\n");
		goto EXIT;
	}
	//create picture sockfd
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(pctxd->port+1);
	/*init cmd socket*/
    pctxd->pic_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (pctxd->pic_sockfd  == -1) {
		I4SC_PRT("create pic udp failed\r\n");
		goto EXIT;
	}
	
	I4SC_PRT("psock:%d\r\n", pctxd->pic_sockfd);
	/*bind socket*/
    ret = bind(pctxd->pic_sockfd, (struct sockaddr *)&addr, sizeof (struct sockaddr));
	if(ret){
		I4SC_PRT("bind pic udp failed\r\n");
		goto EXIT;
	}	
	csi_kernel_task_new((k_task_entry_t)_proto_task, "protoCtx", pctxd, 9, 0, NULL, 2048, &(pctxd->thread));
	I4SC_PRT("protoCtx OK!\r\n");
	return 0;
	
EXIT:
	I4SC_PRT("protoCtx quit!\r\n");
	/*close socket*/
	if(pctxd->cmd_sockfd){
		close(pctxd->cmd_sockfd);
		pctxd->cmd_sockfd = 0;
	}
	if(pctxd->pic_sockfd){
		close(pctxd->pic_sockfd);
		pctxd->pic_sockfd = 0;
	}

	return -1;
}

static int i4s_sendto(int sockfd, const void *data, size_t size, uint8_t flags,
       const struct sockaddr *to, socklen_t tolen)
{
	if(flags == 0){
		return sendto(sockfd, data, size, 0, to, tolen);
	}
	struct cProBasic prohdr, *reshdr;
	int8_t trynum = 0;
    fd_set rdset;
    struct timeval tv = {0, 100000};
	int ret;
	uint8_t request[256] = {0};
	socklen_t addr_len;
	struct sockaddr_in cli_addr;

	os_memcpy(&prohdr, data, CPRO_HDR_SZIE);
	for(trynum = 0; trynum < 3; trynum++){
		if(sendto(sockfd, data, size, 0, to, tolen) != size){
			I4SC_PRT("i4s sendto failed\r\n");
			return -1;
		}
		FD_ZERO(&rdset);
		FD_SET(sockfd, &rdset);	
		tv.tv_sec = 0;
		tv.tv_usec = 100000; /*100ms*/
        ret = select(sockfd+1, &rdset, NULL, NULL, &tv);
		if(ret <= 0) {
			continue;
        }
		addr_len = sizeof(struct sockaddr);
		if(recvfrom(sockfd, request, 256, 0, 
				(struct sockaddr *)&cli_addr, &addr_len) <=0){
			continue;
		}
		reshdr = (struct cProBasic*)request;
		if(prohdr.cid == reshdr->cid && prohdr.seqNo == reshdr->seqNo){
			I4SC_PRT("i4s sendto ok\r\n");
			return size;	
		}
	}

	return 0;	
}

static uint8_t sensor_is_ok(void)
{
	return 1;
}
static int keepalive_notify(event_t *pctxd, uint8_t ack)
{
	uint8_t response[128] = {0}, tmac[6] = {0};	
	struct sockaddr_in addr;	
	struct cProBasic *prohdr = (struct cProBasic *)response;
	struct eventSetEar *evtset = (struct eventSetEar *)(response+CPRO_HDR_SZIE);
	uint8_t status;

	//event type version
	//evtset->type = EVT_TYPE_EAR;
	evtset->type = EVT_TYPE_EAR_V2;

	if(pctxd->wifi_connected == 0 || pctxd->have_addr == 0){
		I4SC_PRT("notify local[%d/%d]!\r\n", pctxd->wifi_connected, pctxd->have_addr);
		return 0;
	}
	//sensor status
	evtset->isSensorOk = sensor_is_ok();
	
	prohdr->cid = CPRO_EVENT_NOTIFY;
	prohdr->head = CPRO_BASIC_MAGIC;
	prohdr->len = sizeof(struct eventSetEar);
	prohdr->ackNeed = ack;
	prohdr->seqNo = lseqNo++;
	prohdr->reFlag = 0;

	int16_t batterinfo = i4s_handle_event(OP_GETBATTERYINFO);
	evtset->battery = (batterinfo&0x7F);
	evtset->isCharge = ((batterinfo&0x80) ?1:0);
	evtset->isLowPowerOff = ((batterinfo&0x100)?1:0);
	status = (uint8_t)i4s_handle_event(OP_GET_PIC_VIDEO_BUTTON);
	evtset->picbutton = ((status >> 4) & 0xf);
	evtset->videobutton = (status & 0xf);
	
	status = (uint8_t)i4s_handle_event(OP_GET_ZOOM_STA);
	evtset->zoomup = ((status >> 4) & 0xf);
	evtset->zoomdown = (status & 0xf);
	I4SC_PRT("P:%d V:%d U:%d D:%d\r\n", evtset->picbutton, evtset->videobutton,
			evtset->zoomup, evtset->zoomdown);
	_get_mac_from_cache(tmac);
	evtset->mac[0] = tmac[4];
	evtset->mac[1] = tmac[5];

	if(common_callback){		
		struct cProComCtr comCtr;
		uint16_t comLen = sizeof(struct cProComCtr);
		os_memset(&comCtr, 0, comLen);
		comCtr.item = CAMCAP_MODE;
		comCtr.type = OP_GET;
		common_callback((uint8_t *)&comCtr, &comLen);
		evtset->workMode = (comCtr.value&0xFF);
		I4SC_PRT("Work Mode K:%d\r\n", comCtr.value);

		//get motor on off status		
		os_memset(&comCtr, 0, comLen);
		comCtr.item = CAMCAP_MOTOR;
		comCtr.type = OP_GET;	
		comCtr.action = OP_BUTTON;
		common_callback((uint8_t *)&comCtr, &comLen);
		evtset->motorOnOff = (comCtr.value&0xFF);
		I4SC_PRT("Motor Status:%d\r\n", comCtr.value);	
	}

    addr.sin_family = AF_INET;
    addr.sin_port = htons(pctxd->port);
	addr.sin_addr = pctxd->addr;

	return i4s_sendto(pctxd->sockfd, (void *)response, CPRO_HDR_SZIE+prohdr->len,
		ack, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
}

static void _event_task(void* arg)
{
	event_t *pctxd = (event_t*)arg;
	int sleep_cnt = 0;

	if(pctxd == NULL){
		return;
	}

	while(WIFI_RUN_STATUS == TASK_RUN){
		/*notify peers*/
		if((sleep_cnt++) %100 == 0){
			keepalive_notify(pctxd, 0);
		}
		os_sleep_ms(10);
	}

	/*deinit resource*/
	close(pctxd->sockfd);
	I4SC_PRT("eventCtx Task Quit\r\n");
}

static void _event_uninit(event_t *pctxd)
{
	csi_kernel_task_del(pctxd->thread);
	os_memset(pctxd, 0, sizeof(event_t));
	I4SC_PRT("eventCtx Quit\r\n");
}

static int _event_init(event_t	*pctxd, uint16_t port)
{
	if(pctxd == NULL){
		return -1;
	}
	
	os_memset(pctxd, 0, sizeof(event_t));
	pctxd->port = port;
	/*init cmd socket*/
    pctxd->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (pctxd->sockfd  == -1) {
		I4SC_PRT("create event sock failed\r\n");
		return -2;
	}

	/*start thread*/
	//OS_TASK_INIT("eventCtx", &(pctxd->thread), _event_task, pctxd, OS_TASK_PRIORITY_NORMAL, 1024);
	csi_kernel_task_new((k_task_entry_t)_event_task, "eventCtx", pctxd, 9, 0, NULL, 1024, &(pctxd->thread));

	I4SC_PRT("eventCtx OK!\r\n");
	return 0;
}

void video_add_pkt_header(TV_HDR_PARAM_PTR param)
{
	struct hVideoUDP* elem_tvhdr = (struct hVideoUDP*)(param->ptk_ptr);
	uint8_t tmac[6] = {0};
	static uint8_t tseq = 0;

	elem_tvhdr->type = VIDEO_TYPE_V1;
	elem_tvhdr->frameId= param->frame_id;
	elem_tvhdr->isEof = param->is_eof;
	elem_tvhdr->frameCnt = param->frame_cnt;
	elem_tvhdr->seq = tseq++;
	//chengjiegai
	elem_tvhdr->gsensorSupport = !(i4s_get_capacity()&CAMCAP_GSENSOR);
	elem_tvhdr->gsensorData = gSensor;
	//elem_tvhdr->gsensorData = 17;
	elem_tvhdr->wide = 640;
	elem_tvhdr->high = 480;
	
	_get_mac_from_cache(tmac);
	elem_tvhdr->mac[0] = tmac[4];
	elem_tvhdr->mac[1] = tmac[5];
}

static void _video_uninit(video_t* pctxd)
{
	if(!pctxd){
		return;
	}

	csi_kernel_task_del(pctxd->thread);
	os_memset(pctxd, 0, sizeof(video_t));	
	I4SC_PRT("videoCtx Quit\r\n");
}

static void _video_task(void* arg)
{
	video_t *pctxd = (video_t*)arg;
	struct list_head* get_f;
	mjpeg_node* mjn;
	jpeg_frame* jpf;	
	uint32_t flen;
	uint8_t frame_id = 0;
	TV_HDR_PARAM_ST jpg_header;
	int send_byte = 0;
	
	if(pctxd == NULL){
		return;
	}
	
	while(WIFI_RUN_STATUS == TASK_RUN){
		if(pctxd->connections.status != VIDEO_START){
			os_sleep_ms(20);
			continue;
		}
		
		NVIC_DisableIRQ(JPG_IRQn);
		get_f = get_frame();
		NVIC_EnableIRQ(JPG_IRQn);
		
		if(get_f){
			usr_p = get_f;
			jpf = list_entry(get_f,jpeg_frame,list);
			flen = jpf->frame_len;
			os_memset(&jpg_header, 0, sizeof(jpg_header));
			jpg_header.frame_id = frame_id++;
			jpg_header.frame_cnt = flen/(JPG_BUF_LEN - JPG_HEAD_RESERVER);
			if(flen % (JPG_BUF_LEN - JPG_HEAD_RESERVER)){
				jpg_header.frame_cnt++;
			}
			
			while(list_empty((struct list_head *)usr_p) != TRUE){
				mjn = list_entry((struct list_head *)usr_p->next,mjpeg_node,list);
				/*
				此处使用当前mjn，获取节点对应的buf数据位置，对数据进行处理
				*/
				jpg_header.ptk_ptr = mjn->buf_addr-sizeof(struct hVideoUDP);
				if(flen > (JPG_BUF_LEN - JPG_HEAD_RESERVER)){
					jpg_header.ptklen = (JPG_BUF_LEN - JPG_HEAD_RESERVER);					
					jpg_header.is_eof = 0;
					flen -= (JPG_BUF_LEN - JPG_HEAD_RESERVER);
				}else{
					jpg_header.ptklen = flen;
					jpg_header.is_eof = 1;
				}
				video_add_pkt_header(&jpg_header);
				jpg_header.ptklen += sizeof(struct hVideoUDP);
				send_byte = sendto(pctxd->connections.sockfd, jpg_header.ptk_ptr, jpg_header.ptklen, MSG_DONTWAIT|MSG_MORE,
						(struct sockaddr *)&(pctxd->connections.addr), sizeof(struct sockaddr_in)); 
				if (send_byte <= 0) {
					/* err */
					//I4SC_PRT("udp send err:%d\r\n", send_byte);
				}
				
				put_node(&free_tab,usr_p);				//节点归还到free_tab,边处理边给下一帧释放空间
			}
			del_frame(usr_p);	
		}else{
			os_sleep_ms(10);
		}
	}

	I4SC_PRT("videoCtx Task Quit\r\n");
}

static int _video_init(video_t* pctxd)
{
	if(!pctxd){
		return -1;
	}

	os_memset(pctxd, 0, sizeof(video_t));
	os_mutex_init(&(pctxd->mutex));
	

	/*start protocol thread*/
	csi_kernel_task_new((k_task_entry_t)_video_task, "videoCtx", pctxd, 9, 0, NULL, 1024, &(pctxd->thread));


	I4SC_PRT("videoCtx OK!\r\n");
	return 0;
}

static void _oem_info_init(oem_info_t *oem_config)
{
	if(oem_config == NULL){
		return ;
	}
	if((oem_config->flags & OEMFLAG_AP) == 0){
		I4SC_PRT("OEM AP Default!\r\n");
		os_memset(&(oem_config->ap_info), 0, sizeof(oem_config->ap_info));
		os_memcpy(oem_config->ap_info.ap_prefix, OEMINFO_DEFAULT_AP_PREFIX, sizeof(oem_config->ap_info.ap_prefix));
		if(os_strlen(OEMINFO_DEFAULT_AP_PASSWORD)){
			os_memcpy(oem_config->ap_info.ap_pasword, OEMINFO_DEFAULT_AP_PASSWORD, sizeof(oem_config->ap_info.ap_pasword));
		}
		oem_config->ap_info.ap_channel = OEMINFO_DEFAULT_AP_CAHNNEL;
		oem_config->flags |= OEMFLAG_AP;
	}

	if((oem_config->flags & OEMFLAG_NET) == 0){
		I4SC_PRT("OEM NET Default!\r\n");
		os_memset(&(oem_config->net_info), 0, sizeof(oem_config->net_info));
		os_memcpy(oem_config->net_info.net_ip, OEMINFO_DEFAULT_NET_IP, sizeof(oem_config->net_info.net_ip));
		os_memcpy(oem_config->net_info.net_mask, OEMINFO_DEFAULT_NET_MASK, sizeof(oem_config->net_info.net_mask));
		os_memcpy(oem_config->net_info.net_gateway, OEMINFO_DEFAULT_NET_GW, sizeof(oem_config->net_info.net_gateway));
		oem_config->flags |= OEMFLAG_NET;
	}
	
	if((oem_config->flags & OEMFLAG_OEM) == 0){
		I4SC_PRT("OEM Firmware Default!\r\n");
		os_memset(&(oem_config->base_info), 0, sizeof(oem_config->base_info));
		os_memcpy(oem_config->base_info.vendor, OEMINFO_DEFAULT_BASE_VENDOR, sizeof(oem_config->base_info.vendor));
		os_memcpy(oem_config->base_info.product, OEMINFO_DEFAULT_BASE_PRODUCT, sizeof(oem_config->base_info.product));	
		os_memcpy(oem_config->base_info.version, OEMINFO_DEFAULT_BASE_VERSION, sizeof(oem_config->base_info.version));
		oem_config->base_info.port = OEMINFO_DEFAULT_BASE_PORT;
		oem_config->base_info.capacity = 0;
		oem_config->flags |= OEMFLAG_OEM;
	}	
}

static int _config_init(config_t *config, void *cfg)
{
	struct sys_config *syscfg = (struct sys_config *)cfg;
	uint8_t default_mac[6] = {0x00, 0x1c, 0xc2, 0x12,0x34,0x56};

	I4SC_WARN("capacity:0x%x\r\n", config->capacity);

	if(IS_ZERO_ADDR(syscfg->mac)) {		
        os_random_bytes(default_mac+3, 3);
		os_memcpy(syscfg->mac, default_mac, 6);	
	}
	
	os_memcpy(config->mac, syscfg->mac, 6);
	I4SC_WARN("mac:%02x:%02x:%02x:%02x:%02x:%02x\r\n", 
			config->mac[0], config->mac[1], config->mac[2],
			config->mac[3], config->mac[4], config->mac[5]);	
	if(os_memcmp(syscfg->ssid, config->oem.ap_info.ap_prefix, os_strlen(config->oem.ap_info.ap_prefix))){
		I4SC_WARN("ap mismatch:%s\r\n", syscfg->ssid);
    	os_sprintf(syscfg->ssid, "%s%02x%02x", 
    		config->oem.ap_info.ap_prefix, config->mac[4], config->mac[5]);
	}
	if(os_memcmp(syscfg->key, config->oem.ap_info.ap_pasword, sizeof(syscfg->key))){
		I4SC_WARN("password mismatch:%s\r\n", syscfg->key);
		os_memset(syscfg->key, 0, sizeof(syscfg->key));
		if(os_strlen(config->oem.ap_info.ap_pasword)){
			os_memcpy(syscfg->key, config->oem.ap_info.ap_pasword, os_strlen(config->oem.ap_info.ap_pasword));
		}
	}
	if(syscfg->channel != config->oem.ap_info.ap_channel){
		I4SC_WARN("channel mismatch:%s\r\n", syscfg->channel);
		syscfg->channel = config->oem.ap_info.ap_channel;
	}

	return 0;
}

/***********************************************************************************/
/*				             CAMERA PUBLIC FUNCTION						  		   */
/*																				   */
/***********************************************************************************/

void i4s_set_oem_info(void *oem, uint8_t flag)
{
	if(flag == OEMFLAG_AP){
		os_memcpy(&(wifi_camera.config.oem.ap_info), oem, sizeof(oeminfo_ap_t));
		wifi_camera.config.oem.flags |= OEMFLAG_AP;
	}else if(flag == OEMFLAG_NET){
		os_memcpy(&(wifi_camera.config.oem.net_info), oem, sizeof(oeminfo_net_t));	
		wifi_camera.config.oem.flags |= OEMFLAG_NET;
	}else if(flag == OEMFLAG_OEM){
		os_memcpy(&(wifi_camera.config.oem.base_info), oem, sizeof(oeminfo_base_t));		
		wifi_camera.config.oem.flags |= OEMFLAG_OEM;
	}
}

int i4s_get_oem_info(void *oem, int len, uint8_t flag)
{
	void *pay = NULL;
	int payLen = 0;
	if(flag&OEMFLAG_AP){
		payLen = sizeof(oeminfo_ap_t);
		pay = &(wifi_camera.config.oem.ap_info);
	}else if(flag&OEMFLAG_NET){
		payLen = sizeof(oeminfo_net_t);
		pay = &(wifi_camera.config.oem.net_info);
	}else if(flag == OEMFLAG_OEM){
		payLen = sizeof(oeminfo_base_t);
		pay = &(wifi_camera.config.oem.base_info);
	}
	if(payLen > len){
		I4SC_PRT("OEM Buffer Len Wrong:%d/%d!\r\n", payLen, len);
		return -1;
	}
	os_memcpy(oem, pay, payLen);

	return 0;
}

int i4s_pre_init(void *cfg)
{
	//oem init
	_oem_info_init(&(wifi_camera.config.oem));
	//init oem information
	_config_init(&(wifi_camera.config), cfg);	
	return 0;
}

int i4s_camera_init(void)
{

   I4SC_WARN("chengjie+++++++++++++++++++++++++%s %s\r\n", __DATE__, __TIME__);
	I4SC_WARN("Camera Libv2:%s %s\r\n", __DATE__, __TIME__);
	if(wifi_camera.status == TASK_RUN){
		I4SC_WARN("Camera Libv2:%s %s inited\r\n", __DATE__, __TIME__);
		return 0;
	}
	//set task run status
	wifi_camera.status = TASK_RUN;
	//init protocol context
	if(_proto_init(&(wifi_camera.protocol), wifi_camera.config.oem.base_info.port) != 0){
		I4SC_PRT("protoCtx init failed\r\n");
		goto cQuit;
	}
	//init event context
	if(_event_init(&(wifi_camera.events), wifi_camera.config.oem.base_info.port + 2) != 0){
		I4SC_PRT("eventCtx init failed\r\n");
		goto cQuit;
	}
	//init video context
	if(_video_init(&(wifi_camera.video)) != 0){
		I4SC_PRT("videoPart init failed\r\n");
		goto cQuit;
	}	
	I4SC_PRT("i4 OK\r\n");
	return 0;
	
cQuit:
	_proto_uninit(&(wifi_camera.protocol));
	_event_uninit(&(wifi_camera.events));
	_video_uninit(&(wifi_camera.video));
	wifi_camera.status = 0;
	
	return -1;
}

void i4s_camera_uninit(void)
{
	//set task run status
	wifi_camera.status = TASK_STOP;
	//stop protocol context
	_proto_uninit(&(wifi_camera.protocol));
	//stop event context
	_event_uninit(&(wifi_camera.events));
	//stop video context
	_video_uninit(&(wifi_camera.video));
}

int i4s_handle_event(int event)
{
	if(event == OP_WIFI_DISCONECTED){	
		//stop send to peer
		app_deal_close_video();
		//set event flag
		wifi_camera.events.wifi_connected = 0;
		wifi_camera.events.have_addr = 0;
	}else if(event == OP_WIFI_CONECTED){
		//set event flag
		wifi_camera.events.wifi_connected = 1;
	}
	if(op_callback){
		return op_callback(event);
	}
	//I4SC_PRT("1112233");
	//I4SC_PRT("change the battey-=-=-=-=-=-=%d",battey);
	return battey;
}

void i4s_handle_event_charge(int charge)
{
	I4SC_PRT("Charge Status:%d\r\n", charge);
}

/* void i4s_update_gensor_data(uint32_t value)
{
	gSensor = value;
} */

//callback register
void i4s_op_callback_register(op_cb callback)
{
	op_callback = callback;
}

void i4s_led_callback_register(int (*callback)(struct cProLed*))
{
	led_callback = callback;
}

void i4s_common_callback_register(int (*callback)(uint8_t *, uint16_t *))
{
	common_callback = callback;
}

void i4s_set_capacity(uint8_t wCap)
{
	if(wCap == CAMCAP_GSENSOR){
		wifi_camera.config.capacity.cap_bits.cap_gsensor = 1;
		I4SC_PRT("[SYS]Capacity GSENSOR\r\n");
	}
	wifi_camera.config.capacity.capacity |= wCap;
	I4SC_PRT("[SYS]Capacity:0x%x\r\n", wifi_camera.config.capacity.capacity);
}

uint8_t i4s_get_capacity(void)
{
	return wifi_camera.config.capacity.capacity;
}

void i4s_auto_generate_license(uint8_t autoGen)
{
	wifi_camera.config.license.lic_auto_gen = autoGen;
	I4SC_PRT("[SYS]License Generate:%d\r\n", wifi_camera.config.license.lic_auto_gen);
}

void set_battey_fun(float battey1)
{
	
	battey=battey1;
}

void i4sGensorDataUpdate(uint32_t value)
{
  gSensor = value;
 // I4SC_PRT("i4sGensorDataUpdate 111");
}