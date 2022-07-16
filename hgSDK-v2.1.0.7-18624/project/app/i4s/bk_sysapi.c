
#include "i4s.h"
/*user include*/
#include "camera_pro.h"

#include "i4md5.h"

#define I4S_TLV_HEADER           (0x00564c54)   // ASCII TLV
#define QUEUE_ITEM_COUNT           4

enum{
	BK_APS_STOP = 0,
	BK_APS_WIFI_DISCONECTED, 
	BK_APS_WIFI_CONECTED,
	BK_APS_APP_DISCONECTED,
	BK_APS_APP_CONECTED,
	BK_APS_QUIT,
};

typedef enum{
	TLV_LICENSE_ITEM	= 0x10000001,
	TLV_WIFI_ITEM		= 0x20000002,
	TLV_NET_ITEM		= 0x30000003,
}BKSOC_TLV_USER;

typedef struct bkSoc_TLV_HEAD_t{
	UINT32 type;
	UINT32 len;
}__attribute__((__packed__)) bkSoc_TLV_HEAD, bkSoc_TLV_ITEM; //must use packed not align

struct firmwareHeader{
	int32_t magic;  /*Firmware Magic*/
	uint8_t md5[32];/*firmware md5*/
	int32_t size;   /*firmware size*/
}__attribute__((__packed__));

typedef struct bkSoc_UpgradeCtx_t{
	UINT8 have_init;
	UINT8 *wr_buf;
	UINT16 wr_last_len ;
	UINT32 flash_address;
	UINT32 flash_len;
	UINT8 *wr_tmp_buf;
	//DD_HANDLE flash_hdl;
	
	uint16_t seqno_init;
	uint16_t seqno_data;
	struct firmwareHeader fw_header;
	int32_t rcv_size;
	uint8_t upgrade_ok;
}bkSoc_UpgradeCtx;

typedef struct AMessage{
	UINT8 ucMessageID;
} xMessage;

typedef struct bkSocCtx_t{
	volatile u32 status;
	xTaskHandle thread;
	beken_queue_t queue;
}bkSocCtx;

typedef struct bkSocWDG_t{
	volatile UINT8 status;
	beken_timer_t wdg_timer;
	UINT32 period;
}bkSocWDG;

bkSocCtx bk7231u = {BK_APS_STOP, NULL, NULL};
struct cProWifi cacheWiFi;
bkSoc_UpgradeCtx bkUpgrade = {.have_init = 0};
bkSocWDG bkWDG = {.status = 0};


extern void flash_protection_op(UINT8 mode,PROTECT_TYPE type);
static void queue_sndmsg(beken_queue_t *queue, UINT8 msg)
{
	xMessage xmsg;
	int ret;
	if(queue == NULL){
		return;
	}
	xmsg.ucMessageID = msg;
	
	ret = rtos_push_to_queue(queue, &xmsg, BEKEN_NO_WAIT);
	if(kNoErr != ret){
		I4SC_PRT("sndmsg failed\r\n");
	}
}

static int bksoc_rw_event_func(enum rw_evt_type evt_type, void *data)
{
	struct rw_evt_payload *evt_payload = (struct rw_evt_payload *)data;

	if(evt_type == RW_EVT_AP_CONNECTED){
		I4SC_WARN("RW_EVT_AP_CONNECTED-(mac="MACSTR")\r\n",  MAC2STR(evt_payload->mac));
		if(bk7231u.status != BK_APS_STOP){
			queue_sndmsg(&(bk7231u.queue), BK_APS_WIFI_CONECTED);
		}
	}else if(evt_type == RW_EVT_AP_DISCONNECTED){
		I4SC_WARN("RW_EVT_AP_DISCONNECTED-(mac="MACSTR")\r\n",  MAC2STR(evt_payload->mac));
		if(bk7231u.status != BK_APS_STOP){
			queue_sndmsg(&(bk7231u.queue), BK_APS_WIFI_DISCONECTED);
		}
	}

	return 0;
}

static UINT32 bksoc_info_item_len(BKSOC_TLV_USER item, UINT8 include_header)
{
	UINT32 len = 0;
	switch(item){
		case TLV_LICENSE_ITEM:
			len = sizeof(struct cProLic);
			break;
		case TLV_WIFI_ITEM:
			len = sizeof(struct cProWifi);
			break;
		case TLV_NET_ITEM:
			len = sizeof(oemInfo_Net);
			break;
		default:
			break;
	}
	if(len == 0){
		return 0;
	}
	if(include_header){
		len += sizeof(bkSoc_TLV_ITEM);
	}
	return len;
}

