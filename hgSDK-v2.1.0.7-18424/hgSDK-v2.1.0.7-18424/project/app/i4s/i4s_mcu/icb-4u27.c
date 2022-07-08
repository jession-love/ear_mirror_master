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
#include "hal/gpio.h"
#include "dev/adc/hgadc_v0.h"
#include "i4s.h"


#define GPIO_DET_CHARGE			PA_3
#define GPIO_CTRL_MCU			PA_4
#define GPIO_LED_RED			PA_5
#define GPIO_LED_BLUE			PA_6	
#define GPIO_PWR_KEY			PA_7
#define GPIO_LED_WHITE			PA_14

static volatile int connect_flag = 0;
struct hgadc_v0* global_hgadc_test;
static float ad_pwr1 = 100;
static float ad_pwr2 = 100;
static volatile uint8_t red_led_flag =0;
static uint8_t ban_continue_flag = 0;


static void i4s_gpio_init(void)
{
	//gpio_iomap_input(GPIO_DET_CHARGE, GPIO_DIR_INPUT);         //charge 


    //程杰改
	gpio_iomap_input(GPIO_DET_CHARGE, GPIO_DIR_INPUT);



	gpio_iomap_input(GPIO_PWR_KEY, GPIO_IOMAP_INPUT);         //USB_EN

	gpio_iomap_output(GPIO_CTRL_MCU, GPIO_IOMAP_OUTPUT);     //mcu control

	gpio_iomap_output(GPIO_LED_RED, GPIO_IOMAP_OUTPUT);     //red LED
	gpio_iomap_output(GPIO_LED_BLUE, GPIO_IOMAP_OUTPUT);      //blue_LED
	gpio_iomap_output(GPIO_LED_WHITE, GPIO_IOMAP_OUTPUT);      //white LED
	
	gpio_set_val(GPIO_LED_RED,0);
	gpio_set_val(GPIO_LED_BLUE,0);
	gpio_set_val(GPIO_LED_WHITE,0);
}

static void _ADC_battey_task(void *arg)
{
	
	
	global_hgadc_test = (struct hgadc_v0*)dev_get(HG_ADC_DEVID);
	    adc_open((struct adc_device *)(global_hgadc_test));
	    adc_add_channel(global_hgadc_test, PA_0);
		int vol = 0;
	    float vol_f = 0;
		int vol_count = 0;
		float vol_sum[5]={0};
		 os_sleep_ms(3000);
	    while(1)
		{
		
				//vol = adc_sample((struct adc_device *)(global_hgadc_test));
				adc_get_value(global_hgadc_test,PA_0, &vol);
				vol+=40;
				vol_f = ((float)vol/4096)*3.3*2;
			//	printf("vol_f:%2.6f,vol:%d\r\n",vol_f,vol);
				  
				//printf("-,- - -! vol_f:%2.6f\r\n",vol_f);
				    if(gpio_get_val(GPIO_DET_CHARGE)==1)
					{
						vol_f = vol_f-0.26;
					} 
				printf("12vol_f:%2.6f,vol:%d\r\n",vol_f,vol);	

				vol_f = 100-1000*(4.2-vol_f)/8;	//4.2满电，3.4空电为基准
				vol_sum[vol_count]=vol_f;
				vol_count++;
				if(vol_count==5)
				{
					ad_pwr1 = (vol_sum[0]+vol_sum[1]+vol_sum[2]+vol_sum[3]+vol_sum[4])/5;
					/* if(red_led_flag==0)
					{
					   ad_pwr1+=60;
					}else if(red_led_flag==1)
					{
						ad_pwr1+=26;
					} */
					printf("ad_pwr1:%2.6f\r\n",ad_pwr1);
					vol_count=0;
					if(ad_pwr2>ad_pwr1)
					{
						ad_pwr2=ad_pwr1;
						printf("now battey is ad_pwr2:%2.6f\r\n",ad_pwr2);
						if(ad_pwr2<0)
						{
						    ad_pwr2=0;
						}	
						if(ad_pwr2>90)
						{
							ad_pwr2=90;
						}
						
						set_battey_fun(ad_pwr2);	
					}
					
				}
           os_sleep_ms(500);
			
		}
}


