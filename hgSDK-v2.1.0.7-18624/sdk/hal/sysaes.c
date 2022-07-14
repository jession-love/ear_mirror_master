#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/sysaes.h"

int32 sysaes_encrypt(struct sysaes_dev *dev, struct sysaes_para *para)
{
    if(dev && dev->encrypt) {
        return dev->encrypt(dev, para);
    }
    return RET_ERR;
}

int32 sysaes_decrypt(struct sysaes_dev *dev, struct sysaes_para *para)
{
    if(dev && dev->decrypt) {
        return dev->decrypt(dev, para);
    }
    return RET_ERR;
}


