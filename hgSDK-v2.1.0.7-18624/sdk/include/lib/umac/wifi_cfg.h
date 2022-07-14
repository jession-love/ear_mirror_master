#ifndef _HGUC_WIFI_CFG_H_
#define _HGUC_WIFI_CFG_H_

int32 wificfg_save(int8 force);

void wificfg_set_default();

int32 wificfg_flush(uint8 ifidx);

void wificfg_dump(void);

void wificfg_set_mode(uint8 mode);

int32 wificfg_get_mode(uint8 ifidx);

int32 wificfg_set_bss_bw(uint8 ifidx, uint8 bss_bw);

int32 wificfg_get_bss_bw(uint8 ifidx);

int32 wificfg_set_tx_mcs(uint8 ifidx, uint8 tx_mcs);

int32 wificfg_set_channel(uint8 ifidx, uint32 channel);

int32 wificfg_set_chan_list(uint8 ifidx, uint16 *chan_list, uint8 chan_cnt);

int32 wificfg_set_macaddr(uint8 ifidx, uint8 *addr);

int32 wificfg_set_ssid(uint8 ifidx, uint8 *ssid);

int32 wificfg_set_bssid(uint8 ifidx, uint8 *bssid);

int32 wificfg_set_mcast_key(uint8 ifidx, uint8 *key);

int32 wificfg_set_wpa_psk(uint8 ifidx, uint8 *key);

int32 wificfg_get_psk(uint8 ifidx, uint8 *key);

int32 wificfg_set_key_mgmt(uint8 ifidx, uint32 key_mgmt);

int32 wificfg_get_ssid(uint8 ifidx, uint8 ssid[32]);

int32 wificfg_get_bssid(uint8 ifidx, uint8 *bssid);

int32 wificfg_get_wpa_psk(uint8 ifidx, uint8 psk[32]);

int32 wificfg_get_chan_list(uint8 ifidx, uint16 *chan_list, uint8 chan_cnt);

int32 wificfg_set_acs(uint8 ifidx, uint8 acs, uint8 tmo);

int32 wificfg_set_pri_chan(uint8 ifidx, uint8 pri_chan);

int32 wificfg_set_tx_power(uint8 ifidx, uint8 txpower);

int32 wificfg_get_txpower(uint8 ifidx);

int32 wificfg_set_beacon_int(uint8 ifidx, uint32 beacon_int);

int32 wificfg_set_heartbeat_int(uint8 ifidx, uint32 heartbeat_int);

int32 wificfg_set_bss_maxidle(uint8 ifidx, uint32 max_idle);

int32 wificfg_set_wkio_mode(uint8 ifidx, uint8 mode);

int32 wificfg_set_dtim_period(uint8 ifidx, uint32 dtim_period);

int32 wificfg_set_psmode(uint8 ifidx, uint8 psmode);

int32 wificfg_set_psconnect(uint8 ifidx, uint8 period, uint8 roundup);

int32 wificfg_load_def(uint8 reset);

int32 wificfg_set_sleep_aplost_time(uint8 ifidx, uint32 time);

int32 wificfg_set_agg_cnt(uint8 ifidx, uint8 agg_cnt);

int32 wificfg_get_agg_cnt(uint8 ifidx);

int32 wificfg_set_auto_chan_switch(uint8 ifidx, uint8 disable);

int32 wificfg_set_reassoc_wkhost(uint8 ifidx, uint8 enable);

int32 wificfg_set_wakeup_io(uint8 ifidx, uint8 io, uint8 edge);

int32 wificfg_get_key_mgmt(uint8 ifidx);

int32 wificfg_set_auto_sleep_time(uint8 ifidx, int32 time);

int32 wificfg_set_pair_autostop(uint8 ifidx, uint8 en);

int32 wificfg_set_supper_pwr_off(uint8 ifidx, uint8 off);

int32 wificfg_set_repeater_ssid(uint8 ifidx, uint8 *ssid);

int32 wificfg_set_repeater_psk(uint8 ifidx, uint8 *data, uint32 data_len);

int32 wificfg_set_auto_save(uint8 ifidx, uint8 off);

int32 wificfg_set_dcdc13(uint8 ifidx, uint8 en);

int32 wificfg_set_acktmo(uint8 ifidx, uint16 tmo);

int32 wificfg_set_pa_pwrctrl_dis(uint8 ifidx, uint8 dis);

int32 wificfg_set_dhcpc_en(uint8 ifidx, uint8 en);

int32 wificfg_get_dhcpc_result(uint8 ifidx, struct sk_buff **skb);

int32 wificfg_set_mcast_txparam(uint8 ifidx, uint8 *data);

int32 wificfg_set_ant_auto_off(uint8 ifidx, uint8 off);

int32 wificfg_set_wkhost_reasons(uint8 ifidx, uint8 *reasons);

int32 wificfg_set_mac_filter_en(uint8 ifidx, uint8 en);

int32 wificfg_get_use2addr(void);
#endif