static UINT32 bksoc_search_info_tbl(bk_partition_t partition, UINT8 *buf, UINT32 *cfg_len)
{
	UINT32 ret = 0, status;
	DD_HANDLE flash_handle;
	bkSoc_TLV_HEAD head;

	bk_logic_partition_t *pt = bk_flash_get_info(partition);

	*cfg_len = 0;
	flash_handle = ddev_open(FLASH_DEV_NAME, &status, 0);
	ddev_read(flash_handle, (char *)&head, sizeof(bkSoc_TLV_HEAD), pt->partition_start_addr);
	if(I4S_TLV_HEADER == head.type){
		*cfg_len = head.len + sizeof(bkSoc_TLV_HEAD);
		ret = 1;
		if(buf != NULL){
			ddev_read(flash_handle, buf, *cfg_len, pt->partition_start_addr);
		}
		//I4SC_WARN("Found TLV Header:%X Length:%d, Total:%d\r\n", head.type, head.len, *cfg_len);
	}
	
	ddev_close(flash_handle);
	return ret;
}

static UINT32 bksoc_search_info_item(BKSOC_TLV_USER type, UINT32 start_addr)
{
	UINT32 status, addr, end_addr;
	DD_HANDLE flash_handle;
	bkSoc_TLV_HEAD head;

	flash_handle = ddev_open(FLASH_DEV_NAME, &status, 0);
	ddev_read(flash_handle, (char *)&head, sizeof(bkSoc_TLV_HEAD), start_addr);
	addr = start_addr + sizeof(bkSoc_TLV_HEAD);
	end_addr = addr + head.len;
	if(I4S_TLV_HEADER != head.type){
//		I4SC_WARN("No TLV Header\r\n");
		return 0;
	}
//	I4SC_WARN("Search: addr=%X end_addr=%X type=%x len=%d\r\n", addr, end_addr, type, head.len);
	while(addr < end_addr){
	    ddev_read(flash_handle, (char *)&head, sizeof(bkSoc_TLV_ITEM), addr);
//		I4SC_WARN("Searching: addr=%X type=%x len=%d\r\n", addr, head.type, head.len);
	    if(type != head.type){
		    addr += sizeof(bkSoc_TLV_ITEM);
	        addr += head.len;
	    }else{
			break;
	    }
	}

	if(addr >= end_addr){
		addr = 0;
	}
	ddev_close(flash_handle);
	return addr;
}

UINT32 bksoc_save_info_item(BKSOC_TLV_USER item, UINT8 *ptr)
{
	UINT32 status, addr_offset,cfg_tbl_len,item_len,tmp;
	UINT8 *tmpptr;
	UINT8 *item_buf;
	UINT32 *wrbuf;
    bkSoc_TLV_HEAD head;
	bkSoc_TLV_ITEM *item_head_ptr;

	bk_logic_partition_t *pt = bk_flash_get_info(BK_PARTITION_USR_CONFIG);

	item_len = bksoc_info_item_len(item, 1);
	if(item_len == 0){
		//I4SC_WARN("TLV length error\r\n");
		return 0;
	}
	head.type = I4S_TLV_HEADER;
	
    if(!bksoc_search_info_tbl(BK_PARTITION_USR_CONFIG, NULL,&cfg_tbl_len)){//no TLV
		cfg_tbl_len = sizeof(bkSoc_TLV_HEAD)+item_len;
		addr_offset = sizeof(bkSoc_TLV_ITEM);
		head.len = item_len;
		wrbuf = os_zalloc(cfg_tbl_len);
		if(wrbuf == NULL){
			return 0;
		}	
    }else{
		addr_offset = bksoc_search_info_item(item, pt->partition_start_addr);	
		if(!addr_offset){//no item
			addr_offset = cfg_tbl_len;
			cfg_tbl_len += item_len;
		}else{
			addr_offset -= pt->partition_start_addr;
		}
		wrbuf = os_zalloc(cfg_tbl_len);
		if(wrbuf == NULL){
			return 0;
		}
		bksoc_search_info_tbl(BK_PARTITION_USR_CONFIG, (UINT8*)wrbuf, &tmp);
		head.len = cfg_tbl_len - sizeof(bkSoc_TLV_HEAD);
	}

	tmpptr = (UINT8*)wrbuf;
	item_head_ptr = (bkSoc_TLV_ITEM*)(tmpptr + addr_offset);
	item_buf =(UINT8*)(tmpptr + addr_offset + sizeof(bkSoc_TLV_ITEM));
	os_memcpy(item_buf, ptr, bksoc_info_item_len(item, 0));

	item_head_ptr->type = item;
	item_head_ptr->len = item_len- sizeof(bkSoc_TLV_ITEM);

	//set TLV header
	os_memcpy(tmpptr,&head,sizeof(bkSoc_TLV_HEAD));
	//assume info cfg tbl size is less than 4k

	bk_flash_enable_security(FLASH_PROTECT_NONE);
	bk_flash_erase(BK_PARTITION_USR_CONFIG,0,cfg_tbl_len);
	bk_flash_write(BK_PARTITION_USR_CONFIG,0,tmpptr,cfg_tbl_len);
	bk_flash_enable_security(FLASH_UNPROTECT_LAST_BLOCK);

	os_free(wrbuf);

	return 1;
}

