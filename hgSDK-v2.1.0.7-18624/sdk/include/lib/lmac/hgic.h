#ifndef _HUGE_IC_H_
#define _HUGE_IC_H_

#define HGIC_WDEV_ID_AP   0
#define HGIC_WDEV_ID_STA  1
#define HGIC_SCAN_MAX_NUM 32

#define HGIC_HDR_TX_MAGIC 0x1A2B
#define HGIC_HDR_RX_MAGIC 0x2B1A

#define HGIC_VENDOR_ID     (0xA012)
#define HGIC_WLAN_AH_6001  (0x6001)
#define HGIC_WLAN_AH_4001  (0x4001)
#define HGIC_WLAN_AH_4002  (0x4002)
#define HGIC_WLAN_BGN_4102 (0x4102)
#define HGIC_WLAN_BGN_4103 (0x4103)
#define HGIC_WLAN_BGN_4104 (0x4104)
#define HGIC_WLAN_BGN_8400 (0x8400)

#define HGIC_CTRL_TIMEOUT 100
#define HGIC_CMD_PRIORITY 0
#define HGIC_TX_WINDOW    20
#define HGIC_TX_COOKIE_MASK 0x7FFF
#define HGIC_BLOCK_ACK_CNT 256

#define HGIC_BOOTDL_CMD_ENTER      (0x00)
#define HGIC_BOOTDL_RSP_ERR_IN_FW  (0xFF)

#define hgic_dbg(fmt, ...) //os_printf("%s:%d::"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define hgic_err(fmt, ...) os_printf(fmt, ##__VA_ARGS__)

#define hgic_enter()       os_printf("enter %s\n", __FUNCTION__)
#define hgic_leave()       os_printf("leave %s\n", __FUNCTION__)

#define HGIC_CMD_TIMEOUT 100
#define HGIC_TX_TIMEOUT  10

enum hgic_dev_flags {
    HGIC_DEV_FLAGS_STOP    = BIT(0),
    HGIC_DEV_FLAGS_RUNNING = BIT(1),
};

enum hgic_bus_type {
    HGIC_BUS_SDIO = 0x1,
    HGIC_BUS_USB,
    HGIC_BUS_HWSIM,
    HGIC_BUS_WFOE,
};


