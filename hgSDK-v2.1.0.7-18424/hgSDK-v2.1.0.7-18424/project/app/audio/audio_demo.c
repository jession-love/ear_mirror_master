#include "typesdef.h"
#include "list.h"
#include "dev.h"
#include "devid.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "osal/task.h"

#include "frame_memt.h"
#include "pdmFilter.h"
#include "pdm_audio.h"


//对PCM数据的放大倍数，最大127*256
#define PCM_GAIN                                10*256//30*256//(22*256)

static void rm_dc_filter_init(TYPE_FIRST_ORDER_FILTER_TYPE *filter)
{
    memset(filter, 0, sizeof(TYPE_FIRST_ORDER_FILTER_TYPE));
}


#define AUDIONUM	8
#define AUDIOLEN	1024
struct audio_frame_priv
{
	void *pdm_msgq;
	void *audio_hardware_hdl;
	const char *app_name;
	struct os_task     audio_demo_hdl;
	void *priv_malloc_resources;
	app_analyze pdm_app;
	//稳定标志和稳定数量(麦克风启动的时候在不稳定状态,要抛stable_num数量来等待麦克风稳定)
	int stable_flag;
	int stable_num;

};

static void audio_deal_task(void *arg)
{
	 struct audio_frame_priv *priv = (struct audio_frame_priv *)arg;
	 int16* addr;
	 int res;
	 int i;
	 struct list_head* get_analy_node;
	 TYPE_FIRST_ORDER_FILTER_TYPE filter_ctl;
	 rm_dc_filter_init(&filter_ctl);
	 while(1)
	 {
		 res = csi_kernel_msgq_get(priv->pdm_msgq,&get_analy_node,-1);
		 if(!res)
		 	{
		 	//中断有get_analy_node传出来,代表可以发送,如果是NULL,则去设置下一个buf
			 if(get_analy_node)
			 {
			 	#if 1
			 	if(!priv->stable_flag)
			 	{
			 		priv->stable_num--;
			 		if(priv->stable_num <= 0)
			 		{
			 			//等待pdm设备稳定,所以将buf重新给到pdm硬件使用
			 			priv->stable_flag = 1;
			 		}
					force_del_frame(get_analy_node);
					continue;
			 	}
				#endif

				//将音频数据滤波处理
			 	addr = (int16*)get_real_buf_adr(get_analy_node);
				for(i=0; i<AUDIOLEN/2; i++) {
					 addr[i] = rm_dc_filter(&filter_ctl, addr[i]);
					 addr[i] = pcm_volum_gain(addr[i], PCM_GAIN);
				}
 
				 //设置处理完毕的时间戳
				 set_real_buf_timestamp(get_analy_node,csi_kernel_get_ticks());
				 printf("N");
				 map_realnode_2_app_node_msg(&(priv->pdm_app),get_analy_node);
				 wake_up_analyze_list_app(&(priv->pdm_app));	

			 } 

		 }
		 else
		 {
		 	printf("%s:%d err @@@@@@@@@@@@\n",__FUNCTION__,__LINE__);
		 }
	 }
}

