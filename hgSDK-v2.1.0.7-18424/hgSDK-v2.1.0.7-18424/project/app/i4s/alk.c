//#include <test_util.h>
#include <csi_config.h>
//#include <test_kernel_config.h>
#include "csi_kernel.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "netif/ethernetif.h"
#include "typesdef.h"
#include "osal\mutex.h"
#include "lib/net/eloop/eloop.h"
#include "event.h"
#include "sys_config.h"
#include <string.h>
#include "lwip/tcpip.h"
#include "osal/sleep.h"
#include "dev/csi/hgdvp.h"
#include "list.h"
#include "dev.h"
#include "g_sensor.h"
#include "devid.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "dev/jpg/hgjpg.h"
#include "jpgdef.h"
#include "lib/video/dvp/jpeg/jpg.h"
#include "alk.h"
#include "math.h"
#include "dev/adc/hgadc_v0.h"
#include "hal/gpio.h"

#if JPG_EN

#define PI 3.1415926

struct hgadc_v0* global_hgadc_test;

k_task_handle_t alk_task_fd;
k_task_handle_t alk_task_fd2;
struct os_mutex alk_socket_mutex;
struct sockaddr alk_udp_client;
int alk_socket_fd   =  - 1;
int alk_socket_led_fd   =  - 1;

uint32_t angle_avl;
uint32_t angle;
uint32_t wifi_angle;
uint32_t angle_tame[2];
uint16 angle_data_num=0;
uint8 angle_data_flag=0;
uint8 angle_num=0;
float ad_pwr = 100;
static struct os_task pwr_det_task;
extern uint8_t connect_led;

int16 x_data_buf[10],z_data_buf[10];

struct os_timer alk_gsensor_timer;

extern volatile struct list_head *usr_p;
extern volatile struct list_head free_tab;	
uint32_t keep_alive_time;
uint8_t get_gsensor_x_y_z(uint8_t* buf);
int32 gpio_set_val(uint32 pin, int32 value);
void alk_led_open(uint8_t enable){
	if(enable){
		gpio_set_val(PC_4,1);
		gpio_set_val(PC_5,0);
	}else{
		gpio_set_val(PC_4,0);
		gpio_set_val(PC_5,1);
	}
}


void alk_user_read(void *d){
	int ret;
	int retval;
	struct sockaddr remote_addr;
	retval = 16;
	char buf[32];
	uint16* pt;
	memset(buf,0,32);
	ret = recvfrom (alk_socket_led_fd, buf, 32, MSG_DONTWAIT, &remote_addr, (socklen_t*)&retval);
	pt = (uint16*)buf;
	if(pt[0]==0x6637){
		alk_led_open(0);
	}

	if(pt[0]==0x6636){
		alk_led_open(1);
	}	
	
}

void get_gsensor_msg(){
	static uint8_t counter = 0;
	uint8_t xz_msg[8];
	float angle_road;
	uint16 x_data,z_data;
		
	get_gsensor_x_y_z(xz_msg);
	x_data = (xz_msg[3]<<8) | (xz_msg[1]);
	x_data &= 0xFFFC;
	
	z_data = (xz_msg[7]<<8) | (xz_msg[5]);
	z_data &= 0xFFFC;	
	x_data_buf[counter] = x_data;
	z_data_buf[counter] = z_data;
	counter++;
	if(counter == 2){
		counter = 0;
		x_data = (x_data_buf[0] + x_data_buf[1])/2;
		z_data = (z_data_buf[0] + z_data_buf[1])/2;
		
		angle_road = atan2((float)x_data,(float)z_data);
	
		__disable_irq();
		if(angle_road>0){
			angle_avl = (uint32)((angle_road*180)/PI);
		}
		else if(angle_road<0){
			angle_avl = (uint32)(360+(angle_road*180)/PI);
		}	
		__enable_irq();		

	}
}