enum hgic_cmd {
    HGIC_CMD_DEV_OPEN = 0x01,       // 1
    HGIC_CMD_DEV_CLOSE,             // 2
    HGIC_CMD_SET_MAC,               // 3
    HGIC_CMD_SET_SSID,              // 4
    HGIC_CMD_SET_BSSID,             // 5
    HGIC_CMD_SET_COUNTERY,          // 6
    HGIC_CMD_SET_CHANNEL,           // 7
    HGIC_CMD_SET_CENTER_FREQ,       // 8
    HGIC_CMD_SET_RTS_THRESHOLD,     // 9
    HGIC_CMD_SET_FRG_THRESHOLD,     // 10
    HGIC_CMD_SET_KEY_MGMT,          // 11
    HGIC_CMD_SET_WPA_PSK,           // 12
    HGIC_CMD_SET_KEY,               // 13
    HGIC_CMD_SCAN,                  // 14
    HGIC_CMD_GET_SCAN_LIST,         // 15
    HGIC_CMD_SET_BSSID_FILTER,      // 16
    HGIC_CMD_DISCONNECT,            // 17
    HGIC_CMD_GET_BSSID,             // 18
    HGIC_CMD_SET_WBNAT,             // 19
    HGIC_CMD_GET_STATUS,            // 20
    HGIC_CMD_SET_LISTEN_INTERVAL,   // 21
    HGIC_CMD_SET_TX_POWER,          // 22
    HGIC_CMD_GET_TX_POWER,          // 23
    HGIC_CMD_SET_TX_LCOUNT,         // 24
    HGIC_CMD_SET_TX_SCOUNT,         // 25
    HGIC_CMD_ADD_STA,               // 26
    HGIC_CMD_REMOVE_STA,            // 27
    HGIC_CMD_SET_TX_BW,             // 28
    HGIC_CMD_SET_TX_MCS,            // 29
    HGIC_CMD_SET_FREQ_RANGE,        // 30
    HGIC_CMD_ACS_ENABLE,            // 31
    HGIC_CMD_SET_PRIMARY_CHAN,      // 32
    HGIC_CMD_SET_BG_RSSI,           // 33
    HGIC_CMD_SET_BSS_BW,            // 34
    HGIC_CMD_TESTMODE_CMD,          // 35
    HGIC_CMD_SET_AID,               // 36
    HGIC_CMD_GET_FW_STATE,          // 37
    HGIC_CMD_SET_TXQ_PARAM,         // 38
    HGIC_CMD_SET_CHAN_LIST,         // 39
    HGIC_CMD_GET_CONN_STATE,        // 40
    HGIC_CMD_SET_WORK_MODE,         // 41
    HGIC_CMD_SET_PAIRED_STATIONS,   // 42
    HGIC_CMD_GET_FW_INFO,           // 43
    HGIC_CMD_PAIRING,               // 44
    HGIC_CMD_GET_TEMPERATURE,       // 45
    HGIC_CMD_ENTER_SLEEP,           // 46
    HGIC_CMD_OTA,                   // 47
    HGIC_CMD_GET_SSID,              // 48
    HGIC_CMD_GET_WPA_PSK,           // 49
    HGIC_CMD_GET_SIGNAL,            // 50
    HGIC_CMD_GET_TX_BITRATE,        // 51
    HGIC_CMD_SET_BEACON_INT,        // 52
    HGIC_CMD_GET_STA_LIST,          // 53
    HGIC_CMD_SAVE_CFG,              // 54
    HGIC_CMD_JOIN_GROUP,            // 55
    HGIC_CMD_SET_ETHER_TYPE,        // 56
    HGIC_CMD_GET_STA_COUNT,         // 57
    HGIC_CMD_SET_HEARTBEAT_INT,     // 58
    HGIC_CMD_SET_MCAST_KEY,         // 59
    HGIC_CMD_SET_AGG_CNT,           // 60
    HGIC_CMD_GET_AGG_CNT,           // 61
    HGIC_CMD_GET_BSS_BW,            // 62
    HGIC_CMD_GET_FREQ_RANGE,        // 63
    HGIC_CMD_GET_CHAN_LIST,         // 64
    HGIC_CMD_RADIO_ONOFF,           // 65
    HGIC_CMD_SET_PS_HEARTBEAT,      // 66
    HGIC_CMD_SET_WAKEUP_STA,        // 67
    HGIC_CMD_SET_PS_HEARTBEAT_RESP, // 68
    HGIC_CMD_SET_PS_WAKEUP_DATA,    // 69
    HGIC_CMD_SET_PS_CONNECT,        // 70
    HGIC_CMD_SET_BSS_MAX_IDLE,      // 71
    HGIC_CMD_SET_WKIO_MODE,         // 72
    HGIC_CMD_SET_DTIM_PERIOD,       // 73
    HGIC_CMD_SET_PS_MODE,           // 74
    HGIC_CMD_LOAD_DEF,              // 75
    HGIC_CMD_DISASSOC_STA,          // 76
    HGIC_CMD_SET_APLOST_TIME,       // 77
    HGIC_CMD_GET_WAKEUP_REASON,     // 78
    HGIC_CMD_UNPAIR,                // 79
    HGIC_CMD_SET_AUTO_CHAN_SWITCH,  // 80
    HGIC_CMD_SET_REASSOC_WKHOST,    // 81
    HGIC_CMD_SET_WAKEUP_IO,         // 82
    HGIC_CMD_DBGINFO_OUTPUT,        // 83
    HGIC_CMD_SET_SYSDBG,            // 84
    HGIC_CMD_SET_AUTO_SLEEP_TIME,   // 85
    HGIC_CMD_GET_KEY_MGMT,          // 86
    HGIC_CMD_SET_PAIR_AUTOSTOP,     // 87
    HGIC_CMD_SET_SUPPER_PWR,        // 88
    HGIC_CMD_SET_REPEATER_SSID,     // 89
    HGIC_CMD_SET_REPEATER_PSK,      // 90
    HGIC_CMD_CFG_AUTO_SAVE,         // 91
    HGIC_CMD_SEND_CUST_MGMT,        // 92
    HGIC_CMD_GET_BATTERY_LEVEL,     // 93
    HGIC_CMD_SET_DCDC13,            // 94
    HGIC_CMD_SET_ACKTMO,            // 95
    HGIC_CMD_GET_MODULETYPE,        // 96
    HGIC_CMD_PA_PWRCTRL_DIS,        // 97
    HGIC_CMD_SET_DHCPC,             // 98
    HGIC_CMD_GET_DHCPC_RESULT,      // 99
    HGIC_CMD_SET_WKUPDATA_MASK,     // 100
    HGIC_CMD_GET_WKDATA_BUFF,       // 101
    HGIC_CMD_GET_DISASSOC_REASON,   // 102
    HGIC_CMD_SET_WKUPDATA_SAVEEN,   // 103
    HGIC_CMD_SET_CUST_DRIVER_DATA,  // 104
    HGIC_CMD_SET_MCAST_TXPARAM,     // 105
    HGIC_CMD_SET_STA_FREQINFO,      // 106
    HGIC_CMD_SET_RESET_STA,         // 107
    HGIC_CMD_SET_UART_FIXLEN,       // 108
    HGIC_CMD_GET_UART_FIXLEN,       // 109
    HGIC_CMD_SET_ANT_AUTO,          // 110
    HGIC_CMD_SET_ANT_SEL,           // 111
    HGIC_CMD_GET_ANT_SEL,           // 112
    HGIC_CMD_SET_WKUP_HOST_REASON,  // 113
    HGIC_CMD_SET_MAC_FILTER_EN,     // 114
    HGIC_CMD_SET_ATCMD,             // 115
    HGIC_CMD_SET_ROAMING,           // 116
    HGIC_CMD_SET_AP_HIDE,           // 117
    HGIC_CMD_SET_DUAL_ANT,          // 118
    HGIC_CMD_SET_MAX_TCNT,          // 119
    HGIC_CMD_SET_ASSERT_HOLDUP,     // 120
    HGIC_CMD_SET_AP_PSMODE_EN,      // 121
    HGIC_CMD_SET_DUPFILTER_EN,      // 122
    HGIC_CMD_SET_DIS_1V1_M2U,       // 123
    HGIC_CMD_SET_DIS_PSCONNECT,     // 124
    HGIC_CMD_SET_RTC,               // 125
    HGIC_CMD_GET_RTC,               // 126
    HGIC_CMD_SET_KICK_ASSOC,        // 127
    HGIC_CMD_START_ASSOC,           // 128
    HGIC_CMD_SET_AUTOSLEEP,         // 129
    HGIC_CMD_SET_BLENC_DATA,        // 130
    HGIC_CMD_SET_BLENC_EN,          // 131
    HGIC_CMD_RESET,                 // 132
    HGIC_CMD_SET_HWSCAN,            // 133
};

