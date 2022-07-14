#include "typesdef.h"
#include "list.h"
#include "dev.h"
#include "devid.h"
#include "hal_i2s_audio.h"
#include "frame_memt.h"
#include "osal/task.h"
#include "hal/i2s.h"
#include "hal/i2c.h"




typedef struct audio_8311_frame_priv
{
	void *mic_msgq;
	const char *app_name;
	app_analyze mic_app;
	app_frame_management* speaker_afmemt;
	void *mic_malloc_resource;
	struct os_task     mic_task_hdl;

}audio_8311_frame_priv;

typedef struct {
	struct list_head* (*get_frame)(app_frame_management* afmemt); 
	void (*del_frame)(struct list_head *d,app_frame_management* afmemt);
	int (*write)(void *fp,void *d,int flen);
}speaker_user_action;        //jpg的私有行为

static speaker_user_action speaker_action={
	.get_frame = get_app_node_from_deal_list_with_del,
	.del_frame = finish_app_node,
	.write     = NULL,
};


#define AUDIONUM	8
#define AUDIOLEN	1024




static void *audio_mic_set_buf(void *priv_el,void *el_point,int *buf_size)
{
	struct audio_8311_frame_priv *priv = (struct audio_8311_frame_priv *)priv_el;
	struct list_head* get_analy_node;
	struct list_head **point = (struct list_head**)el_point;
	void *buf = NULL;
	get_analy_node = get_app_analyze_node(&(priv->mic_app));
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
		*buf_size =get_real_node_buf_len(get_analy_node);
	}
	return buf;
	
}

