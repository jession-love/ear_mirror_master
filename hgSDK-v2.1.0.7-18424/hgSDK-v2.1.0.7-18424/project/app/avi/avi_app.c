#include "sys_config.h"
#include "typesdef.h"
#include "osal/semaphore.h"
#include "osal/sleep.h"

#if FRAME_MANAGEMENT == 1 && SDH_EN == 1 && FS_EN == 1
#include "frame_memt.h"
#include "avidemux.h"
#include "osal_file.h"

#define AVI_FRAME_NUM 2
#define AVI_FRAME_LEN 100*1024

static uint8_t avi_frame_buf[AVI_FRAME_LEN*AVI_FRAME_NUM] __attribute__ ((section(".psram.src")));
app_analyze avi_app_analyze;

app_frame_management* avi_afmemt;
void avi_app_init()
{
	app_analyze_init_with_name(&avi_app_analyze,"playback",AVI_FRAME_NUM,avi_frame_buf,AVI_FRAME_LEN);
}

void avi_app_reinit()
{
	app_analyze_reinit_with_name(&avi_app_analyze,"playback",AVI_FRAME_NUM,avi_frame_buf,AVI_FRAME_LEN);
}

void *avi_file_parse(char *filepath)
{


	static int init = 0;
	if(!init)
	{
		avi_app_init();
	}
	else
	{
		avi_app_reinit();
	}
	init = 1;

	int res = 1;
	void *ret = NULL;
	struct avifp *a_f = (struct avifp*)malloc(sizeof(struct avifp));
	if(!a_f)
	{
		goto err;
	}
	a_f->fp = NULL;
	a_f->aviinfo_point = NULL;
	a_f->running = 0;
	printf("%s filepath:%s\n",__FUNCTION__,filepath);
	a_f->fp = osal_fopen(filepath,"rb");
	if(!a_f->fp)
	{
		goto err;
	}

	a_f->aviinfo_point = (struct aviinfo*)malloc(sizeof(struct aviinfo));
	if(!a_f->fp)
	{
		goto err;
	}
	ret = avidemux_parse(a_f->fp,a_f->aviinfo_point);
	if(!ret)
	{
		goto err;
	}
	a_f->thread_handle = NULL;
	res = 0;

	err:
		if(!ret && a_f)
		{
			if(a_f->fp)
			{
				osal_fclose(a_f->fp);
				a_f->fp = NULL;
			}
			if(a_f->aviinfo_point)
			{
				free(a_f->aviinfo_point);
				a_f->aviinfo_point = NULL;
			}
			free(a_f);
			a_f = NULL;
		}
		return a_f;
}

void avi_file_free(struct avifp *a_f)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	if(a_f)
	{
	
		//首先关闭线程,然后再关闭相关资源,否则可能引起资源释放,线程却在使用的问题
		if(a_f->thread_handle)
		{
			csi_kernel_task_del(a_f->thread_handle);
		}

		if(a_f->fp)
		{
			osal_fclose(a_f->fp);
			a_f->fp = NULL;
		}
		if(a_f->aviinfo_point)
		{
			free(a_f->aviinfo_point);
			a_f->aviinfo_point = NULL;
		}

		free(a_f);
	}

}


int global_avi_running = 0;
void  avi_read_thread(void *d)
{
	void *fp = NULL;
	int jpg_size  = 0;
	int res = 0;
	uint32_t cur = 0;
	uint32_t base, size;
	struct avistrinfo *str;
	struct aviinfo *aviinfo_point = NULL;
	struct list_head* get_analy_node = NULL;
	unsigned int last_avi_time = 0;
	uint8_t* addr = NULL;
	int err_times = 0;
	aviinfo_point = (struct aviinfo*)malloc(sizeof(struct aviinfo));

	if(!aviinfo_point)
	{
		goto avi_read_err;
	}
	memset(aviinfo_point,0,sizeof(struct aviinfo));
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	fp = osal_fopen("0:DCIM/avi1.AVI","rb");
	if(!fp)
	{
		printf("%s open avi err\n",__FUNCTION__);
		goto avi_read_err;
	}
	avidemux_parse(fp,aviinfo_point);
	str = avidemux_stream(aviinfo_point,0);


	while(!global_avi_running)
	{
		os_sleep_ms(1);
	}
	
	while(!res)
	{
		avidemux_set_cur(str,cur);
		res = avidemux_read_begin(str, &base, &size);
		
		if(res)
		{
			printf("base:%X\tsize:%X\tres:%d\n",base,size,res);
			break;
		}

		
		get_analy_node = get_app_analyze_node(&avi_app_analyze);
		if(get_analy_node)
		{
		
			
			//set_real_buf_timestamp(get_analy_node,krhino_sys_tick_get());
			addr = get_real_buf_adr(get_analy_node);
			jpg_size = avidemux_read(str,addr,size);
			if(jpg_size != size)
			{
				printf("%s:%d err\n",__FUNCTION__,__LINE__);
			}
			printf("jpgsize:%d\n",jpg_size);
			set_app_buf_len(get_analy_node,jpg_size);
		}
		else
		{
			os_sleep_ms(1);
			err_times++;
			if(err_times > 30)
			{
				printf("%s:%d err times too large\n",__FUNCTION__,__LINE__);
				err_times = 0;
			}
			continue;
		}
		if(addr)
		{
			map_realnode_2_app_node_msg(&avi_app_analyze,get_analy_node);
			//等待是否需要播放文件,机制暂时比较简单,后续要看播放机制如何设置
			while(os_jiffies()-last_avi_time <= 30)
			{
				os_sleep_ms(1);
			}
			last_avi_time = os_jiffies();
			wake_up_analyze_list_app(&avi_app_analyze);	
			addr = NULL;
		}
		
		cur++;
	}
	printf("cur:%d\n",cur);
avi_read_err:
	if(fp)
	{
		osal_fclose(fp);
	}
	if(aviinfo_point)
	{
		free(aviinfo_point);
	}
}