void *audio_creat(void *argument) {
	int ret = 0;
	int res;
	const char *name = (const char *)argument;
    struct audio_frame_priv *priv = NULL;
	priv = (struct audio_frame_priv*)malloc(sizeof(struct audio_frame_priv));
	if(!priv)
	{
		ret = -1;
		goto audio_creat_end;
	}
	memset(priv,0,sizeof(struct audio_frame_priv));
	priv->stable_flag = 0;
	priv->stable_num = 5;
	priv->priv_malloc_resources = (void*)malloc(AUDIONUM*AUDIOLEN);
	if(!priv->priv_malloc_resources)
	{
		ret = -2;
		goto audio_creat_end;
	}
	memset(priv->priv_malloc_resources,0,AUDIONUM*AUDIOLEN);
	
	res = app_analyze_init_with_name(&(priv->pdm_app),name,AUDIONUM,priv->priv_malloc_resources,AUDIOLEN);
	if(res < 0)
	{
		ret = -3;
		goto audio_creat_end;
	}
	priv->app_name = name;
	priv->pdm_msgq = csi_kernel_msgq_new(1,sizeof(uint8_t*));
	OS_TASK_INIT("hgpdm_sample_task", &priv->audio_demo_hdl, audio_deal_task, priv, OS_TASK_PRIORITY_NORMAL, 1024);
	return priv;
	
	
	audio_creat_end:
	printf("%s:%d err,ret:%d\n",__FUNCTION__,__LINE__,ret);
	if(priv)
	{
		if(priv->priv_malloc_resources)
		{
			free(priv->priv_malloc_resources);
		}
		free(priv);
	}
	return NULL;
    
}


int audio_destory(void *priv_hdl)
{
	struct audio_frame_priv *priv = (struct audio_frame_priv *)priv_hdl;
	int ret = RET_OK;

	if(priv)
	{
		//先关闭pdm的音频
		if(priv->audio_hardware_hdl)
		{
			pdm_audio_close(priv->audio_hardware_hdl);
		}
		//删除创建的任务
		if(priv->audio_demo_hdl.hdl)
		{
			os_task_del(&priv->audio_demo_hdl);
		}

		//框架流删除
		if(priv->app_name)
		{
			ret = app_analyze_uninstall_with_name(priv->app_name);
			if(ret != RET_OK)
			{
				goto audio_destory_end;
			}
		}

		//释放malloc的资源
		if(priv->priv_malloc_resources)
		{
			free(priv->priv_malloc_resources);
		}
		//释放信号量
		if(priv->pdm_msgq)
		{
			csi_kernel_msgq_del(priv->pdm_msgq);
		}
		//释放句柄
		free(priv);
	}
	audio_destory_end:
	return ret;
}

static void *audio_set_buf(void *priv_el,void *el_point)
{
	struct audio_frame_priv *priv = (struct audio_frame_priv *)priv_el;
	struct list_head* get_analy_node;
	struct list_head **point = (struct list_head**)el_point;
	void *buf = NULL;
	get_analy_node = get_app_analyze_node(&(priv->pdm_app));
	if(get_analy_node)
	{
		
		
		buf = get_real_buf_adr(get_analy_node);
		if(!buf)
		{
			*point = NULL;
			return NULL;
		}
		*point = get_analy_node;
		set_app_buf_len(get_analy_node,AUDIOLEN);
	}
	return buf;
	
}

static void audio_get_buf(void *priv_el,void *el_point)
{
	struct audio_frame_priv *priv = (struct audio_frame_priv *)priv_el;
	struct list_head *get_analy_node = (struct list_head*)el_point;
	int res;
	if(!get_analy_node)
	{
		printf("%s:%d err\n",__FUNCTION__,__LINE__);
		return;
	}
	res = csi_kernel_msgq_put(priv->pdm_msgq,&get_analy_node,0,0);
	//正常应该保证不进这里,如果进来代表任务没有获取队列,直接配置下一个buf导致的
	if(res)
	{
		printf("%s:%d err res:%d!!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__,res);
		force_del_frame(get_analy_node);
	}

	return;
	
}




void *audio_task(const char *name)
{
	struct audio_frame_priv *priv_el;
	void *audio_priv;
	priv_el = audio_creat((void *)name);
	printf("el:%X\n",priv_el);
	audio_priv = pdm_audio_open(PDM_SAMPLE_FREQ_16K,PDM_CHANNEL_RIGHT);
	if(audio_priv)
	{
		priv_el->audio_hardware_hdl = audio_priv;
		pdm_audio_register(audio_priv,priv_el,AUDIOLEN,audio_set_buf,audio_get_buf);
    	pdm_audio_start(audio_priv);
	}
	return (void*)priv_el;
}