void alk_get_gsensor(){
	static uint8_t counter = 0;
	uint8_t itk;
	uint8_t xz_msg[8];
	float angle_road;
	int16 x_data,z_data;
	printf("cheng jie dao ci yi you");
	while(1){
		get_gsensor_x_y_z(xz_msg);
		x_data = (xz_msg[3]<<8) | (xz_msg[1]);
		x_data &= 0xFFFC;
		
		z_data = (xz_msg[7]<<8) | (xz_msg[5]);
		z_data &= 0xFFFC;	
		x_data_buf[counter] = x_data;
		z_data_buf[counter] = z_data;
		counter++;
		if(counter == 10){
			counter = 0;
			x_data = 0;
			z_data = 0;
			for(itk = 0;itk < 10;itk++){
				x_data += x_data_buf[itk];
				z_data += z_data_buf[itk];
			}
			x_data = x_data/10;
			z_data = z_data/10;
		}			
		angle_road = atan2((float)x_data,(float)z_data);
		//printf("angle_road:%6.3f\r\n",angle_road);
		__disable_irq();
		if(angle_road>0){
			angle_avl = (uint32)((angle_road*180)/PI);
		}
		else if(angle_road<0){
			angle_avl = (uint32)(360+(angle_road*180)/PI);
		}	
		__enable_irq(); 	
		


		angle = angle_avl;
	
		//os_printf("==================[%d] [%d] \r\n",angle,angle_avl);


		//os_printf("[%d]",angle);
		if(angle<90){angle=270+angle;}else{angle=angle-90;}
		
		//printf("angle=[%d]\r\n",angle);

		angle_tame[1]=angle;
		//os_printf("=[%d],[%d],[%d],[%d]",angle_tame[0],angle_tame[1],angle_tame[1]-angle_tame[0],angle_tame[0]-angle_tame[0]);
	 
		//angle_tame[0]=angle;
		//if(((angle_tame[1]-angle_tame[0])<3)||((angle_tame[0]-angle_tame[1])<3))
		//{angle=angle_tame[1];}else{angle_tame[1]=angle;}
		if(angle_tame[1]>=angle_tame[0])
		{
			if((angle_tame[1]-angle_tame[0])<5)
			{
				angle_data_num++;
				if((angle_data_num>375)&&(angle_data_flag==0)){angle_data_num=0;angle_data_flag=1;angle_tame[0]=angle;}
				angle=angle_tame[0];
			}else
			{
				angle_data_num=0;
				angle_data_flag=0;
				angle_tame[0]=angle;
			}
		}else{
			if((angle_tame[0]-angle_tame[1])<5)
			{
				angle_data_num++;
				if((angle_data_num>375)&&(angle_data_flag==0)){angle_data_num=0; angle_data_flag=1;angle_tame[0]=angle;}
				angle=angle_tame[0];
			}else
			{
				angle_data_num=0;
				angle_data_flag=0;
				angle_tame[0]=angle;
			}
		}
		//os_printf("=[%d]",angle_data_flag);
	
		if(angle_data_flag){
			wifi_angle=angle_tame[0];
		}else{
			wifi_angle=angle;
		}	
		
		wifi_angle=angle;
		printf("wifi=[%d]\r\n",wifi_angle);

		
		os_sleep_ms(8);
	}	


}

