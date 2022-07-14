 #include "typesdef.h"
#include "list.h"
#include "dev.h"
#include "hal/uart.h"
#include "csi_core.h"
#include "csi_kernel.h"
#include "cmd_input.h"
#include "lib/atcmd/libatcmd.h"
#include "dev/uart/hguart_v2.h"
#include "syscfg.h"
#include "lib/syscfg/syscfg.h"
#include "string.h"
#include "sys_config.h"
#include "devid.h"


static struct cmd_list_head global_cmd_list;
static k_task_handle_t cmd_input_handle_test;
static struct hguart_v2 *self_uart;
static void *cmd_input_signal;
static struct cmd_list_struct *last_cmd_list = NULL;
#define UART_CMD_MAX	1200*3

static uint8_t cmd_input_buf[UART_CMD_MAX] __attribute__ ((section(".uart_cmd")));


static cmd_uart_handle_func cmd_uart_handle[CMD_INPUT_MAX] = {NULL};


 
 
int cmd_uart_handle_register(cmd_uart_handle_func func,int index)
{
	int ret = -1;
	if(index<CMD_INPUT_MAX)
	{
		if(cmd_uart_handle[index])
		{
			printf("%s already be registered,will be covered,last func:%X\tnow func:%X\n",__FUNCTION__,(int)cmd_uart_handle[index],(int)func);
		}
		cmd_uart_handle[index] = func;
		ret = 0;
	}

	return ret;
}
int cmd_uart_handle_unregister(int index)
{
	int ret = -1;
	if(index<CMD_INPUT_MAX)
	{
		cmd_uart_handle[index] = NULL;
		ret = 0;
	}

	return ret;

}


int32 cmd_input_rx_timeout(uint32 irq, uint32 data, uint32 rx_len)
{
	struct cmd_list_struct *list;
	int res;
	int array_rx_max = (int)data;
	list = get_cmd_list();
	//空间不足,使用原来的buf,正常不应该在这里出现
	if(!list)
	{
		if(last_cmd_list)
		{
			printf("L");
			uart_gets(&self_uart->dev,(uint8_t*)last_cmd_list->buf,array_rx_max);
		}
		return 1;
	}

	//切换buf,发送信号量
	uart_gets(&self_uart->dev,(uint8_t*)list->buf,array_rx_max);
	last_cmd_list->buf[rx_len] = 0;
	last_cmd_list->rx_len = rx_len;
	res = csi_kernel_msgq_put(cmd_input_signal,&last_cmd_list,0,0);
	//某些原因导致信号量put失败
	if(res)
	{
		if(last_cmd_list)
		{
			free_cmd_list(last_cmd_list);
		}
	}
	last_cmd_list = list;
	return 0;
	//printf("U\n");
}

int32 cmd_input_rx_done(uint32 irq, uint32 data, uint32 rx_len)
{
	struct cmd_list_struct *list;
	int res;
	int array_rx_max = (int)data;
	list = get_cmd_list();
	//空间不足,使用原来的buf,正常不应该在这里出现
	if(!list)
	{
		printf("%s err\n",__FUNCTION__);
		if(last_cmd_list)
		{
			uart_gets(&self_uart->dev,(uint8_t*)last_cmd_list->buf,array_rx_max);
		}
		return 1;
	}
	//切换buf,发送信号量
	uart_gets(&self_uart->dev,(uint8_t*)list->buf,array_rx_max);
	last_cmd_list->rx_len = last_cmd_list->len;
	res = csi_kernel_msgq_put(cmd_input_signal,&last_cmd_list,0,0);
	//某些原因导致信号量put失败,释放对应空间
	if(res)
	{
		if(last_cmd_list)
		{
			free_cmd_list(last_cmd_list);
		}
	}
	
	last_cmd_list = list;
	return 0;

}

uint32 str_2_int(char *s){
	uint8 i = 0;
	uint8 j = 0;
	uint8 len = 0;
	uint32 result = 0;
	s = s+2;
	for(i = 0;i < 8;i++){
		if(((s[i] >= '0') && (s[i] <= '9'))||((s[i] >= 'a') && (s[i] <= 'f'))){
			len++;
		}else{
			break;
		}
	}
	for(i = len;i > 0;i--){
		if((s[i-1] >= '0') && (s[i-1] <= '9'))
		{
			result |= (s[i-1] - '0')<<(j*4);
		}else if((s[i-1] >= 'a') && (s[i-1] <= 'f')){
			result |= (s[i-1] - 'a'+0x0a)<<(j*4);
		}
		j++;
	}
	return result;
}

uint32 str_2_ip(char *s)
{
	uint8 i = 0;
	uint8 j = 0;
	uint8 k = 0;
	uint8 len = 0;	
	uint8 n = 0;
	uint32 result = 0;
	for(k = 0;k < 4;k++){
		len =0;
		for(i = 0;i < 8;i++){
			if((s[i] >= '0') && (s[i] <= '9')){
				len++;
			}else{
				break;
			}
		}
		n = 0;
		j = 1;
		for(i = len;i > 0;i--){
			n += (s[i-1] - '0')*(1*j);
			j=10*j;
		}
		s=s+len+1;
		result = result + (n << (k*8));
	}

	return result;
}


