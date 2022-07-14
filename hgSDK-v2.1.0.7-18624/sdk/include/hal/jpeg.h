#ifndef _HAL_JPEG_H_
#define _HAL_JPEG_H_

enum jpg_device_irq_flag {
    JPG_IRQ_FLAG_JPG_DONE     = BIT(0),
    JPG_IRQ_FLAG_JPG_BUF_FULL = BIT(1),
    JPG_IRQ_FLAG_ERROR        = BIT(2),
};

enum jpg_ioctl_cmd {
    JPG_IOCTL_CMD_SET_ADR,
    JPG_IOCTL_CMD_SET_QT,
    JPG_IOCTL_CMD_SET_SIZE,
    JPG_IOCTL_CMD_UPDATE_QT,

};


typedef int32(*jpg_irq_hdl)(uint32 irq_flag, uint32 irq_data, uint32 param1, uint32 param2);

struct jpg_device {
    struct dev_obj dev;
    int32(*open)(struct jpg_device *jpg);
    int32(*close)(struct jpg_device *jpg);
    int32(*init)(struct jpg_device *jpg, uint32 table_idx, uint32 qt);
    int32(*ioctl)(struct jpg_device *jpg, enum jpg_ioctl_cmd cmd, uint32 param1, uint32 param2);
    int32(*request_irq)(struct jpg_device *jpg, jpg_irq_hdl irq_hdl, uint32 irq_flag,  uint32 irq_data);
    int32(*release_irq)(struct jpg_device *jpg, uint32 irq_flag);
};


int32 jpg_open(struct jpg_device *jpg);
int32 jpg_close(struct jpg_device *jpg);
int32 jpg_init(struct jpg_device *jpg, uint32 table_idx, uint32 qt);
int32 jpg_updata_dqt(struct jpg_device *p_jpg, uint32 *dqtbuf);

int32 jpg_set_size(struct jpg_device *p_jpg, uint32 h, uint32 w);

int32 jpg_set_addr(struct jpg_device *jpg, uint32 addr, uint32 buflen);
int32 jpg_set_qt(struct jpg_device *jpg, uint32 qt);
int32 jpg_request_irq(struct jpg_device *jpg, jpg_irq_hdl irq_hdl, uint32 irq_flags,  void *irq_data);
int32 jpg_release_irq(struct jpg_device *jpg, uint32 irq_flags);


#endif
