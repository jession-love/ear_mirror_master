#include "sys_config.h"
#include "typesdef.h"
#include "list.h"
#include "dev.h"
#include "devid.h"
#include "osal/string.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "osal/task.h"
#include "osal/sleep.h"
#include "osal/timer.h"
#include "hal/gpio.h"
#include "hal/watchdog.h"
#include "hal/uart.h"
#include "lib/common/common.h"
#include "lib/umac/ieee80211.h"
#include "lib/umac/umac.h"
#include "lib/syscfg/syscfg.h"
#include "lib/atcmd/libatcmd.h"
#include "lwip/ip_addr.h"
#include "lwip/netdb.h"
#include "syscfg.h"

struct sys_config sys_cfgs = {
    .default_wifi_mode     = WIFI_MODE_AP,
    .channel       = CHANNEL_DEFAULT,
    .beacon_int    = 100,
    .dtim_period   = 1,
    .bss_max_idle  = 300,
    .key_mgmt      = WPA_KEY_MGMT_NONE,
    .ipaddr        = IP_ADDR_DEFAULT,
    .netmask       = NET_MASK_DEFAULT,
    .gw_ip         = GW_IP_DEFAULT,
    .dhcpd_startip = DHCP_START_IP_DEFAULT,
    .dhcpd_endip   = DHCP_END_IP_DEFAULT,
    .dhcpd_lease_time = 3600,
    .dhcpd_en      = 1,
    .dhcpc_en      = 1,
};

int32 syscfg_save(void)
{
    return syscfg_write(&sys_cfgs, sizeof(sys_cfgs));
}

#ifdef CONFIG_UMAC4
int32 wificfg_flush(uint8 ifidx)
{
    if (!IS_ZERO_ADDR(sys_cfgs.mac)) {
        ieee80211_conf_set_mac(ifidx, sys_cfgs.mac);
    }
    ieee80211_conf_set_channel(ifidx, sys_cfgs.channel);
    ieee80211_conf_set_beacon_int(ifidx, sys_cfgs.beacon_int);
    ieee80211_conf_set_dtim_int(ifidx, sys_cfgs.dtim_period);
    ieee80211_conf_set_bss_max_idle(ifidx, sys_cfgs.bss_max_idle);
    ieee80211_conf_set_ssid(ifidx, sys_cfgs.ssid);
    ieee80211_conf_set_keymgmt(ifidx, sys_cfgs.key_mgmt);
    ieee80211_conf_set_psk(ifidx, sys_cfgs.key);
    return 0;
}

#else

//hgics configs.
struct umac_config umac_cfg;
struct umac_config *sys_get_umaccfg()
{
    struct umac_config *cfg = &umac_cfg;
    int hg0_sz = 0;
    uint8 channel = 0 ;
    cfg->bss_bw = 8;
    cfg->tx_mcs = 7;
    cfg->primary_chan = 1;
    cfg->chan_cnt = 2;
    cfg->chan_list[0] = 9020;
    cfg->chan_list[1] = 9100;
    if (sys_cfgs.wifi_mode == WIFI_MODE_STA) {
        hg0_sz += (int)os_sprintf(cfg->hg0, "update_config=1\n" \
                                  "network={\n" \
                                  "proto=WPA RSN\n"\
                                  "key_mgmt=WPA-PSK\n" \
                                  "pairwise=TKIP CCMP\n" \
                                  "group=TKIP CCMP\n" \
                                 );
        hg0_sz += (int)os_sprintf(&cfg->hg0[hg0_sz], "ssid=\"%s\"\n", "BGN_test");
        hg0_sz += (int)os_sprintf(&cfg->hg0[hg0_sz], "psk=\"%s\"\n", "12345678");
        hg0_sz += (int)os_strcpy(&cfg->hg0[hg0_sz], "}\n");
        printf("cfg->hg0:%s\r\n", cfg->hg0);
    } else {
        hg0_sz += (int)os_sprintf(cfg->hg0, "country_code=CN\n");
        hg0_sz += (int)os_sprintf(&cfg->hg0[hg0_sz], "ssid=%s\n", sys_cfgs.ssid);
        if (sys_cfgs.channel == 0) {

        } else {
            channel = sys_cfgs.channel;
        }
        hg0_sz += (int)os_sprintf(&cfg->hg0[hg0_sz], "channel=%d\n", channel);
        if (sys_cfgs.key_mgmt == WPA_KEY_MGMT_NONE) {
            hg0_sz += (int)os_strcpy(&cfg->hg0[hg0_sz],
                                     "wpa=0\n" \
                                     "hw_mode=g\n"\
                                    );
        } else {
            hg0_sz += (int)os_sprintf(&cfg->hg0[hg0_sz], "wpa_passphrase=%s\n", sys_cfgs.key);
            hg0_sz += (int)os_strcpy(&cfg->hg0[hg0_sz],
                                     "wpa=2\n" \
                                     "wpa_key_mgmt=WPA-PSK\n" \
                                     "wpa_pairwise=CCMP\n" \
                                     "hw_mode=g\n"\
                                     "ieee80211n=1\n" \
                                     "ht_capab=\n"\
                                    );
        }
    }

