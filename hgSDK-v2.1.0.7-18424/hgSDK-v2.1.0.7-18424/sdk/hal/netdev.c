#include "typesdef.h"
#include "errno.h"
#include "list.h"
#include "dev.h"
#include "osal/task.h"
#include "hal/netdev.h"

int32 netdev_open(struct netdev *ndev, netdev_input_cb input_cb, netdev_event_cb evt_cb)
{
    if (ndev && ndev->open) {
        return ndev->open(ndev, input_cb, evt_cb);
    }
    return -EINVAL;
}

int32 netdev_close(struct netdev *ndev)
{
    if (ndev && ndev->close) {
        return ndev->close(ndev);
    }
    return -EINVAL;
}

int32 netdev_send_data(struct netdev *ndev, uint8 *p_data, uint32 size)
{
    if (ndev && ndev->send_data) {
        return ndev->send_data(ndev, p_data, size);
    }
    return -ENOTSUPP;
}

int32 netdev_send_scatter_data(struct netdev *ndev, scatter_data *p_data, uint32 count)
{
    if (ndev && ndev->send_scatter_data) {
        return ndev->send_scatter_data(ndev, p_data, count);
    }
    return -ENOTSUPP;
}


int32 netdev_ioctl(struct netdev *ndev, uint32 cmd, uint32 param1, uint32 param2)
{
    if (ndev && ndev->ioctl) {
        return ndev->ioctl(ndev, cmd, param1, param2);
    }
    return -EINVAL;
}

void netdev_mdio_write(struct netdev *ndev, uint16 phy_addr, uint16 reg_addr, uint16 data)
{
    if (ndev && ndev->mdio_write) {
        ndev->mdio_write(ndev, phy_addr, reg_addr, data);
    }
}

int32 netdev_mdio_read(struct netdev *ndev, uint16 phy_addr, uint16 reg_addr)
{
    if (ndev && ndev->mdio_read) {
        return ndev->mdio_read(ndev, phy_addr, reg_addr);
    }
    return 0;
}