int32 txcmd_recv(const char *data, int32 len)
{
	char *cmd;
	char *arg;
	cmd = (char*)data+3;
	if(NULL != strstr(cmd,"key_mgmt")){
		arg = strchr(data,'=');
		arg++;
		sys_cfgs.key_mgmt = str_2_int(arg);
	}else if(NULL != strstr(cmd,"wifi_mode")){
		arg = strchr(data,'=');
		arg++;
		sys_cfgs.wifi_mode = (uint8)str_2_int(arg);
	}else if(NULL != strstr(cmd,"channel")){
		arg = strchr(data,'=');
		arg++;
		sys_cfgs.channel = (uint8)str_2_int(arg);
	}else if(NULL != strstr(cmd,"ssid")){
		arg = strchr(data,'=');
		arg++;
		memset(sys_cfgs.ssid,0,32);
		memcpy(sys_cfgs.ssid,arg,strlen(arg));
	}else if(NULL != strstr(cmd,"key")){
		arg = strchr(data,'=');
		arg++;
		memset(sys_cfgs.key,0,32);
		memcpy(sys_cfgs.key,arg,strlen(arg));		
	}else if(NULL != strstr(cmd,"ipaddr")){
		arg = strchr(data,'=');
		arg++;
		sys_cfgs.ipaddr = str_2_ip(arg);
	}else if(NULL != strstr(cmd,"netmask")){
		arg = strchr(data,'=');
		arg++;
		sys_cfgs.netmask = str_2_ip(arg);
	}else if(NULL != strstr(cmd,"gw_ip")){
		arg = strchr(data,'=');
		arg++;
		sys_cfgs.gw_ip = str_2_ip(arg);
	}else if(NULL != strstr(cmd,"dhcp_startip")){
		arg = strchr(data,'=');
		arg++;
		sys_cfgs.dhcpd_startip = str_2_ip(arg);
	}else if(NULL != strstr(cmd,"dhcp_endip")){
		arg = strchr(data,'=');
		arg++;
		sys_cfgs.dhcpd_endip = str_2_ip(arg);
	}else if(NULL != strstr(cmd,"save")){
		syscfg_save();
	}else if(NULL != strstr(cmd,"ota"))	{
		k_task_handle_t ota_task_handle;
		static int ota_running = 0;
		extern int32 xmodem_ota(const char *cmd, char *argv[], uint32 argc);
		if(ota_running == 0)
		{
			printf("111%s:%d\n",__FUNCTION__,__LINE__);
#if DVP_EN	
			void *dvp = (void *)dev_get(HG_DVP_DEVID);
			if(dvp)
			{
                int32 dvp_close(struct dvp_device *p_dvp);
				dvp_close(dvp);
			}
#endif
						
#if USB_EN	
			SYSCTRL_REG_OPT(sysctrl_usb20_clk_close());
#endif
			csi_kernel_task_new((k_task_entry_t)xmodem_ota, "xmodem_ota", &ota_running, 5, 0, NULL, 2048, &ota_task_handle);
		}
		
	}else{
		printf("no cmd[%s]\r\n",cmd);
	}
	return 0;
	
}

/*********************************************************************************
arg:  
	buf:串口输入的数据
    len:串口输入数据的长度

注意:函数不建议调用耗时操作,如果调用耗时操作,在串口输入数据频繁的时候,会导致缓冲
     数据满后丢失数据
**********************************************************************************/

__attribute__((weak)) int cmd_input_user_deal(const char *buf,unsigned int len)
{
	printf("%s:%s,len:%d\n",__FUNCTION__,buf,len);
     #if 1
    //for lmac at cmd
    if(((buf[0] == 'A')||(buf[0] == 'a'))&&((buf[1] == 'T')||(buf[1] == 't'))){
		atcmd_recv((uint8*)buf, len);
	}else if(((buf[0] == 'T')||(buf[0] == 't'))&&((buf[1] == 'X')||(buf[1] == 'x'))){
		txcmd_recv(buf, len);
	}    
	#endif
	return 0;
}
//hal_irq *uart_hal;
//int32 hal_handler_irq(uint32 irq, uint32 irq_data, uint32 param);

int32 uart_handler_irq(uint32 irq, uint32 irq_data, uint32 param1, uint32 param2)
{
	if(UART_IRQ_FLAG_TIME_OUT == irq)
		cmd_input_rx_timeout(irq,irq_data,param1);

	if(UART_IRQ_FLAG_DMA_RX_DONE == irq)
		cmd_input_rx_done(irq,irq_data,param1);
		
	return 0;
}



