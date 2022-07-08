#include <stdio.h>
#include "lwip\sockets.h"
#include "lwip\netif.h"
#include "lwip\dns.h"

#include "lwip\api.h"
#include "lwip\tcp.h"

#include <event.h>
#include <log.h>
#include <frame.h>
#include <rtp.h>
#include "spook_config.h"
//#include <linux/linux_mutex.h>
#include "lwip/api.h"
#include <csi_kernel.h>
#include "ota.h"
//#include "flash.h"
#include "socket_module.h"
#include "dev/csi/hgdvp.h"
#include "boot_lib.h"
#include "hal/spi_nor.h"
#include "lib/ota/fw.h"


static int global_ota_num = -1;



int tcp_server_fd = -1;
int tcp_test_server_fd = -1;



#define RECV_MAX_SIZE	1460

enum TcpServerCmd
{
	REQ_NET_UPGRADE = 0xA0,
	ACK_NET_UPGRADE = 0xA1,
};

typedef struct tcpServerHeader
{
	unsigned int Command;
	unsigned int Reserved[8];
}TCP_SERVER_H;

typedef struct tcpNetUpgradeInfo
{
	char version[16];
	int fileSize;
}NetUpgrade_Info;


int InitSrv(uint16_t port)
{
	int srvsock;
	srvsock = creat_tcp_socket(port);   
	if(listen(srvsock,3)== -1)
	{  
		perror("listen()error\n");  
		return -1;  
	}  
	return srvsock;
}


void set_global_ota_num(int num)
{
	global_ota_num = num;
}

int get_global_ota_num()
{
	return global_ota_num;
}




extern int32 libotaV2_write_fw(uint32 tot_len,uint8 is_once_earse,uint8 isSector,uint8 isVerify,uint32 off, uint8 *data, uint16 len);
void tcpNetUpgrade(int tcp_connect_fd)
{

	if(get_global_ota_num()<0)
	{
		printf("%s err,check global_ota_num\n",__FUNCTION__);
	}
	int res = 0;
	char *tcp_rcv_buff = NULL;
	int size = 0;
	int offset = 0;
	int i;
	uint32_t earse_size = 0;
	uint32_t earse_block = 0;
	tcp_rcv_buff = (char*)malloc(RECV_MAX_SIZE + 4);
	memset(tcp_rcv_buff, 0, RECV_MAX_SIZE + 4);
	TCP_SERVER_H tcpHeader;
	memset(&tcpHeader, 0, sizeof(TCP_SERVER_H));
	tcpHeader.Command = ACK_NET_UPGRADE;

	
	//spi_nor_open(flash);
	if(send(tcp_connect_fd, (char*)&tcpHeader, sizeof(TCP_SERVER_H), 0) > 0)
	{
		NetUpgrade_Info netUpgradeInfo;
		memset(&netUpgradeInfo, 0, sizeof(NetUpgrade_Info));
		recv(tcp_connect_fd, (char*)&netUpgradeInfo, sizeof(NetUpgrade_Info), 0);
		while(offset < netUpgradeInfo.fileSize)
		{
			memset(tcp_rcv_buff, 0, RECV_MAX_SIZE + 4);
			size = recv(tcp_connect_fd, tcp_rcv_buff, RECV_MAX_SIZE, 0);
			if(size <= 0)
			{
				printf("%s:%d err,offset:%d\n",__FUNCTION__,__LINE__,offset);
				break;
			}

			//res = libotaV2_write_fw(netUpgradeInfo.fileSize,1,0,0,offset, tcp_rcv_buff, size);
			res = libota_write_fw(netUpgradeInfo.fileSize,offset,tcp_rcv_buff,size);
			if(res)
			{
				send(tcp_connect_fd, "Upgrade_err", 12, 0);
				printf("%s:%d libota_write_fw err,offset:%d,res:%d\n",__FUNCTION__,__LINE__,offset,res);
				break;
			}
			
			offset += size;
			//printf("recv size: %d, offset: %d(%d%%)\n", size, offset, offset*100/netUpgradeInfo.fileSize);
		
		};
		if(offset == netUpgradeInfo.fileSize)
		{
			//将数据头回写,代表已经完成
			//spi_nor_write(flash,update_adr+0,(unsigned char*)&ota_flag,sizeof(ota_flag));
			size = send(tcp_connect_fd, "Upgrade_ok", 11, 0);
			printf("NetUpgrade ok ###\n");
		}
		else
		{
			size = send(tcp_connect_fd, "Upgrade_err", 12, 0);
			printf("NetUpgrade err offset = %d, fileSize = %d ###\n", offset, netUpgradeInfo.fileSize);
		}
	}
	free(tcp_rcv_buff);
	close(tcp_connect_fd);
	//spi_nor_close(flash);
}



