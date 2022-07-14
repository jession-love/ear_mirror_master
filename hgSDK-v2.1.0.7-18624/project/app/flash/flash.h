#include "tx_platform.h"

#include "sys_config.h"
#include "typesdef.h"
#include "osal/irq.h"
#ifdef FLASH_INIT
void update_flash_erase();
void update_flash_write(uint8_t *dst,uint8_t *src,uint32_t len);


#endif



