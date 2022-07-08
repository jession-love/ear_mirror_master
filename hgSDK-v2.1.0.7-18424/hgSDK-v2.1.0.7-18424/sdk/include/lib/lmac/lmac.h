#ifndef _HGIC_LMAC_H_
#define _HGIC_LMAC_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************ for lmac rate ************************************/
#define LMAC_RATE_MCS_MASK                  0xFF
#define LMAC_RATE_MCS_SHIFT                 0
#define LMAC_RATE_NSS_MASK                  0x0F
#define LMAC_RATE_NSS_SHIFT                 8
#define LMAC_RATE_PHY_TYPE_MASK             0x3F
#define LMAC_RATE_PHY_TYPE_SHIFT            12

//PHY TYPE
#define LMAC_PHY_S1G                        0
#define LMAC_PHY_DSSS_CCK                   1
#define LMAC_PHY_NON_HT                     2
#define LMAC_PHY_HT                         3

//flags
#define LMAC_RATE_SHORT_PREAMBLE            (1 << 24)
#define LMAC_RATE_SHORT_GI                  (1 << 24)
#define LMAC_RATE_GREEN_FILED               (1 << 25)

//lmac rate define
#define LMAC_RATE_DEF(phy_type, nss, mcs, flags)            ((((phy_type) & LMAC_RATE_PHY_TYPE_MASK) << LMAC_RATE_PHY_TYPE_SHIFT) |    \
                                                             (((nss) & LMAC_RATE_NSS_MASK) << LMAC_RATE_NSS_SHIFT)                |    \
                                                             (((mcs) & LMAC_RATE_MCS_MASK) << LMAC_RATE_MCS_SHIFT) | (flags))
/***************************************************************************************/


struct lmac_init_param {
    uint32 rxbuf, rxbuf_size;
    uint32 tdma_buff, tdma_buff_size;
    uint32 uart0_sel : 1;
};

struct lmac_acs_ctl {
    uint32 enable:1, scan_ms:8, chn_bitmap:23;
};

typedef void (*lmac_blenc_hdl)(void *priv, uint8 *data, uint32 len);

enum DSLEEP_MODE{
    DSLEEP_MODE_NONE,
    DSLEEP_MODE_STA_ALIVE,
    DSLEEP_MODE_AP_ALIVE,
    DSLEEP_MODE_UCASTWK,
    DSLEEP_MODE_APWK,
    
    DSLEEP_MODE_NUM,
};

enum DSLEEP_WK_REASON{
    DSLEEP_WK_REASON_TIMER = 1,
    DSLEEP_WK_REASON_TIM,           // 2
    DSLEEP_WK_REASON_BC_TIM,        // 3
    DSLEEP_WK_REASON_IO,            // 4
    DSLEEP_WK_REASON_BEACON_LOST,   // 5
    DSLEEP_WK_REASON_AP_ERROR,      // 6
    DSLEEP_WK_REASON_HB_TIMEOUT,    // 7
    DSLEEP_WK_REASON_WK_DATA,       // 8
    DSLEEP_WK_REASON_MCLR,          // 9
    DSLEEP_WK_REASON_LVD,           // 10
    DSLEEP_WK_REASON_PIR,           // 11
    DSLEEP_WK_REASON_APWK,          // 12
    DSLEEP_WK_REASON_PS_DISCONNECT, // 13
    
    DSLEEP_WK_REASON_STA_ERROR = 20,
    DSLEEP_WK_REASON_SLEEPED_STA_ERROR, // 21
    DSLEEP_WK_REASON_STA_DATA,          // 22
    DSLEEP_WK_REASON_AP_PAIR,           // 23
};

