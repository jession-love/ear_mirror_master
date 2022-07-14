#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/spi.h"

int32 spi_open(struct spi_device *spi, uint32 clk_freq, uint32 work_mode, uint32 wire_mode, uint32 clk_mode)
{
    if(spi && spi->open) {
        return spi->open(spi, clk_freq, work_mode, wire_mode, clk_mode);
    }
    return RET_ERR;
}

int32 spi_close(struct spi_device *spi)
{
    if(spi && spi->close) {
        return spi->close(spi);
    }
    return RET_ERR;
}

int32 spi_ioctl(struct spi_device *spi, uint32 cmd, uint32 param1, uint32 param2)
{
    if(spi && spi->ioctl) {
        return spi->ioctl(spi, cmd, param1, param2);
    }
    return RET_ERR;
}

int32 spi_read(struct spi_device *spi, void *buf, uint32 size)
{
    if(spi && spi->read) {
        return spi->read(spi, buf, size);
    }
    return RET_ERR;
}

int32 spi_write(struct spi_device *spi, const void *buf, uint32 size)
{
    if(spi && spi->write) {
        return spi->write(spi, buf, size);
    }
    return RET_ERR;
}

inline int32 spi_set_cs(struct spi_device *spi, uint32 cs, uint32 value)
{
    if(spi && spi->ioctl) {
        return spi->ioctl(spi, SPI_SET_CS, cs, value);
    }
    return RET_ERR;
}

int32 spi_request_irq(struct spi_device *spi, uint32 irq_flag, spi_irq_hdl irqhdl, uint32 irq_data)
{
    if(spi && spi->request_irq) {
        return spi->request_irq(spi, irq_flag, irqhdl, irq_data);
    }
    return RET_ERR;
}

int32 spi_release_irq(struct spi_device *spi, uint32 irq_flag) {

    if (spi && spi->release_irq) {
        return spi->release_irq(spi, irq_flag);
    }
    return RET_ERR;
}


