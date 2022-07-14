#include "sys_config.h"
#include "typesdef.h"
#include "devid.h"
#include "list.h"
#include "dev.h"
#include "osal/task.h"
#include "osal/semaphore.h"
#include "lib/sdhost/sdhost.h"
#include "hal/gpio.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "osal/task.h"
#include "osal/sleep.h"
#include "osal/timer.h"



struct sdh_device *sdh_test;

#define be32_to_cpu(x) ((uint32_t)(              \
    (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |        \
    (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |        \
    (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |        \
    (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

static uint32_t __rt_fls(uint32_t val)
{
	uint32_t  bit = 32;

	if (!val)
		return 0;
	if (!(val & 0xffff0000u)) 
	{
		val <<= 16;
		bit -= 16;
	}
	if (!(val & 0xff000000u)) 
	{
		val <<= 8;
		bit -= 8;
	}
	if (!(val & 0xf0000000u)) 
	{
		val <<= 4;
		bit -= 4;
	}
	if (!(val & 0xc0000000u)) 
	{
		val <<= 2;
		bit -= 2;
	}
	if (!(val & 0x80000000u)) 
	{
		bit -= 1;
	}

	return bit;
}




static const uint32_t tran_unit[] =
{
    10000, 100000, 1000000, 10000000,
    0,     0,      0,       0
};

static const uint8_t tran_value[] =
{
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};

static const uint32_t tacc_uint[] =
{
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
};

static const uint8_t tacc_value[] =
{
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};


__inline uint32_t GET_BITS(uint32_t *resp,
                               uint32_t  start,
                               uint32_t  size)
{                               
        const int32_t __size = size;
        const uint32_t __mask = (__size < 32 ? 1 << __size : 0) - 1; 
        const int32_t __off = 3 - ((start) / 32);
        const int32_t __shft = (start) & 31;
        uint32_t __res;

        __res = resp[__off] >> __shft;
        if (__size + __shft > 32)
            __res |= resp[__off-1] << ((32 - __shft) % 32);

        return __res & __mask;
}


void sdhost_io_func_init(uint32 req){
	if(req == 1)
		pin_func(HG_SDC_HOST_DEVID,4);
	else
		pin_func(HG_SDC_HOST_DEVID,1);
}

static int32_t sd_parse_scr(struct sdh_device *host)
{
    struct rt_sd_scr *scr = &host->scr;
    uint32_t resp[4];

    resp[3] = host->resp_scr[1];
    resp[2] = host->resp_scr[0];
    scr->sd_version = GET_BITS(resp, 56, 4);
    scr->sd_bus_widths = GET_BITS(resp, 48, 4);

    return 0;
}

static int32_t sd_parse_csd(struct sdh_device *host)
{
    struct rt_mmcsd_csd *csd = &host->csd;
    uint32_t *resp = host->resp_csd;

    csd->csd_structure = GET_BITS(resp, 126, 2);

    switch (csd->csd_structure)
    {
    case 0:
        csd->taac = GET_BITS(resp, 112, 8);
        csd->nsac = GET_BITS(resp, 104, 8);
        csd->tran_speed = GET_BITS(resp, 96, 8);
        csd->card_cmd_class = GET_BITS(resp, 84, 12);
        csd->rd_blk_len = GET_BITS(resp, 80, 4);
        csd->rd_blk_part = GET_BITS(resp, 79, 1);
        csd->wr_blk_misalign = GET_BITS(resp, 78, 1);
        csd->rd_blk_misalign = GET_BITS(resp, 77, 1);
        csd->dsr_imp = GET_BITS(resp, 76, 1);
        csd->c_size = GET_BITS(resp, 62, 12);
        csd->c_size_mult = GET_BITS(resp, 47, 3);
        csd->r2w_factor = GET_BITS(resp, 26, 3);
        csd->wr_blk_len = GET_BITS(resp, 22, 4);
        csd->wr_blk_partial = GET_BITS(resp, 21, 1);
        csd->csd_crc = GET_BITS(resp, 1, 7);

        host->card_blksize = 1 << csd->rd_blk_len;
        host->card_capacity = (csd->c_size + 1) << (csd->c_size_mult + 2);
        host->card_capacity *= host->card_blksize;
        host->card_capacity >>= 10; /* unit:KB */
        host->tacc_clks = csd->nsac * 100;
        host->tacc_ns = (tacc_uint[csd->taac&0x07] * tacc_value[(csd->taac&0x78)>>3] + 9) / 10;
        host->max_data_rate = tran_unit[csd->tran_speed&0x07] * tran_value[(csd->tran_speed&0x78)>>3];

    #if 0
        val = GET_BITS(resp, 115, 4);
        unit = GET_BITS(resp, 112, 3);
        csd->tacc_ns     = (tacc_uint[unit] * tacc_value[val] + 9) / 10;
        csd->tacc_clks   = GET_BITS(resp, 104, 8) * 100;

        val = GET_BITS(resp, 99, 4);
        unit = GET_BITS(resp, 96, 3);
        csd->max_data_rate    = tran_unit[unit] * tran_value[val];
        csd->ccc      = GET_BITS(resp, 84, 12);

        unit = GET_BITS(resp, 47, 3);
        val = GET_BITS(resp, 62, 12);
        csd->device_size      = (1 + val) << (unit + 2);

        csd->read_bl_len = GET_BITS(resp, 80, 4);
        csd->write_bl_len = GET_BITS(resp, 22, 4);
        csd->r2w_factor = GET_BITS(resp, 26, 3);
    #endif
        break;
    case 1:
        host->cardflags |= CARD_FLAG_SDHC;

        /*This field is fixed to 0Eh, which indicates 1 ms. 
          The host should not use TAAC, NSAC, and R2W_FACTOR
          to calculate timeout and should uses fixed timeout
          values for read and write operations*/
        csd->taac = GET_BITS(resp, 112, 8);
        csd->nsac = GET_BITS(resp, 104, 8);
        csd->tran_speed = GET_BITS(resp, 96, 8);
        csd->card_cmd_class = GET_BITS(resp, 84, 12);
        csd->rd_blk_len = GET_BITS(resp, 80, 4);
        csd->rd_blk_part = GET_BITS(resp, 79, 1);
        csd->wr_blk_misalign = GET_BITS(resp, 78, 1);
        csd->rd_blk_misalign = GET_BITS(resp, 77, 1);
        csd->dsr_imp = GET_BITS(resp, 76, 1);
        csd->c_size = GET_BITS(resp, 48, 22);

        csd->r2w_factor = GET_BITS(resp, 26, 3);
        csd->wr_blk_len = GET_BITS(resp, 22, 4);
        csd->wr_blk_partial = GET_BITS(resp, 21, 1);
        csd->csd_crc = GET_BITS(resp, 1, 7);

        host->card_blksize = 512;
        host->card_capacity = (csd->c_size + 1) * 512;  /* unit:KB */
        host->tacc_clks = 0;
        host->tacc_ns = 0;
        host->max_data_rate = tran_unit[csd->tran_speed&0x07] * tran_value[(csd->tran_speed&0x78)>>3];

    #if 0
        csd->tacc_ns     = 0;
        csd->tacc_clks   = 0;

        val = GET_BITS(resp, 99, 4);
        unit = GET_BITS(resp, 96, 3);
        csd->max_data_rate    = tran_unit[unit] * tran_value[val];
        csd->ccc      = GET_BITS(resp, 84, 12);

        val = GET_BITS(resp, 48, 22);
        csd->device_size     = (1 + val) << 10;

        csd->read_bl_len = 9;
        csd->write_bl_len = 9;
        /* host should not use this factor and should use 250ms for write timeout */
        csd->r2w_factor = 2;
    #endif
        break;
    default:
        printf("unrecognised CSD structure version %d!", csd->csd_structure);

        return -EINVAL;
    }
    printf("SD card capacity %d KB.\r\n", host->card_capacity);

    return 0;
}

uint32 select_voltage(struct sdh_device *host, uint32_t ocr)
{
    int bit;
    //extern int ffs32_lsb(uint32_t value);

    ocr &= host->valid_ocr;

    bit = 15;//ffs32_lsb(ocr);
    if (bit) 
    {
        bit -= 1;

        ocr &= 3 << bit;

        host->io_cfg.vdd = bit;
        //mmcsd_set_iocfg(host);
        if(host->iocfg)
			host->iocfg(host,&host->io_cfg);
    } 
    else 
    {
        printf("host doesn't support card's voltages!");
        ocr = 0;
    }

    return ocr;	
}

uint32 sd_power_up(struct sdh_device *host,uint8 bus_w)
{
	int bit = __rt_fls(host->valid_ocr) - 1;
	
	 host->io_cfg.vdd = bit;
	 if (controller_is_spi(host))
	 {
		 host->io_cfg.chip_select = MMCSD_CS_HIGH;
		 host->io_cfg.bus_mode = MMCSD_BUSMODE_PUSHPULL;
	 } 
	 else
	 {
		 host->io_cfg.chip_select = MMCSD_CS_IGNORE;
		 host->io_cfg.bus_mode = MMCSD_BUSMODE_OPENDRAIN;
	 }
	 host->io_cfg.power_mode = MMCSD_POWER_UP;
	 if(bus_w == MMCSD_BUSWIDTH_4)
	 	host->io_cfg.bus_width = MMCSD_BUS_WIDTH_4;
	 else
	 	host->io_cfg.bus_width = MMCSD_BUS_WIDTH_1;
	 
	 host->io_cfg.clock = 400000;
	 
	 if(host->iocfg)
		  host->iocfg(host,&host->io_cfg);

	
	 /*
	  * This delay should be sufficient to allow the power supply
	  * to reach the minimum voltage.
	  */
	 os_sleep_ms(10);
	
	 host->io_cfg.clock = host->freq_min;
	 host->io_cfg.power_mode = MMCSD_POWER_ON;
	 if(host->iocfg)
		  host->iocfg(host,&host->io_cfg);

	
	 /*
	  * This delay must be at least 74 clock sizes, or 1 ms, or the
	  * time required to reach a stable voltage.
	  */
	 os_sleep_ms(10);
	 return 0;
}

void sd_set_clk(struct sdh_device * host,uint32_t clk)
{
	host->io_cfg.clock = clk;
	host->iocfg(host,&host->io_cfg);
}

void sd_set_bus_width(struct sdh_device * host,uint32_t width)
{
	host->io_cfg.bus_width = width;
	host->iocfg(host,&host->io_cfg);
}

uint32 send_idle(struct sdh_device * host)
{
	uint32 ret;
	struct rt_mmcsd_cmd cmd;
	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = GO_IDLE_STATE;
    cmd.arg = 0;
    cmd.flags = RESP_SPI_R1 | RESP_NONE | CMD_BC;

	if(host->cmd)
		ret = host->cmd(host,&cmd);
	else{
		printf("no cmd action register\r\n");
		return 0;
	}
	return ret;
}

uint32 send_all_get_cid(struct sdh_device * host,uint32_t *cid)
{
	uint32 ret;
    struct rt_mmcsd_cmd cmd;

    memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = ALL_SEND_CID;
    cmd.arg = 0;
    cmd.flags = RESP_R2 | CMD_BCR;
	ret = host->cmd(host,&cmd);

	if(ret==0)
		memcpy(cid, cmd.resp, sizeof(uint32_t) * 4);
	
	return ret;
}

uint32 send_get_card_addr(struct sdh_device * host,uint32_t *rca)
{
	uint32 ret;
    struct rt_mmcsd_cmd cmd;
    memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
    cmd.cmd_code = SD_SEND_RELATIVE_ADDR;
    cmd.arg = 0;
    cmd.flags = RESP_R6 | CMD_BCR;	
	ret = host->cmd(host,&cmd);

	*rca = cmd.resp[0] >> 16;
	return 0;
}

uint32 send_select_card(struct sdh_device * host)
{
    struct rt_mmcsd_cmd cmd;
	int ret = 0;
    memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = SELECT_CARD;

    if (host->rca) 
    {
        cmd.arg = host->rca << 16;
        cmd.flags = RESP_R1 | CMD_AC;
    } 
	else 
    {
        cmd.arg = 0;
        cmd.flags = RESP_NONE | CMD_AC;
    }
	
	if(host->cmd)
	   ret = host->cmd(host,&cmd);

	return ret;
}

uint32 send_if_cond(struct sdh_device * host,uint32_t ocr)
{
	struct rt_mmcsd_cmd cmd;
	int ret = 0;
	uint8_t pattern;
	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

	cmd.cmd_code = SD_SEND_IF_COND;
	cmd.arg = ((ocr & 0xFF8000) != 0) << 8 | 0xAA;
	cmd.flags = RESP_SPI_R7 | RESP_R7 | CMD_BCR;

	if(host->cmd)
	   ret = host->cmd(host,&cmd);

	//if (controller_is_spi(host))
	//   pattern = cmd.resp[1] & 0xFF;
	//else
	pattern = cmd.resp[0] & 0xFF;

	if (pattern != 0xAA)
	   return -EINVAL;

	return ret;

}

uint32 send_get_csd(struct sdh_device * host,uint32_t *csd)
{
	int ret;	
	struct rt_mmcsd_cmd cmd;
	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
	
	cmd.cmd_code = SEND_CSD;
	cmd.arg = host->rca << 16;
	cmd.flags = RESP_R2 | CMD_AC;
	ret = host->cmd(host,&cmd);
	
	memcpy(csd, cmd.resp, sizeof(uint32_t) * 4);
	return ret;
}


uint32 send_app_cmd(struct sdh_device *host,uint32 rca)
{
    struct rt_mmcsd_cmd cmd = {0};
	int ret = 0;

    cmd.cmd_code = APP_CMD;
	if(rca){
        cmd.arg = rca << 16;
        cmd.flags = RESP_R1 | CMD_AC;		
	}
	else
	{
        cmd.arg = 0;
        cmd.flags = RESP_R1 | CMD_BCR;		
	}
	if(host->cmd)
	   ret = host->cmd(host,&cmd);
	return ret;
}

uint32 sd_app_set_bus_width(struct sdh_device *host,int32_t width)
{
	int ret = 0;
    struct rt_mmcsd_cmd cmd;

	send_app_cmd(host,host->rca);


    memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = SD_APP_SET_BUS_WIDTH;
    cmd.flags = RESP_R1 | CMD_AC;

    switch (width) 
    {
    case MMCSD_BUS_WIDTH_1:
        cmd.arg = MMCSD_BUS_WIDTH_1;
        break;
    case MMCSD_BUS_WIDTH_4:
        cmd.arg = MMCSD_BUS_WIDTH_4;
        break;
    default:
        return -EINVAL;
    }	

	if(host->cmd)
	   ret = host->cmd(host,&cmd);

	return ret;
}

uint32 send_get_scr(struct sdh_device *host,uint32* scr)
{
    struct rt_mmcsd_cmd cmd;
	int ret;	
	send_app_cmd(host,host->rca);
	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
    cmd.cmd_code = SD_APP_SEND_SCR;
    cmd.arg = 0;
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;
	if(host->cmd)
	   ret = host->cmd(host,&cmd);
	
	host->data.blksize = 8;
	host->data.blks    = 1;
	host->data.err     = 0;
	if(host->read)
		ret = host->read(host,(uint8*)scr);
	
	if(host->data.err != 0)
		return 0;

    scr[0] = be32_to_cpu(scr[0]);
    scr[1] = be32_to_cpu(scr[1]);

	printf("scr:%x %x\r\n",scr[0],scr[1]);
	return 1;
}

uint32 sd_tran_stop(struct sdh_device * host)
{
	int ret = 0;	
	struct rt_mmcsd_cmd  cmd;
	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
	cmd.cmd_code = STOP_TRANSMISSION;
	cmd.arg = 0;
	cmd.flags = RESP_SPI_R1B | RESP_R1B | CMD_AC;
	host->sd_opt  = SD_IDLE;
	if(host->cmd)
	   ret = host->cmd(host,&cmd);

	return ret;
	
}

int sd_multiple_write(struct sdh_device * host,uint32 lba,uint32 len,uint8* buf)
{
	int ret;
	int send_cmd = 1;
	struct rt_mmcsd_cmd  cmd;	
	if(((lba != host->new_lba)||(host->sd_opt != SD_M_W))&& host->sd_stop)
	{
		sd_tran_stop(host);		
		host->new_lba = lba;
	}
	else if(host->sd_opt == SD_IDLE)
	{
		send_cmd = 1;
	}
	else
	{
		send_cmd = 0;
	}


	host->sd_stop = 1;
	host->new_lba = host->new_lba + len/SECTOR_SIZE;
	host->sd_opt  = SD_M_W;
	///////////////////////////////////////////////////////
	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
	cmd.cmd_code = WRITE_MULTIPLE_BLOCK;
	cmd.arg = lba;
    if (!(host->cardflags & CARD_FLAG_SDHC)) 
    {
        cmd.arg <<= 9;
    }	
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;
	if((host->cmd) && send_cmd){	
		ret = host->cmd(host,&cmd);  
		if(ret){
			return -1;
		}
	}
	
	///////////////////////////////////////////////////////
	host->data.blksize = SECTOR_SIZE;
	host->data.blks    = len/SECTOR_SIZE;
	host->data.err     = 0;	
	if(host->write)
		ret = host->write(host,buf);	
	return ret;
}

int sd_multiple_read(struct sdh_device * host,uint32 lba,uint32 len,uint8* buf)
{
	int ret;
	int send_cmd = 1;
	struct rt_mmcsd_cmd  cmd;	
	if(((lba != host->new_lba)||(host->sd_opt != SD_M_R))&& host->sd_stop)
	{
		sd_tran_stop(host);
		host->new_lba = lba;
	}
	else if(host->sd_opt == SD_IDLE)
	{
		send_cmd = 1;
	}
	else
	{
		send_cmd = 0;
	}
	
	host->sd_stop = 1;
	host->new_lba = host->new_lba + len/SECTOR_SIZE;
	host->sd_opt  = SD_M_R;
	///////////////////////////////////////////////////////
	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
	cmd.cmd_code = READ_MULTIPLE_BLOCK;
	cmd.arg = lba;
    if (!(host->cardflags & CARD_FLAG_SDHC)) 
    {
        cmd.arg <<= 9;
    }	
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;
	if((host->cmd) && send_cmd){
		ret = host->cmd(host,&cmd);
		if(ret){
			return -1;
		}
	}
	
	
	///////////////////////////////////////////////////////
	host->data.blksize = SECTOR_SIZE;
	host->data.blks    = len/SECTOR_SIZE;
	host->data.err     = 0;	
	if(host->read)
		ret = host->read(host,buf);	
	return ret;
}


uint32 send_app_op_cond(struct sdh_device *host,
                                uint32_t           ocr,
                                uint32_t          *rocr)
{
    struct rt_mmcsd_cmd cmd;
    uint32_t i;
	int ret;
	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
    cmd.cmd_code = SD_APP_OP_COND;
    cmd.arg = ocr;
    cmd.flags = RESP_SPI_R1 | RESP_R3 | CMD_BCR;

	for(i = 100;i;i--){
		ret = send_app_cmd(host,0);
		if(ret){
			printf("cmd err\r\n");
			break;
		}
		
//        memset(cmd->resp, 0, sizeof(cmd->resp));		
		ret = host->cmd(host,&cmd);
		if(ret){
			printf("cmd2 err\r\n");
			break;
		}
		printf("cmd resp:%x\r\n",cmd.resp[0]);
		if (cmd.resp[0] & CARD_BUSY){
			printf("card busy ok\r\n");
			break;
		}
		os_sleep_ms(10);		
	}

	if(rocr)
		*rocr = cmd.resp[0];

	return ret;
}

void stop_card(){	
	sd_tran_stop(sdh_test);
}

uint32 sd_init(struct sdh_device * host)
{
	uint32 ret;
	uint32_t resp[4];
	uint32_t  ocr;
	uint8_t   bw = 1;
	printf("open_width:%d\r\n",bw);
	if(host->open)
		host->open(host,bw);
	
	sdhost_io_func_init(host->flags&MMCSD_BUSWIDTH_4);
	printf("host->flags:%x\r\n",host->flags);
	if(bw == 4)
		sd_power_up(host,MMCSD_BUSWIDTH_4);
	else
		sd_power_up(host,0);
		
	ret = send_idle(host);
	if(ret)
	{
		printf("idle cmd err\r\n");
		return 1;
	}
	send_if_cond(host,host->valid_ocr);

	ret = send_app_op_cond(host,0x40ff8000,&ocr);
	if(ret){
		printf("init card err\r\n");
		return 1;
	}
	printf("ocr:%x\r\n",ocr);
	ocr = select_voltage(host,ocr);
	printf("cur_ocr:%x\r\n",ocr);

    if (!ocr)
    {
		printf("cal ocr error\r\n");
        return 1;
    }	

	send_idle(host);
    void __delay_asm(uint32 n);
	__delay_asm(100);
	ret = send_if_cond(host,ocr);
	if(ret==0)
		ocr |= 1 << 30;

	ret = send_app_op_cond(host,ocr,NULL);
	if(ret){
		printf("init card app_op_cond err\r\n");
		return 1;
	}	

	send_all_get_cid(host,resp);
	host->card_type = CARD_TYPE_SD;
	memcpy(host->resp_cid,resp,sizeof(host->resp_cid));
	send_get_card_addr(host,&host->rca);
	send_get_csd(host,host->resp_csd);
	sd_parse_csd(host);
	send_select_card(host);
	send_get_scr(host,host->resp_scr);
	sd_parse_scr(host);
	

	if(bw == 4){
		sd_power_up(host,MMCSD_BUSWIDTH_4);
	}
	sd_set_clk(host,24000000);
    /*switch bus width*/
    if ((host->flags & MMCSD_BUSWIDTH_4) &&
        (host->scr.sd_bus_widths & SD_SCR_BUS_WIDTH_4)) 
    {
        ret = sd_app_set_bus_width(host, MMCSD_BUS_WIDTH_4);
        if (ret){
			printf("set bus width err\r\n");
			return 1;
		}
        sd_set_bus_width(host, MMCSD_BUS_WIDTH_4);	
    }	
	
	return 0;
}



uint8 sdbuf_test[1024];
void hg_sdh_test_state(struct sdh_device * host)
{
	uint8 action = 0;
	while(1){
		os_sleep(2);
		action++;
		sd_multiple_read(host,0,1024,sdbuf_test);
		sd_multiple_read(host,2,1024,sdbuf_test);
		sd_multiple_read(host,4,1024,sdbuf_test);
		sd_multiple_read(host,6,1024,sdbuf_test);
		sd_multiple_read(host,8,1024,sdbuf_test);
		printf("sdbuf:%x %x\r\n",sdbuf_test[0],sdbuf_test[1]);
		sdbuf_test[0] = 0x11;
		sdbuf_test[1] = 0x55+action;		
//		printf("write\r\n");
		sd_multiple_write(host,8,1024,sdbuf_test);
	}
}

void sd_open()
{
	sdh_test = (struct sdh_device *)dev_get(HG_SDC_HOST_DEVID);
}

void sdhost_test()
{
	printf("enter sdhost test\r\n");
	sdh_test = (struct sdh_device *)dev_get(HG_SDC_HOST_DEVID);
	//sdhost_io_func_init();

	sd_init(sdh_test);
#if (FS_EN == 0)	
	OS_TASK_INIT("hg_sdh_test",&sdh_test->state_task, hg_sdh_test_state, sdh_test, OS_TASK_PRIORITY_NORMAL, 1024);
#endif	
}


uint32 sdhost_init()
{
	uint32 err = 1;
	struct sdh_device *sdh;
	sdh = (struct sdh_device *)dev_get(HG_SDC_HOST_DEVID);
	if(sdh)
	{
		err = sd_init(sdh);
	}
	return err;
}