UINT32 bksoc_get_info_item(BKSOC_TLV_USER item, UINT8 *ptr)
{
	UINT32 status, addr_offset,cfg_tbl_len,item_len,tmp;
	DD_HANDLE flash_handle;
	bkSoc_TLV_HEAD head;

	bk_logic_partition_t *pt = bk_flash_get_info(BK_PARTITION_USR_CONFIG);

	item_len = bksoc_info_item_len(item, 1);
	if(item_len == 0){
		//I4SC_WARN("TLV length error\r\n");
		return 0;
	}
	addr_offset = bksoc_search_info_item(item, pt->partition_start_addr);	
	if(!addr_offset){//no item
		//I4SC_WARN("TLV Head no found[0x%x]\r\n", item);
		return 0;
	}
	flash_handle = ddev_open(FLASH_DEV_NAME, &status, 0);
	ddev_read(flash_handle, (char *)&head, sizeof(bkSoc_TLV_HEAD), addr_offset);
	addr_offset += sizeof(bkSoc_TLV_HEAD);
	ddev_read(flash_handle, (char *)ptr, head.len, addr_offset);
	
	ddev_close(flash_handle);
	return 1;
}

static int getMAC(UINT8 *mac) /*mac length is 6*/
{
	if(mac == NULL){
		return -1;
	}
	if(manual_cal_get_macaddr_from_flash(mac) > 0){
		I4SC_WARN("TLV Head Read Mac Successful:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
				mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		return 0;
	}
	//read 
	DD_HANDLE flash_handle;
	UINT32 status;
	
	bk_logic_partition_t *pt = bk_flash_get_info(BK_PARTITION_RF_FIRMWARE);
	flash_handle = ddev_open(FLASH_DEV_NAME, &status, 0);
	ddev_read(flash_handle, (char *)mac, 6, pt->partition_start_addr);
	
	ddev_close(flash_handle);
	I4SC_WARN("Flah Read Mac Successful:%02x:%02x:%02x:%02x:%02x:%02x\r\n", 
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return 0;
}

static int networkParameterGet(general_param_t *pNetInfo, ap_param_t *pAPInfo)
{
	if(pNetInfo == NULL || pAPInfo == NULL){
		return -1;
	}

    os_memset(pNetInfo, 0x0, sizeof(general_param_t));	
    os_memset(pAPInfo, 0x0, sizeof(ap_param_t));

	oemInfo_AP oemap;
	oemInfo_Net oemnet;

	struct cProWifi fWifi;
    os_memset(&fWifi, 0x0, sizeof(struct cProWifi));
	UINT8 mac[6] = {0};
	getMAC(mac);
	if(bksoc_get_info_item(TLV_WIFI_ITEM, (UINT8*)(&fWifi)) == 0){
		if(i4sCamera_getOEM(&(oemap), sizeof(oemInfo_AP), OEMFLAG_AP) < 0){
			return -1;
		}
		os_snprintf(fWifi.SSID, sizeof(fWifi.SSID)-1, "%s%02X%02X", oemap.apPrefix, mac[4], mac[5]);
		if(os_strlen(oemap.apPasword)){
			os_memcpy(fWifi.PASSWD, oemap.apPasword, sizeof(fWifi.PASSWD));
		}	
		fWifi.channel = oemap.apChan;
	}

	if(bksoc_get_info_item(TLV_NET_ITEM, (UINT8*)(&oemnet)) > 0){
		I4SC_WARN("network information read flash ok\r\n");
	}else if(i4sCamera_getOEM(&(oemnet), sizeof(oemnet), OEMFLAG_NET) < 0){
		return -1;
	}
	pNetInfo->role = SOFT_AP;
	pNetInfo->dhcp_enable = 1;
	pNetInfo->ip_addr = inet_addr(oemnet.netIP);
	pNetInfo->ip_mask = inet_addr(oemnet.netMask);
	pNetInfo->ip_gw = inet_addr(oemnet.netGW);
	//init ap struct
	os_memcpy(pAPInfo->bssid.array, mac, sizeof(pAPInfo->bssid.array));
	os_memcpy(pAPInfo->ssid.array, fWifi.SSID, sizeof(pAPInfo->ssid.array));
	pAPInfo->ssid.length = os_strlen(fWifi.SSID);
	pAPInfo->chann = fWifi.channel;
	pAPInfo->key_len = os_strlen(fWifi.PASSWD);
	if(pAPInfo->key_len){
		os_memcpy(pAPInfo->key, fWifi.PASSWD, sizeof(pAPInfo->key));
	}
	
	return 0;
}

void bkSoc_networkInit(void)
{
	network_InitTypeDef_st wNetConfig;
	general_param_t pNetInfo;
	ap_param_t pAPinfo;
	
    os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));  
	if(networkParameterGet(&pNetInfo, &pAPinfo) < 0){
		I4SC_WARN("Network Information Failed\r\n");
		return;
	}
	os_strcpy((char *)wNetConfig.local_ip_addr, inet_ntoa(pNetInfo.ip_addr));
	os_strcpy((char *)wNetConfig.net_mask, inet_ntoa(pNetInfo.ip_mask));
	os_strcpy((char *)wNetConfig.gateway_ip_addr, inet_ntoa(pNetInfo.ip_gw));
	os_strcpy((char *)wNetConfig.dns_server_ip_addr, inet_ntoa(pNetInfo.ip_gw));

    os_strcpy((char *)wNetConfig.wifi_ssid, pAPinfo.ssid.array);
	if(pAPinfo.key_len){
    	os_strcpy((char *)wNetConfig.wifi_key, pAPinfo.key);	
	}
    
    wNetConfig.wifi_mode = SOFT_AP;
    wNetConfig.dhcp_mode = DHCP_SERVER;
    wNetConfig.wifi_retry_interval = 100;

	//set wifi channel	
    bk_wlan_ap_set_default_channel(pAPinfo.chann);
	I4SC_WARN("set ip info: %s,%s,%s\r\n",
				wNetConfig.local_ip_addr,
				wNetConfig.net_mask,
				wNetConfig.dns_server_ip_addr);
	I4SC_WARN("ssid:%s  key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);	
	//start wifi driver
	bk_wlan_start(&wNetConfig);
	os_memset(&cacheWiFi, 0, sizeof(cacheWiFi));
	os_memcpy(cacheWiFi.SSID, wNetConfig.wifi_ssid, sizeof(cacheWiFi.SSID));
	if(os_strlen(wNetConfig.wifi_key)){
		os_memcpy(cacheWiFi.PASSWD, wNetConfig.wifi_key, sizeof(cacheWiFi.PASSWD));
	}
	cacheWiFi.channel = pAPinfo.chann;
}

static void bkSoc_networkUnnit(void)
{
	I4SC_WARN("stop wlan driver\r\n");
	extern int hostapd_main_disconnect_ap(void);
	hostapd_main_disconnect_ap();
	bk_wlan_stop(SOFT_AP);	
	os_memset(&cacheWiFi, 0, sizeof(cacheWiFi));
}

static void bkSocCtx_task(beken_thread_arg_t arg)
{
	OSStatus err;
	int ret = 0;

	//set app wifi status
	bk7231u.status = BK_APS_WIFI_DISCONECTED;
	//set app wifi status
    rw_evt_set_callback(RW_EVT_AP_CONNECTED, bksoc_rw_event_func);
    rw_evt_set_callback(RW_EVT_AP_DISCONNECTED, bksoc_rw_event_func);

	//wifi configuration
	bkSoc_networkInit();
	while(i4sCamera_init() == -1){
	   I4SC_WARN("Try Aagin!\r\n");
	   rtos_delay_milliseconds(20);
	}

	xMessage xmsg;
	while(1){
		err = rtos_pop_from_queue(&(bk7231u.queue), &xmsg, BEKEN_WAIT_FOREVER);
	 	if(kNoErr == err){
			switch(xmsg.ucMessageID){
			 case BK_APS_WIFI_DISCONECTED:
				bk7231u.status = BK_APS_WIFI_DISCONECTED;
				i4sAppEventAction(OP_WIFI_DISCONECTED);
				break;
			 case BK_APS_WIFI_CONECTED:
				I4SC_WARN("Wi-Fi Connected Wait DHCP!\r\n");
				bk7231u.status = BK_APS_WIFI_CONECTED;
				//i4sAppEventAction(OP_WIFI_CONECTED);
				break;
			 case BK_APS_APP_CONECTED:
				 I4SC_WARN("APP Connected!\r\n");
				 bk7231u.status = BK_APS_APP_CONECTED; 
				 break;
			 case BK_APS_APP_DISCONECTED:
				 I4SC_WARN("APP Disconnected!\r\n");
				 bk7231u.status = BK_APS_APP_DISCONECTED;
				 break;
			 case BK_APS_QUIT:
				 goto kQuit;
				 break;
			 default:
				 break;
			}
	 } 

	}
	
kQuit:
	/*unint camera context*/
	i4sCamera_uninit();
	/*turn off wifi driver*/
	bkSoc_networkUnnit();
	bk7231u.thread = NULL;
	rtos_delete_thread(NULL);
}

void bkSocCtx_init(void)
{
	int ret;
	I4SC_WARN("BK7231U Start:%s %s\r\n", __DATE__, __TIME__);
	i4sCamera_initOEM();
	if(bk7231u.status != BK_APS_STOP){
		I4SC_WARN("BK7231U Status wrong:%d\r\n", bk7231u.status);
		bkSocCtx_uninit();
	}
	os_memset(&bk7231u, 0, sizeof(bkSocCtx));
	ret = rtos_init_queue(&(bk7231u.queue), 
					"bkSocCtxqueue",
						sizeof(xMessage), QUEUE_ITEM_COUNT);
	if (kNoErr != ret){
		I4SC_WARN("BK7231U create queue failed\r\n");
		return;
	}		
	ret = rtos_create_thread(&(bk7231u.thread),
				BEKEN_DEFAULT_WORKER_PRIORITY,
				"bkSocCtx_init",
				(beken_thread_function_t)bkSocCtx_task,
				2048, (beken_thread_arg_t)NULL);
	if(ret != kNoErr){
		I4SC_WARN("BK7231U create thread failed\r\n");
		rtos_deinit_queue(bk7231u.queue);
		return;
	}
}

void bkSocCtx_uninit(void)
{
	int wait_time = 0;
	if(bk7231u.status == BK_APS_STOP){
		I4SC_WARN("BK7231U have unnit\r\n");
		return;
	}
	bk7231u.status = BK_APS_STOP;
	/*send quit msg to queue*/
	if(bk7231u.queue){
		queue_sndmsg(&(bk7231u.queue), BK_APS_QUIT);
	}
    while(bk7231u.thread){
		rtos_delay_milliseconds(10);
		if(wait_time++ >= 200){
			I4SC_PRT("bkSocCtx_uninit timeout\r\n");
			break;
		}
	}	
	rtos_deinit_queue(bk7231u.queue);
	os_memset(&(bk7231u), 0, sizeof(bkSocCtx));	
	I4SC_PRT("BK7231U Quit\r\n");
}

int bkapi_get_apinfo(struct cProWifi *wifi)
{
	if(wifi == NULL){
		return -1;
	}
	os_memcpy(wifi, &(cacheWiFi), sizeof(struct cProWifi));
	return 0;
}

int bkapi_set_apinfo(struct cProWifi *wifi)
{
	if(wifi == NULL){
		return -1;
	}
	
	I4SC_PRT("WiFi Information Change:%s[%s]-->%s[%s]\r\n", cacheWiFi.SSID, cacheWiFi.PASSWD, wifi->SSID, wifi->PASSWD);
	os_memcpy(&(cacheWiFi), wifi, sizeof(struct cProWifi));
	/*save to flash*/
	bksoc_save_info_item(TLV_WIFI_ITEM, (UINT8*)&cacheWiFi);
	return 0;
}

int bkapi_set_license(struct cProLic *lic)
{
	UINT32 ret = 0;
	if(lic == NULL){
		return -1;
	}
	//save to flash
	ret = bksoc_save_info_item(TLV_LICENSE_ITEM, (UINT8*)lic);
	return (ret == 0?-1:0);
}

int bkapi_get_license(struct cProLic *lic)
{
	UINT32 ret = 0;
	if(lic == NULL){
		return -1;
	}
	//get from flash
	ret = bksoc_get_info_item(TLV_LICENSE_ITEM, (UINT8*)lic);
	return (ret == 0?-1:0);
}

void bkapi_system_reboot(void)
{
	UINT32 wdt_val = 1;
	os_printf("wdt reboot\r\n");
    sddev_control(WDT_DEV_NAME, WCMD_SET_PERIOD, &wdt_val);
    sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, NULL);
}

