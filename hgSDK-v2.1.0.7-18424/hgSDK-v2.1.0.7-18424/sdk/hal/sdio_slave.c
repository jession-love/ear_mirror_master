#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/sdio_slave.h"

int32 sdio_slave_open(struct sdio_slave *slave, enum sdio_slave_speed speed, uint32 flags)
{
    if (slave && slave->open) {
        return slave->open(slave, speed, flags);
    }
    return RET_ERR;
}

int32 sdio_slave_close(struct sdio_slave *slave)
{
    if (slave && slave->close) {
        return slave->close(slave);
    }
    return RET_ERR;
}

int32 sdio_slave_write(struct sdio_slave *slave, uint8 *buff, uint32 len, int8 sync)
{
    if (slave && slave->write) {
        return slave->write(slave, buff, len, sync);
    }
    return RET_ERR;
}

int32 sdio_slave_read(struct sdio_slave *slave, uint8 *buff, uint32 len, int8 sync)
{
    if (slave && slave->read) {
        return slave->read(slave, buff, len, sync);
    }
    return RET_ERR;
}

int32 sdio_slave_ioctl(struct sdio_slave *slave, uint32 cmd, uint32 param)
{
    if (slave && slave->ioctl) {
        return slave->ioctl(slave, cmd, param);
    }
    return RET_ERR;
}

int32 sdio_slave_set_id(struct sdio_slave *slave, uint16 manufacturer_id, uint16 device_id)
{
    if (slave && slave->set_id) {
        return slave->set_id(slave, manufacturer_id, device_id);
    }
    return RET_ERR;
}

int32 sdio_slave_request_irq(struct sdio_slave *slave, sdio_slave_irq_hdl handle, uint32 data)
{
    if (slave && slave->request_irq) {
        return slave->request_irq(slave, handle, data);
    }
    return RET_ERR;
}

