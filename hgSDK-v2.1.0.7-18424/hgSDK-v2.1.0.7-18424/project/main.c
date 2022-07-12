#include "sys_config.h"
#include "typesdef.h"
#include "list.h"
#include "dev.h"
#include "devid.h"
#include "osal/string.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "osal/task.h"
#include "osal/sleep.h"
#include "osal/timer.h"
#include "osal/work.h"
#include "hal/gpio.h"
#include "hal/watchdog.h"
#include "hal/uart.h"
#include "lib/common/common.h"
#include "lib/common/sysevt.h"
#include "lib/syscfg/syscfg.h"
#include "lib/lmac/lmac.h"
#include "lib/skb/skbpool.h"
#include "lib/atcmd/libatcmd.h"
#include "lib/bus/xmodem/xmodem.h"
#include "lib/net/dhcpd/dhcpd.h"
#include "lib/umac/ieee80211.h"
#include "lib/umac/umac.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/sys.h"
#include "lwip/ip_addr.h"
#include "lwip/tcpip.h"
#include "netif/ethernetif.h"
#include "syscfg.h"
#include "lib/net/eloop/eloop.h"
#include "app/speedTest/speedTest.h"
#include "app/record/avi_record.h"
#include "app/update/ota.h"
#include "app/dhcpd_eloop/dhcpd_eloop.h"
#include "app/sta_interface/sta_interface.h"
#include "ota.h"
#include "cmd_input.h"
#include "lib/heap/sysheap.h"
#include "spook.h"
#include "remote_control.h"
#include "lib/sdhost/sdhost.h"
#include "i4s.h"
#include "alk.h"
#include "i4sgsensor.h"

static struct os_work main_wk;
static struct os_work wtdog_wk;
uint8_t connect_led = 0;
uint8_t mac_adr[6];

uint8_t qc_mode = 0;
void delay_us(uint32 n);

struct hgic_atcmd_normal {
    struct atcmd_dataif dataif;
};


struct hgic_atcmd_normal *atcmd_uart_normal = NULL;


#ifdef CONFIG_UMAC4
int32 sys_wifi_event(uint8 ifidx, uint16 evt, uint32 param1, uint32 param2)
{
    switch (evt) {
        case IEEE80211_EVENT_SCAN_START:
            os_printf("inteface%d: start scanning ...\r\n", ifidx);
            break;
        case IEEE80211_EVENT_SCAN_DONE:
            os_printf("inteface%d: scan done!\r\n", ifidx);
            break;
        case IEEE80211_EVENT_CONNECT_START:
            os_printf("inteface%d: start connecting ...\r\n", ifidx);
            break;
        case IEEE80211_EVENT_CONNECTED:
            os_printf("inteface%d: sta "MACSTR" cj connected\r\n", ifidx, MAC2STR((uint8 *)param1));
            extern int i4s_connected(int connect);
	        i4s_connected(1);	
            break;
        case IEEE80211_EVENT_DISCONNECTED:
            os_printf("inteface%d: sta "MACSTR" disconnected\r\n", ifidx, MAC2STR((uint8 *)param1));
            extern int i4s_connected(int connect);
	        i4s_connected(0);
            break;
        case IEEE80211_EVENT_RX_MGMT_FRAME:
            //os_printf("inteface%d rx mgmt frame, data:%x, len:%d\r\n", ifidx, param1, param2);
            break;
        case IEEE80211_EVENT_RX_EXT_FRAME:
            //os_printf("inteface%d rx RXT frame, data:%x, len:%d\r\n", ifidx, param1, param2);
            break;
        case IEEE80211_EVENT_RSSI:
            os_printf("inteface%d rssi: %d\r\n", ifidx, param1);
            break;
        case IEEE80211_EVENT_NEW_BSS:
            os_printf("inteface%d find new bss: "MACSTR"-%s\r\n", ifidx, MAC2STR((uint8 *)param1), (uint8 *)param2);
            break;
        default:
            break;
    }
    return RET_OK;
}
#endif

__init static void sys_efuse_load(void)
{
    sysctrl_efuse_validity_handle();
}

__init static void sys_cfg_load()
{
    #if 0
    if (syscfg_init(&sys_cfgs, sizeof(sys_cfgs)) == RET_OK) {
        return;
    }
    os_printf("use default params.\r\n");
    syscfg_set_default_val();
    syscfg_save();
#else
	/* extern int i4s_mcu_init(void);
	i4s_mcu_init(); */
    if (syscfg_init(&sys_cfgs, sizeof(sys_cfgs)) == RET_OK) {
		//add by zhangwei
		i4s_pre_init((void*)&sys_cfgs);		
        return;
    }
    os_printf("use default params.\r\n");
	i4s_pre_init((void*)&sys_cfgs); 	
    syscfg_save();
#endif
}

