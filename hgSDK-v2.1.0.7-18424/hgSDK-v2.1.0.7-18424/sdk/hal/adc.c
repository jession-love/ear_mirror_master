#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/adc.h"


int32 adc_open(struct adc_device *adc) {

    if(adc && adc->open) {
        return adc->open(adc);
    }

    return RET_ERR;
}

int32 adc_close(struct adc_device *adc) {

    if(adc && adc->close) {
        return adc->close(adc);
    }

    return RET_ERR;
}

int32 adc_add_channel(struct adc_device *adc, uint32 channel) {

    if (adc && adc->add_channel) {
        return adc->add_channel(adc, channel);
    }
    
    return RET_ERR;

}

int32 adc_delete_channel(struct adc_device *adc, uint32 channel) {

    if (adc && adc->delete_channel) {
        return adc->delete_channel(adc, channel);
    }
    
    return RET_ERR;
}

int32 adc_get_value(struct adc_device *adc, uint32 channel, uint32 *raw_data) {

    if (adc && adc->get_value) {
        return adc->get_value(adc, channel, raw_data);
    }
    
    return RET_ERR;
}

int32 adc_ioctl(struct adc_device *adc, enum adc_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2) {

    if(adc && adc->ioctl) {
        return adc->ioctl(adc, ioctl_cmd, param1, param2);
    }

    return RET_ERR;
}

int32 adc_request_irq(struct adc_device *adc, enum adc_irq_flag irq_flag, adc_irq_hdl irq_hdl, uint32 irq_data) {

    if(adc && adc->request_irq) {
        return adc->request_irq(adc, irq_flag, irq_hdl, irq_data);
    }

    return RET_ERR;
}

int32 adc_release_irq(struct adc_device *adc, enum adc_irq_flag irq_flag) {

    if(adc && adc->release_irq) {
        return adc->release_irq(adc, irq_flag);
    }

    return RET_ERR;
}