enum LMAC_IOCTL_CMD {
    /*Set CMDs*/
    LMAC_IOCTL_SET_AGGCNT = 0x1,
    LMAC_IOCTL_SET_BSS_BW,
    LMAC_IOCTL_SET_TX_BW,
    LMAC_IOCTL_SET_TX_MCS,
    LMAC_IOCTL_SET_RTS_TH,
    LMAC_IOCTL_SET_TXPOWER,
    LMAC_IOCTL_SET_BSS_MAX_IDLE,
    LMAC_IOCTL_SET_TX_ORDERED,
    LMAC_IOCTL_SET_AUTO_CHAN_SWITCH,
    LMAC_IOCTL_SET_PRI_CHAN,
    LMAC_IOCTL_SET_RADIO_ONOFF,
    LMAC_IOCTL_SET_PS_HEARBEAT,
    LMAC_IOCTL_SET_PS_HEARBEAT_RESP,
    LMAC_IOCTL_SET_PS_WAKEUP_DATA,
    LMAC_IOCTL_SET_PS_HEARBEAT_PERIOD,
    LMAC_IOCTL_SET_SSID,
    LMAC_IOCTL_SET_WAKEUPIO_MODE,
    LMAC_IOCTL_SET_PS_MODE,
    LMAC_IOCTL_SET_SLEEP_APLOST_TIME,
    LMAC_IOCTL_SET_SLEEP_WAKEIUP_IO,
    LMAC_IOCTL_SET_SUPPER_PWR,
    LMAC_IOCTL_SET_PA_PWR_CTRL,
    LMAC_IOCTL_SET_STA_ROAMING,
    LMAC_IOCTL_SET_HW_SCAN,
    LMAC_IOCTL_SET_COMPRESSED_SSID,
    LMAC_IOCTL_SET_VDD13,
    LMAC_IOCTL_SET_ACK_TIMEOUT_EXTRA,
    LMAC_IOCTL_SET_TX_CNT_MAX,
    LMAC_IOCTL_SET_SLEEP_GPIOA_RESV,
    LMAC_IOCTL_SET_SLEEP_GPIOB_RESV,
    LMAC_IOCTL_SET_SLEEP_GPIOC_RESV,
    LMAC_IOCTL_SET_ENTER_SLEEP_CB,
    LMAC_IOCTL_SET_KEEP_ALIVE_CB,
    LMAC_IOCTL_SET_WAKE_UP_CB,
    LMAC_IOCTL_SET_PSCONNECT_PERIOD,
    LMAC_IOCTL_SET_MCAST_TX_RATE,
    LMAC_IOCTL_SET_MCAST_TXBW,
    LMAC_IOCTL_SET_MCAST_TXPOWER,
    LMAC_IOCTL_SET_MCAST_DUP_TXCNT,
    LMAC_IOCTL_SET_MCAST_CLEAR_CHN,
    LMAC_IOCTL_SET_ANT_CTRL_PIN,
    LMAC_IOCTL_SET_ANT_DUAL_EN,
    LMAC_IOCTL_SET_ANT_AUTO_EN,
    LMAC_IOCTL_SET_ANT_SEL,
    LMAC_IOCTL_SET_REASSOCIATION,
    LMAC_IOCTL_SET_APSLEEP_RC,
    LMAC_IOCTL_SET_APSLEEP_WKIO_SEL,
    LMAC_IOCTL_SET_BSSID,
    LMAC_IOCTL_SET_AP_PSMODE_EN,
    LMAC_IOCTL_SET_WKSRC_DETECT,
    LMAC_IOCTL_SET_RTC,
    LMAC_IOCTL_SET_AID,
    LMAC_IOCTL_SET_TXQ_PARAM,
    LMAC_IOCTL_SET_MAC_ADDR,
    LMAC_IOCTL_SET_MCAST_KEY,
    LMAC_IOCTL_DEL_MCAST_KEY,
    LMAC_IOCTL_SET_PROMISC_MODE,
    LMAC_IOCTL_SET_STA_SUPP_RATE,
    LMAC_IOCTL_SET_SUPP_RATE,
    LMAC_IOCTL_SET_MAX_STA_CNT,
    LMAC_IOCTL_SET_MAX_PS_FRAME,
    LMAC_IOCTL_SET_TX_DUTY_CYCLE,
    LMAC_IOCTL_SET_BLENC_START,
    LMAC_IOCTL_SET_BLENC_STOP,
    LMAC_IOCTL_SET_SSID_FILTER,
    LMAC_IOCTL_SET_BSS_SCAN,
    LMAC_IOCTL_SET_WPHY_DPD,

