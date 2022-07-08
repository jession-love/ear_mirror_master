#ifndef USB20_DEV_API_H
#define USB20_DEV_API_H

/// include & define here
#include "sys_config.h"
#include "hal/usb_device.h"
#include "dev/usb/hgusb20_v1_base.h"
#include "dev/usb/hgusb20_v1_dev_ep0.h"

#define USB_DEV_DEBUG_ERR_ENABLE           0
#if USB_DEV_DEBUG_ERR_ENABLE
#define USB_DEV_ERR_PRINTF(fmt, args...)               os_printf(fmt, ##args)
#else
#define USB_DEV_ERR_PRINTF(fmt, args...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8 ep0_pkt;          //
    uint8 max_lun;          //MaxLun
    uint8 bulk_rx;          //
    uint8 bulk_tx;          //
    uint16 time_out;         //
    uint8 stall;
    uint8 if_mass;
    uint8 res;
    uint16 rx_pkt_size;
    uint16 tx_pkt_size;
} type_usb_host;


typedef struct {
    uint8 offset;
    uint8 rx_len;
    uint16 rx_total;
} type_ep0_ptr;

struct hgusb20_dev {
    struct usb_device       dev;
    void                   *usb_hw;
    uint32                  test_mode;
    long                    flags;
    usbdev_irq_hdl          irq_hdl;
    uint32                  ep_irq_num;
    uint32                  dma_irq_num;
    uint8                  *rx_buff;
    uint32                  rx_len;
    uint32                  opened;
    uint32                  irq_data;
    struct usb_device_cfg   cfg;

    /* basic */
    struct hgusb20_dev_chk     usb_chk;
    struct hgusb20_dev_ctrl    usb_ctrl;
    struct hgusb20_dev_info    usb_dev;
    type_ep0_ptr               ep0_ptr;
    type_usb_host              usb_host;
    
    struct hgusb20_audio_ctrl  usb_audio_ctl;
    volatile uint32_t          usb_dma_int;  /* dma intr status reg value */
    volatile uint32_t          usb_rxdma_addr;  /* rx dma start addr */
    volatile uint32_t          usb_rx_len_limit; /* rx req len */
    volatile uint16_t          usb_rxdma_len; /* latest packet len */
    volatile uint8_t           usb_rxdma_flag;
#define USBDMA_RX_FLAG_RXING    BIT(0)
#define USBDMA_RX_FLAG_OVERFLOW BIT(1)
#define USBDMA_RX_FLAG_FIFOFULL BIT(2)

    volatile uint8_t           usb_txdma_flag;
#define USBDMA_TX_FLAG_TXING        BIT(0)
#define USBDMA_TX_FLAG_NULLPACKET   BIT(1)
#define USBDMA_TX_FLAG_MODE1        BIT(2)

    volatile uint32_t          usb_tx_len_limit; /* tx dma len */
    volatile uint16_t          usb_tx_total_len;    /*  */
    volatile uint8_t           rx_dma_done;
    volatile uint8_t           tx_dma_done;
    
    uint8_t                 rsv1;
    uint8_t                 usb_exit ;
    uint8_t                 usb_dev_flag ;
    uint8_t                 rsv[3];
    uint8_t                 usb_ep0buf[68];
    uint8_t                 usb_ep0_rxbuf[68];
    uint8_t                 usb_device_descriptor[18];
    uint8_t                 usb_device_qualifier[12];
    uint8_t                 usb_config_all_descriptor[512];
    uint8_t                 usb_str_serial_number[30];

    uint16_t                usb_vid;    /*  */
    uint16_t                usb_pid;    /*  */

