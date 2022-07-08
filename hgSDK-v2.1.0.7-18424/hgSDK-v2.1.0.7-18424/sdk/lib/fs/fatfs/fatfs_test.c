#include "sys_config.h"

#include "integer.h"
#include "diskio.h"
#include "ff.h"
#include <stdio.h>
#include "osal/sleep.h"
#include "typesdef.h"
#include "osal/task.h"
#include "osal/semaphore.h"
#include "list.h"
#include "dev.h"
#include "sdhost.h"
#include "devid.h"

//#include "test.h"

//#include "osal.h"
#define TEST_INFO_SHOW(...) 
#if FS_EN


static DSTATUS fatfs_status(void *status);
static DSTATUS fatfs_init(void *init_dev);
static DRESULT fatfs_read(void *dev, BYTE* buf, DWORD sector, UINT count);
static DRESULT fatfs_write(void *dev, BYTE* buf, DWORD sector, UINT count);
static DRESULT fatfs_ioctl(void *init_dev, BYTE cmd, void* buf);


static struct fatfs_diskio sdcdisk_driver = {
  .status = fatfs_status,
  .init = fatfs_init,
  .read = fatfs_read,
  .write = fatfs_write,
  .ioctl = fatfs_ioctl
}; 

static FATFS fatfs[1];

DWORD get_fatbase(int num)
{
	return fatfs[num].fatbase;
}


DWORD get_fatfree(int num)
{
	DWORD fre_clust,fre_sect, tot_sect;
	FATFS *fs = &fatfs[num];
	fre_clust = fs->free_clst;
	tot_sect = (fs->n_fatent - 2) * fs->csize;
	fre_sect = fre_clust * fs->csize;
	printf("%s %ldKB\n",__FUNCTION__,fre_sect>>1);
	return fre_sect>>1;
}



static DSTATUS fatfs_status(void *status){
	//TEST_INFO_SHOW ("fatfs_status_test\r\n");
	return 0;
}

static DSTATUS fatfs_init(void *init_dev){
	//TEST_INFO_SHOW ("fatfs_init_test\r\n");
	uint32 err;
	err = sdhost_init();
	return err;
}

static DRESULT fatfs_read(void *dev, BYTE* buf, DWORD sector, UINT count){
	int err;
	TEST_INFO_SHOW ("fatfs_read_test:%d %x %d\r\n",sector,buf,count);
	err = sd_multiple_read((struct sdh_device*)dev,sector,count*512,buf);
	
	return err;
}

static DRESULT fatfs_write(void *dev, BYTE* buf, DWORD sector, UINT count){
	int err;
	TEST_INFO_SHOW ("fatfs_write_test:%d %x %d\r\n",sector,buf,count);
	err = sd_multiple_write((struct sdh_device*)dev,sector,count*512,buf);
	return err;	
}  

static DRESULT fatfs_ioctl(void *init_dev, BYTE cmd, void* buf){
	uint8 ret	= RES_OK;
	switch(cmd)
    {
		case CTRL_SYNC:
			
			break;

		case GET_SECTOR_SIZE:
			*(WORD *)buf = 512;
			ret = RES_OK;
			
			break;
		case GET_BLOCK_SIZE:
			*(DWORD *)buf = 4;
			// printf("*0B:%d\n",*B);
			ret = RES_OK;
			break;

				
		default:
			ret = RES_ERROR;            //not finish
			printf("rtos_sd_ioctl err\n");
			break;
    }
	return ret;

}


bool fatfs_register(){

	int ret = 1;
	struct sdh_device *fatfs_sdh;
	TEST_INFO_SHOW("enter %s test\r\n",__func__);
	fatfs_sdh = (struct sdh_device *)dev_get(HG_SDC_HOST_DEVID);
	if(fatfs_sdh)
	{
		fatfs_register_drive(0,&sdcdisk_driver,fatfs_sdh);
		ret = f_mount (&fatfs[0],_SYSDSK_, 1);
		if (ret) {
			printf("%s ret:%d\n",__FUNCTION__,ret);
			return ret;
		}	
	}
	
	return ret;

}

#endif