int bkapi_upgrade_init(uint16_t seq_num)
{
	UINT32 status;

	if(bkUpgrade.have_init == 1){
		os_printf("upgrade have init!!! something wrong\r\n");
		if(bkUpgrade.seqno_init != seq_num){
			os_printf("new seqno:%d/%d upgrade init\r\n", bkUpgrade.seqno_init, seq_num);
			bkapi_upgrade_end();
		}
	}
	bkUpgrade.wr_buf = NULL;
	bkUpgrade.wr_tmp_buf = NULL;

	if(!bkUpgrade.wr_buf){
		bkUpgrade.wr_buf = os_malloc(FLASH_WR_BUF_MAX * sizeof(char));
		if(!bkUpgrade.wr_buf){
			os_printf("wr_buf malloc err\r\n");
			return -1;
		}
	}

	if(!bkUpgrade.wr_tmp_buf){
		bkUpgrade.wr_tmp_buf = os_malloc(FLASH_WR_BUF_MAX * sizeof(char));
		if(!bkUpgrade.wr_tmp_buf){
			os_printf("wr_tmp_buf malloc err\r\n");			
			os_free(bkUpgrade.wr_buf);
			return -1;
		}
	}

	bkUpgrade.flash_hdl = ddev_open(FLASH_DEV_NAME, &status, 0);
	if(DD_HANDLE_UNVALID == bkUpgrade.flash_hdl){		
		os_printf("flash open err\r\n");
		os_free(bkUpgrade.wr_buf);		
		os_free(bkUpgrade.wr_tmp_buf);
		return -1;
	}
	bkUpgrade.wr_last_len = 0;
	bk_logic_partition_t *pt = bk_flash_get_info(BK_PARTITION_OTA_DL_PART);  
	if(pt == NULL){
		os_printf("No OTA_DL_PART in flash\r\n");
		os_free(bkUpgrade.wr_buf);		
		os_free(bkUpgrade.wr_tmp_buf);
		ddev_close(bkUpgrade.flash_hdl);
		return -1;
	}else{
		bkUpgrade.flash_address = pt->partition_start_addr;
		bkUpgrade.flash_len = pt->partition_length;
	}

	flash_protection_op(FLASH_XTX_16M_SR_WRITE_ENABLE, FLASH_PROTECT_NONE);
	bkUpgrade.seqno_init = seq_num;
	bkUpgrade.seqno_data = 0;
	bkUpgrade.upgrade_ok = 0;
	bkUpgrade.have_init = 1;
	
	return 0;
}