void alk_jpeg_user()				//应用层
{
	uint8_t count = 0;
	uint8_t pkt = 0;
	uint32_t flen;
	int ret;
	int vol_count = 0;
	struct list_head* get_f;
	mjpeg_node* mjn;
	jpeg_frame* jpf;

	uint8_t* jbuf;
	alk_net_msg alk_hdr;
	uint32_t t_now;
	uint8_t resend;
	uint8_t send_frame = 0;
	uint32_t angle_avl_tmp;
	uint32_t *len_p;
	char buf[10];
	struct sockaddr remote_addr;
	int retval,frame_loss;
	int vol = 0;
	float vol_f = 0;
	float vol_sum[5];
	retval = 16;
	global_hgadc_test = (struct hgadc_v0*)dev_get(HG_ADC_DEVID);
	adc_open((struct adc_device *)(global_hgadc_test));
	adc_add_channel(global_hgadc_test, PA_0);
	while(1){
		
		ret = recvfrom (alk_socket_fd, buf, 10, MSG_DONTWAIT, &remote_addr, (socklen_t*)&retval);
		if(ret > 0){
			memcpy(&alk_udp_client, &remote_addr, sizeof(struct sockaddr));
			keep_alive_time = os_jiffies();
			if((buf[0] == 0x20)&&(buf[1] == 0x36)){
				printf("start send...............................\r\n");
				send_frame = 1;
			}
			
			if((buf[0] == 0x20)&&(buf[1] == 0x37)){ 
				printf("end send...............................\r\n");
				send_frame = 0;
			}
		}
		
		//mjpeg_sema_down(-1);
		t_now = os_jiffies();
		//send_frame = 0;		
		if(keep_alive_time){
			if((t_now - keep_alive_time) < 6000){
//				send_frame = 1;
			}
		}

		NVIC_DisableIRQ(JPG_IRQn);
		get_f = get_frame();
		NVIC_EnableIRQ(JPG_IRQn);

		
		if(get_f){
			__disable_irq();
			angle_avl_tmp = wifi_angle;
			angle_avl_tmp &= 0xffff;
			//printf("wifi_angle:%d\r\n",wifi_angle);
			__enable_irq();
			count++;
			if(count == 255){
				count = 0;
			}
			if((count%6) == 0){
				//vol = adc_sample((struct adc_device *)(global_hgadc_test));
				adc_get_value(global_hgadc_test, PA_0, &vol);
				vol_f = ((float)vol/4096)*3.3;
				printf("vol_f:%2.6f,vol:%x\r\n",vol_f,vol);
				vol_f = 100-1000*(4.2-2*vol_f)/8;	//4.2满电，3.4空电为基准
				vol_sum[vol_count]=vol_f;
				vol_count++;
				if(vol_count==5){
					ad_pwr = (vol_sum[0]+vol_sum[1]+vol_sum[2]+vol_sum[3]+vol_sum[4])/5;
					printf("ad_pwr:%2.6f\r\n",ad_pwr);
					vol_count=0;
				}

			}

			
			alk_hdr.frame_num = count;
			alk_hdr.size_H	= 0x50;
			alk_hdr.size_L	= 0x50;
			alk_hdr.is_eof	= 0;
			alk_hdr.pkt_cnt = 0;
			alk_hdr.angle_H = angle_avl_tmp>>8; 	//g_sensor===da280	  angle_pt[0] angle_pt[1]
			alk_hdr.angle_L = angle_avl_tmp&0xff;
			alk_hdr.power	= (uint8_t)ad_pwr;  //adc get power count		
			
			usr_p = get_f;			
			jpf = list_entry(get_f,jpeg_frame,list);
			flen = jpf->frame_len;
			pkt = 0;
			frame_loss = 0; 	
			while(list_empty((struct list_head *)usr_p) != TRUE)
			{
				mjn = list_entry((struct list_head *)usr_p->next,mjpeg_node,list);
				
				//printf("mjn:%x\r\n",mjn);
				/*
					此处使用当前mjn，获取节点对应的buf数据位置，对数据进行处理
				*/
				pkt++;
				resend = 0;
				if(flen > (JPG_BUF_LEN - JPG_HEAD_RESERVER)){
					flen -= (JPG_BUF_LEN - JPG_HEAD_RESERVER);
					jbuf = mjn->buf_addr;
					jbuf -= JPG_HEAD_RESERVER;
					if((send_frame)&&(frame_loss == 0)){
						memcpy(jbuf,&alk_hdr,8);
						os_mutex_lock(&alk_socket_mutex,osWaitForever);
						do{
							if(resend != 0){
								os_sleep_ms(1);
							}							
							ret = sendto(alk_socket_fd, jbuf, JPG_BUF_LEN, MSG_DONTWAIT, &alk_udp_client, sizeof(struct sockaddr));
							resend++;
							if(resend == 11){
								if(ret == -1)
									frame_loss = 1;
								
								break;
							}
						}while(ret == -1);
						
						os_mutex_unlock(&alk_socket_mutex);
						
					}
					
				}
				else{		
					alk_hdr.is_eof	= 1;
					alk_hdr.pkt_cnt = pkt;
					jbuf = mjn->buf_addr;
					jbuf -= JPG_HEAD_RESERVER;
					len_p = (uint32*)&jbuf[flen+JPG_HEAD_RESERVER];
					len_p[1] = jpf->frame_len+3;
					len_p[0] = 0xd9ffff;
					//jbuf[flen+JPG_HEAD_RESERVER-1]=0xff;
					if((send_frame)&&(frame_loss == 0)){
						memcpy(jbuf,&alk_hdr,8);
						os_mutex_lock(&alk_socket_mutex,osWaitForever);
						do{
							if(resend != 0){
								os_sleep_ms(1);
							}
							ret = sendto(alk_socket_fd, jbuf, flen+JPG_HEAD_RESERVER+8, MSG_DONTWAIT, &alk_udp_client, sizeof(struct sockaddr));
							resend++;
							if(resend == 11){
								if(ret == -1)
									frame_loss = 1;
								
								break;
							}

						}while(ret == -1);

						os_mutex_unlock(&alk_socket_mutex); 
					}
				
					flen = 0;
				}
				put_node(&free_tab,usr_p);				//节点归还到free_tab,边处理边给下一帧释放空间
			}
			del_frame(usr_p);							//删除frame			

		}
		else{
			os_sleep_ms(1);
		}
		
	}
}