    /* os */
    struct os_task          ep0_task __attribute__((aligned(4)));
	struct os_task          ep0_hub_task __attribute__((aligned(4)));
	struct os_task          ep0_usb_connect_task __attribute__((aligned(4)));
    struct os_semaphore     ep0_sema __attribute__((aligned(4)));
    struct os_mutex         tx_lock __attribute__((aligned(4)));
    struct os_semaphore     tx_done __attribute__((aligned(4)));
	struct os_semaphore     hub_sema __attribute__((aligned(4)));
	struct os_semaphore     hub_finish_sema __attribute__((aligned(4)));
}__packed;

/* hgusb20_base */
/** 
  * @brief  read usb register
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep      : 
  * @param  addr    : register address offset
  * @param  data    : data return 
  * @return         : TRUE or FLASE
  * @note  
  */
bool hgusb20_read_reg(struct hgusb20_dev *p_dev, uint8 ep, uint32 addr, uint32 *data);
/** 
  * @brief  write usb register
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep      : 
  * @param  addr    : register address offset
  * @param  data    : data return 
  * @return         : TRUE or FLASE
  * @note  
  */
bool hgusb20_write_reg(struct hgusb20_dev *p_dev, uint8 ep, uint32 addr, uint32 data);

void hgusb20_switch_otg(struct hgusb20_dev *p_dev);
void hgusb20_switch_host(struct hgusb20_dev *p_dev);
void hgusb20_switch_stable_host(struct hgusb20_dev *p_dev);
void hgusb20_switch_stable_device(struct hgusb20_dev *p_dev);
void hgusb20_switch_stable_otg(struct hgusb20_dev *p_dev);
void hgusb20_intr_dis(struct hgusb20_dev *p_dev);

uint32 hgusb20_get_rx_crc_error_cnt(struct hgusb20_dev *p_dev);

/** 
  * @brief  usb ep rx data kick
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep      : [1,2]
  * @param  addr    : data address 
  * @param  len     : data length 
  * @return         : TRUE or FLASE
  * @note  use for host & device
  */
bool hgusb20_ep_rx_kick(struct hgusb20_dev *p_dev, uint8_t ep, uint32_t addr, uint32_t len);
/** 
  * @brief  usb ep tx data kick
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep      : [1,2]
  * @param  addr    : data address 
  * @param  len     : data length 
  * @return         : TRUE or FLASE
  * @note  use for host & device
  */
bool hgusb20_ep_tx_kick(struct hgusb20_dev *p_dev, uint8_t ep, uint32_t addr, uint32_t len);

/** 
  * @brief  usb ep get rx data length from dma 
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep      : [1,2]
  * @return         : rx data length
  * @note  use for host & device
  */
uint32_t hgusb20_ep_get_dma_rx_len(struct hgusb20_dev *p_dev, uint8_t ep);
/** 
  * @brief  usb ep get rx data length from sie 
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep      : [1,2]
  * @return         : rx data length
  * @note  use for host & device
  */
uint32_t hgusb20_ep_get_sie_rx_len(struct hgusb20_dev *p_dev, uint8_t ep);
/** 
  * @brief  usb ep get tx data length  
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep      : [1,2]
  * @return         : tx data length (same with len in hgusb20_ep_tx_kick)
  * @note  use for host & device
  */
uint32 hgusb20_ep_get_tx_len(struct hgusb20_dev *p_dev, uint8 ep);


/** 
  * @brief  check if usb device online
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  *         - TRUE    : online.
  *         - FALSE   : not online.
  * @note             : For USB Host
  */
bool hgusb20_is_device_online(struct hgusb20_dev *p_dev);

/** 
  * @brief  check if usb host online
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  *         - TRUE    : online.
  *         - FALSE   : not online.
  * @note             : For USB Device
  */
bool hgusb20_is_host_online(struct hgusb20_dev *p_dev);

/** 
  * @brief  suspend usb device .
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return          : 
  * @note            : For USB Host
  */
void hgusb20_host_suspend(struct hgusb20_dev *p_dev);
/** 
  * @brief  resume usb device .
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return          : 
  * @note            : For USB Host
  */
void hgusb20_host_resume(struct hgusb20_dev *p_dev);

