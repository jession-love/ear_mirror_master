#ifndef _HGIC_IEEE80211_H_
#define _HGIC_IEEE80211_H_

#include "lib/lmac/hgic.h"

#define WPA_KEY_MGMT_PSK  BIT(1)
#define WPA_KEY_MGMT_NONE BIT(2)
#define WPA_KEY_MGMT_SAE  BIT(10)
#define WPA_KEY_MGMT_OWE  BIT(22)

enum IEEE80211_BAND {
    IEEE80211_BAND_2GHZ,
    IEEE80211_BAND_5GHZ,
    IEEE80211_BAND_60GHZ,
    IEEE80211_BAND_S1GHZ,

    NUM_IEEE80211_BANDS,
};

enum country_code {
    COUNTRY_US,
    COUNTRY_EU,
    COUNTRY_JP,
    COUNTRY_CN,
    COUNTRY_KR,
    COUNTRY_SG,
    COUNTRY_AU,
    COUNTRY_NZ,

    NUM_COUNTRIES,
};

enum ieee80211_event {
    IEEE80211_EVENT_PAIR_START = 1,
    IEEE80211_EVENT_PAIR_SUCCESS,
    IEEE80211_EVENT_PAIR_FAIL,
    IEEE80211_EVENT_PAIR_DONE,
    IEEE80211_EVENT_SCAN_START,
    IEEE80211_EVENT_SCAN_DONE,
    IEEE80211_EVENT_CONNECT_START,
    IEEE80211_EVENT_CONNECTED,
    IEEE80211_EVENT_CONNECT_FAIL,
    IEEE80211_EVENT_DISCONNECTED,
    IEEE80211_EVENT_RX_MGMT_FRAME,
    IEEE80211_EVENT_RX_EXT_FRAME,
    IEEE80211_EVENT_RSSI,
    IEEE80211_EVENT_STATE_CHANGE,
    IEEE80211_EVENT_NEW_BSS,
    IEEE80211_EVENT_UPDATE_BSS,
    IEEE80211_EVENT_PS_START,
    IEEE80211_EVENT_PS_END,
    IEEE80211_EVENT_STA_PS_START,
    IEEE80211_EVENT_STA_PS_END,
    IEEE80211_EVENT_INTERFACE_ENABLE,
    IEEE80211_EVENT_INTERFACE_DISABLE,
    IEEE80211_EVENT_ADD_EXTRA_IE,
    IEEE80211_EVENT_MAC_CHANGE,
    IEEE80211_EVENT_EVM,
};

enum wpa_states {
    WPA_DISCONNECTED,
    WPA_INTERFACE_DISABLED,
    WPA_INACTIVE,
    WPA_SCANNING,
    WPA_AUTHENTICATING,
    WPA_ASSOCIATING,
    WPA_ASSOCIATED,
    WPA_4WAY_HANDSHAKE,
    WPA_GROUP_HANDSHAKE,
    WPA_COMPLETED
};

typedef enum {
    IEEE80211_EXTRA_IE_BEACON,
    IEEE80211_EXTRA_IE_PROBE_REQ,
    IEEE80211_EXTRA_IE_PROBE_RESP,
    IEEE80211_EXTRA_IE_ASSOC_REQ,
    IEEE80211_EXTRA_IE_ASSOC_RESP,
} IEEE80211_EXTRA_IE;

typedef enum {
    IEEE80211_PKTHDL_CONTINUE = 0,
    IEEE80211_PKTHDL_CONSUMED = 1,
} ieee80211_pkthdl_res;

struct ieee80211_pkthook;
typedef ieee80211_pkthdl_res(*ieee80211_pkthdl)(struct ieee80211_pkthook *hook, uint8 ifidx, uint8 *data, uint32 len);
struct ieee80211_pkthook {
    struct list_head list;
    const char *name;
    void  *priv;
    uint16 protocol;
    ieee80211_pkthdl tx;
    ieee80211_pkthdl rx;
};

typedef int32(*ieee80211_evt_cb)(uint8 ifidx, uint16 evt, uint32 param1, uint32 param2);
struct ieee80211_initparam {
    uint8            vif_maxcnt;
    uint8            bss_maxcnt;
    uint8            headroom, tailroom;
    uint16           sta_maxcnt;
    uint16           bss_lifetime;
    ieee80211_evt_cb evt_cb;
};

struct ieee80211_scandata {
    uint8 ssid[32];
    uint8 passive_scan: 1,
          flush: 1;
};

struct ieee80211_stainfo {
    uint16 aid;
    uint8  addr[6];
    int8   rssi, evm;
    uint8  wake_reason;
    uint8  ps: 1, connected: 1, wmm: 1;
    uint64 tx_bytes, rx_bytes;
};

