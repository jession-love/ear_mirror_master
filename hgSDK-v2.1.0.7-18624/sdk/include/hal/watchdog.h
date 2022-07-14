#ifndef  _HAL_WATCHDOG_H_
#define  _HAL_WATCHDOG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef int32(*wdt_irq_hdl)(void);

struct watchdog_device {
    struct dev_obj dev;
    void(*open)(struct watchdog_device *watchdog, uint32 flags);
    uint32(*start)(struct watchdog_device *watchdog, uint32 timeout_s);
    void(*stop)(struct watchdog_device *watchdog);
    void(*feed)(struct watchdog_device *watchdog);
    int32(*request_irq)(struct watchdog_device *watchdog, wdt_irq_hdl irqhdl);
};

void watchdog_device_open(struct watchdog_device *watchdog, uint32 flags);
void watchdog_device_start(struct watchdog_device *watchdog,uint32 timeout_s);
void watchdog_device_stop(struct watchdog_device *watchdog);
void watchdog_device_feed(struct watchdog_device *watchdog);
int32 watchdog_device_request_irq(struct watchdog_device *watchdog, wdt_irq_hdl irqhdl);


#ifdef __cplusplus
}
#endif
#endif

