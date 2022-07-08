
#include "sys_config.h"
#include "typesdef.h"
#include "list.h"
#include "dev.h"
#include "devid.h"
#include "osal/string.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/msgqueue.h"
#include "osal/irq.h"
#include "osal/task.h"
#include "osal/sleep.h"
#include "tx_platform.h"

#include "hal/usb_device.h"
#include "lib/usb/usb_device_wifi.h"
//#include "hal/usb_device_audio.h"
#include "jpgdef.h"
#include "dev/usb/hgusb20_v1_dev_api.h"
#include "dev/usb/hgusb_dev_tbl.h"
#include "hal/gpio.h"
#include "dev/usb/uvc_host.h"

#define USB_TEST_WIFI           1
#define USB_TEST_AUDIO          0


struct hg_usb_device_test {
#if USB_TEST_WIFI

    //正常host不需要使用
    #if USB_HOST_EN
    int8   rx_buf[4];   /* usb rx buffer reserved 4byte for no overflow control */
    int8   tx_buf[4];

    #else
    int8   rx_buf[512*16+4];   /* usb rx buffer reserved 4byte for no overflow control */
    int8   tx_buf[512*16];
    #endif
#endif
#if USB_TEST_AUDIO
    int8   rx_buf[512*4+4];
    int8   tx_buf[512*4];
#endif
    int32  ready;
    uint32 total_tx_len;
    uint32 total_rx_len;
    
    struct os_mutex    tx_lock;
    struct usb_device *dev;
    struct os_task state_task;
    
} usb_test;

#if USB_TEST_WIFI
static void usb_bus_irq_test(uint32 irq, uint32 param1, uint32 param2, uint32 param3)
{
    switch (irq) {
        case USB_DEV_RESET_IRQ: 
            os_printf("reset\r\n");
            usb_device_wifi_read(usb_test.dev, usb_test.rx_buf, sizeof(usb_test.rx_buf));
            usb_test.ready = 1;
            break;
        case USB_DEV_SUSPEND_IRQ: break;
        case USB_DEV_RESUME_IRQ: break;
        case USB_DEV_SOF_IRQ: break;
        case USB_DEV_CTL_IRQ: break;
        case USB_EP_RX_IRQ: 
            //os_printf("rx=%d\r\n", param3);
            usb_test.total_rx_len += param3;
            usb_test.ready = 1;
            usb_device_wifi_read(usb_test.dev, usb_test.rx_buf, sizeof(usb_test.rx_buf));
            usb_test.ready = 1;
            usb_device_wifi_write(usb_test.dev, usb_test.rx_buf, param3);
            break;
        case USB_EP_TX_IRQ: 
            //os_printf("tx_ok\r\n");
            //os_mutex_unlock(&usb_test.tx_lock);
            usb_test.ready = 1;
            usb_test.total_tx_len += param3;
            break;
        default:
            break;
    }
}

static void hg_usb_test_state(void *argument)
{
    os_printf("hg_usb_test_state running ...\r\n");
    while (1) {
        os_sleep(5);
        os_printf("RX/TX Speed:%dKB/s, %dKB/s\r\n",
                  (usb_test.total_rx_len / 1024) / 5,
                  (usb_test.total_tx_len / 1024) / 5);
        usb_test.total_rx_len = 0;
        usb_test.total_tx_len = 0;
    }
}






struct os_semaphore     global_delay_sem;
//struct test_start         global_start;
k_task_handle_t global_delay_test_handle;
k_task_handle_t global_uvc_handle;





#if 0
//需要查找有没有足够bank才能去kick
void delay_kick(void *d)
{
    global_start.dev = NULL;
    global_start.buf = NULL;
    global_start.len = 0;
    while(1)
    {
        os_sema_down(&global_delay_sem, osWaitForever); 
        if(global_start.dev)
        {
            os_sleep_ms(1);
            //printf("d:%d\n",krhino_sys_tick_get()-time);
            //time = krhino_sys_tick_get();

            usb_uvc_rx_kick_dma_start(global_start.dev,(uint32)global_start.buf,global_start.len);

        }
    }
}
#endif

extern void uvc_user2(void *d);
int32 usb_host_uvc_ioctl(struct usb_device *p_usb_d, uint32 cmd, uint32 param1, uint32 param2);

void hg_usb_disable()
{
    usb_device_request_irq(usb_test.dev, NULL, 0);

}

void uvc_ioctl_index(uint8 uvc_idx){
    uint8 msgbuf[1];
    uint8* pt;

    struct usb_device *dev;
    dev = usb_test.dev;
    pt = msgbuf;
    pt[0] = uvc_idx;
    usb_host_uvc_ioctl(dev,USB_HOST_IO_CMD_SET_IDX,msgbuf,0);   
}




