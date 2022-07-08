#ifndef HGIC_WIFI_MGR_H_
#define HGIC_WIFI_MGR_H_

#include "typesdef.h"
#include "errno.h"
#include "list.h"
#include "dev.h"
#include "devid.h"
#include "list.h"
#include "osal/string.h"
#include "osal/mutex.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/task.h"
#include "osal/timer.h"
//#include "hal/gpio.h"
#include "hal/netdev.h"
#include "lib/common/rbuffer.h"
#include "lib/bus/macbus/mac_bus.h"
#include "lib/skb/skb.h"
#include "lib/skb/skb_list.h"
#include "lib/lmac/hgic.h"
#include "lib/net/utils.h"
#include "lib/umac/ieee80211.h"

#define ETH_P_WIFIMGR (0x4855)
#define ETH_P_HGRAW   (0x4884)

#define WIFIMGR_WORK_CNT (4)

struct wifi_mgr;

enum wifimgr_frm_type {
    WIFIMGR_FRM_TYPE_ETHER = 0,
    WIFIMGR_FRM_TYPE_HGIC,
    WIFIMGR_FRM_TYPE_RAW,
};

enum wifi_mode {
    WIFI_MODE_NONE = 0,
    WIFI_MODE_STA,
    WIFI_MODE_AP,
    WIFI_MODE_APSTA,
};

enum wifimgr_stype {
    WIFI_MGR_STYPE_NONE    = 0,
    WIFI_MGR_STYPE_PSALIVE = 1,
};

struct wifimgr_hdr {
    uint16 magic, len;
    uint16 crc;
    uint8  type, rev;
};

typedef int32(*wifimgr_work_cb)(struct wifi_mgr *mgr, uint32 param1, uint32 param2);
struct wifimgr_work {
    uint32 param1, param2;
    wifimgr_work_cb run;
};

struct wifi_dev {
    struct netdev   ndev;
    netdev_input_cb input_cb;
    uint8  addr[6];
    uint8  ifidx;
    uint8  open:1;
    struct wifi_mgr *mgr;
};

struct wifi_mgr {
    struct wifi_dev *devs[2];
    uint8  frm_type, def_ifidx;
    uint32 to_local: 1, to_host: 1;
    uint32 if_recv, if_write, if_err;
    uint16 bus_txcookie, bus_rxcookie;
    ieee80211_evt_cb old_cb;
    struct list_head smod;
    struct mac_bus *bus;
    struct skb_list cmdlist;
    struct skb_list up2host;
    struct os_work  work;
};

struct wifimgr_submod {
    struct list_head list;
    struct wifi_mgr *mgr;
    uint8 type;
    const char *name;
    int32(*event_proc)(struct wifimgr_submod *smod, uint8 ifidx, uint16 evt, uint32 param1, uint32 param2);
    int32(*cmd_proc)(struct wifimgr_submod *smod, struct sk_buff **skb);
    int32(*rx_wifimgr)(struct wifimgr_submod *smod, uint8 ifidx, uint8 *data, uint32 len);
};

struct sk_buff *wifi_mgr_alloc_resp(struct sk_buff *skb, int8 *data, int32 len);
int32 wifi_mgr_notify_host(struct wifi_mgr *mgr, uint8 ifidx, uint8 event_id, uint8 *args, int32 len);
int32 wifi_mgr_buswrite_cmdresp(struct wifi_mgr *mgr, struct sk_buff *skb, int status);
void wifi_mgr_buswrite_data(struct wifi_mgr *mgr, uint8 ifidx, uint8 *data, uint32 len);
int32 wifi_mgr_send_wifimgr_data(uint8 ifidx, uint8 *dest, uint8 stype, uint8 *data, uint32 len);
void wifi_mgr_switch(int8 to_host);
void wifi_mgr_add_submod(struct wifimgr_submod *smod);
int32 wifi_mgr_init(enum mac_bus_type bus_type, int8 frm_type);
void wifi_mgr_proc_hgic_cmd(struct wifi_mgr *mgr, struct sk_buff *skb);

#endif