void cmd_input_test_thread(void *d)
{
	struct cmd_list_struct *head;
	struct cmd_list_struct *list;
	int i;
	int res;
	int max_len = (int)d;
	printf("%s max_len:%d\n",__FUNCTION__,max_len);
	cmd_input_signal = csi_kernel_msgq_new(global_cmd_list.count,sizeof(void*));
	//9216是921600波特率, timeout = (to_bit_len+1)*baud
	uart_ioctl(&self_uart->dev,UART_IOCTL_CMD_SET_TIME_OUT,9216/5,1);
	uart_request_irq(&self_uart->dev,uart_handler_irq,UART_IRQ_FLAG_TIME_OUT|UART_IRQ_FLAG_DMA_RX_DONE,(uint32_t)max_len);
	uart_ioctl(&self_uart->dev,UART_IOCTL_CMD_USE_DMA,1,0);	//要手动开启是否为dma
	list = get_cmd_list();
	last_cmd_list = list;
	ASSERT(list != NULL);
	if(list)
	{
		uart_gets(&self_uart->dev,(uint8_t*)list->buf,max_len);
	}
	while(1)
	{
		res = csi_kernel_msgq_get(cmd_input_signal,&head,-1);
		//printf("head->buf:%s,len:%d\n",head->buf,head->rx_len);
		//命令分析或者缓存,由用户决定
		for(i=0;i<CMD_INPUT_MAX;i++)
		{
			if(cmd_uart_handle[i])
			{
				cmd_uart_handle[i]((const char *)head->buf,head->rx_len);
			}
		}
		free_cmd_list(head);
	}
}


/***************************************************
串口命令buf的初始化
***************************************************/

int cmd_init(void *cmd_uart,unsigned int num,unsigned int len)
{
	if(!cmd_uart)
	{
		printf("%s:%d input arg uart is NULL\n",__FUNCTION__,__LINE__);
		return 1;
	}
	self_uart = (struct hguart_v2 *)cmd_uart;
	struct cmd_list_struct *head;
	int max_len = len;
	unsigned char *buf = NULL;
	//因为要地址4byte对齐,所以需要计算4byte对齐,但接收依然按照实际
	len = ((len+3)&  (~0x03));
	printf("len:%d\n",len);
	unsigned int i;
	INIT_LIST_HEAD(&global_cmd_list.list);
	if(UART_CMD_MAX >= len+sizeof(struct cmd_list_struct))
	{
		buf = cmd_input_buf;
	}
	//unsigned char *buf = (unsigned char*)malloc(num*(len+sizeof(struct cmd_list_struct)));
	unsigned char *tmp_buf;
	if(!buf)
	{
		goto cmd_init_err;
	}
	global_cmd_list.freebuf = buf;
	tmp_buf = buf;
	
	for(i=0;i<num;i++,tmp_buf+=(len+sizeof(struct cmd_list_struct)))
	{
		head = (struct cmd_list_struct*)tmp_buf;
		INIT_LIST_HEAD(&head->list);
		head->len = max_len;
		head->buf = (unsigned char*)head+sizeof(struct cmd_list_struct);
		#if CMD_CHECK == 1
			printf("%s head:%X\thead->buf:%X\n",__FUNCTION__,(int)head,(int)head->buf);
			head->magic = CMD_CHECK_PASSWORD;
		#endif
		list_add_tail(&head->list, &global_cmd_list.list);
	}
	global_cmd_list.count = num;

	//默认注册一个函数
	cmd_uart_handle_register(cmd_input_user_deal,CMD_INPUT_DEFAULT);
	csi_kernel_task_new((k_task_entry_t)cmd_input_test_thread, "cmd_input", (void*)max_len, 10, 0, NULL, 512,&cmd_input_handle_test);
	return 0;
	cmd_init_err:
	if(buf)
	{
		free(buf);
		global_cmd_list.freebuf = NULL;
	}
	return 1;
}

/****************************************
从cmd_buf链表获取一个buf(前提要预先初始化)
*****************************************/
struct cmd_list_struct *get_cmd_list()
{
	struct cmd_list_struct *head;
	if(list_empty(&global_cmd_list.list))
	{
		return NULL;
	}
	head = list_first_entry(&global_cmd_list.list, struct cmd_list_struct, list);
	if(head)
	{
		#if CMD_CHECK == 1
				//printf("%s head:%X\thead->buf:%X\n",__FUNCTION__,(int)head,(int)head->buf);
				ASSERT(head->magic == CMD_CHECK_PASSWORD);
		#endif
		list_del(&head->list);
		return head;
	}
	//不应该出现这种情况
	else
	{
		printf("%s err\n",__FUNCTION__);
		return NULL;
	}
	
}


void free_cmd_list(struct cmd_list_struct *free_list)
{
	if(!free_list)
	{
		return;
	}
	struct cmd_list_struct *head = free_list;
	if(head)
	{
		#if CMD_CHECK == 1
			//printf("%s head:%X\tfree_list:%X\n",__FUNCTION__,(int)head,(int)free_list);
			ASSERT(head->magic == CMD_CHECK_PASSWORD);
		#endif

		list_add_tail(&head->list, &global_cmd_list.list);
		return ;
	}
	//不应该出现这种情况
	else
	{
		printf("%s err\n",__FUNCTION__);
		return ;
	}

	
}