enum hgic_event {
    HGIC_EVENT_STATE_CHG = 0x1,      // 1
    HGIC_EVENT_CH_SWICH,             // 2
    HGIC_EVENT_DISCONNECT_REASON,    // 3
    HGIC_EVENT_ASSOC_STATUS,         // 4
    HGIC_EVENT_SCANNING,             // 5
    HGIC_EVENT_SCAN_DONE,            // 6
    HGIC_EVENT_TX_BITRATE,           // 7
    HGIC_EVENT_WNB_PAIR_START,       // 8
    HGIC_EVENT_WNB_PAIR_SUCCESS,     // 9
    HGIC_EVENT_WNB_PAIR_DONE,        // 10
    HGIC_EVENT_WNB_CONECT_START,     // 11
    HGIC_EVENT_WNB_CONECTED,         // 12
    HGIC_EVENT_WNB_DISCONECTED,      // 13
    HGIC_EVENT_WNB_SIGNAL,           // 14
    HGIC_EVENT_DISCONNET_LOG,        // 15
    HGIC_EVENT_REQUEST_PARAM,        // 16
    HGIC_EVENT_TESTMODE_STATE,       // 17
    HGIC_EVENT_FWDBG_INFO,           // 18
    HGIC_EVENT_CUSTOMER_MGMT,        // 19
    HGIC_EVENT_SLEEP_FAIL,           // 20
    HGIC_EVENT_DHCPC_DONE,           // 21
    HGIC_EVENT_CONNECT_FAIL,         // 22
    HGIC_EVENT_CUST_DRIVER_DATA,     // 23
    HGIC_EVENT_UNPAIR_STA,           // 24
    HGIC_EVENT_BLENC_DATA,           // 25
    HGIC_EVENT_HWSCAN_RESULT,        // 26
};

