#include "typesdef.h"
#include "errno.h"
#include "list.h"
#include "dev.h"
#include "devid.h"
#include "list.h"
#include "hal/gpio.h"
#include "osal/string.h"
#include "lib/bus/xmodem/xmodem.h"
#include "cmd_input.h"
#include "boot_lib.h"
#include "hal/spi_nor.h"
#include "ota.h"
#include "hal/uart.h"
#define SERIAL_MAX_LEN	2048
struct xmodem_ota_s
{
	int read;
	int write;
	char buf[SERIAL_MAX_LEN];
};


struct xmodem_ota_s *tmp_s;
static void *xmodem_uart = NULL;

//接收到数据,需要copy到缓冲的buf,因为是xmodem,所以最大缓冲的buf不可能大于2048字节
int cmd_input_xmodem(unsigned char *buf,unsigned int len)
{
	uint32_t flags;

	flags = disable_irq();
	if(tmp_s->read == tmp_s->write)
	{
		tmp_s->read = 0;
		tmp_s->write = 0;
	}
	enable_irq(flags);
    if(SERIAL_MAX_LEN-tmp_s->write >= len)
    {
    	//printf("%s:%s,len:%d\n",__FUNCTION__,buf,len);
    	memcpy(&tmp_s->buf[tmp_s->write],buf,len);
		flags = disable_irq();
		tmp_s->write += len;
		enable_irq(flags);
    }
	
	//printf("%s len:%d\t%X\twrite:%d\tread:%d\n",__FUNCTION__,len,tmp_s.buf[0],tmp_s.write,tmp_s.read);
	return 0;
}

static int32 xmodem_serial_read(uint8 *buf, int32 len)
{
	uint32_t flags;
	int32 ret = RET_OK;
	flags = disable_irq();
	if(tmp_s->write- tmp_s->read >= len)
	{
		memcpy(buf,&tmp_s->buf[tmp_s->read],len);
		tmp_s->read += len;
		goto xmodem_serial_read_end;
	}
	ret = RET_ERR;


xmodem_serial_read_end:
	enable_irq(flags);
	if(ret == RET_OK)
	{
	}
	return ret;
}


static void xmodem_tx_out(uint8 ch)
{
	if(xmodem_uart)
	{
		uart_putc(xmodem_uart,ch);
		//printf("%s tx:%X\n",__FUNCTION__,ch);
	}
	else
	{
		printf("%s no register serial\n",__FUNCTION__);
	}

}

static uint8 xmodem_tx_in(uint32 tmo, int32 *err)
{
    uint8 val = 0;
    *err = -1;
    while (tmo > 0) {
        if (xmodem_serial_read(&val, 1) == RET_OK) {
            *err = 0;
            goto xmodem_tx_in_end;
        } else {
			//printf("tmo:%d\n",tmo);
			os_sleep_ms(1);
            tmo -= 1;
            
        }
    }
xmodem_tx_in_end:
    return val;
}
extern int32 libotaV2_write_fw(uint32 tot_len,uint8 is_once_earse,uint8 isSector,uint8 isVerify,uint32 off, uint8 *data, uint16 len);

void xmodem_ota(void *arg)
{
	uint32_t update_adr;
	uint32_t offset = 0;
	uint32_t ota_flag =- ~0;
	uint32_t earse_block_tmp = -1;
	uint32_t earse_block;
	int *ota_running = (int*)arg;
	int res = RET_OK;
	if(*ota_running == 1)
	{
		return ;
	}
	*ota_running = 1;

	tmp_s = (struct xmodem_ota_s*)malloc(sizeof(struct xmodem_ota_s));
	
	if(!tmp_s)
	{
		printf("not enough malloc space\n");
		*ota_running = 0;
		return;
	}
	tmp_s->write = 0;
	tmp_s->read = 0;

    int32 off  = 0;
    int32 rlen = 0;
    uint32 tot_len;
 
	//注册处理
	xmodem_uart = (void *)dev_get(HG_UART1_DEVID);
	cmd_uart_handle_register(cmd_input_xmodem,CMD_INPUT_XMODEM);
    struct xmodem_hdl xmodem = {
        .out = xmodem_tx_out,
        .in  = xmodem_tx_in,
    };

	
	printf("xmodem_uart:%X\n",(int)xmodem_uart);
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    uint8 *buff = os_malloc(1200);
    if (buff) {
        mcu_disable_watchdog();
        disable_print();
__start:
        off = 0;
        tot_len = 0;
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		//获取第一包数据
        while (off == 0) 
		{
            rlen = xmodem_receive(&xmodem, (off == 0 ? XMODEM_START : XMODEM_ACK), buff + off, 1100);
            if (rlen > 0) 
			{
                off += rlen;
				break;
            }
			//异常
			else
			{
				off = -1;
			}
        }
		//正常第一个数据包需要足够长(1024),能尽量判断到版本号,所以xmodem要配置传输数据长度为1K
		if(off > 0)
		{
			if(off < 1024)
			{
				printf("%s err\n",__FUNCTION__);
				off = -1;
				
			}
			else
			{
				res = libotaV2_write_fw(~0,0,0,0,offset, buff, rlen);
				offset += rlen;
			}


		}
		
		while(off > 0 && res == RET_OK)
		{
			//数据写入
			rlen = xmodem_receive(&xmodem, XMODEM_ACK, buff, 1100);
			if(rlen <= 0)
			{
				//xmodem发送完毕,头数据回写
				if(rlen == 0)
				{
					printf("offset:%d\n",offset);
					res = libotaV2_write_fw(offset,0,0,0,offset, buff, 0);
				}
				break;
			}
			else
			{
				res = libotaV2_write_fw(~0,0,0,0,offset, buff, rlen);
				offset += rlen;

			}
		}

		if(res == RET_ERR)
		{
			xmodem_stop(&xmodem);
		}
        
        os_free(buff);
		cmd_uart_handle_unregister(CMD_INPUT_XMODEM);
        enable_print();
    } 
	else 
	{
        printf("%s:%d err:not enough space\n",__FUNCTION__,__LINE__);
    }
	if(rlen == 0)
	{
		printf("ota xmodem success\n");
	}
	printf("%s end\n",__FUNCTION__);
	*ota_running = 0;
	free(tmp_s);
    return ;

//__retry:
    //xmodem_receive(&xmodem, XMODEM_STOP, buff, 1100);
	
    goto __start;
}