    /*Get CMDs*/
    LMAC_IOCTL_GET_AGGCNT = 0x20000000,
    LMAC_IOCTL_GET_BSS_BW,
    LMAC_IOCTL_GET_TX_BW,
    LMAC_IOCTL_GET_TX_MCS,
    LMAC_IOCTL_GET_RTS_TH,
    LMAC_IOCTL_GET_TXPOWER,
    LMAC_IOCTL_GET_BSS_MAX_IDLE,
    LMAC_IOCTL_GET_TX_ORDERED,
    LMAC_IOCTL_GET_LO_FREQ,
    LMAC_IOCTL_GET_QA_BW,
    LMAC_IOCTL_GET_QA_MCS,
    LMAC_IOCTL_GET_QA_PER,
    LMAC_IOCTL_GET_AID,
    LMAC_IOCTL_GET_PRI_CHAN,
    LMAC_IOCTL_GET_MAC_ADDR,
    LMAC_IOCTL_GET_BSS_ID,
    LMAC_IOCTL_GET_BKN_INTERVAL,
    LMAC_IOCTL_GET_DTIM_PERIOD,
    LMAC_IOCTL_GET_SLEEP_USER_DATA_ADDR,
    LMAC_IOCTL_GET_SLEEP_USER_DATA_LEN,
    LMAC_IOCTL_GET_ANT_SEL,
    LMAC_IOCTL_GET_BSS_LOAD,
    LMAC_IOCTL_GET_RTC,
    LMAC_IOCTL_GET_PROMISC_MODE,
    LMAC_IOCTL_GET_STA_SUPP_RATE,
    LMAC_IOCTL_GET_SUPP_RATE,
    LMAC_IOCTL_GET_MAX_STA_CNT,
    LMAC_IOCTL_GET_BG_RSSI,
};

enum LMAC_RATE {
/*** for AH ***/
    LMAC_RATE_S1G_1_NSS_MCS0                = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 0, 0),
    LMAC_RATE_S1G_1_NSS_MCS1                = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 1, 0),
    LMAC_RATE_S1G_1_NSS_MCS2                = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 2, 0),
    LMAC_RATE_S1G_1_NSS_MCS3                = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 3, 0),
    LMAC_RATE_S1G_1_NSS_MCS4                = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 4, 0),
    LMAC_RATE_S1G_1_NSS_MCS5                = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 5, 0),
    LMAC_RATE_S1G_1_NSS_MCS6                = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 6, 0),
    LMAC_RATE_S1G_1_NSS_MCS7                = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 7, 0),
    LMAC_RATE_S1G_1_NSS_MCS10               = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 10, 0),
    
    LMAC_RATE_S1G_1_NSS_MCS0_SGI            = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 0, LMAC_RATE_SHORT_GI),
    LMAC_RATE_S1G_1_NSS_MCS1_SGI            = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 1, LMAC_RATE_SHORT_GI),
    LMAC_RATE_S1G_1_NSS_MCS2_SGI            = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 2, LMAC_RATE_SHORT_GI),
    LMAC_RATE_S1G_1_NSS_MCS3_SGI            = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 3, LMAC_RATE_SHORT_GI),
    LMAC_RATE_S1G_1_NSS_MCS4_SGI            = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 4, LMAC_RATE_SHORT_GI),
    LMAC_RATE_S1G_1_NSS_MCS5_SGI            = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 5, LMAC_RATE_SHORT_GI),
    LMAC_RATE_S1G_1_NSS_MCS6_SGI            = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 6, LMAC_RATE_SHORT_GI),
    LMAC_RATE_S1G_1_NSS_MCS7_SGI            = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 7, LMAC_RATE_SHORT_GI),
    LMAC_RATE_S1G_1_NSS_MCS10_SGI           = LMAC_RATE_DEF(LMAC_PHY_S1G, 1, 10, LMAC_RATE_SHORT_GI),

