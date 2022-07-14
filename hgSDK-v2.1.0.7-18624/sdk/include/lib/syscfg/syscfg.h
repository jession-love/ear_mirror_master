#ifndef _HGIC_SYSCFG_H_
#define _HGIC_SYSCFG_H_

#define SYSCFG_MAGIC (0x1234)

struct syscfg_info {
    struct spi_nor_flash *flash1, *flash2;
    uint32 addr1, addr2;
    uint32 size;
};

int32 syscfg_init(void *cfg, uint32 size);
uint16 syscfg_crc(uint8 *data, uint32 len);
int32 syscfg_read(void *cfg, uint32 size);
int32 syscfg_write(void *cfg, uint32 size);
int32 syscfg_info_get(struct syscfg_info *pinfo);
void syscfg_loaddef(void);
int32 syscfg_save(void);

#endif