int alk_udp_create_server(uint16 port)
{
	int socket_c, err;
	struct sockaddr_in addr;

	printf("enter %s\n", __FUNCTION__);
	
	addr.sin_family = AF_INET;
	addr.sin_len = sizeof(struct sockaddr_in);
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htons(INADDR_ANY);

	socket_c = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_c < 0)
	{
		printf("get socket err");
		return  - 1;
	} 
	err = bind(socket_c, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
	if (err ==  - 1)
	{
		close(socket_c);
		return  - 1;

	}

	printf("udp server ok:%d\n", socket_c);
	
	return socket_c;
}



uint32 pwr_det_happen = 0;


void alk_pwr_close_det(){
	uint8_t pwr_det_keep = 0;
	uint32_t count = 0;
	uint32_t time_no_runing = 0;
	time_no_runing = os_jiffies();
	while(1){
		os_sleep_ms(100);
		count++;
		if(count == 4){
			printf("start  pwr.............................\r\n");
			gpio_set_val(PA_1,1);
		}
		
		if(count > 30){
			if(gpio_get_val(PA_6) == 1){
				pwr_det_keep++;
				if(pwr_det_keep == 15){
					printf("close  pwr.............................(pwr det high)\r\n");
					gpio_set_val(PA_15,0);
					gpio_set_val(PC_5,1);
					os_sleep_ms(1000);
					gpio_set_val(PA_1,0);
				}
			}else{
				pwr_det_keep = 0;
			}
		}

		if(count > 30){
			if(ad_pwr <= 0){	
				printf("close  pwr.............................(bat low)\r\n");		
				gpio_set_val(PA_15,0);
				gpio_set_val(PC_5,1);
				os_sleep_ms(1000);			
				gpio_set_val(PA_1,0);
			}
		}

		if(gpio_get_val(PA_8) == 1){   
			printf("input usb\r\n");
			if(gpio_get_val(PA_6) == 1){
				gpio_set_val(PA_2,0);
			}else{	                              //充电
				if((count%4) > 2){
					gpio_set_val(PA_2,0);
				}else{
					gpio_set_val(PA_2,1);
				}
			}
			gpio_set_val(PA_15,1);
			gpio_set_val(PC_5,0);			
		}else{

			gpio_set_val(PA_2,0);
			if(connect_led == 0){
				if((count%16) > 8){
					gpio_set_val(PA_15,1);
				}else{
					gpio_set_val(PA_15,0);
				}
			
				if((os_jiffies() - time_no_runing)>(10*60*1000)){							
					printf("close  pwr.............................(no connect to long)\r\n"); 
					gpio_set_val(PA_15,0);
					gpio_set_val(PC_5,1);
					os_sleep_ms(1000);			
					gpio_set_val(PA_1,0);				
				}
			}else{
				gpio_set_val(PA_15,1);
				time_no_runing = os_jiffies();
			}

		}


		
	}
}

