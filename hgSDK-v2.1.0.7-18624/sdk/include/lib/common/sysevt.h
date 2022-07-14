#ifndef _SDK_SYSEVT_H_
#define _SDK_SYSEVT_H_

typedef enum {
    SYSEVT_CONTINUE = 0,
    SYSEVT_CONSUMED = 1,
} sysevt_hdl_res;

typedef sysevt_hdl_res(*sysevt_hdl)(uint32 event_id, uint32 data, uint32 priv);

int32 sys_event_init(uint16 evt_max_cnt);
int32 sys_event_new(uint32 event_id, uint32 data);
int32 sys_event_track(uint32 event_id, sysevt_hdl hdl, uint32 priv);
void sys_event_untrack(uint32 event_id, sysevt_hdl hdl);

#define SYS_EVENT(main, sub) ((main)<<16|(sub&0xffff))

enum SYSEVT_MAINID { /* uint16 */
    SYS_EVENT_NETWORK = 1,
    SYS_EVENT_WIFI,
    SYS_EVENT_LMAC,

    ////////////////////////////////////
    SYSEVT_MAINID_ID,
};

//////////////////////////////////////////
enum SYSEVT_LMAC_SUBEVT { /* uint16 */
    SYSEVT_LMAC_ACS_DONE = 1,
};
#define SYSEVT_NEW_LMAC_EVT(subevt, data)    sys_event_new(SYS_EVENT(SYS_EVENT_LMAC, subevt), data)

//////////////////////////////////////////
enum SYSEVT_WIFI_SUBEVT { /* uint16 */
    SYSEVT_WIFI_CONNECT_START = 1,
    SYSEVT_WIFI_CONNECTTED,
    SYSEVT_WIFI_CONNECT_FAIL,
    SYSEVT_WIFI_DISCONNECT,
    SYSEVT_WIFI_SCAN_START,
    SYSEVT_WIFI_SCAN_DONE,
    SYSEVT_WIFI_STA_DISCONNECT,
    SYSEVT_WIFI_STA_CONNECTTED,
    SYSEVT_WIFI_STA_PS_START,
    SYSEVT_WIFI_STA_PS_END,
};
#define SYSEVT_NEW_WIFI_EVT(subevt, data)    sys_event_new(SYS_EVENT(SYS_EVENT_WIFI, subevt), data)

//////////////////////////////////////////
enum SYSEVT_NETWORK_SUBEVT { /* uint16 */
    SYSEVT_GMAC_LINK_UP = 1,
    SYSEVT_GMAC_LINK_DOWN,
    SYSEVT_LWIP_DHCPC_START,
    SYSEVT_LWIP_DHCPC_DONE,
    SYSEVT_WIFI_DHCPC_START,
    SYSEVT_WIFI_DHCPC_DONE,
    SYSEVT_DHCPD_NEW_IP,
    SYSEVT_DHCPD_IPPOOL_FULL,
};
#define SYSEVT_NEW_NETWORK_EVT(subevt, data) sys_event_new(SYS_EVENT(SYS_EVENT_NETWORK, subevt), data)

//////////////////////////////////////////

#endif