enum hgic_hdr_type {
    HGIC_HDR_TYPE_ACK = 0x1,
    HGIC_HDR_TYPE_FRM,
    HGIC_HDR_TYPE_CMD,
    HGIC_HDR_TYPE_EVENT,
    HGIC_HDR_TYPE_FIRMWARE,
    HGIC_HDR_TYPE_NLMSG,
    HGIC_HDR_TYPE_BOOTDL,
    HGIC_HDR_TYPE_TEST,
    HGIC_HDR_TYPE_FRM2,
    HGIC_HDR_TYPE_TEST2,
    HGIC_HDR_TYPE_SOFTFC,
    HGIC_HDR_TYPE_OTA,
    HGIC_HDR_TYPE_CMD2,
    HGIC_HDR_TYPE_EVENT2,
    HGIC_HDR_TYPE_BOOTDL_DATA,
    HGIC_HDR_TYPE_IFBR,
    HGIC_HDR_TYPE_BEACON,

    HGIC_HDR_TYPE_MAX,
};

enum hgic_hdr_flags2 {
    HGIC_HDR_FLAGS2_AFT_BEACON       = BIT(0),
    HGIC_HDR_FLAGS2_MGMT_ACK         = BIT(1),
};

/*data packet header*/
struct hgic_hdr {
    unsigned short magic;
    unsigned char  type;
    unsigned char  ifidx: 4, flags: 4;
    unsigned short length;
    unsigned short cookie;
} __packed;

struct hgic_rx_info {
    uint8  band;
    uint8  mcs: 4, bw: 4;
    int8   evm;
    int8   signal;
    uint16 freq;
    int16  freq_off;
    uint8  rx_flags;
    uint8  antenna;
    uint8  vht_nss : 4, s1g_nss : 4;
    uint8  vht_flag : 3, s1g_flag : 5;
};

struct hgic_tx_info {
    uint8  band;
    uint8  tx_bw;
    uint8  tx_mcs;
    uint8  freq_idx: 5, antenna: 3;
    uint32 tx_flags;
    uint16 tx_flags2;
    uint8  priority;
    uint8  tx_power;
};

#ifndef CONFIG_UMAC
enum ieee80211_key_flags {
	IEEE80211_KEY_FLAG_GENERATE_IV_MGMT	= BIT(0),
	IEEE80211_KEY_FLAG_GENERATE_IV	    = 1<<1,
	IEEE80211_KEY_FLAG_GENERATE_MMIC    = 1<<2,
	IEEE80211_KEY_FLAG_PAIRWISE	        = 1<<3,
	IEEE80211_KEY_FLAG_SW_MGMT_TX	    = 1<<4,
	IEEE80211_KEY_FLAG_PUT_IV_SPACE     = 1<<5,
	IEEE80211_KEY_FLAG_RX_MGMT	        = 1<<6,
	IEEE80211_KEY_FLAG_GROUP_TX	        = BIT(7),
	IEEE80211_KEY_FLAG_GROUP_RX	        = BIT(8),
};
#endif