/*** for BGN ***/
    //DSSS/CCK long preamble
    LMAC_RATE_DSSS_CCK_RATE0                = LMAC_RATE_DEF(LMAC_PHY_DSSS_CCK, 1, 0, 0),        //RATE=1Mbps
    LMAC_RATE_DSSS_CCK_RATE1                = LMAC_RATE_DEF(LMAC_PHY_DSSS_CCK, 1, 1, 0),        //RATE=2Mbps
    LMAC_RATE_DSSS_CCK_RATE2                = LMAC_RATE_DEF(LMAC_PHY_DSSS_CCK, 1, 2, 0),        //RATE=5.5Mbps
    LMAC_RATE_DSSS_CCK_RATE3                = LMAC_RATE_DEF(LMAC_PHY_DSSS_CCK, 1, 3, 0),        //RATE=11Mbps
    //DSSS/CCK short preamble
    LMAC_RATE_DSSS_CCK_RATE0_SHORT          = LMAC_RATE_DEF(LMAC_PHY_DSSS_CCK, 1, 1, LMAC_RATE_SHORT_PREAMBLE),        //RATE=2Mbps
    LMAC_RATE_DSSS_CCK_RATE1_SHORT          = LMAC_RATE_DEF(LMAC_PHY_DSSS_CCK, 1, 2, LMAC_RATE_SHORT_PREAMBLE),        //RATE=5.5Mbps
    LMAC_RATE_DSSS_CCK_RATE2_SHORT          = LMAC_RATE_DEF(LMAC_PHY_DSSS_CCK, 1, 3, LMAC_RATE_SHORT_PREAMBLE),        //RATE=11Mbps
    //NON HT
    LMAC_RATE_NON_HT_RATE0                  = LMAC_RATE_DEF(LMAC_PHY_NON_HT, 1, 0, 0),
    LMAC_RATE_NON_HT_RATE1                  = LMAC_RATE_DEF(LMAC_PHY_NON_HT, 1, 1, 0),
    LMAC_RATE_NON_HT_RATE2                  = LMAC_RATE_DEF(LMAC_PHY_NON_HT, 1, 2, 0),
    LMAC_RATE_NON_HT_RATE3                  = LMAC_RATE_DEF(LMAC_PHY_NON_HT, 1, 3, 0),
    LMAC_RATE_NON_HT_RATE4                  = LMAC_RATE_DEF(LMAC_PHY_NON_HT, 1, 4, 0),
    LMAC_RATE_NON_HT_RATE5                  = LMAC_RATE_DEF(LMAC_PHY_NON_HT, 1, 5, 0),
    LMAC_RATE_NON_HT_RATE6                  = LMAC_RATE_DEF(LMAC_PHY_NON_HT, 1, 6, 0),
    LMAC_RATE_NON_HT_RATE7                  = LMAC_RATE_DEF(LMAC_PHY_NON_HT, 1, 7, 0),
    //HT MF long GI
    LMAC_RATE_HT_MF_1_NSS_MCS0              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 0, 0),
    LMAC_RATE_HT_MF_1_NSS_MCS1              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 1, 0),
    LMAC_RATE_HT_MF_1_NSS_MCS2              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 2, 0),
    LMAC_RATE_HT_MF_1_NSS_MCS3              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 3, 0),
    LMAC_RATE_HT_MF_1_NSS_MCS4              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 4, 0),
    LMAC_RATE_HT_MF_1_NSS_MCS5              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 5, 0),
    LMAC_RATE_HT_MF_1_NSS_MCS6              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 6, 0),
    LMAC_RATE_HT_MF_1_NSS_MCS7              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 7, 0),
    //HT MF short GI
    LMAC_RATE_HT_MF_1_NSS_MCS0_SGI          = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 0, LMAC_RATE_SHORT_GI),
    LMAC_RATE_HT_MF_1_NSS_MCS1_SGI          = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 1, LMAC_RATE_SHORT_GI),
    LMAC_RATE_HT_MF_1_NSS_MCS2_SGI          = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 2, LMAC_RATE_SHORT_GI),
    LMAC_RATE_HT_MF_1_NSS_MCS3_SGI          = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 3, LMAC_RATE_SHORT_GI),
    LMAC_RATE_HT_MF_1_NSS_MCS4_SGI          = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 4, LMAC_RATE_SHORT_GI),
    LMAC_RATE_HT_MF_1_NSS_MCS5_SGI          = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 5, LMAC_RATE_SHORT_GI),
    LMAC_RATE_HT_MF_1_NSS_MCS6_SGI          = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 6, LMAC_RATE_SHORT_GI),
    LMAC_RATE_HT_MF_1_NSS_MCS7_SGI          = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 7, LMAC_RATE_SHORT_GI),
    //HT GF
    LMAC_RATE_HT_GF_1_NSS_MCS0              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 0, LMAC_RATE_GREEN_FILED),
    LMAC_RATE_HT_GF_1_NSS_MCS1              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 1, LMAC_RATE_GREEN_FILED),
    LMAC_RATE_HT_GF_1_NSS_MCS2              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 2, LMAC_RATE_GREEN_FILED),
    LMAC_RATE_HT_GF_1_NSS_MCS3              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 3, LMAC_RATE_GREEN_FILED),
    LMAC_RATE_HT_GF_1_NSS_MCS4              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 4, LMAC_RATE_GREEN_FILED),
    LMAC_RATE_HT_GF_1_NSS_MCS5              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 5, LMAC_RATE_GREEN_FILED),
    LMAC_RATE_HT_GF_1_NSS_MCS6              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 6, LMAC_RATE_GREEN_FILED),
    LMAC_RATE_HT_GF_1_NSS_MCS7              = LMAC_RATE_DEF(LMAC_PHY_HT, 1, 7, LMAC_RATE_GREEN_FILED),
};


