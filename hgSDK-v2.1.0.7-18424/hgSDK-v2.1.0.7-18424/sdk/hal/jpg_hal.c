#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "devid.h"
#include "hal/jpeg.h"



int32 jpg_open(struct jpg_device *p_jpg){
	if(p_jpg->open){
		return p_jpg->open(p_jpg);
	}
    return RET_ERR;
}

int32 jpg_close(struct jpg_device *p_jpg){
	if(p_jpg->close){
		return p_jpg->close(p_jpg);
	}
    return RET_ERR;
}

int32 jpg_init(struct jpg_device *p_jpg, uint32 table_idx,uint32 qt){
	if(p_jpg->init){
		return p_jpg->init(p_jpg,table_idx,qt);
	}
    return RET_ERR;
}

int32 jpg_set_size(struct jpg_device *p_jpg,uint32 h,uint32 w){
    return p_jpg->ioctl(p_jpg,JPG_IOCTL_CMD_SET_SIZE,h,w);
}

//no user
/*
int32 jpg_set_len(struct jpg_device *p_jpg,uint32 buflen,uint32 head_reserver){
	if(p_jpg->set_len){
		return p_jpg->set_len(p_jpg,buflen,head_reserver);
	}
    return RET_ERR;
}
*/

int32 jpg_updata_dqt(struct jpg_device *p_jpg,uint32* dqtbuf){
	
    return p_jpg->ioctl(p_jpg,JPG_IOCTL_CMD_UPDATE_QT,(uint32)dqtbuf,0);
}


int32 jpg_set_addr(struct jpg_device *p_jpg,uint32 addr,uint32 len){
	if(p_jpg->ioctl){
		return p_jpg->ioctl(p_jpg,JPG_IOCTL_CMD_SET_ADR,addr,len);
	}
	return RET_ERR;
}

int32 jpg_set_qt(struct jpg_device *p_jpg,uint32 qt){
	if(p_jpg->ioctl){
		return p_jpg->ioctl(p_jpg,JPG_IOCTL_CMD_SET_QT,qt,0);
	}
	return RET_ERR;
}

int32 jpg_request_irq(struct jpg_device *p_jpg,jpg_irq_hdl isr,uint32 irq, void *dev_id){
	if(p_jpg->request_irq){
		return p_jpg->request_irq(p_jpg,isr,irq,dev_id);
	}
	return RET_ERR;
}

int32 jpg_release_irq(struct jpg_device *p_jpg,uint32 irq){
	if(p_jpg->release_irq){
		return p_jpg->release_irq(p_jpg,irq);
	}
	return RET_ERR;
}