void user_io_preconfig();
void user_protocol();


__weak void user_io_preconfig()
{

}

__weak void user_protocol()
{
    //spook_init();
    extern int i4s_mcu_init(void);
	i4s_mcu_init();
    i4s_camera_init();
}

__weak uint8_t set_qc_mode_io()
{
	return 255;
}

void sys_mode_confirm()
{
	if(qc_mode){
		sys_cfgs.wifi_mode = WIFI_MODE_STA;
	}else{
		sys_cfgs.wifi_mode = sys_cfgs.default_wifi_mode;
	}	
}

uint8_t qc_ret = 0;
uint8_t mode_io_1 = 0;
uint8_t io_dect = 0;


//period 100ms
__weak int32 is_qc_mode(uint8_t mode_io)
{
	int32 io_direct,io_direct1,io_direct2,io_direct3;
	io_direct = 0;
	gpio_iomap_input(mode_io, GPIO_DIR_INPUT);
	
	io_direct1 = gpio_get_val(mode_io);
	delay_us(5000);       //delay 5ms
	io_direct2 = gpio_get_val(mode_io);
	delay_us(5000);       //delay 5ms
	io_direct3 = gpio_get_val(mode_io);
	
	if((io_direct1 == io_direct2)&&(io_direct3 == io_direct2)){
		io_direct = io_direct2;
		delay_us(45000);  //delay 45ms
	}else if((io_direct1 == io_direct2)&&(io_direct3 != io_direct2)){
		io_direct = io_direct1;
		delay_us(40000);  //delay 40ms		
	}else if((io_direct1 != io_direct2)&&(io_direct3 == io_direct2)){
		io_direct = io_direct3;
		delay_us(50000);  //delay 50ms		
	}
	
	if(gpio_get_val(mode_io) == io_direct){
		return 0;
	}
	
	delay_us(50000);      //delay 50ms
	if(gpio_get_val(mode_io) != io_direct){		
		return 0;
	}	

	delay_us(50000);      //delay 50ms
	if(gpio_get_val(mode_io) == io_direct){
		return 0;
	}

	delay_us(50000);      //delay 50ms
	if(gpio_get_val(mode_io) != io_direct){
		return 0;
	}
	//check 200ms,all correct,return true
	return 1;
	
}

void wifi_qc_mode_inspect()
{
	uint8_t wifi_qc_mode_io;
	wifi_qc_mode_io = set_qc_mode_io();
	if(wifi_qc_mode_io != 255){
		if(is_qc_mode(wifi_qc_mode_io)){
			//sys_cfgs.wifi_mode = WIFI_MODE_STA;
			qc_mode = 1;
		}else{
			//sys_cfgs.wifi_mode = sys_cfgs.default_wifi_mode;
			qc_mode = 0;
		}	
	}else{
		//sys_cfgs.wifi_mode = sys_cfgs.default_wifi_mode;
		qc_mode = 0;
	}
}


static int32 atcmd_normal_write(struct atcmd_dataif *dataif, uint8 *buf, int32 len)
{
    //uart_puts(atcmd_mgr->uart, buf, len);
    int32 i = 0;
    for(i = 0;i < len;i++){
        printf("%c",buf[i]);
    }
    return 0;
}


