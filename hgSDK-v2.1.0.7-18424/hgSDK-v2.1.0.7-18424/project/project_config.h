#ifndef __SYS_PROJECT_CONFIG_H__

#define __SYS_PROJECT_CONFIG_H__



#define USB_HOST_EN                     1
#define MACBUS_USB


#define USB_EN                          0
#define DVP_EN                          1
#define JPG_EN                         (1 && DVP_EN)
#define SDH_EN                          0
#define FS_EN                           0
#define VCAM_EN                        (0 || DVP_EN)

#define FRAME_MANAGEMENT                0
#define OPENDML_EN                      0
#define UART_INPUT_EN                   1
#define UART_FLY_CTRL_EN                0
#define PWM_EN                          0


#define AUDIO_EN                        (1&&  FRAME_MANAGEMENT)
#define RTP_SOUND                       (1&&AUDIO_EN)

#define MJPEG_VIDEO                     (1 && FRAME_MANAGEMENT&&OPENDML_EN&&FS_EN&&SDH_EN&&JPG_EN)          //基于框架的mjpeg录像    
#define UVC_VIDEO                       (1 && FRAME_MANAGEMENT&&OPENDML_EN&&FS_EN&&SDH_EN&&USB_EN)          //基于框架的uvc录像

#define DCDC_EN                         0           //板子是否使用DCDC电路

///////////////wifi parameter////////////
#define WIFI_RTS_THRESHOLD              2304        //RTS阈值，2304等效于不用RTS
#define WIFI_RTS_MAX_RETRY              2           //RTS重试次数，范围为2~16
#define WIFI_TX_MAX_RETRY               7           //最大传输次数，范围为1~31
#define WIFI_TX_MAX_POWER               7           //TX最大发射功率，有0~7档
/* 每1bit代表一种速率。每bit代表的格式：
 * bit 0  : DSSS 1M
 * bit 1  : DSSS 2M
 * bit 2  : DSSS 5.5M
 * bit 3  : DSSS 11M
 * bit 4  : NON-HT 6M
 * bit 5  : NON-HT 9M
 * bit 6  : NON-HT 12M
 * bit 7  : NON-HT 18M
 * bit 8  : NON-HT 24M
 * bit 9  : NON-HT 36M
 * bit 10 : NON-HT 48M
 * bit 11 : NON-HT 54M
 * bit 12 : HT MCS0
 * bit 13 : HT MCS1
 * bit 14 : HT MCS2
 * bit 15 : HT MCS3
 * bit 16 : HT MCS4
 * bit 17 : HT MCS5
 * bit 18 : HT MCS6
 * bit 19 : HT MCS7
 */
#define WIFI_TX_SUPP_RATE               0x0FFFFF    //TX速率支持，每1bit对应一种速率
#define WIFI_MAX_STA_CNT                8           //最多连接sta的数量。有效值为1~8
#define WIFI_MULICAST_RETRY             0           //组播帧传输次数
#define WIFI_ACS_CHAN_LISTS             0x03FF      //要扫描的信道。每1bit对应1个信道(bit 0~11 -> chan 1~12)
#define WIFI_ACS_SCAN_TIME              150         //每个信道的扫描时间，单位ms
#define WIFI_MAX_PS_CNT                 0           //底层为休眠sta缓存的帧最大数量。0代表sta休眠由umac全程管理，底层不缓存
#define CHANNEL_DEFAULT                 0
#define SSID_DEFAULT                    "HG-WIFI_"
#define WIFI_TX_DUTY_CYCLE              100         //tx发送占空比，单位是%，范围是0~100
#define WIFI_SSID_FILTER_EN             0           //是否使能SSID过滤功能。使能后，只有隐藏SSID和指定SSID的beacon才会上传
#define WIFI_RF_USE_IPEX                0           //天线接口是否为IPEX
#define IP_ADDR_DEFAULT                 0x0101A8C0  //192.168.1.1
#define NET_MASK_DEFAULT                0x00FFFFFF  //255.255.255.0
#define GW_IP_DEFAULT                   0x0101A8C0  //192.168.1.1
#define DHCP_START_IP_DEFAULT           0x6401A8C0  //192.168.1.100
#define DHCP_END_IP_DEFAULT             0xFE01A8C0  //192.168.1.254

///////////////uart1////////////
#define PIN_UART1_TX PA_13
#define PIN_UART1_RX PA_12

#define PIN_UART0_TX 255
#define PIN_UART0_RX 255



#define PIN_PDM_MCLK PA_4
#define PIN_PDM_DATA PA_3

/*---------------------------------------*/
/*---------SDH PIN DEFINITION------------*/
/*---------------------------------------*/


#define PIN_SDH_CLK  PA_1
#define PIN_SDH_CMD  PA_2
#define PIN_SDH_DAT0 PA_0
#define PIN_SDH_DAT1 PA_15
#define PIN_SDH_DAT2 PA_3
#define PIN_SDH_DAT3 PA_4


///////////////IIC2/////////////
#if 0
#define PIN_IIC2_SCL PC_3
#define PIN_IIC2_SDA PC_4
#else
#define PIN_IIC2_SCL PC_2
#define PIN_IIC2_SDA PC_3
#endif


///////////////DVP//////////////
#define CMOS_AUTO_LOAD   1
//#define DEV_SENSOR_GC0308  1


#define PIN_DVP_RESET  255//PC_2
#define PIN_DVP_PDN    255


//////////////////////////////////////////////////jpg app room cfg////////////////////////////////////////////////////
#define JPG_HEAD_RESERVER  24       //每段JPEG BUF前面预留用于填充其他数据的数据量，无需预留填0
#define JPG_BUF_LEN        1308//2880//4096+24   // 1*1024// 1*1024//       JPG buf长度+reserver长度          1308
#define JPG_NODE           40//18//30*2
#define JPG_TAIL_RESERVER  0




#endif