int bkapi_upgrade_data(uint16_t seq_num, char *page, UINT32 len)
{
	struct firmwareHeader *fhead;
	char *pay; 
	UINT32 paylen;
	char md5str[33] = {0};

	if(!page || !len){
		return 0;
	}
	if(bkUpgrade.have_init == 0){
		return -1;
	}
	fhead = &(bkUpgrade.fw_header);
	pay = page;
	paylen = len;
	
	if(seq_num == (bkUpgrade.seqno_init+1)){
		os_printf("first upgrade data:%d\r\n", seq_num);
		bkUpgrade.seqno_data = seq_num;
		//init firmware struct
		if(len < sizeof(struct firmwareHeader)){
			os_printf("upgrade head wrong:%d\r\n", len);
			return -2;
		}
		os_memcpy(fhead, page, sizeof(struct firmwareHeader));
		if(fhead->magic != 0x22334455){
			os_printf("firmare invaild[0x%x]\r\n", fhead->magic);
			return -3;
		}
		pay += sizeof(struct firmwareHeader);
		paylen -= sizeof(struct firmwareHeader);
	}else if(seq_num != (bkUpgrade.seqno_data+1)){
		//return successl just send ack to peer  may be disorder
		os_printf("data seq wrong:%d/%d\r\n", seq_num, bkUpgrade.seqno_data);
		return 0;
	}
	if(paylen > FLASH_WR_BUF_MAX){
		os_printf("data too big:%d\r\n", len);
		return -4;
	}
	
	if(bkUpgrade.rcv_size + paylen == fhead->size){
		os_printf("firmware last part receive\r\n");
		if(bkapi_upgrade_data_wrbuffer(pay, paylen) < 0 ||
				bkapi_upgrade_data_sync() < 0){
			os_printf("firmware last write failed\r\n");
			return -5;
		}	
		os_printf("firmware wirte finish  check it\r\n");
		if(bkapi_upgrade_data_check(fhead->size, md5str) < 0){
			os_printf("firmware md5 calute failed\r\n");
			return -6;
		}
		if(os_memcmp(fhead->md5, md5str, sizeof(fhead->md5)) != 0){
			os_printf("MD5 Not Same:[%s/%s]\r\n", fhead->md5, md5str);
			return -7;
		}else{
			os_printf("MD5 OK:[%s]\r\n", md5str);
			bkUpgrade.upgrade_ok = 1;
		}
		return 0;
	}else if(bkUpgrade.rcv_size + paylen > fhead->size){
		os_printf("firmware size attack:[%d/%d]\r\n", bkUpgrade.rcv_size + paylen, fhead->size);
		return -8;
	}else{
		if(bkapi_upgrade_data_wrbuffer(pay, paylen) < 0){
			return -5;
		}
	}
	bkUpgrade.rcv_size += paylen;
	bkUpgrade.seqno_data = seq_num;
	

	os_printf("firmware wirte successful[%d:%d/%d]\r\n", 
			bkUpgrade.seqno_data, bkUpgrade.rcv_size, bkUpgrade.fw_header.size);

	return 0;
}