static void _mcu_task(void *arg)
{
	int idx = 0;
	int cnt = 0;
	int time_ctr = 0;
	int time_10_min=0;
	os_printf("11111111111111111\r\n");

	//程杰改
   /*   while(1)
	 {
		if(cnt >= 40){
			os_printf("Power off\r\n");	//	
			gpio_set_val(GPIO_LED_BLUE,0);
			gpio_set_val(GPIO_CTRL_MCU,0);
			gpio_set_val(GPIO_LED_WHITE,0);
			os_printf("Power finish\r\n");	
			while(1);
		}
		if(gpio_get_val(GPIO_PWR_KEY)){
			os_printf("jjjjjjjj power key:%d\r\n", cnt);
			cnt++;
			
			if(cnt%2)
			{
				
				gpio_set_val(GPIO_LED_RED,1);
				gpio_set_val(GPIO_LED_BLUE,0);
			}else
			{
				
                gpio_set_val(GPIO_LED_RED,0);
				gpio_set_val(GPIO_LED_BLUE,1);
			}
		}else
		{
			cnt=0;
		}
		
		os_sleep_ms(200);
	 } */
	 os_sleep_ms(2000);
	while(1){
		
		if(cnt >= 30){
			os_printf("Power off\r\n");	//	
			gpio_set_val(GPIO_LED_BLUE,0);
			gpio_set_val(GPIO_CTRL_MCU,0);
			gpio_set_val(GPIO_LED_WHITE,0);
			os_printf("Power finish\r\n");	
			while(1);
		}
		if(gpio_get_val(GPIO_PWR_KEY)){
			os_printf("power key:%d\r\n", cnt);
			cnt++;
		}else{
			cnt = 0;
		}
        
		 if(gpio_get_val(GPIO_DET_CHARGE)==1)
		 {
			red_led_flag=1;
			
		}else
		{
			//gpio_set_val(GPIO_LED_RED,0);
			red_led_flag=0;

		}
		if(red_led_flag==1&&ban_continue_flag==0)
		{
			gpio_set_val(GPIO_LED_RED,1);
			ban_continue_flag=1;

		}else if(red_led_flag==0&&ban_continue_flag==1)
		{
			gpio_set_val(GPIO_LED_RED,0);
			ban_continue_flag=0;
		}
		
		if(connect_flag&&red_led_flag!=1){
			gpio_set_val(GPIO_LED_BLUE,1);
			time_10_min=0;
		}else{
			if(time_ctr >= 10&&red_led_flag!=1){
				idx ^=0x01;
				gpio_set_val(GPIO_LED_BLUE, idx);
				time_ctr = 0;
				time_10_min++;
			}
		}
		if(time_10_min==1200&&connect_flag!=1)
		{
			os_printf("Power off\r\n");	//	
			gpio_set_val(GPIO_LED_BLUE,0);
			gpio_set_val(GPIO_CTRL_MCU,0);
			gpio_set_val(GPIO_LED_WHITE,0);
			os_printf("Power finish\r\n");
			time_10_min=0;
		}
		os_sleep_ms(50);
		time_ctr++;
	}
}

int i4s_mcu_init(void)
{
	void *thread;
	void *thread1;

	i4s_gpio_init();
	os_printf("----------i4s mcu init--------\r\n");
	
	gpio_set_val(GPIO_CTRL_MCU,1);
	gpio_set_val(GPIO_LED_WHITE,1);
	gpio_set_val(GPIO_LED_RED,0);
	//这个函数类似于开线程
	csi_kernel_task_new((k_task_entry_t)_mcu_task, "i4s_mcu_init", NULL, 9, 0, NULL, 1024, &thread);
	csi_kernel_task_new((k_task_entry_t)_ADC_battey_task, "i4s_mcu_init", NULL, 9, 0, NULL, 1024, &thread1);

	return 0;
}

int i4s_connected(int connect)
{
	connect_flag = connect;

	return 0;
}