/** 
  * @brief  usb host set address(host address)
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  addr     : 
  * @return 
  * @note   use after usb_host_set_address(device address) cmd
  */
void hgusb20_set_address(struct hgusb20_dev *p_dev, uint8 addr);

/** 
  * @brief  usb host bus reset request
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note   
  */
uint8 hgusb20_host_reset(struct hgusb20_dev *p_dev);

/** 
  * @brief  remote_wakeup usb host .
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return          : 
  * @note            : For USB Device
  */
void hgusb20_dev_remote_wakeup(struct hgusb20_dev *p_dev);


/* hgusb20_dev_api */
/** 
  * @brief  attach usb device
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  dev_id  : 
  * @return         : err_code
  * @note  
  */
int32 hgusb20_dev_attach(uint32 dev_id, struct hgusb20_dev *p_dev);

/** 
  * @brief  init usb to device mode .
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  *         - RET_OK  : Successfully.
  *         - RET_ERR : Failed.
  * @note
  */
bool hgusb20_dev_init(struct hgusb20_dev *p_dev);

/** 
  * @brief  exit usb to device mode .
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  *         - RET_OK  : Successfully.
  *         - RET_ERR : Failed.
  * @note
  */
void hgusb20_dev_exit(struct hgusb20_dev *p_dev);

/** 
  * @brief  usb device ep0 tx one packet: 
  *         addr = p_dev->ep0_ptr; len = p_dev->ep0_len;
  *         1、if buf is not NULL（first packet）, update p_dev->ep0_ptr & p_dev->ep0_len
  *         2、TX DATA only one packet 
  *         3、data end if len != ep0_pkt
  *         4、auto called repeat when last tx data done while (p_dev->usb_dev.ep0_state == TRANSER_OVER_STATE)
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  buf      : valid when first packet, set NULL when other packet
  * @param  len      : valid when first packet, set 0 when other packet.
  * @return 
  *         - RET_OK  : Successfully.
  *         - RET_ERR : Failed.
  * @note             : weak function, Users can use a strong statement function replace the weak statement function.
  */
bool hgusb20_dev_ep0_tx(struct hgusb20_dev *p_dev, void *buf, uint16_t len);
/** 
  * @brief  usb ep rx data kick
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep      : [1,2]
  * @param  addr    : data address 
  * @param  len     : data length 
  * @return         : TRUE or FLASE
  * @note  use for host & device
  */
bool hgusb20_dev_ep0_rx(struct hgusb20_dev *p_dev, void *buf, uint16_t len);

/** 
  * @brief  usb device clear_stall_flag
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note
  */
void hgusb20_dev_clear_stall_flag(struct hgusb20_dev *p_dev);
/** 
  * @brief  usb device tx zero-packet & rx packet to usb_ep0_rxbuf
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note
  */
bool hgusb20_dev_ep0_clrrx_pkt0(struct hgusb20_dev *p_dev);

/** 
  * @brief  usb device bus reset
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note
  */
void hgusb20_dev_reset(struct hgusb20_dev *p_dev);
/** 
  * @brief  usb device retrive rx packet to usb_ctrl.cmd
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note
  */
void hgusb20_dev_ep0_get_request(struct hgusb20_dev *p_dev);
/** 
  * @brief  usb device stall_ep
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  epnum    : epnum, set BIT(7) when tx endpiont.
  * @return 
  * @note
  */
void hgusb20_dev_stall_ep(struct hgusb20_dev *p_dev, uint8_t epnum);
/** 
  * @brief  usb device clear stall_ep
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  epnum    : epnum, set BIT(7) when tx endpiont.
  * @return 
  * @note
  */
void hgusb20_dev_clear_ep(struct hgusb20_dev *p_dev, uint8_t epnum);
/** 
  * @brief  usb device ep reset all endpiont in ep_cfg
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note
  */