static void audio_mic_get_buf(void *priv_el,void *el_point,int *buf_size)
{
	struct audio_8311_frame_priv *priv = (struct audio_8311_frame_priv *)priv_el;
	struct list_head *get_analy_node = (struct list_head*)el_point;
	int res;
	if(!get_analy_node)
	{
		printf("%s:%d err\n",__FUNCTION__,__LINE__);
		return;
	}
	res = csi_kernel_msgq_put(priv->mic_msgq,&get_analy_node,0,0);
	//正常应该保证不进这里,如果进来代表任务没有获取队列,直接配置下一个buf导致的
	if(res)
	{
		printf("%s:%d err res:%d!!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__,res);
		force_del_frame(get_analy_node);
	}

	return;
	
}


static void audio_mic_deal_thread(void *arg)
{
	 printf("%s:%d\n",__FUNCTION__,__LINE__);
	 audio_8311_frame_priv *priv = (audio_8311_frame_priv *)arg;
	 int16* addr;
	 int res;
	 int i;
	 struct list_head* get_analy_node;
	 while(1)
	 {
		 res = csi_kernel_msgq_get(priv->mic_msgq,&get_analy_node,-1);
		 if(!res)
		 {
		 	//中断有get_analy_node传出来,代表可以发送,如果是NULL,则去设置下一个buf
			 if(get_analy_node)
			 {
				//将音频数据滤波处理
			 	addr = (int16*)get_real_buf_adr(get_analy_node);
				 //设置处理完毕的时间戳
				 set_real_buf_timestamp(get_analy_node,csi_kernel_get_ticks());
				 //printf("audio len:%d\n",get_app_buf_len(get_analy_node));
				 printf("N");
				 map_realnode_2_app_node_msg(&(priv->mic_app),get_analy_node);
				 wake_up_analyze_list_app(&(priv->mic_app));	
			 } 

		 }
		 else
		 {
		 	printf("%s:%d err @@@@@@@@@@@@\n",__FUNCTION__,__LINE__);
		 }
	 }
}



static void *audio_play_set_buf(void *priv_el,void *el_point,int *buf_size)
{
	audio_8311_frame_priv *priv = (audio_8311_frame_priv*)priv_el;
	speaker_user_action *speaker_act = NULL;
	void * get_f = NULL;
	void *buf = NULL;
	struct list_head **point = (struct list_head**)el_point;
	if(priv->speaker_afmemt)
	{	
		speaker_act = (speaker_user_action*)priv->speaker_afmemt->app_action;
	}
	else
	{
		return NULL;
	}

	get_f = speaker_act->get_frame(priv->speaker_afmemt);
	if(get_f)
	{
		buf = get_app_buf_adr(get_f);
		*buf_size = get_app_buf_len(get_f);
		*point = get_f;
	}
	else
	{
		printf("e");
		return NULL;
	}
	return buf;
	
}


//因为播放音频不需要get_buf,所以基本为空函数,或者处理上一次播放完毕的buf

static void audio_play_get_buf(void *priv_el,void *el_point,int *buf_size)
{

	audio_8311_frame_priv *priv = (audio_8311_frame_priv*)priv_el;
	struct list_head *get_f = (struct list_head*)el_point;
	speaker_user_action* speaker_act = NULL;
	int res;
	if(!get_f)
	{
		printf("%s:%d err\n",__FUNCTION__,__LINE__);
		return;
	}

	if(priv->speaker_afmemt)
	{	
		speaker_act = (speaker_user_action*)priv->speaker_afmemt->app_action;
	}
	else
	{
		return ;
	}

	if(speaker_act)
	{
		speaker_act->del_frame(get_f,priv->speaker_afmemt);
		*buf_size = get_app_buf_len(get_f);
	}


	return;
	
}






audio_8311_frame_priv *audio_8311_creat(void *argument) {
	int ret = 0;
	int res;
	const char *name = (const char *)argument;
    struct audio_8311_frame_priv *priv = NULL;
	priv = (struct audio_8311_frame_priv*)malloc(sizeof(struct audio_8311_frame_priv));
	if(!priv)
	{
		ret = -1;
		goto audio_8311_creat_end;
	}
	memset(priv,0,sizeof(struct audio_8311_frame_priv));
	priv->mic_malloc_resource = (void*)malloc(AUDIONUM*AUDIOLEN);
	if(!priv->mic_malloc_resource)
	{
		ret = -2;
		goto audio_8311_creat_end;
	}
	memset(priv->mic_malloc_resource,0,AUDIONUM*AUDIOLEN);
	
	res = app_analyze_init_with_name(&(priv->mic_app),name,AUDIONUM,priv->mic_malloc_resource,AUDIOLEN);
	if(res < 0)
	{
		ret = -3;
		goto audio_8311_creat_end;
	}
	priv->app_name = name;
	priv->mic_msgq = csi_kernel_msgq_new(1,sizeof(uint8_t*));




	//喇叭的流框架连接,暂时播放pdm的音频
	//创建喇叭的框架空间,同时将麦克风的绑定到喇叭上
	app_analyze *audio_app_analyze = NULL;
	app_frame_management* speaker_afmemt = NULL;
	audio_app_analyze = find_app_analyze("pdm");
	if(audio_app_analyze)
	{
		printf("audio_app_analyze:%X\n",audio_app_analyze);
		speaker_afmemt = register_user_action(audio_app_analyze,(void*)&speaker_action,"speaker");//g_app_analyze(mjpeg)\uvc_app_analyze(uvc)
		app_node_init(speaker_afmemt,8);
		
		//创建speaker切换buf的任务
		priv->speaker_afmemt = speaker_afmemt;
		app_frame_enable(speaker_afmemt,1);
		printf("!!!!!!!!!!!!!!!!!!!!!!%s:%d\n",__FUNCTION__,__LINE__);
	}

	OS_TASK_INIT("hgpdm_sample_task", &priv->mic_task_hdl, audio_mic_deal_thread, priv, OS_TASK_PRIORITY_NORMAL, 1024);
	return priv;
	
	
	audio_8311_creat_end:
	printf("%s:%d err,ret:%d\n",__FUNCTION__,__LINE__,ret);
	if(priv)
	{
		if(priv->mic_malloc_resource)
		{
			free(priv->mic_malloc_resource);
		}
		free(priv);
	}
	return NULL;
    
}

static uint8 sim_mic_frame_buf[AUDIONUM * AUDIOLEN] __attribute__((aligned(4)));

#define MUSIC_FILE_NAME     "music16.pcm"

static void sim_mic_thread(void *arg)
{
	int res;
	int readLen;
	void *fp = osal_fopen(MUSIC_FILE_NAME,"rb");
	struct list_head* get_analy_node;
	app_analyze sim_mic_app_analyze;
	if(!fp)
	{
		printf("%s:%d no found file\n",__FUNCTION__,__LINE__);
	}

	printf("%s:%d\n",__FUNCTION__,__LINE__);

	res = app_analyze_init_with_name(&sim_mic_app_analyze,"pdm",AUDIONUM,sim_mic_frame_buf,AUDIOLEN);

	while(1)
	{
		get_analy_node = get_app_analyze_node(&sim_mic_app_analyze);
		if(get_analy_node)
		{
			//printf("rd:%X\n",get_real_buf_adr(get_analy_node));
			readLen = osal_fread(get_real_buf_adr(get_analy_node),1,AUDIOLEN,fp);
			if(readLen<=0)
			{
				if(readLen<0)
				{
					printf("%s:%d err\n",__FUNCTION__,__LINE__);
					return;
				}
				osal_fseek(fp,0);
				force_del_frame(get_analy_node);
				continue;
				
			}

			if(readLen<AUDIOLEN)
			{
				osal_fseek(fp,0);
			}
			
			//配置当前中断的buf
			set_app_buf_len(get_analy_node,readLen);
			map_realnode_2_app_node_msg(&sim_mic_app_analyze,get_analy_node);
			wake_up_analyze_list_app(&sim_mic_app_analyze);  
			printf("S");
			

		}
		else
		{
			//printf(".");
			os_sleep_ms(1);
		}
	}


}


//8311  的i2c配置寄存器初始化
int audio_8311_init(int i2c_devid) 
{

	int res = RET_ERR;
    int8  addr_8311 = 0x18;
    uint32 i = 0;

    typedef struct _regs_8311 {
        uint8 addr;
        uint8 value;
    }res_8311;

    res_8311 init_reg_8311[] = {
        {0x45,0x00},
        {0x01,0x30},
        {0x02,0x10},
        
        {0x02,0x00},
        
        {0x03,0x10},
        {0x16,0x24},
        {0x04,0x10},
        {0x05,0x00},
        {0x0B,0x00},
        {0x0C,0x00},
        {0x10,0x1F},
        {0x11,0x7F},
        {0x00,0x80},
        
        //delay 专属标志
        {0xAA,0x55},
        
        {0x0D,0x01},
        
        {0x01,0x3F},
        
        {0x14,0x10},    //PGA GAIN
        {0x12,0x00},
        {0x13,0x10},

        {0x09,0x0c},
        {0x0A,0x0c},

        {0x0E,0x02},
        {0x0F,0x44},
        {0x15,0x00},
        {0x1B,0x0A},
        {0x1C,0x6A},
        {0x37,0x08},
        {0x44,0x08},
        
        {0x17,0xE8},    //MIC声音
        
        {0x32,0xBF},    //音量
    };

    struct i2c_device *m_iic  = (struct i2c_device *)dev_get(i2c_devid);

    if (!m_iic) {
        goto audio_8311_init_end;
    }


    i2c_open(m_iic, IIC_MODE_MASTER, IIC_ADDR_7BIT, addr_8311);
    i2c_set_baudrate(m_iic, 100000);
    i2c_ioctl(m_iic, IIC_SDA_OUTPUT_DELAY, 0);

    //8311初始化
    for (i=0; i < (sizeof(init_reg_8311) / sizeof(init_reg_8311[0])); i++) {
        //判断delay标志
        if (((0xAA) == init_reg_8311[i].addr) && ((0x55) == init_reg_8311[i].value)) {
            os_sleep_ms(20);
            continue;
        }
        i2c_write(m_iic, &addr_8311, 0, (int8 *)&init_reg_8311[i], 2);
    }

    /* 等待8311初始化完成 */
    os_sleep(1);

    i2c_close(m_iic);
	res = RET_OK;
    printf("----> 8311 init success\n\r");
	audio_8311_init_end:

	return res;
}



#define PLAY_EMPTY_SIZE 1024

struct os_task     sim_mic_task;


audio_8311_frame_priv * audio_8311_demo_init()
{
	int res = RET_ERR;
	int ret;
	audio_8311_config *play = NULL;
	audio_8311_config *mic = NULL;
	void *empty_buf = NULL;
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	play = (audio_8311_config*)malloc(sizeof(audio_8311_config));
	mic = (audio_8311_config*)malloc(sizeof(audio_8311_config));
	empty_buf = (void*)malloc(PLAY_EMPTY_SIZE);

	if(!play || !mic || !empty_buf)
	{
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		goto audio_8311_demo_init_err;
	}
	memset(play,0,sizeof(audio_8311_config));
	memset(mic,0,sizeof(audio_8311_config));
	memset(empty_buf,0,PLAY_EMPTY_SIZE);
	#if 1
	//先通过i2c初始化8311硬件
	ret = audio_8311_init(HG_I2C1_DEVID);
	if(ret != RET_OK)
	{
		printf("%s:%d\tret:%d\n",__FUNCTION__,__LINE__,ret);
		goto audio_8311_demo_init_err;
	}
	#endif
	//OS_TASK_INIT("sim_mic", &sim_mic_task, sim_mic_thread, (uint32)NULL, OS_TASK_PRIORITY_BELOW_NORMAL, 1024);
	//os_sleep_ms(200);

	//该任务及资源初始化
	audio_8311_frame_priv *audio_8311_priv = audio_8311_creat("pdm");
	//8311 install,注册对应函数
	//对play mic config初始化
	play->type = PLAY_MODE;
	play->i2s_devid = HG_IIS0_DEVID;
	play->sample_freq = I2S_SAMPLE_FREQ_16K;
	play->sample_bit = I2S_SAMPLE_BITS_16BITS;
	//play->data_fmt= I2S_DATA_FMT_LSB;
	play->priv_el = audio_8311_priv;
	play->set_buf = audio_play_set_buf;
	play->get_buf = audio_play_get_buf;
	play->buf_size = PLAY_EMPTY_SIZE;
	play->play_empty_buf = empty_buf;


	mic->type = MIC_MODE;
	mic->i2s_devid = HG_IIS1_DEVID;
	mic->sample_freq = I2S_SAMPLE_FREQ_16K;
	mic->sample_bit = I2S_SAMPLE_BITS_16BITS;
	//mic->data_fmt= I2S_DATA_FMT_LSB;
	mic->priv_el = audio_8311_priv;
	mic->set_buf = audio_mic_set_buf;
	mic->get_buf = audio_mic_get_buf;
	audio_8311_driver_install(play,mic);
	//启动对应的任务
	printf("%s:%d\n",__FUNCTION__,__LINE__);	
	return audio_8311_priv;
	audio_8311_demo_init_err:
	if(play)
	{
		free(play);
	}

	if(mic)
	{
		free(mic);
	}
	if(empty_buf)
	{
		free(empty_buf);
	}
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return NULL;

	
}