void alk_pwr_close_det1(){
	uint8_t pwr_det_keep = 0;
	uint32_t count = 0;
	uint32_t time_no_runing = 0;
	time_no_runing = os_jiffies();
	while(1){
		os_sleep_ms(100);
		count++;
		if(count == 4){
			printf("start  pwr.............................\r\n");
			gpio_set_val(PA_2,1);
		}

		if(count > 30){
			if(ad_pwr <= 0){	
				printf("close  pwr.............................(bat low)\r\n");		
				gpio_set_val(PA_15,0);
				gpio_set_val(PC_4,0);
				os_sleep_ms(1000);			
				//gpio_set_val(PA_2,0);
			}
		}

		if(gpio_get_val(PA_7) == 1){           
			if(gpio_get_val(PA_12) == 1){
				gpio_set_val(PA_2,0);
			}else{	                              //充电
				if((count%4) > 2){
					gpio_set_val(PA_2,0);
				}else{
					gpio_set_val(PA_2,1);
				}
			}
			gpio_set_val(PA_15,0);
			gpio_set_val(PA_14,0);			
		}else{
			gpio_set_val(PA_2,1);

			if(connect_led == 0){
				if((count%16) > 8){
					gpio_set_val(PA_15,1);
				}else{
					gpio_set_val(PA_15,0);
				}
			
				if((os_jiffies() - time_no_runing)>(10*60*1000)){							
					printf("close  pwr.............................(no connect to long)\r\n"); 
					gpio_set_val(PA_15,0);
					gpio_set_val(PC_4,0);
					os_sleep_ms(1000);			
					//gpio_set_val(PA_2,0);				
				}
			}else{
				gpio_set_val(PA_15,1);
				time_no_runing = os_jiffies();
			}

		}


		
	}
}


void alk_io_init()
{
	gpio_iomap_input(PA_6, GPIO_DIR_INPUT);         //PWR_DET
	gpio_iomap_input(PA_8, GPIO_DIR_INPUT);         //USB_EN
	gpio_iomap_input(PA_6, GPIO_DIR_INPUT);         //PWR_DET
	gpio_iomap_output(PA_15, GPIO_IOMAP_OUTPUT);     //WIFI_LED
	gpio_iomap_output(PA_1, GPIO_IOMAP_OUTPUT);      //PWR_EN
	gpio_iomap_output(PA_2, GPIO_IOMAP_OUTPUT);      //PMM_EN
	gpio_iomap_output(PC_5, GPIO_IOMAP_OUTPUT);      //sensor_led2
	gpio_set_val(PA_1,1);
	gpio_set_val(PC_5,0);	
		
	OS_TASK_INIT("pwr_det", &pwr_det_task, alk_pwr_close_det, 0, OS_TASK_PRIORITY_NORMAL, 256);
}

void alk_io_init1()
{
	gpio_iomap_input(PA_6, GPIO_DIR_INPUT);         //PWR_DET
	gpio_iomap_input(PA_7, GPIO_DIR_INPUT);         //USB_EN
	gpio_iomap_input(PA_12, GPIO_DIR_INPUT);         //PWR_DET
	gpio_iomap_output(PA_15, GPIO_IOMAP_OUTPUT);     //WIFI_LED
	gpio_iomap_output(PA_2, GPIO_IOMAP_OUTPUT);      //PWM_LED
	gpio_iomap_output(PC_4, GPIO_IOMAP_OUTPUT);      //sensor_led1
	gpio_iomap_output(PC_5, GPIO_IOMAP_OUTPUT);      //sensor_led2
	gpio_set_val(PA_1,0);
	gpio_set_val(PA_15,0);	
	gpio_set_val(PC_4,1);
	gpio_set_val(PC_5,1);
		
	OS_TASK_INIT("pwr_det", &pwr_det_task, alk_pwr_close_det1, 0, OS_TASK_PRIORITY_NORMAL, 256);
}



uint8_t g_sensor_init();
void alk_net_data_init()
{
	printf("1jjjjjjjjjjjjjjjjjjjjjjjjjjjj\n");
	uint8_t ret = 0;
	uint16 port = 7080;
//	alk_io_init();
	ret = g_sensor_init();
	if(ret){
		csi_kernel_task_new((k_task_entry_t)alk_get_gsensor, "alk_gsensor", 0, 14, 0, NULL, 1024, &alk_task_fd2);
	}
	printf("> alk_net_data_init\nudp port:%d\n", port);
/* 	os_mutex_init(&alk_socket_mutex);
	alk_socket_fd = alk_udp_create_server(port);	
	keep_alive_time = 0;

	csi_kernel_task_new((k_task_entry_t)alk_jpeg_user, "alk_send_udp_pkt", 0, 15, 0, NULL, 2048, &alk_task_fd);
	
	port = 7090;
	alk_socket_led_fd = alk_udp_create_server(port);
	eloop_add_fd( alk_socket_led_fd, 0, EVENT_F_ENABLED, (void*)alk_user_read, 0 ); */

	

}
#endif

