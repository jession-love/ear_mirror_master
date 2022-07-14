#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/capture.h"


int32 capture_init(struct capture_device *capture, enum capture_channel channel, enum capture_mode mode) {

    if(capture && capture->init) {
        return capture->init(capture, channel, mode);
    }

    return RET_ERR;
}

int32 capture_deinit(struct capture_device *capture, enum capture_channel channel) {

    if(capture && capture->deinit) {
        return capture->deinit(capture, channel);
    }

    return RET_ERR;
}

int32 capture_start(struct capture_device *capture, enum capture_channel channel) {

    if(capture && capture->start) {
        return capture->start(capture, channel);
    }

    return RET_ERR;
}

int32 capture_stop(struct capture_device *capture, enum capture_channel channel) {

    if(capture && capture->stop) {
        return capture->stop(capture, channel);
    }

    return RET_ERR;
}

int32 capture_ioctl(struct capture_device *capture, enum capture_channel channel, enum capture_ioctl_cmd cmd, uint32 param1, uint32 param2) {

    if(capture && capture->ioctl) {
        return capture->ioctl(capture, channel, cmd, param1, param2);
    }

    return RET_ERR;
}

int32 capture_request_irq(struct capture_device *capture, enum capture_channel channel, enum capture_irq_flag irq_flag, capture_irq_hdl irq_hdl, uint32 data) {

    if(capture && capture->request_irq) {
        return capture->request_irq(capture, channel, irq_flag, irq_hdl, data);
    }

    return RET_ERR;
}

int32 capture_release_irq(struct capture_device *capture, enum capture_channel channel) {

    if(capture && capture->release_irq) {
        return capture->release_irq(capture, channel);
    }

    return RET_ERR;
}