extern void tcp_update(void *d);
void tcpServerAccept(void *e, void *d)
{
	int tcp_connect_fd = -1;
	k_task_handle_t task_hdl;
	struct  sockaddr_in client;  
	socklen_t  addrlen;

	TCP_SERVER_H tcpHeader;
	memset(&tcpHeader, 0, sizeof(TCP_SERVER_H));

	addrlen =sizeof(client);  
	tcp_connect_fd = accept(tcp_server_fd, (struct sockaddr*)&client, &addrlen);
	if(tcp_connect_fd < 0)
	{
		printf("accept()error\n");  
	}

	#if 0
#if DVP_EN	
	//dvp_close();
	void *dvp = (void *)dev_get(HG_DVP_DEVID);
	if(dvp)
	{
		dvp_close(dvp);
	}
#endif

#if USB_EN	
    SYSCTRL_REG_OPT(sysctrl_usb20_clk_close());
#endif
	if(recv(tcp_connect_fd, (char*)&tcpHeader, sizeof(TCP_SERVER_H), 0) > 0)
	{
		printf("tcpHeader.Command = %x ###\n", tcpHeader.Command);
		switch(tcpHeader.Command)
		{
			case REQ_NET_UPGRADE:
			{
				update_flash_erase();
				tcpNetUpgrade(tcp_connect_fd);
				break;
			}
			default:
				break;
		}
	}
	close(tcp_connect_fd);
	#endif
	csi_kernel_task_new((k_task_entry_t)tcp_update, "tcp_update", (void*)tcp_connect_fd, 10, 0, NULL, 2048, &task_hdl);

}


void ota_Tcp_Server()
{
	uint16_t port = 5007;

	printf("[test] init tcp server: port: %d\n", port);

	tcp_server_fd = InitSrv(port);
	
	eloop_add_fd( tcp_server_fd, EVENT_READ, EVENT_F_ENABLED, tcpServerAccept, 0 );
}


void tcp_recv_data(void *d)
{

	int tcp_connect_fd = (int)d;
	uint8_t *tcp_rcv_buff;
	int size;
	tcp_rcv_buff = (uint8_t*)malloc(1024 + 4);
	memset(tcp_rcv_buff, 0, sizeof(1024+4));

	
#if DVP_EN	
		//dvp_close();
		void *dvp = (void *)dev_get(HG_DVP_DEVID);
		if(dvp)
		{
			dvp_close(dvp);
		}
#endif
	
#if USB_EN	
		SYSCTRL_REG_OPT(sysctrl_usb20_clk_close());
#endif
		while(1){
			size = recv(tcp_connect_fd, tcp_rcv_buff, 1024, 0);
			if(size > 0){
				//printf("tcp_rcv_buff(%d):%x %x %x %x\r\n",size,tcp_rcv_buff[0],tcp_rcv_buff[1],tcp_rcv_buff[2],tcp_rcv_buff[3]);
			}else{
				printf("recv error\r\n");
				goto end;
			}
		}

end:
		close(tcp_connect_fd);


}



void tcpAccept(void *e, void *d)
{
	int tcp_connect_fd = -1;
	k_task_handle_t task_hdl;
	struct  sockaddr_in client;  
	socklen_t  addrlen;

	TCP_SERVER_H tcpHeader;
	memset(&tcpHeader, 0, sizeof(TCP_SERVER_H));

	addrlen =sizeof(client);  
	tcp_connect_fd = accept(tcp_test_server_fd, (struct sockaddr*)&client, &addrlen);
	if(tcp_connect_fd < 0)
	{
		printf("accept()error\n");  
	}

	csi_kernel_task_new((k_task_entry_t)tcp_recv_data, "tcp_recv", (void*)tcp_connect_fd, 10, 0, NULL, 2048, &task_hdl);

}


void Tcp_Server_test()
{
	uint16_t port = 8090;

	printf("[test] init tcp server: port: %d\n", port);

	tcp_test_server_fd = InitSrv(port);
	
	eloop_add_fd( tcp_test_server_fd, EVENT_READ, EVENT_F_ENABLED, tcpAccept, 0 );
}



static int tcp_update_running = 0;
int get_tcp_update_status()
{
	return tcp_update_running;
}

void tcp_update(void *d)
{

	int tcp_connect_fd = (int)d;

	if(tcp_update_running)
	{
		goto end;
	}
	tcp_update_running = 1;
	TCP_SERVER_H tcpHeader;
	memset(&tcpHeader, 0, sizeof(TCP_SERVER_H));
 
	
#if DVP_EN	
		//dvp_close();
		void *dvp = (void *)dev_get(HG_DVP_DEVID);
		if(dvp)
		{
			dvp_close(dvp);
		}
#endif
	
#if USB_EN	
		SYSCTRL_REG_OPT(sysctrl_usb20_clk_close());
#endif
		if(recv(tcp_connect_fd, (char*)&tcpHeader, sizeof(TCP_SERVER_H), 0) > 0)
		{
			printf("tcpHeader.Command = %x ###\n", tcpHeader.Command);
			switch(tcpHeader.Command)
			{
				case REQ_NET_UPGRADE:
				{
					tcpNetUpgrade(tcp_connect_fd);
					break;
				}
				default:
					break;
			}
		}
		tcp_update_running = 0;
end:
		
		close(tcp_connect_fd);

}