k_task_handle_t uvcavi_handle;
extern void uvc_avi_thread(void *d);
extern void uvc_user3(void *d);
extern void uvc_user();
struct uvc_user_arg uvc_arg;
void usb_hub_device_insert(struct hgusb20_dev *p_dev);
int enum_deal= 0;
extern int usb_dma_irq_times;
static void usb_host_enum_task(void *argument)
{
    struct hgusb20_dev *p_dev = (struct hgusb20_dev *)argument;
    while (1) {
        os_sema_down(&p_dev->ep0_sema, osWaitForever);                              //收到connect信号，开始枚举        
        bool usb_uvc_host_enum(struct hgusb20_dev *p_dev);
        usb_uvc_host_enum(p_dev);
		enum_deal = 1;
    }
}

void usb_iso_device_connect_det(void *argument){
    struct hgusb20_dev *p_dev = (struct hgusb20_dev *)argument;
	int count_iso = 0;
	while(1){
		printf("enum_deal:%d  count_iso:%d  usb_dma_irq_times:%d\r\n",enum_deal,count_iso,usb_dma_irq_times);
		if(enum_deal){
			if(count_iso == usb_dma_irq_times){
				hgusb20_host_reset_phy(p_dev);	
				enum_deal = 0;
			}
			count_iso = usb_dma_irq_times;
		}
		os_sleep_ms(2000);
	}
}

void usb_host_task_init(struct hgusb20_dev *p_dev)
{
    os_sema_init(&p_dev->ep0_sema, 0);
	os_sema_init(&p_dev->hub_sema, 0);
	os_sema_init(&p_dev->hub_finish_sema, 0);
    OS_TASK_INIT((const uint8 *)"USB_ENUM", &p_dev->ep0_task,
                 usb_host_enum_task, (uint32)p_dev, OS_TASK_PRIORITY_BELOW_NORMAL, 2048);
	
    OS_TASK_INIT((const uint8 *)"USB_HUB", &p_dev->ep0_hub_task,
                 usb_hub_device_insert, (uint32)p_dev, OS_TASK_PRIORITY_BELOW_NORMAL, 1024);	
	
    OS_TASK_INIT((const uint8 *)"USB_DISCONN", &p_dev->ep0_usb_connect_task,
                 usb_iso_device_connect_det, (uint32)p_dev, OS_TASK_PRIORITY_BELOW_NORMAL, 1024);	
}

void hg_usb_test(void)
{
#if USB_HOST_EN
    os_printf("hg_usb20_test init2 \r\n");
    os_sema_init(&global_delay_sem, 0);

    //csi_kernel_task_new((k_task_entry_t)delay_kick, "delay_kick", 0, 40, 0, NULL, 512,&global_delay_test_handle);
    //os_sleep_ms(1000);
    usb_test.dev = (struct usb_device *)dev_get(HG_USBDEV_DEVID);
    if (usb_test.dev) {
        usb_host_task_init(usb_test.dev);
        if (!usb_device_open(usb_test.dev, NULL)) {
            printf("%s:%d\n",__FUNCTION__,__LINE__);
            uint32 usb_host_bus_irq(uint32 irq, uint32 param1, uint32 param2, uint32 param3);
			
            usb_device_request_irq(usb_test.dev, usb_host_bus_irq, (int32)usb_test.dev);
        }
    }
#if FRAME_MANAGEMENT
    uvc_app_user_init();
    uvc_arg.state = 0;
    uvc_arg.usb_dev = (void*)usb_test.dev;

    //csi_kernel_task_new((k_task_entry_t)uvc_user3, "uvc_user3", &uvc_arg, 15, 0, NULL, 1024,&global_uvc_handle);
#else
    uvc_sema_init();
#if         TEST_UVC_DEBUG
    csi_kernel_task_new((k_task_entry_t)uvc_user2, "uvc_user2", usb_test.dev, 15, 0, NULL, 1024,&global_uvc_handle);
#else
    csi_kernel_task_new((k_task_entry_t)uvc_user, "uvc_user", usb_test.dev, 15, 0, NULL, 1024,&global_uvc_handle);
#endif

#endif  

    //uvc_user2();
#else

    os_printf("hg_usb20_test init \r\n");
    usb_test.dev = (struct usb_device *)dev_get(HG_USBDEV_DEVID);

    if (usb_test.dev) {
        if (!usb_device_open(usb_test.dev, (struct usb_device_cfg *)&usb_dev_wifi_cfg)) 
        {
            usb_device_wifi_auto_tx_null_pkt_enable(usb_test.dev);
            usb_device_request_irq(usb_test.dev, usb_bus_irq_test, (int32)usb_test.dev);

            os_printf("hg_usb20_test start \r\n");

            OS_TASK_INIT("hg_usb_test",&usb_test.state_task, hg_usb_test_state, NULL, OS_TASK_PRIORITY_NORMAL, 1024);
        }
    }

#endif

    
}
#endif