    return cfg;
}

//save hgics configs into system.
int sys_save_umaccfg(struct umac_config *cfg)
{
    return 0;
}
#endif

int32 wificfg_atcmd_set_ssid(const char *cmd, char *argv[], uint32 argc)
{
    if (argc == 1) {
        if (os_strlen(argv[0]) >= 32) {
            os_printf("cut off too long ssid!!\r\n");
        }
        os_strncpy(sys_cfgs.ssid, argv[0], 31);
        os_printf("set new ssid:%s\r\n", sys_cfgs.ssid);
#ifdef CONFIG_UMAC4
        ieee80211_conf_set_ssid(sys_cfgs.wifi_mode, sys_cfgs.ssid);
#else
#endif
        syscfg_save();
    }
    return 0;
}

int32 wificfg_atcmd_set_key(const char *cmd, char *argv[], uint32 argc)
{
    if (argc == 1) {
        os_printf("set new key:%s\r\n", argv[0]);
#ifdef CONFIG_UMAC4
        wpa_passphrase(sys_cfgs.ssid, argv[0], sys_cfgs.key);
        ieee80211_conf_set_psk(sys_cfgs.wifi_mode, sys_cfgs.key);
#else
        os_strcpy(sys_cfgs.key, argv[0]);
#endif
        syscfg_save();
    }
    return 0;
}

int32 wificfg_atcmd_set_wifimode(const char *cmd, char *argv[], uint32 argc)
{
    if (argc == 1) {
        os_printf("set wifi mode:%s\r\n", argv[0]);
        if (os_strcmp(argv[0], "ap") == 0 && sys_cfgs.wifi_mode != WIFI_MODE_AP) {
            sys_cfgs.wifi_mode = WIFI_MODE_AP;
#ifdef CONFIG_UMAC4
            ieee80211_iface_stop(WIFI_MODE_STA);
            wificfg_flush(WIFI_MODE_AP);
            ieee80211_iface_start(WIFI_MODE_AP);
#else
#endif
        } else if (os_strcmp(argv[0], "sta") == 0 && sys_cfgs.wifi_mode != WIFI_MODE_STA) {
            sys_cfgs.wifi_mode = WIFI_MODE_STA;
#ifdef CONFIG_UMAC4
            ieee80211_iface_stop(WIFI_MODE_AP);
            wificfg_flush(WIFI_MODE_STA);
            ieee80211_iface_start(WIFI_MODE_STA);
#else
#endif
        }
        syscfg_save();
    }
    return 0;
}

int32 wificfg_atcmd_set_encrypt(const char *cmd, char *argv[], uint32 argc)
{
    if (argc == 1) {
        if ('1' == *argv[0]) {
            os_printf("encrypt = WPA_KEY_MGMT_PSK->BIT(1)\r\n");
            sys_cfgs.key_mgmt = WPA_KEY_MGMT_PSK;
            os_printf("%d\r\n", sys_cfgs.key_mgmt);
            syscfg_save();
        } else if ('0' == *argv[0]){
            os_printf("encrypt = WPA_KEY_MGMT_NONE->BIT(2)\r\n");
            sys_cfgs.key_mgmt = WPA_KEY_MGMT_NONE;
            os_printf("%d\r\n", sys_cfgs.key_mgmt);
            syscfg_save();
        } else {
            os_printf("encrypt atcmd err\r\n");
        }
    }
    return 0;
}

void syscfg_set_default_val()
{
    sysctrl_efuse_mac_addr_calc(sys_cfgs.mac);
    if (IS_ZERO_ADDR(sys_cfgs.mac)) {
        os_random_bytes(sys_cfgs.mac, 6);
        sys_cfgs.mac[0] &= 0xfe;
        os_printf("use random mac "MACSTR"\r\n", MAC2STR(sys_cfgs.mac));
    }

    os_sprintf(sys_cfgs.ssid,"%s%02x%02x%02x",SSID_DEFAULT,sys_cfgs.mac[5],sys_cfgs.mac[4],sys_cfgs.mac[3]);
#ifdef CONFIG_UMAC4
    wpa_passphrase(sys_cfgs.ssid, "12345678", sys_cfgs.key);
#else
    os_strcpy(sys_cfgs.key, "12345678");
#endif
}