void hgusb20_dev_bot_reset(struct hgusb20_dev *p_dev);
/** 
  * @brief  usb device cmd handle ：get descriptor
  *         1、kick next rx to p_dev->usb_ep0_rxbuf
  *         2、tx data(addr = buf, length = len)
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return         : TRUE or FLASE
  * @note
  */
bool hgusb20_dev_ep0_descriptor(struct hgusb20_dev *p_dev, void *buf, uint16_t len);
/** 
  * @brief  usb device standard cmd handle 
  *         1、
  *         2、handle DATA(IN or OUT)
  *         3、STATUS(zero-len packet)  
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note            : weak function, Users can use a strong statement function replace the weak statement function.
  *                    these function is called by func "hgusb20_dev_ep0_standard"
  */
__weak bool hgusb20_dev_ep0_get_descriptor(struct hgusb20_dev *p_dev);
__weak bool hgusb20_dev_ep0_get_configuration(struct hgusb20_dev *p_dev);
__weak bool hgusb20_dev_ep0_set_configuration(struct hgusb20_dev *p_dev);
__weak bool hgusb20_dev_ep0_get_interface(struct hgusb20_dev *p_dev);
__weak bool hgusb20_dev_ep0_set_interface(struct hgusb20_dev *p_dev);
__weak bool hgusb20_dev_ep0_set_address(struct hgusb20_dev *p_dev);
__weak bool hgusb20_dev_ep0_get_status(struct hgusb20_dev *p_dev);
__weak bool hgusb20_dev_ep0_clr_feature(struct hgusb20_dev *p_dev);
__weak bool hgusb20_dev_ep0_set_feature(struct hgusb20_dev *p_dev);

/** 
  * @brief  usb device ep0 setup handle (standard cmd)
  *         1、
  *         2、handle DATA(IN or OUT)
  *         3、STATUS(zero-len packet)
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return         : TRUE or FLASE
  * @note            : weak function, Users can use a strong statement function replace the weak statement function.
  *                    the function is called by func "hgusb20_dev_ep0_request"
  */
__weak bool hgusb20_dev_ep0_standard(struct hgusb20_dev *p_dev);
//__weak bool hgusb20_dev_ep0_class_reset(struct hgusb20_dev *p_dev);
//__weak bool hgusb20_dev_ep0_class_get_lun(struct hgusb20_dev *p_dev);
//__weak bool hgusb20_dev_ep0_audio_sample_ctrl(struct hgusb20_dev *p_dev);
//__weak bool hgusb20_dev_ep0_audio_mute_ctrl(struct hgusb20_dev *p_dev);
//__weak bool hgusb20_dev_ep0_audio_volume_ctrl(struct hgusb20_dev *p_dev);

/** 
  * @brief  usb device ep0 setup handle (class cmd)
  *         1、
  *         2、handle DATA(IN or OUT)
  *         3、STATUS(zero-len packet)
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return         : TRUE or FLASE
  * @note            : weak function, Users can use a strong statement function replace the weak statement function.
  *                    the function is called by func "hgusb20_dev_ep0_request"
  */
__weak bool hgusb20_dev_ep0_class(struct hgusb20_dev *p_dev);
/** 
  * @brief  usb device ep0 setup handle (vendor cmd)
  *         1、
  *         2、handle DATA(IN or OUT)
  *         3、STATUS(zero-len packet)
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return         : TRUE or FLASE
  * @note            : weak function, Users can use a strong statement function replace the weak statement function.
  *                    the function is called by func "hgusb20_dev_ep0_request"
  */
__weak bool hgusb20_vendor_cmd(struct hgusb20_dev *p_dev);
/** 
  * @brief  usb device ep0 setup handle
  *         1、
  *         2、handle DATA(IN or OUT)
  *         3、STATUS(zero-len packet)
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return         : TRUE or FLASE
  * @note            : weak function, Users can use a strong statement function replace the weak statement function.
  *                    
  */