struct hgic_key_conf {
	unsigned int cipher;
	unsigned int flags;
	unsigned char keyidx;
	unsigned char keylen;
	unsigned char key[0];
};

struct hgic_sta_info {
    unsigned char aid;
    unsigned char ps: 1, rev: 7;
    unsigned char addr[6];
    char          rssi;
    char          evm;
};

struct hgic_frm_hdr {
    struct hgic_hdr hdr;
    union {
        struct hgic_rx_info rx_info;
        struct hgic_tx_info tx_info;
        uint8  rev[24];
    };
} __packed;

struct hgic_frm_hdr2 {
    struct hgic_hdr hdr;
} __packed;

struct hgic_acs_ctl {
    uint32 enable:1, scan_ms:8, chn_bitmap:23;
};

#define HDR_CMDID(ctl) ((ctl)->hdr.type==HGIC_HDR_TYPE_CMD2?(ctl)->cmd2.cmd_id:(ctl)->cmd.cmd_id)
#define HDR_EVTID(ctl) ((ctl)->hdr.type==HGIC_HDR_TYPE_EVENT2?(ctl)->event2.event_id:(ctl)->event.event_id)
#define HDR_CMDID_SET(ctl, id) if(id>255){\
        (ctl)->hdr.type =HGIC_HDR_TYPE_CMD2;\
        (ctl)->cmd2.cmd_id = id;\
    }else{\
        (ctl)->hdr.type =HGIC_HDR_TYPE_CMD;\
        (ctl)->cmd.cmd_id = id;\
    }
#define HDR_EVTID_SET(ctl, id) if(id>255){\
        (ctl)->hdr.type =HGIC_HDR_TYPE_EVENT2;\
        (ctl)->event2.event_id = id;\
    }else{\
        (ctl)->hdr.type =HGIC_HDR_TYPE_EVENT;\
        (ctl)->event.event_id = id;\
    }

/*contro pakcet header*/
struct hgic_ctrl_hdr {
    struct hgic_hdr hdr;
    union {
        struct {
            unsigned char  cmd_id;
            short status;
        } cmd;
        struct {
            unsigned short  cmd_id;
            short status;
        } cmd2;
        struct {
            unsigned char event_id;
        } event;
        struct {
            unsigned short event_id;
        } event2;        
        unsigned char info[4];
    };
} __packed;

//ack packet
struct hgic_dack_hdr {
    struct hgic_hdr hdr;
    uint16_t cookies[0];
} __packed;

struct hgic_bootdl_cmd_hdr {
    struct hgic_hdr      hdr;
    unsigned char   cmd;
    unsigned char   cmd_len;
    unsigned char   cmd_flag;
    unsigned char   addr[4];
    unsigned char   len[4];
    unsigned char   check;
} __packed;

struct hgic_bootdl_resp_hdr {
    struct hgic_hdr hdr;
    unsigned char   cmd;
    unsigned char   rsp;
    unsigned char   rsp_data[4];
    unsigned char   reserved;
    unsigned char   check;
} __packed;

struct hgic_fw_info {
    uint32 version;
    uint32 svn_version;
    uint16 chip_id;
    uint16 cpuid;
    uint8  mac[6];
    uint8  resv[2]; //align 4 for app_version
    uint32 app_version;
} __packed;

struct hgic_bss_info {
    uint8 bssid[6];
    uint8 ssid[32];
    uint8 encrypt;
    int8  signal;
    uint16 freq;
};

struct hgic_freqinfo {
    uint8  bss_bw, chan_cnt;
    uint16 freq_start, freq_end;
    uint16 chan_list[16];
};

struct hgic_module_hwinfo{
    union{
        struct{
            uint8 type;
            uint8 saw:1, rev:7;
        };
        unsigned short v;
    };
};

#endif
