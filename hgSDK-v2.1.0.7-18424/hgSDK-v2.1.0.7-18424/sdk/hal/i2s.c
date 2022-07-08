#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/i2s.h"

int32 i2s_open(struct i2s_device *i2s, enum i2s_mode mode, enum i2s_sample_freq frequency, enum i2s_sample_bits bits)
{
    if (i2s && i2s->open) {
        return i2s->open(i2s, mode, frequency, bits);
    }

    return RET_ERR;
}

int32 i2s_close(struct i2s_device *i2s){

    if (i2s && i2s->close) {
        return i2s->close(i2s);
    }

    return RET_ERR;
}

int32 i2s_write(struct i2s_device *i2s, const void* buf, uint32 len) {

    if (i2s && i2s->write) {
        return i2s->write(i2s, buf, len);
    }

    return RET_ERR;
}

int32 i2s_read(struct i2s_device *i2s, void* buf, uint32 len) {

    if (i2s && i2s->read) {
        return i2s->read(i2s, buf, len);
    }

    return RET_ERR;
}

int32 i2s_ioctl(struct i2s_device *i2s, uint32 cmd, uint32 param) {

    if (i2s && i2s->ioctl) {
        return i2s->ioctl(i2s, cmd, param);
    }

    return RET_ERR;
}

int32 i2s_request_irq(struct i2s_device *i2s, uint32 irq_flag, i2s_irq_hdl irqhdl, uint32 irq_data) {

    if (i2s && i2s->request_irq) {
        return i2s->request_irq(i2s, irq_flag, irqhdl, irq_data);
    }

    return RET_ERR;
}

int32 i2s_release_irq(struct i2s_device *i2s, uint32 irq_flag) {

    if (i2s && i2s->release_irq) {
        return i2s->release_irq(i2s, irq_flag);
    }

    return RET_ERR;
}



