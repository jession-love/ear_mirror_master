#include "sys_config.h"
#include "typesdef.h"
#include "osal/string.h"
#include "socket_module.h"
#include "lib/net/eloop/eloop.h"
#include "typesdef.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include "lwip/etharp.h"
#include "sta_interface.h"
#include "event.h"
#include "math.h"
#include "syscfg.h"

k_task_handle_t sta_msg_task;
extern uint8_t mac_adr[6];
int send_bcase_socket_fd   =  - 1;

ip_addr_t lwip_netif_get_netmask(struct netdev *ndev);
__init ip_addr_t sys_network_get_ip(void);
int get_rssi_msg(uint8_t *sta,char* ifname);

__init static ip_addr_t sys_network_get_netmask(void){
    struct netdev *ndev = (struct netdev *)dev_get(HG_WIFI0_DEVID);
    if (ndev == NULL) {
        ndev = (struct netdev *)dev_get(HG_WIFI1_DEVID);
    }
	return lwip_netif_get_netmask(ndev);
}


void sta_interface_read()
{
	int ret;
	char* strpt;
	int itk;
	int retval;
	struct sockaddr remote_addr;
	retval = 16;
	char buf[256];
	memset(buf,0,256);
	ret = recvfrom (send_bcase_socket_fd, buf, 256, MSG_DONTWAIT, &remote_addr, (socklen_t*)&retval);
	for(itk = 0;itk<256;itk++){
		printf("%02x ",buf[itk]);
	}
	strpt = buf;
	memset(sys_cfgs.ssid,0,32);
	//memcpy(sys_cfgs.ssid,strpt,strlen(strpt));
	os_sprintf(sys_cfgs.ssid,"%s%02x%02x%02x",strpt,sys_cfgs.mac[5],sys_cfgs.mac[4],sys_cfgs.mac[3]);

	strpt = &buf[100];
	if(strlen(strpt)){
		memset(sys_cfgs.key,0,32);
		memcpy(sys_cfgs.key,strpt,strlen(strpt));
	}else{
		printf("no set key\r\n");
	}
	syscfg_save();
}


void sta_send_udp_msg_task()
{
	struct sockaddr_in server_socket_addr;	
	uint32_t ipaddr;
	uint32_t netmask;
	uint32_t broadcast_ip;
	uint32_t tmp;
	int *pt;
	int rssi;
	uint8_t *tmp_8;
	uint8_t mac[8];
	uint8_t send_buf[128];
	pt = (int*)send_buf;
	do{
		ipaddr = sys_network_get_ip().addr;
		os_sleep_ms(100);
	}while((ipaddr&0xff000000) == 0x1000000);
	printf("ipaddr:%x\r\n",ipaddr);
	netmask = sys_network_get_netmask().addr;
	
//	while(*(uint32_t*)&p_netif->ip_addr == 0)
//	{
//		csi_kernel_delay(100);
//	}
	memset(send_buf,0,128);
	broadcast_ip = netmask;
	broadcast_ip = ~broadcast_ip;
	tmp = ipaddr;
	broadcast_ip |= tmp;
	tmp_8 = (uint8_t*)&tmp;

	send_buf[1] = tmp_8[0]&0x0f;
	send_buf[0] = (tmp_8[0]>>4)&0x0f;
	send_buf[3] = tmp_8[1]&0x0f;
	send_buf[2] = (tmp_8[1]>>4)&0x0f;
	send_buf[5] = tmp_8[2]&0x0f;
	send_buf[4] = (tmp_8[2]>>4)&0x0f;
	send_buf[7] = tmp_8[3]&0x0f;
	send_buf[6] = (tmp_8[3]>>4)&0x0f;

	memcpy(&send_buf[12],sys_cfgs.ssid,32);

	printf("netmask:%x  ip:%x%x%x%x%x%x%x%x\r\n",netmask,send_buf[0],send_buf[1],send_buf[2],send_buf[3],send_buf[4],send_buf[5],send_buf[6],send_buf[7]);

	printf("broadcast_ip:%x\r\n",broadcast_ip);
	send_bcase_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	printf("build socket finish:%d\r\n",send_bcase_socket_fd);
	if(send_bcase_socket_fd < 0)
	{
		printf("send_socket_fd err!!\r\n");
		return;
	}
	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(11011);
	server_socket_addr.sin_addr.s_addr = broadcast_ip;
	printf("server_socket_addr.sin_addr.s_addr:%x\r\n",server_socket_addr.sin_addr.s_addr);
	printf("server_socket_addr.sin_port:%x\r\n",server_socket_addr.sin_port);
    eloop_add_fd( send_bcase_socket_fd, 0, EVENT_F_ENABLED, (void*)sta_interface_read, 0 );	
	memset(server_socket_addr.sin_zero,0,8);
	memcpy(mac,mac_adr,6);
	printf("MAC_ADDR:%02x %02x %02x %02x %02x %02x %02x %02x\r\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],mac[6],mac[7]);
	while(1)
	{
		os_sleep_ms(1000);
		rssi = get_rssi_msg(mac,"hg0");
		pt[2] = rssi;
		printf("send_msg start:%d\r\n\r\n",rssi);
		memset(&send_buf[12],0,32);	
		memcpy(&send_buf[12],sys_cfgs.ssid,32);		
		sendto(send_bcase_socket_fd,send_buf,128,0,(const struct sockaddr*)&server_socket_addr,sizeof(struct sockaddr));
  	    printf("stop_msg = 1  :%x  \r\n\r\n",(int)send_buf);

	}
	close(send_bcase_socket_fd);
	
}


void sta_send_udp_msg_init()
{
	csi_kernel_task_new((k_task_entry_t)sta_send_udp_msg_task, "test_ip_udp_pkt", 0, 15, 0, NULL, 2048, &sta_msg_task);
}