__weak bool hgusb20_dev_ep0_request(struct hgusb20_dev *p_dev);
/** 
  * @brief  usb device ep0 receive data handle, DATA(OUT) interrupt, such as usb videa "set volume"
  *         1、
  *         2、handle DATA
  *         3、STATUS(tx zero-len packet)
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note            : weak function, Users can use a strong statement function replace the weak statement function.
  *                    
  */
__weak void hgusb20_dev_ep0_receive(struct hgusb20_dev *p_dev);

///** 
//  * @brief  usb device ep0 interrupt handle
//  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
//  * @return 
//  * @note            : weak function, Users can use a strong statement function replace the weak statement function.
//  *                    !! Don't recommend  a strong statement function
//  */
//__weak void hgusb20_dev_ep0_func(struct hgusb20_dev *p_dev);



/* hgusb20_host_api */

/** 
  * @brief  attach usb host
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  dev_id  : 
  * @return         : err_code
  * @note  
  */
int32 hgusb20_host_attach(uint32 dev_id, struct hgusb20_dev *p_dev);

/** 
  * @brief  init usb to host mode .
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  *         - RET_OK  : Successfully.
  *         - RET_ERR : Failed.
  * @note
  */
bool hgusb20_host_init(struct hgusb20_dev *p_dev);

/** 
  * @brief  usb host ep0 cmd request
  *         1. SETUP: send setup packet , addr = p_dev->usb_ctrl.cmd, len = 8
  *         2. DATA : addr = p_buf, len = p_dev->usb_ctrl.cmd.length for rx/tx(p_dev->usb_ctrl.cmd.rtype & USB_DIR_IN)
            3. STA  : if DATA tx null packet else rx null packet
  * @param  p_dev   : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  p_buf   : data buffer.
  * @return         : TRUE or FLASE
  * @note   Another choice user can realize function by calling "hgusb20_host_ep0_setup" & "hgusb20_host_ep0_rx" & "hgusb20_host_ep0_tx"
  */
bool usb_host_ep0_request(struct hgusb20_dev *p_dev, uint8 *pBuf);

/** 
  * @brief  usb host ep0 send SETUP packet
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note
  */
void hgusb20_host_ep0_setup(struct hgusb20_dev *p_dev);

/** 
  * @brief  usb host ep0 send IN packet & request a data packet
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @return 
  * @note   addr = p_dev->usb_ep0_buf, len = p_dev->ep0_ptr.rx_len
  */
void hgusb20_host_ep0_rx(struct hgusb20_dev *p_dev, uint8 *pBuf, bool first_pkt);
/** 
  * @brief  usb host ep0 send IN packet & request a data packet
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  len      : 
  * @return 
  * @note   addr = p_dev->usb_ep0_buf, len = p_dev->ep0_ptr.rx_len
  */
void hgusb20_host_ep0_tx(struct hgusb20_dev *p_dev, uint8 *pBuf, uint8 len);

/** 
  * @brief  usb host endpiont init
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep_host  : ep pipe of host
  * @param  ep_dev   : ep pipe of device
  * @param  ep_type  : USB_ENDPOINT_XFERTYPE_MASK @ref usb_ch9.h
  * @param  max_pkt_size : 
  * @return 
  * @note 
  */
void hgusb20_host_tx_ep_init(struct hgusb20_dev *p_dev, uint8 ep_host, uint8 ep_dev, uint8 ep_type, uint16 max_pkt_size);
void hgusb20_host_rx_ep_init(struct hgusb20_dev *p_dev, uint8 ep_host, uint8 ep_dev, uint8 ep_type, uint16 max_pkt_size);

/** 
  * @brief  hgusb20_host_clear_data_toggle
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep_num   : [1,2]
  * @param  ep_type  : EndpointDir BIT[7]：IN = 1, OUT = 0
  * @return          : 
  * @note  
  */