void reboot_thread_func( beken_thread_arg_t arg)
{
	rtos_delay_milliseconds(1000);
	bkapi_system_reboot();
	rtos_delete_thread(NULL);
}

void thread_reboot(void)
{
	rtos_create_thread( NULL, BEKEN_DEFAULT_WORKER_PRIORITY,
                              "thread_reboot",
                              reboot_thread_func,
                              512,
                              0);
}

void bkapi_upgrade_end(void)
{
	if(bkUpgrade.have_init == 0){
		return;
	}
	os_free(bkUpgrade.wr_buf);		
	os_free(bkUpgrade.wr_tmp_buf);
	bkUpgrade.wr_buf = NULL;
	bkUpgrade.wr_tmp_buf = NULL;
	bkUpgrade.wr_last_len = 0; 
	bkUpgrade.flash_address = 0;
	bkUpgrade.flash_len = 0;
	bkUpgrade.seqno_init = 0;
	bkUpgrade.seqno_data = 0;

	ddev_close(bkUpgrade.flash_hdl);
	flash_protection_op(FLASH_XTX_16M_SR_WRITE_ENABLE, FLASH_UNPROTECT_LAST_BLOCK);
	bkUpgrade.have_init = 0;
	if(bkUpgrade.upgrade_ok){
		bkUpgrade.upgrade_ok = 0;
		os_printf("prepare reboot\r\n");
		thread_reboot();	
	}
}