void user_sta_add(char *addr){
	connect_led++;
	memcpy(mac_adr,addr,6);	
	printf("user_sta_add:%x %x %x %x %x %x\r\n",addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
    printf("1111111111jj111111");
    i4s_handle_event(OP_WIFI_CONECTED);
	extern int i4s_connected(int connect);
	i4s_connected(1);	
}

void user_sta_del(char *addr){
	connect_led--;
	memset(mac_adr,0,6); 
	printf("user_sta_del:%x %x %x %x %x %x\r\n",addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
    i4s_handle_event(OP_WIFI_DISCONECTED);
	extern int i4s_connected(int connect);
	i4s_connected(0);
}

sysevt_hdl_res sysevt_wifi_event(uint32 event_id, uint32 data, uint32 priv){
    char *addr;
    addr = (char*)data;
    switch(event_id&0xffff){
        case SYSEVT_WIFI_DISCONNECT:
            user_sta_del(addr);
            break;
        case SYSEVT_WIFI_CONNECTTED:
            printf("222222jjj2222222222");
            user_sta_add(addr);
            break;
        default:
            printf("no this event...\r\n");
            break;
    }
    return SYSEVT_CONTINUE;
}

__init static void sys_atcmd_init(void)
{
    atcmd_uart_normal = os_zalloc(sizeof(struct hgic_atcmd_normal));
    atcmd_uart_normal->dataif.write = atcmd_normal_write;
    atcmd_uart_normal->dataif.read  = NULL;   
    atcmd_init(&atcmd_uart_normal->dataif);
    atcmd_register("AT+FWUPG", xmodem_fwupgrade_hdl, NULL);
    atcmd_register("AT+SSID",  wificfg_atcmd_set_ssid, NULL);
    atcmd_register("AT+KEY",   wificfg_atcmd_set_key, NULL);
    atcmd_register("AT+ENCRYPT", wificfg_atcmd_set_encrypt, NULL);
}

__init static void sys_wifi_parameter_init(void *ops)
{
    int32 ret;
    struct lmac_acs_ctl acs_ctl;

    lmac_set_wphy_dpd(ops, WIFI_RF_USE_IPEX);
    if(sys_cfgs.wifi_mode == WIFI_MODE_AP){
        if(sys_cfgs.channel == 0) {
            acs_ctl.enable     = 1;
            acs_ctl.scan_ms    = WIFI_ACS_SCAN_TIME;;
            acs_ctl.chn_bitmap = WIFI_ACS_CHAN_LISTS;
            
            ret = lmac_start_acs(ops, &acs_ctl, 1);  //阻塞式扫描
            if(ret != RET_ERR) {
                sys_cfgs.channel = ret;
            }
        }
    }

    lmac_set_rts(ops, WIFI_RTS_THRESHOLD);
    lmac_set_retry_cnt(ops, WIFI_TX_MAX_RETRY, WIFI_RTS_MAX_RETRY);
    lmac_set_txpower(ops, WIFI_TX_MAX_POWER);
    lmac_set_supp_rate(ops, WIFI_TX_SUPP_RATE);
    lmac_set_max_sta_cnt(ops, WIFI_MAX_STA_CNT);
    lmac_set_mcast_dup_txcnt(ops, WIFI_MULICAST_RETRY);
    lmac_set_max_ps_frame(ops, WIFI_MAX_PS_CNT);
    lmac_set_tx_duty_cycle(ops, WIFI_TX_DUTY_CYCLE);
#if WIFI_SSID_FILTER_EN
    lmac_set_ssid_filter(ops, sys_cfgs.ssid, strlen(sys_cfgs.ssid));
#endif
}

__init static void sys_wifi_init()
{
    void *ops;
    struct lmac_init_param lparam;

#if DCDC_EN
    pmu_dcdc_open();
#endif
    lparam.rxbuf      = WIFI_RX_BUFF_ADDR;
    lparam.rxbuf_size = WIFI_RX_BUFF_SIZE;
    ops = lmac_bgn_init(&lparam);
    
#ifdef CONFIG_UMAC4
    struct ieee80211_initparam param;
    os_memset(&param, 0, sizeof(param));
    param.vif_maxcnt = 4;
    param.sta_maxcnt = 8;
    param.bss_maxcnt = 32;
    param.bss_lifetime  = 300; //300 seconds
    param.evt_cb = sys_wifi_event;
    ieee80211_init(&param);
    //ieee80211_deliver_init(32, 60*1000);
    ieee80211_support_txw80x(ops);
    sys_wifi_parameter_init(ops);

    ieee80211_iface_create_ap(WIFI_MODE_AP, IEEE80211_BAND_2GHZ);
    ieee80211_iface_create_sta(WIFI_MODE_STA, IEEE80211_BAND_2GHZ);
    //ieee80211_conf_stabr_table(WIFI_MODE_STA, 32);

    if (sys_cfgs.wifi_mode == WIFI_MODE_AP || sys_cfgs.wifi_mode == WIFI_MODE_APSTA) {
        wificfg_flush(WIFI_MODE_AP);
        ieee80211_iface_start(WIFI_MODE_AP);
    }
    if (sys_cfgs.wifi_mode == WIFI_MODE_STA || sys_cfgs.wifi_mode == WIFI_MODE_APSTA) {
        wificfg_flush(WIFI_MODE_STA);
        ieee80211_iface_start(WIFI_MODE_STA);
    }
#else
    umac_init();
    umac_add_txw80x(ops);
    sys_wifi_parameter_init(ops);
    if (sys_cfgs.wifi_mode == WIFI_MODE_AP) {
        hapd_init();
        hapd_start("hg0");
    } else {
        wpas_init();
        wpas_start("hg0");
    }
#endif
}

__init static void sys_dhcpd_start()
{
#if 0
    struct dhcpd_param param;

    os_memset(&param, 0, sizeof(param));
    param.start_ip   = sys_cfgs.dhcp_startip;
    param.end_ip     = sys_cfgs.dhcp_endip;
    param.netmask    = sys_cfgs.netmask;
    param.lease_time = sys_cfgs.dhcpd_lease_time;
    if (dhcpd_start("w0", &param)) {
        os_printf("dhcpd start error\r\n");
    }


#else
    //使用eloop机制去实现dhcp,所以要在lwip初始化完成并且eloop启动才能使用
    struct dhcpd_param param;
    os_memset(&param, 0, sizeof(param));
    param.start_ip   = sys_cfgs.dhcpd_startip;
    param.end_ip     = sys_cfgs.dhcpd_endip;
    param.netmask    = sys_cfgs.netmask;
    param.router     = sys_cfgs.gw_ip;
    param.dns1       = sys_cfgs.gw_ip;
    param.lease_time = sys_cfgs.dhcpd_lease_time;
    if (dhcpd_start_eloop("w0", &param)) {
        os_printf("dhcpd start error\r\n");
    }

#endif
}

__init static void sys_network_init(void)
{
    ip_addr_t ipaddr, netmask, gw;
    struct netdev *ndev;

    tcpip_init(NULL, NULL);
    //sock_monitor_init();

#ifdef CONFIG_UMAC4
    if(sys_cfgs.wifi_mode == WIFI_MODE_AP){
        ndev = (struct netdev *)dev_get(HG_WIFI1_DEVID);
    }else{
        ndev = (struct netdev *)dev_get(HG_WIFI0_DEVID);
    }
#else
    ndev = (struct netdev *)dev_get(HG_WIFI0_DEVID);
#endif

    if (ndev) {
        ipaddr.addr  = sys_cfgs.ipaddr;
        netmask.addr = sys_cfgs.netmask;
        gw.addr      = sys_cfgs.gw_ip;
        lwip_netif_add(ndev, "w0", &ipaddr, &netmask, &gw);
        lwip_netif_set_default(ndev);
        if (sys_cfgs.wifi_mode == WIFI_MODE_STA && sys_cfgs.dhcpc_en) {
            lwip_netif_set_dhcp(ndev, 1);
        }
        os_printf("add w0 interface!\r\n");
    }else{
        os_printf("**not find wifi device**\r\n");
    }

//dhcp放到app_network_init,因为相当于属于network的应用
#if 0
    if (sys_cfgs.wifi_mode == WIFI_MODE_AP) { //AP
        sys_dhcpd_start();
    }
#endif
}

ip_addr_t lwip_netif_get_ip(struct netdev *ndev);
__init ip_addr_t sys_network_get_ip(void){
    struct netdev *ndev = (struct netdev *)dev_get(HG_WIFI0_DEVID);
    if (ndev == NULL) {
        ndev = (struct netdev *)dev_get(HG_WIFI1_DEVID);
    }
    return lwip_netif_get_ip(ndev);
}

/********************************************
应用相关的网络应用,eventLoop、默认一些socket
的监听等
********************************************/
__init static void app_network_init()
{
    printf("%s:%d\n",__FUNCTION__,__LINE__);
    #define ELOOP_TASK_HEAP 2048
    #define ELOOP_TASK_PRI  10
    //不需要保存的handle用局部变量,需要保存需要用全局
    k_task_handle_t eloop_task_handle;
    eloop_init ();  
    csi_kernel_task_new((k_task_entry_t)user_eloop_run, "eloop_run", NULL, ELOOP_TASK_PRI, 0, NULL, ELOOP_TASK_HEAP, &eloop_task_handle);
    os_sleep_ms(1);
    if (sys_cfgs.wifi_mode == WIFI_MODE_AP) { //AP
        sys_dhcpd_start();
    }
    ota_Tcp_Server();
    set_global_ota_num(pmu_get_boot_code_pos());
    printf("ota num:%d\tversion:%d\n",get_global_ota_num(),get_boot_svn_version());
    if (sys_cfgs.wifi_mode == WIFI_MODE_STA){
        //sta_send_udp_msg_init();
    }
    //speedTest_udp_server(8060);
    //speedTest_udp_client("192.168.1.100",43210);
    //speedTest_tcp_server(43211);
    /* extern int i4s_mcu_init(void);
	i4s_mcu_init(); */
#if USB_EN || JPG_EN
     /* void alk_net_data_init();
      alk_net_data_init(); */
    //careskin_net_data_init();
   // i4s_gsensor_init();
    user_protocol();
#endif

#if SDH_EN && FS_EN && FRAME_MANAGEMENT && OPENDML_EN
    remote_control_Server(43210);
#endif

#if UART_INPUT_EN
    void *cmd_uart = (void *)dev_get(HG_UART1_DEVID);
    cmd_init(cmd_uart,3,1200);
#endif


}

uint8 vcam_en()
{
    uint8 ret = TRUE;
#if VCAM_EN
    pmu_set_vcam_vol(VCAM_VOL_2V8);
    pmu_vcam_oc_detect_dis();
    pmu_vcam_oc_int_dis();
    pmu_vcam_discharge_dis();
    pmu_vcam_en();
#ifdef VCAM_33
    pmu_vcam_pg_en();
#else
    pmu_vcam_pg_dis();
#endif
    os_sleep_ms(1);
    sys_reset_pending_clr();

    pmu_vcam_oc_detect_en();
    //pmu_vcam_oc_int_en();
    //pmu_lvd_oe_en();
    if(PMU_VCAM_OC_PENDING){
        return FALSE;
    }
    pmu_vcam_oc_detect_dis();
    pmu_vcam_oc_int_dis();
    pmu_lvd_oe_en();	
#endif
    return ret;
}


void hardware_init(uint8 vcam)
{
    if(vcam == FALSE)
        return;

#if JPG_EN
    void jpg_start();
    jpg_start();
#endif
        
#if DVP_EN
    bool csi_open();
    csi_open();
#endif
    
#if SDH_EN && FS_EN
    extern bool fatfs_register();
    sd_open();
    fatfs_register();
#endif
    
    
#if AUDIO_EN
        //pdm_open();
    #include "pdm_audio.h"
    audio_task("pdm");
        //音频模拟,实际应该从pdm生成
        //csi_kernel_task_new((k_task_entry_t)jpg_file_audio, "pdm", 0, 10, 0, NULL, 512,&timer_test_handle);
#endif
    //audio_8311_demo_init();
#if USB_EN  
    void hg_usb_test(void);
    hg_usb_test();
#endif

}

static void main_loop(struct os_work *work)
{
    static int8 print_interval = 0;
    if (print_interval++ >= 5) {
        //sysheap_status();
        uint32 sysheap_freesize(void);
        printf("ip:%x  freemem:%d\r\n", sys_network_get_ip().addr,sysheap_freesize());
        print_interval = 0;
    }
    os_run_work_delay(&main_wk, 1000);
}

static void watchdog_feed_work(struct os_work *work)
{
    struct watchdog_device *wd = (struct watchdog_device *)dev_get(HG_WDTV1_DEVID);
    if(wd){
        watchdog_device_feed(wd);
        os_run_work_delay(&wtdog_wk, 1000);
    }
}

static __init void sys_watchdog_init()
{
    struct watchdog_device *wd = (struct watchdog_device *)dev_get(HG_WDTV1_DEVID);
    OS_WORK_INIT(&wtdog_wk, watchdog_feed_work, 0);
    if(wd){
        watchdog_device_open(wd, 0);        
        watchdog_device_start(wd, 5);
        watchdog_feed_work(&wtdog_wk);

    }
}

static __init void sys_watchdog_pre_init()
{
    struct watchdog_device *wd = (struct watchdog_device *)dev_get(HG_WDTV1_DEVID);
    if(wd){
        watchdog_device_open(wd, 0);        
        watchdog_device_start(wd, 20);
    }
}

//---test
int main(void)
{
    uint8 vcam;
    //sys_watchdog_init(); 
    wifi_qc_mode_inspect();
    sys_watchdog_pre_init();
    skbpool_init(SKB_POOL_ADDR, SKB_POOL_SIZE, 80, 0);
    sys_efuse_load();
    sys_cfg_load();
    vcam = vcam_en();
    user_io_preconfig();
    sys_mode_confirm();
    sys_event_init(32);
    sys_event_track(SYS_EVENT(SYS_EVENT_WIFI, 0),sysevt_wifi_event,0);
//  sys_event_track(SYS_EVENT(SYS_EVENT_WIFI, SYSEVT_WIFI_DISCONNECT),user_sta_del,0);
    sys_atcmd_init();
    sys_wifi_init();
    sys_network_init();

    hardware_init(vcam);
    app_network_init();
    sys_watchdog_init();
    OS_WORK_INIT(&main_wk, main_loop, 0);
    os_run_work_delay(&main_wk, 1000);

    pmu_clr_deadcode_pending();
    return 0;
}