void set_web_avi_running(void *d)
{
	struct avifp *a_f = (struct avifp*)d;
	a_f->running = 1;
}
void  web_avi_read_thread(void *d)
{

	void *fp = NULL;
	int jpg_size  = 0;
	int res = 0;
	uint32_t cur = 0;
	uint32_t base, size;
	struct avistrinfo *str;
	struct aviinfo *aviinfo_point = NULL;
	struct list_head* get_analy_node = NULL;
	unsigned int last_avi_time = 0;
	uint8_t* addr = NULL;
	int err_times = 0;
	struct avifp *a_f = (struct avifp*)d;
	aviinfo_point = a_f->aviinfo_point;

	if(!aviinfo_point)
	{
		goto web_avi_read_err;
	}
	fp = a_f->fp;
	if(!fp)
	{
		printf("%s open avi err\n",__FUNCTION__);
		goto web_avi_read_err;
	}
	str = avidemux_stream(aviinfo_point,0);


	while(!a_f->running)
	{
		os_sleep_ms(1);
	}
	
	while(!res)
	{
		//printf("str:%x\tcur:%d\n",str,cur);
		avidemux_set_cur(str,cur);
		res = avidemux_read_begin(str, &base, &size);
		
		if(res)
		{
			printf("base:%X\tsize:%X\tres:%d\n",base,size,res);
			break;
		}

		
		get_analy_node = get_app_analyze_node(&avi_app_analyze);
		if(get_analy_node)
		{
		
			if(size > AVI_FRAME_LEN)
			{
				printf("%s:%d err,read size:%d\n",__FUNCTION__,__LINE__,size);
			}
			//set_real_buf_timestamp(get_analy_node,krhino_sys_tick_get());
			addr = get_real_buf_adr(get_analy_node);
			jpg_size = avidemux_read(str,addr,size);
			if(jpg_size != size)
			{
				printf("%s:%d err\n",__FUNCTION__,__LINE__);
			}
			printf("jpgsize:%d\t%X\t%X\n",jpg_size,addr[0],addr[1]);
			set_app_buf_len(get_analy_node,jpg_size);
		}
		else
		{
			os_sleep_ms(1);
			err_times++;
			if(err_times > 30)
			{
				printf("%s:%d err times too large:%d\tcur:%d\n",__FUNCTION__,__LINE__,os_jiffies(),cur);
				err_times = 0;
			}
			continue;
		}
		if(addr)
		{
			map_realnode_2_app_node_msg(&avi_app_analyze,get_analy_node);
			//等待是否需要播放文件,机制暂时比较简单,后续要看播放机制如何设置
			while(os_jiffies()-last_avi_time <= 30)
			{
				os_sleep_ms(1);
			}
			last_avi_time = os_jiffies();
			wake_up_analyze_list_app(&avi_app_analyze);	
			printf("wake up:%d\n",os_jiffies());
			err_times = 0;
			addr = NULL;
		}
		
		cur++;
	}
	printf("cur:%d\n",cur);
web_avi_read_err:
	//文件关闭由外部去关闭,这里可以发送类似信号量,通知webfile那边teardown,这里就while等待其他任务删除这个任务
	#if 0
	if(fp)
	{
		osal_fclose(fp);
	}
	if(aviinfo_point)
	{
		free(aviinfo_point);
	}
	if(a_f)
	{
		free(a_f);
	}
	#endif
	while(1)
	{
		printf("%s end or err\n",__FUNCTION__);
		//1s去唤醒一次,实际是没有buf,让一些线程知道这里读文件结束
		wake_up_analyze_list_app(&avi_app_analyze);
		os_sleep(1);
	}
}

#endif