int bkapi_upgrade_data_wrflash(UINT8 *src, unsigned len)
{
	UINT32 param , or_crc;
	UINT32 param1;
	GLOBAL_INT_DECLARATION();

	if(bkUpgrade.flash_address % 0x1000 == 0){
		param = bkUpgrade.flash_address;
		GLOBAL_INT_DISABLE();
		ddev_control(bkUpgrade.flash_hdl, CMD_FLASH_ERASE_SECTOR, (void *)&param);
		GLOBAL_INT_RESTORE();
	}

	if((u32)bkUpgrade.flash_address >= 0x200000 || (u32)bkUpgrade.flash_address < 0x27000){
		os_printf ("err_addr:%x \r\n", bkUpgrade.flash_address);
		return -1;
	}

	if((u32)bkUpgrade.flash_address < 0x400000){
		GLOBAL_INT_DISABLE();
		ddev_write(bkUpgrade.flash_hdl, src, len, (u32)bkUpgrade.flash_address);
		GLOBAL_INT_RESTORE();
		if(bkUpgrade.wr_tmp_buf){
			GLOBAL_INT_DISABLE();
			ddev_read(bkUpgrade.flash_hdl, bkUpgrade.wr_tmp_buf, len , (u32)bkUpgrade.flash_address);
			GLOBAL_INT_RESTORE();
			if(os_memcmp(src , bkUpgrade.wr_tmp_buf, len )){
				os_printf ("wr flash write err\r\n");
				return -1;
			}
		}
		bkUpgrade.flash_address += len;
	}

	return 0;
}

int bkapi_upgrade_data_wrbuffer(char *page, UINT32 len)
{
	UINT8 *tmp;
	UINT32 w_l = 0,i=0;

	i =0;
	tmp = (UINT8 *)page;
	while(i < len ){
		w_l = min(len - i, FLASH_WR_BUF_MAX-bkUpgrade.wr_last_len);
		os_memcpy(bkUpgrade.wr_buf+bkUpgrade.wr_last_len, tmp+i, w_l);
		i+=w_l;
		bkUpgrade.wr_last_len+=w_l;
		if(bkUpgrade.wr_last_len>=FLASH_WR_BUF_MAX){
			os_printf ("+");
			//direct wrtie to flash
			if(bkapi_upgrade_data_wrflash(bkUpgrade.wr_buf, FLASH_WR_BUF_MAX) < 0){
				return -1;
			}
			bkUpgrade.wr_last_len = 0;
		}
	}

	return 0;
}