#define lmac_set_aggcnt(ops, aggcnt)                        lmac_ioctl(ops, LMAC_IOCTL_SET_AGGCNT, (uint32)aggcnt, 0)
#define lmac_get_aggcnt(ops)                                lmac_ioctl(ops, LMAC_IOCTL_GET_AGGCNT, 0, 0)
#define lmac_set_bss_bw(ops, bss_bw)                        lmac_ioctl(ops, LMAC_IOCTL_SET_BSS_BW, (uint32)bss_bw, 0)
#define lmac_get_bss_bw(ops)                                lmac_ioctl(ops, LMAC_IOCTL_GET_BSS_BW, 0, 0)
#define lmac_set_tx_bw(ops, tx_bw)                          lmac_ioctl(ops, LMAC_IOCTL_SET_TX_BW, (uint32)tx_bw, 0)
#define lmac_get_tx_bw(ops)                                 lmac_ioctl(ops, LMAC_IOCTL_GET_TX_BW, 0, 0)
#define lmac_set_tx_mcs(ops, tx_mcs)                        lmac_ioctl(ops, LMAC_IOCTL_SET_TX_MCS, (uint32)tx_mcs, 0)
#define lmac_get_tx_mcs(ops)                                lmac_ioctl(ops, LMAC_IOCTL_GET_TX_MCS, 0, 0)
#define lmac_set_rts(ops, rts_th)                           lmac_ioctl(ops, LMAC_IOCTL_SET_RTS_TH, (uint32)rts_th, 0)
#define lmac_get_rts(ops)                                   lmac_ioctl(ops, LMAC_IOCTL_GET_RTS_TH, 0, 0)
#define lmac_set_txpower(ops, txpower)                      lmac_ioctl(ops, LMAC_IOCTL_SET_TXPOWER, (uint32)txpower, 0)
#define lmac_get_txpower(ops)                               lmac_ioctl(ops, LMAC_IOCTL_GET_TXPOWER, 0, 0)
#define lmac_set_bss_max_idle(ops, max_idle_tu)             lmac_ioctl(ops, LMAC_IOCTL_SET_BSS_MAX_IDLE, (uint32)max_idle_tu, 0)
#define lmac_get_bss_max_idle(ops)                          lmac_ioctl(ops, LMAC_IOCTL_GET_BSS_MAX_IDLE, 0, 0)
#define lmac_set_strictly_ordered(ops, enable)              lmac_ioctl(ops, LMAC_IOCTL_SET_TX_ORDERED, (uint32)enable, 0)
#define lmac_get_strictly_ordered(ops)                      lmac_ioctl(ops, LMAC_IOCTL_GET_TX_ORDERED, 0, 0)
#define lmac_get_lo_freq(ops)                               lmac_ioctl(ops, LMAC_IOCTL_GET_LO_FREQ, 0, 0)
#define lmac_set_pri_chan(ops, prichan)                     lmac_ioctl(ops, LMAC_IOCTL_SET_PRI_CHAN, (uint32)prichan, 0)
#define lmac_get_pri_chan(ops)                              lmac_ioctl(ops, LMAC_IOCTL_GET_PRI_CHAN, 0, 0)
#define lmac_set_aid(ops, ifidx, aid)                       lmac_ioctl(ops, LMAC_IOCTL_SET_AID, (uint32)ifidx, (uint32)aid)
#define lmac_set_ssid(ops, ssid)                            lmac_ioctl(ops, LMAC_IOCTL_SET_SSID, (uint32)ssid, 32)
#define lmac_set_wkio_mode(ops, mode)                       lmac_ioctl(ops, LMAC_IOCTL_SET_WAKEUPIO_MODE, (uint32)mode, 0)
#define lmac_set_ps_mode(ops, mode)                         lmac_ioctl(ops, LMAC_IOCTL_SET_PS_MODE, (uint32)mode, 0)
#define lmac_set_sleep_aplost_time(ops, time)               lmac_ioctl(ops, LMAC_IOCTL_SET_SLEEP_APLOST_TIME, (uint32)time, 0)
#define lmac_set_auto_chan_switch(ops, disable)             lmac_ioctl(ops, LMAC_IOCTL_SET_AUTO_CHAN_SWITCH, (uint32)disable, 0)
#define lmac_set_wakeup_io(ops, io, edge)                   lmac_ioctl(ops, LMAC_IOCTL_SET_SLEEP_WAKEIUP_IO, (uint32)io, (uint32)edge)
#define lmac_set_supper_pwr(ops, en)                        lmac_ioctl(ops, LMAC_IOCTL_SET_SUPPER_PWR, (uint32)en, 0)
#define lmac_set_pa_pwr_ctrl(ops, en)                       lmac_ioctl(ops, LMAC_IOCTL_SET_PA_PWR_CTRL, (uint32)en, 0)
#define lmac_set_sta_roaming(ops, addr, en)                 lmac_ioctl(ops, LMAC_IOCTL_SET_STA_ROAMING, (uint32)addr, en)
#define lmac_set_hw_scan(ops, probe, interval)              lmac_ioctl(ops, LMAC_IOCTL_SET_HW_SCAN, (uint32)probe, interval)
#define lmac_set_vdd13(ops, en)                             lmac_ioctl(ops, LMAC_IOCTL_SET_VDD13, (uint32)en, 0)
#define lmac_set_ack_timeout_extra(ops, val)                lmac_ioctl(ops, LMAC_IOCTL_SET_ACK_TIMEOUT_EXTRA, (uint32)val, 0)
#define lmac_set_retry_cnt(ops, frm_max, rts_max)           lmac_ioctl(ops, LMAC_IOCTL_SET_TX_CNT_MAX, (uint32)frm_max, (uint32)rts_max)
#define lmac_set_psconnect_period(ops, period)              lmac_ioctl(ops, LMAC_IOCTL_SET_PSCONNECT_PERIOD, (uint32)period, 0)
#define lmac_set_mcast_txmcs(ops, rate)                     lmac_ioctl(ops, LMAC_IOCTL_SET_MCAST_TX_RATE, (uint32)(rate), 0)
#define lmac_set_mcast_txbw(ops, txbw)                      lmac_ioctl(ops, LMAC_IOCTL_SET_MCAST_TXBW, (uint32)txbw, 0)
#define lmac_set_mcast_txpower(ops, txpwr)                  lmac_ioctl(ops, LMAC_IOCTL_SET_MCAST_TXPOWER, (uint32)txpwr, 0)
#define lmac_set_mcast_dup_txcnt(ops, dupcnt)               lmac_ioctl(ops, LMAC_IOCTL_SET_MCAST_DUP_TXCNT, (uint32)dupcnt, 0)
#define lmac_set_mcast_clear_chn(ops, en)                   lmac_ioctl(ops, LMAC_IOCTL_SET_MCAST_CLEAR_CHN, (uint32)en, 0)
#define lmac_set_ant_dual_en(ops, en)                       lmac_ioctl(ops, LMAC_IOCTL_SET_ANT_DUAL_EN, (uint32)en, 0)
#define lmac_set_ant_ctrl_pin(ops, pin)                     lmac_ioctl(ops, LMAC_IOCTL_SET_ANT_CTRL_PIN, (uint32)pin,0)
#define lmac_set_ant_auto_en(ops, en)                       lmac_ioctl(ops, LMAC_IOCTL_SET_ANT_AUTO_EN, (uint32)en, 0)
#define lmac_set_ant_sel(ops, sel)                          lmac_ioctl(ops, LMAC_IOCTL_SET_ANT_SEL, (uint32)sel, 0)
#define lmac_get_ant_sel(ops)                               lmac_ioctl(ops, LMAC_IOCTL_GET_ANT_SEL, 0, 0)
#define lmac_set_bssid(ops, bssid)                          lmac_ioctl(ops, LMAC_IOCTL_SET_BSSID, (uint32)bssid, 0)
#define lmac_set_ap_psmode_en(ops, en)                      lmac_ioctl(ops, LMAC_IOCTL_SET_AP_PSMODE_EN, (uint32)en, 0)
#define lmac_set_wksrc_detect(ops, io, level)               lmac_ioctl(ops, LMAC_IOCTL_SET_WKSRC_DETECT, (uint32)io, (uint32)level)
#define lmac_set_rtc(ops, rtc)                              lmac_ioctl(ops, LMAC_IOCTL_SET_RTC, (uint32)rtc, 0)
#define lmac_get_rtc(ops)                                   lmac_ioctl(ops, LMAC_IOCTL_GET_RTC, 0, 0)
#define lmac_set_reassociation(ops, aid, addr)              lmac_ioctl(ops, LMAC_IOCTL_SET_REASSOCIATION, (uint32)aid, (uint32)addr)
#define lmac_get_bss_load(ops, bss_load)                    lmac_ioctl(ops, LMAC_IOCTL_GET_BSS_LOAD, (uint32)bss_load, 0)
#define lmac_set_sleep_gpioa_resv(ops, resv)                lmac_ioctl(ops, LMAC_IOCTL_SET_SLEEP_GPIOA_RESV, (uint32)resv, 0)
#define lmac_set_sleep_gpiob_resv(ops, resv)                lmac_ioctl(ops, LMAC_IOCTL_SET_SLEEP_GPIOB_RESV, (uint32)resv, 0)
#define lmac_set_sleep_gpioc_resv(ops, resv)                lmac_ioctl(ops, LMAC_IOCTL_SET_SLEEP_GPIOC_RESV, (uint32)resv, 0)
#define lmac_get_sleep_user_data_addr(ops)                  lmac_ioctl(ops, LMAC_IOCTL_GET_SLEEP_USER_DATA_ADDR, 0, 0)
#define lmac_get_sleep_user_data_len(ops)                   lmac_ioctl(ops, LMAC_IOCTL_GET_SLEEP_USER_DATA_LEN, 0, 0)
#define lmac_set_enter_sleep_cb(ops, func)                  lmac_ioctl(ops, LMAC_IOCTL_SET_ENTER_SLEEP_CB, (uint32)func, 0)
#define lmac_set_keep_alive_cb(ops, func)                   lmac_ioctl(ops, LMAC_IOCTL_SET_KEEP_ALIVE_CB, (uint32)func, 0)
#define lmac_set_wake_up_cb(ops, func)                      lmac_ioctl(ops, LMAC_IOCTL_SET_WAKE_UP_CB, (uint32)func, 0)
#define lmac_radio_onoff(ops, onoff)                        lmac_ioctl(ops, LMAC_IOCTL_SET_RADIO_ONOFF, (uint32)onoff, 0)
#define lmac_get_qa_bw(ops)                                 lmac_ioctl(ops, LMAC_IOCTL_GET_QA_BW, 0, 0)
#define lmac_get_qa_mcs(ops)                                lmac_ioctl(ops, LMAC_IOCTL_GET_QA_MCS, 0, 0)
#define lmac_get_qa_per(ops)                                lmac_ioctl(ops, LMAC_IOCTL_GET_QA_PER, 0, 0)
#define lmac_set_ps_heartbeat(ops, ip, port)                lmac_ioctl(ops, LMAC_IOCTL_SET_PS_HEARBEAT, (uint32)ip, (uint32)port)
#define lmac_set_ps_heartbeat_resp(ops, data, size)         lmac_ioctl(ops, LMAC_IOCTL_SET_PS_HEARBEAT_RESP, (uint32)data, size)
#define lmac_set_ps_wakeup_data(ops, data, size)            lmac_ioctl(ops, LMAC_IOCTL_SET_PS_WAKEUP_DATA, (uint32)data, size)
#define lmac_set_ps_heartbeat_period(ops, period)           lmac_ioctl(ops, LMAC_IOCTL_SET_PS_HEARBEAT_PERIOD, (uint32)period, 0)
#define lmac_set_mcast_key(ops, key, key_len)               lmac_ioctl(ops, LMAC_IOCTL_SET_MCAST_KEY, (uint32)key, key_len)
#define lmac_set_txq_param(ops, ifidx, txq, param)          lmac_ioctl(ops, LMAC_IOCTL_SET_TXQ_PARAM, (uint32)(ifidx<<16|txq), (uint32)param)
#define lmac_set_mac_addr(ops, ifidx, addr)                 lmac_ioctl(ops, LMAC_IOCTL_SET_MAC_ADDR, (uint32)ifidx, (uint32)addr)
#define lmac_set_promisc_mode(ops, enable)                  lmac_ioctl(ops, LMAC_IOCTL_SET_PROMISC_MODE, enable, 0)
#define lmac_get_promisc_mode(ops)                          lmac_ioctl(ops, LMAC_IOCTL_GET_PROMISC_MODE, 0, 0)
#define lmac_set_sta_supp_rate(ops, addr, supp_rate)        lmac_ioctl(ops, LMAC_IOCTL_SET_STA_SUPP_RATE, (uint32)addr, supp_rate)
#define lmac_get_sta_supp_rate(ops, addr)                   lmac_ioctl(ops, LMAC_IOCTL_GET_STA_SUPP_RATE, (uint32 *)addr, 0)
#define lmac_set_supp_rate(ops, supp_rate)                  lmac_ioctl(ops, LMAC_IOCTL_SET_SUPP_RATE, supp_rate, 0)
#define lmac_get_supp_rate(ops)                             lmac_ioctl(ops, LMAC_IOCTL_GET_SUPP_RATE, 0, 0)
#define lmac_set_max_sta_cnt(ops, cnt)                      lmac_ioctl(ops, LMAC_IOCTL_SET_MAX_STA_CNT, cnt, 0)
#define lmac_get_max_sta_cnt(ops)                           lmac_ioctl(ops, LMAC_IOCTL_GET_MAX_STA_CNT, 0, 0)
#define lmac_set_max_ps_frame(ops, cnt)                     lmac_ioctl(ops, LMAC_IOCTL_SET_MAX_PS_FRAME, cnt, 0)
#define lmac_set_tx_duty_cycle(ops, duty)                   lmac_ioctl(ops, LMAC_IOCTL_SET_TX_DUTY_CYCLE, duty, 0)
#define lmac_set_ble_start(ops, chan, recv_cb)        		lmac_ioctl(ops, LMAC_IOCTL_SET_BLENC_START, chan, (uint32)recv_cb)
#define lmac_set_ble_stop(ops)           		    		lmac_ioctl(ops, LMAC_IOCTL_SET_BLENC_STOP, 0, 0)
#define lmac_set_ssid_filter(ops, ssid, len)                lmac_ioctl(ops, LMAC_IOCTL_SET_SSID_FILTER, ssid, len)
#define lmac_get_bg_rssi(ops, chn)                          lmac_ioctl(ops, LMAC_IOCTL_GET_BG_RSSI, chn, 0)
#define lmac_set_bss_scan(ops, period, tmo)                 lmac_ioctl(ops, LMAC_IOCTL_SET_BSS_SCAN, period, tmo)
#define lmac_set_wphy_dpd(ops, param1)                      lmac_ioctl(ops, LMAC_IOCTL_SET_WPHY_DPD, param1, 0)




int32 lmac_ioctl(void *ops, uint32 cmd, uint32 param1, uint32 param2);
int32 lmac_start_acs(void *lops, struct lmac_acs_ctl *p_ctl, uint32 sync);
int32 lmac_set_chan_list(void *lops, uint16 *chan_list, uint16 count);
void *lmac_ah_init(struct lmac_init_param *param);
void *lmac_bgn_init(struct lmac_init_param *param);

#ifdef __cplusplus
}
#endif

#endif