void hgusb20_host_clear_data_toggle(struct hgusb20_dev *p_dev, uint8 ep_num, uint8 ep_type);

/** 
  * @brief  hgusb20_host_flush_fifo
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep_num   : [1,2]
  * @param  ep_type  : EndpointDir BIT[7]：IN = 1, OUT = 0
  * @return          : 
  * @note  
  */
void hgusb20_host_flush_fifo(struct hgusb20_dev *p_dev, uint8 ep_num, uint8 ep_type);

/** 
  * @brief  usb host get stall status 
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep_num   : [0,2]
  * @param  ep_type  : EndpointDir BIT[7]：IN = 1, OUT = 0
  * @return          : TRUE or FLASE
  * @note  
  */
bool hgusb20_host_is_rx_stall(struct hgusb20_dev *p_dev, uint8 ep_num, uint8 ep_type);
/** 
  * @brief  usb host get xact error status (continous no response three times)
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep_num   : [0,2]
  * @param  ep_type  : EndpointDir BIT[7]：IN = 1, OUT = 0
  * @return          : TRUE or FLASE
  * @note  
  */
bool hgusb20_host_is_xact_err(struct hgusb20_dev *p_dev, uint8 ep_num, uint8 ep_type);
/** 
  * @brief  usb host get nak timeout status 
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep_num   : [0,2]
  * @param  ep_type  : EndpointDir BIT[7]：IN = 1, OUT = 0
  * @return          : TRUE or FLASE
  * @note  
  */
bool hgusb20_host_is_nak_timeout(struct hgusb20_dev *p_dev, uint8 ep_num, uint8 ep_type);
/** 
  * @brief  usb host get rx crc status 
  * @param  p_dev    : hgusb20_dev use @ref dev_get() function to get the handle.
  * @param  ep_num   : [0,2]
  * @return          : TRUE or FLASE
  * @note  
  */
bool hgusb20_host_is_crc_err(struct hgusb20_dev *p_dev, uint8 ep_num);


#if USB20_ROM_CODE_ENABLE
/* ROM FUNC */
void hgusb20_int_init_b(struct hgusb20_dev  *usb_sw);
void hgusb20_clear_ep(struct hgusb20_dev *p_dev, uint8_t epnum);
bool hgusb20_ep0_clrrx_pkt0(struct hgusb20_dev *p_dev);
bool hgusb20_ep0_clr_feature(struct hgusb20_dev *p_dev);
void hgusb20_stall_ep(struct hgusb20_dev *p_dev, uint8_t epnum);
bool hgusb20_ep0_get_status(struct hgusb20_dev *p_dev);
bool hgusb20_ep0_set_feature(struct hgusb20_dev *p_dev);
bool hgusb20_ep0_set_configuration(struct hgusb20_dev *p_dev);
bool hgusb20_ep0_get_wcid_descriptor(struct hgusb20_dev  *usb_sw, uint8 index, uint8 len);
void hgusb20_ep_set_txrdy(struct hgusb20_dev  *usb_sw, uint8 ep);
bool hgusb20_ep0_tx(struct hgusb20_dev  *usb_sw, uint8 len);
bool hgusb20_ep_rx_kick(struct hgusb20_dev  *usb_sw, uint8 ep, uint32 addr, uint32 len);
bool hgusb20_ep0_class(struct hgusb20_dev  *usb_sw);
bool hgusb20_ep0_set_address(struct hgusb20_dev  *usb_sw);
void hgusb20_ep_set_rxrdy(struct hgusb20_dev  *usb_sw, uint8 ep);
bool hgusb20_ep_tx_kick(struct hgusb20_dev  *usb_sw, uint8 ep, uint32 addr, uint32 len);
bool hgusb20_ep0_transfer(struct hgusb20_dev  *usb_sw);
bool hgusb20_ep0_descriptor(struct hgusb20_dev  *usb_sw, void *buf, uint16 len);
#endif


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif



#endif /* USB_DEV_API_H */