int bkapi_upgrade_data_sync(void)
{
	if(bkUpgrade.wr_last_len){
		os_printf ("sync last data\r\n");
		if(bkapi_upgrade_data_wrflash(bkUpgrade.wr_buf, bkUpgrade.wr_last_len) < 0){
			return -1;
		}		
	}else{
		os_printf ("no need sync data\r\n");
	}
	return 0;
}


uint8_t apiBuffer[FLASH_WR_BUF_MAX] = {0};
int bkapi_upgrade_data_check(int md5len, char md5str[33])
{
	DD_HANDLE flash_hdl;
	UINT32 status, curlen = 0, readlen = 0;
	unsigned char decrypt[16] = {0};

	flash_hdl = ddev_open(FLASH_DEV_NAME, &status, 0);
	if(DD_HANDLE_UNVALID == flash_hdl){		
		os_printf("flash open err\r\n");
		return -1;
	}
	bk_logic_partition_t *pt = bk_flash_get_info(BK_PARTITION_OTA_DL_PART);  
	if(pt == NULL){
		os_printf("No OTA_DL_PART in flash\r\n");
		ddev_close(flash_hdl);
		return -1;
	}
	MD5_CTX ctx;
	i4MD5Init(&ctx);
	while(curlen < md5len){
		os_memset(apiBuffer, 0, FLASH_WR_BUF_MAX);
		if(md5len-curlen >= FLASH_WR_BUF_MAX){
			readlen = FLASH_WR_BUF_MAX;
		}else{
			readlen = md5len-curlen;
		}
		ddev_read(flash_hdl, (char *)apiBuffer, readlen, pt->partition_start_addr+curlen);
		curlen += readlen;
		i4MD5Update(&ctx, apiBuffer, readlen);
	}
	i4MD5Final(decrypt, &ctx);

	int i;
	for(i=0;i<16;i++){	
		os_snprintf(&(md5str[i*2]), 3, "%02x",decrypt[i]);
	}

	ddev_close(flash_hdl);

	os_printf ("md5:%s\r\n", md5str);
	return 0;
}

void bkWDG_stop(void)
{
	if(bkWDG.status == 0){
		return ;
	}
	OSStatus ret;
    UINT32 rwdg;

    rwdg = sddev_control(WDT_DEV_NAME, WCMD_POWER_DOWN, 0);
   	if(rwdg != WDT_SUCCESS){
		os_printf ("stop wdg failed\r\n");
	}
	if(rtos_is_timer_init(&(bkWDG.wdg_timer))){
		if(rtos_is_timer_running(&(bkWDG.wdg_timer))){
			ret = rtos_stop_timer(&(bkWDG.wdg_timer));
			if(ret != kNoErr){
				os_printf ("stop wdg timer failed\r\n");
			}
		}
		ret = rtos_deinit_timer(&(bkWDG.wdg_timer));
		if(ret != kNoErr){
			os_printf ("deinit wdg timer failed\r\n");
		}
	}
	bkWDG.status = 0;
}

void wdg_ticks(void *param)
{
	if(bkWDG.status == 0){
		I4SC_PRT("wdg not start\r\n");
		return;
	}
	sddev_control(WDT_DEV_NAME, WCMD_RELOAD_PERIOD, 0);
}

int bkWDG_start(uint32 period)
{
	if(bkWDG.status){	
		I4SC_PRT("stop wdg first\r\n");
		bkWDG_stop();
	}
	OSStatus ret;
	uint32_t tmout = 0;
	if(period < 100){
		tmout = 100;
	}else{
		tmout = period;
	}
    ret = rtos_init_timer(&(bkWDG.wdg_timer),
                            tmout>>3,
                            wdg_ticks,
                            (void *)0);
	if(kNoErr != ret){
		os_printf ("init wdg timer failed\r\n");
		return -1;
	}
	bkWDG.period = tmout;
	bkWDG.status = 1;
	ret = rtos_start_timer(&(bkWDG.wdg_timer));
	if(kNoErr != ret){
		os_printf ("init wdg timer failed\r\n");
		bkWDG.status = 0;
		return -1;
	}
	sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, 0);
	sddev_control(WDT_DEV_NAME, WCMD_SET_PERIOD, &tmout);
	os_printf("strat wdg :%dms period:%dms\r\n", tmout, tmout>>3);

    return 0;
}
