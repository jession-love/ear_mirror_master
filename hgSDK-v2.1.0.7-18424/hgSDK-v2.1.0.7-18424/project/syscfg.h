#ifndef _PROJECT_SYSCFG_H_
#define _PROJECT_SYSCFG_H_

enum WIFI_WORK_MODE {
    WIFI_MODE_NONE = 0,
    WIFI_MODE_STA,
    WIFI_MODE_AP,
    WIFI_MODE_APSTA,
};

struct sys_config {
    uint16 magic_num, crc;
    uint16 size, rev1, rev2;
    ///////////////////////////////////////
	uint8  rev3,default_wifi_mode;
    uint8  wifi_mode, bss_bw, tx_mcs, channel;
    uint8  bssid[6], mac[6];
    uint8  ssid[32];
    uint8  key[32];
    uint16 bss_max_idle, beacon_int;
    uint16 ack_tmo, dtim_period;
    uint32 key_mgmt;

    uint32 dhcpc_en:1, dhcpd_en:1, xxxxxx: 30;
    uint32 ipaddr, netmask, gw_ip;
    uint32 dhcpd_startip, dhcpd_endip, dhcpd_lease_time;
    
};

extern struct sys_config sys_cfgs;

int32 wificfg_atcmd_set_ssid(const char *cmd, char *argv[], uint32 argc);
int32 wificfg_atcmd_set_key(const char *cmd, char *argv[], uint32 argc);
int32 wificfg_atcmd_set_wifimode(const char *cmd, char *argv[], uint32 argc);
int32 wificfg_atcmd_load_def(const char *cmd, char *argv[], uint32 argc);
int32 wificfg_atcmd_set_encrypt(const char *cmd, char *argv[], uint32 argc);

void syscfg_set_default_val();
int32 wificfg_flush(uint8 ifidx);

#endif