extern int32 wpa_passphrase(uint8 *ssid, char *passwd, uint8 psk[32]);
extern int32 ieee80211_init(struct ieee80211_initparam *param);
extern int32 ieee80211_deliver_init(int32 max, uint32 lifetime);
extern int32 ieee80211_support_txw830x(void *ops);
extern int32 ieee80211_support_txw80x(void *ops);
extern int32 ieee80211_iface_create_ap(uint8 ifidx, uint8 band);
extern int32 ieee80211_iface_create_sta(uint8 ifidx, uint8 band);
extern int32 ieee80211_iface_start(uint8 ifidx);
extern int32 ieee80211_iface_stop(uint8 ifidx);
extern int32 ieee80211_iface_suspend(uint8 ifidx);
extern void  ieee80211_status(void);
extern int32 ieee80211_scan(uint8 index, uint8 start, struct ieee80211_scandata *scan_param);
extern int32 ieee80211_scatter_tx(uint8 ifidx, scatter_data *data, uint32 count);
extern int32 ieee80211_tx(uint8 ifidx, uint8 *data, uint32 len);
extern void ieee80211_input(uint8 ifidx, uint8 *data, uint32 len);
extern int32 ieee80211_tx_mgmt(uint8 ifidx, uint8 *mgmt, uint32 len);
extern int32 ieee80211_tx_ether(uint8 ifidx, uint8 *dest, uint16 proto, uint8 *data, uint32 len);
extern int32 ieee80211_disassoc(uint8 ifidx, uint8 *addr);
extern int32 ieee80211_register_pkthook(struct ieee80211_pkthook *hook);
extern ieee80211_evt_cb ieee80211_event_cb(ieee80211_evt_cb cb);
extern const char *ieee80211_cipher_txt(uint32 cipher);
extern const char *ieee80211_key_mgmt_txt(uint32 key_mgmt, uint32 proto);
extern int32 ieee80211_get_bsslist(struct hgic_bss_info *bsslist, int32 list_size);
extern int32 ieee80211_get_stalist(struct hgic_sta_info *stalist, int32 list_size);
extern int32 ieee80211_conf_stabr_table(uint8 ifidx, uint8 max);
extern int32 ieee80211_conf_set_bssbw(uint8 ifidx, uint8 bssbw);
extern int32 ieee80211_conf_set_chanlist(uint8 ifidx, uint16 *chan_list, uint32 count);
extern int32 ieee80211_conf_set_txmcs(uint8 ifidx, uint8 txmcs);
extern int32 ieee80211_conf_set_prichan(uint8 ifidx, uint8 prichan);
extern int32 ieee80211_conf_set_ssid(uint8 ifidx, uint8 *ssid);
extern int32 ieee80211_conf_set_keymgmt(uint8 ifidx, uint32 keymgmt);
extern int32 ieee80211_conf_set_psk(uint8 ifidx, uint8 key[32]);
extern int32 ieee80211_conf_set_beacon_int(uint8 ifidx, uint32 beacon_int);
extern int32 ieee80211_conf_set_dtim_int(uint8 ifidx, uint16 dtim_int);
extern int32 ieee80211_conf_set_bssid(uint8 ifidx, uint8 *bssid);
extern int32 ieee80211_conf_set_channel(uint8 ifidx, uint8 channel);
extern int32 ieee80211_conf_get_mac(uint8 ifidx, uint8 *mac);
extern int32 ieee80211_conf_set_mac(uint8 ifidx, uint8 *mac);
extern int32 ieee80211_conf_set_country(uint8 ifidx, uint8 country);
extern int32 ieee80211_conf_set_bss_max_idle(uint8 ifidx, uint16 max_idle_period);
extern int32 ieee80211_conf_get_connstate(uint8 ifidx);
extern int32 ieee80211_conf_wakeup_sta(uint8 ifidx, uint8 *addr, uint32 reason);
extern int32 ieee80211_conf_set_txpower(uint8 ifidx, uint8 txpower);
extern int32 ieee80211_conf_set_agg_cnt(uint8 ifidx, uint8 agg_cnt);
extern int32 ieee80211_conf_set_psmode(uint8 ifidx, uint8 psmode);
extern int32 ieee80211_conf_set_psconnect(uint8 ifidx, uint8 period, uint8 roundup);
extern int32 ieee80211_conf_set_heartbeat_int(uint8 ifidx, uint32 heartbeat_int);
extern int32 ieee80211_conf_set_wkio_mode(uint8 ifidx, uint8 mode);
extern int32 ieee80211_conf_set_aplost_time(uint8 ifidx, uint32 time);
extern int32 ieee80211_conf_set_auto_chsw(uint8 ifidx, uint8 auto_chsw);
extern int32 ieee80211_conf_set_reassoc_wkhost(uint8 ifidx, uint8 enable);
extern int32 ieee80211_conf_set_wakeup_io(uint8 ifidx, uint8 io, uint8 edge);
extern int32 ieee80211_conf_set_supper_pwr(uint8 ifidx, uint8 enable);
extern int32 ieee80211_conf_set_ack_timeout(uint8 ifidx, uint16 tmo);
extern int32 ieee80211_conf_set_pa_pwr_ctrl(uint8 ifidx, uint8 enable);
extern int32 ieee80211_conf_set_ant_auto(uint8 ifidx, uint8 enable);
extern int32 ieee80211_conf_set_acs(uint8 ifidx, uint8 acs, uint8 tmo);
extern int32 ieee80211_conf_set_wmm(uint8 ifidx, uint8 enable);
extern int32 ieee80211_conf_set_use4addr(uint8 ifidx, uint8 enable);
extern int32 ieee80211_conf_get_stainfo(uint8 ifidx, uint16 aid, struct ieee80211_stainfo *sta);
extern int32 ieee80211_conf_get_stalist(uint8 ifidx, struct ieee80211_stainfo *sta, uint32 count);
extern int32 ieee80211_conf_get_stacnt(uint8 ifidx);
extern int32 ieee80211_conf_set_aphide(uint8 ifidx, uint8 hide);
extern int32 ieee80211_conf_get_bgrssi(uint8 ifidx, uint8 channel);
extern int32 ieee80211_conf_set_mcast_txrate(uint8 ifidx, uint32 txrate);

#endif
