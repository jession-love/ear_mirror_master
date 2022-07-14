#include "sys_config.h"
#include "avi_record.h"
#include "remote_control.h"
#include "osal/string.h"
#if FRAME_MANAGEMENT
#include "frame_memt.h"
#endif

#if OPENDML_EN && FRAME_MANAGEMENT


#include "openDML.h"


#include "osal_file.h"
#include "media.h"
#include "osal/sleep.h"
#define RECORD_DIR "0:/DCIM"



extern void *create_video_file(char *dir_name);
extern DWORD get_fatfree(int num);
extern DWORD get_fatbase(int num);


int record_one_photo_for_app(void *fp,void *d,int flen);
int record_one_audio_for_app(void *fp,void *d,int flen);
  
typedef struct {
	struct list_head* (*get_frame)(app_frame_management* afmemt); 
	void (*del_frame)(struct list_head *d,app_frame_management* afmemt);
	int (*write)(void *fp,void *d,int flen);
}user_action;        //jpg的私有行为
 

static user_action record_action = {
	.get_frame = get_app_node_from_deal_list,
	.del_frame = finish_app_node,
	.write     = record_one_photo_for_app,
};

static user_action record_pdm_action = {
	.get_frame = get_app_node_from_deal_list,
	.del_frame = finish_app_node,
	.write     = record_one_audio_for_app,
};




static uint32_t *record_cache_buf = NULL;

#define RECORD_WRITE_UINT	1024
#define RECORD_AUDIO_WRITE_UINT	1024




int record_one_photo_for_app(void *fp,void *d,int flen){
	uint8_t *bp;
	uint32_t wlen = 0;
	if(!d)
	{
		return 1;
	}
	struct list_head* get_f = (struct list_head*) d;
	bp = get_app_buf_adr(get_f);

	while(flen){
		if(flen > RECORD_WRITE_UINT){
		
			flen -= (RECORD_WRITE_UINT);
		
			//printf("H1:%X\t%X\t%X\t%X\n",mjn->buf_addr[0],mjn->buf_addr[1],mjn->buf_addr[0+JPG_HEAD_RESERVER],mjn->buf_addr[1+JPG_HEAD_RESERVER]);
			hw_memcpy0(record_cache_buf,bp+wlen,RECORD_WRITE_UINT);
			osal_fwrite(record_cache_buf, RECORD_WRITE_UINT, 1, fp);
			wlen += RECORD_WRITE_UINT; 
			
		}
		else{	
			//printf("H2:%X\t%X\t%X\t%X\n",mjn->buf_addr[0],mjn->buf_addr[1],mjn->buf_addr[0+JPG_HEAD_RESERVER],mjn->buf_addr[1+JPG_HEAD_RESERVER]);
			hw_memcpy0(record_cache_buf,bp+wlen,flen);
			osal_fwrite(record_cache_buf, flen, 1, fp);
			flen = 0;
		}	

	}
	return 0;

}



int record_one_audio_for_app(void *fp,void *d,int flen){
	uint8_t *bp;
	uint32_t wlen = 0;
	if(!d)
	{
		return 1;
	}
	struct list_head* get_f = (struct list_head*) d;
	bp = get_app_buf_adr(get_f);

	while(flen){
		if(flen > RECORD_AUDIO_WRITE_UINT){
			flen -= RECORD_AUDIO_WRITE_UINT;
			hw_memcpy0(record_cache_buf,bp+wlen,RECORD_AUDIO_WRITE_UINT);
			osal_fwrite(record_cache_buf, RECORD_AUDIO_WRITE_UINT, 1, fp);
			wlen += RECORD_AUDIO_WRITE_UINT; 
			
		}
		else{	
			hw_memcpy0(record_cache_buf,bp+wlen,flen);
			osal_fwrite(record_cache_buf, flen, 1, fp);
			flen = 0;
		}	

	}
	return 0;

}


app_frame_management *afmemt_register(const char *name,const char *action_name,int node_num,void *action)
{
	app_analyze *rtp_app_analyze = NULL;
	app_frame_management* rtp_afmemt = NULL;
	rtp_app_analyze = find_app_analyze(name);
	if(rtp_app_analyze)
	{
		rtp_afmemt = register_user_action(rtp_app_analyze,(void*)action,(char*)action_name);//g_app_analyze(mjpeg)\uvc_app_analyze(uvc)
		app_node_init(rtp_afmemt,node_num);
	}
	return rtp_afmemt;

}



//网络控制是否录像
extern int global_record_avi_thread_flag;
void mjpeg_app_avi_control_thread(void *d){

	//global_record_avi_thread_flag = 1;
	//信号量数量应该根据框架接收数量来配置,最好是等于接收的总数量就可以了
	void *signal = msg_sema_init(NULL,12,sizeof(void*),"avi");
	struct record_video *re_vi = (struct record_video*)d;

	printf("%s r_v:%X\n",__FUNCTION__,(int)re_vi);
	struct list_head* get_f = NULL;
	struct list_head* get_audio_f = NULL;
	void *pavi = NULL;
	void *which = NULL;
	uint32_t flen;
	uint32_t count = 0;
	int32 res;
	int insert = 0;
	int tim_diff = 0;
	int end = 1;
	app_frame_management *avi_app = NULL;
	app_frame_management *pdm_app = NULL;
	AVI_INFO *odmlmsg = NULL;
	uint8_t *headerbuf = NULL;
	char *del_file;
	ODMLBUFF *odml_buff = NULL;
	//写卡缓冲区申请,申请不到就不能执行这个线程
	if(!record_cache_buf)
	{
		record_cache_buf = malloc(RECORD_WRITE_UINT);
	}
	//正常这里是应该退出线程的,但是因为本意是打算录卡,所以报错,好检查什么原因导致申请空间失败
	if(!record_cache_buf)
	{
		while(1)
		{
			printf("%s err,photo_write_cache_buf not enought space:%d\n",__FUNCTION__,RECORD_WRITE_UINT);
			os_sleep(1);
			goto record_finish;
		}
	}


	odmlmsg = malloc(sizeof(AVI_INFO));
	headerbuf = malloc(_ODML_AVI_HEAD_SIZE__);
	user_action* jpg_act = NULL;
	user_action* pdm_act = NULL;
	odml_buff = malloc(sizeof(ODMLBUFF));
#if JPG_EN
	avi_app = afmemt_register("dvp_video","avi_video",6,(void*)&record_action);
#elif USB_EN
	avi_app = afmemt_register("uvc_video","avi_video",6,(void*)&record_action);
#endif
	pdm_app = afmemt_register("pdm","avi_pdm",6,(void*)&record_pdm_action);

 
	if(avi_app)
	{
		jpg_act = (user_action*)avi_app->app_action;
		user_action_access_msg(avi_app,signal);

		app_frame_enable(avi_app,1);
	}
	if(pdm_app)
	{
		pdm_act = (user_action*)pdm_app->app_action;
		user_action_access_msg(pdm_app,signal);
		app_frame_enable(pdm_app,1);
	}
	if(pdm_app)
	{
		printf("%s:%d#############\n",__FUNCTION__,__LINE__);
	}


	printf("odml size:%d\n",_ODML_AVI_HEAD_SIZE__);
	//正常这里是要先判断文件系统是否准备好了没有
	printf("%s:%d   jpg_act:%x %x\n",__FUNCTION__,__LINE__,(int)avi_app,(int)jpg_act);

	if(!avi_app)
	{
		//这里没有找到video,正常要关闭线程
		while(1)
		{
			printf("%s no found video\n",__FUNCTION__);
			os_sleep(1);
			goto record_finish;
		}

	}

	if(!re_vi->running)
	{
		printf("%s not running\n",__FUNCTION__);
		goto record_finish;
	}
	
	while(end)
	{
		//avi_app->down(&avi_app->sem,-1);
		which = avi_app->msg_down(avi_app->signal_msg,-1);
		get_f = NULL;
		get_audio_f = NULL;

		if(which == avi_app)
		{
			get_f = jpg_act->get_frame(avi_app);
		}
		else if(which == pdm_app)
		{
			get_audio_f = pdm_act->get_frame(pdm_app);
		}
		if(get_f)
		{
			if(!pavi)
			{

				//先判断容量空间是否足够,如果不足,查看是否有可以删除的文件,删除对应视频文件直到达到容量位置
				while(get_fatfree(0)<850*1024)
				{
					//文件查找,找出标号最小的进行删除,如果文件系统异常,也要退出
					del_file = get_min_video_file(RECORD_DIR);
					if(del_file)
					{
						osal_unlink(del_file);
					}
					else
					{
						printf("%s no found more file\n",__FUNCTION__);
						break;
					}

					
				}
 
				pavi = create_video_file(RECORD_DIR);
				if(!pavi)
				{
					printf("%s creat file fail\n",__FUNCTION__);
					jpg_act->del_frame(get_f,avi_app);
					goto record_finish;
				}
				ODMLbuff_init(odml_buff);
				odml_buff->ef_time = 1000/FPS;
				odml_buff->ef_fps = FPS;
				odml_buff->vframecnt = 0;
				odml_buff->aframecnt = 0;
				odmlmsg->audiofrq = 16000;
				if(pdm_app)
				{
					odmlmsg->pcm = 1;
				}
				else
				{
					odmlmsg->pcm = 0;
				}
				odmlmsg->win_w = re_vi->width;//
				odmlmsg->win_h = re_vi->height;//
				OMDLvideo_header_write(NULL, pavi,odmlmsg,(ODMLAVIFILEHEADER *)headerbuf);

			}
			
			if(pavi)
			{
				//判断音视频数量是否匹配
				if(odmlmsg->pcm && (odml_buff->vframecnt*odmlmsg->audiofrq*2/(odml_buff->ef_fps*(PCM_SHEET_SIZE-8))>odml_buff->aframecnt))
				{
					printf("video frame and audio frame is not syn\r\n");
					printf("vf:%d\tva:%d\r\n",odml_buff->vframecnt,odml_buff->aframecnt);
					jpg_act->del_frame(get_f,avi_app);
					odml_buff->last_timestamp = csi_kernel_get_ticks();
					continue;
				}

				flen = get_app_buf_len(get_f);		
				odml_buff->cur_timestamp = csi_kernel_get_ticks();
				opendml_write_video2(odml_buff,pavi,jpg_act->write,flen,get_f);
				//printf("flen1:%d\t%d\n",flen,count);
				if(count%90 == 0)
				{
					printf("had record %d min\n",count/1800);
				}

				count ++;
				
				if(pavi)
				{
					//进行补帧操作
					insert = insert_frame(odml_buff,pavi,&tim_diff);
					odml_buff->last_timestamp = odml_buff->cur_timestamp ;
					count += insert;
				}


				if(!re_vi->running || count>60*30)
				{
					stdindx_updata(pavi,odml_buff);
					ODMLUpdateAVIInfo(pavi,odml_buff,odmlmsg->pcm,NULL,(ODMLAVIFILEHEADER *)headerbuf);
					osal_fclose(pavi);
					pavi = NULL;
					count = 0;

					//判断是否需要循环录像
					if(0 || !re_vi->running)
					{
						end = 0;
					}

				}

			}
			else
			{
				end = 0;
			}
			printf("v");
			jpg_act->del_frame(get_f,avi_app);
			
		}
		else if(get_audio_f)
		{
			if(pavi)
			{
				flen = get_app_buf_len(get_audio_f);	
				opendml_write_audio(odml_buff,pavi,pdm_act->write,flen,get_audio_f);
				printf("a");
				pdm_act->del_frame(get_audio_f,pdm_app);
			}
			else
			{
				printf("no avi\n");
				pdm_act->del_frame(get_audio_f,pdm_app);
			} 
		}
	}


record_finish:

	printf("%s finish\n",__FUNCTION__);
	free(odmlmsg);
	free(headerbuf);
	free(odml_buff);
	if(record_cache_buf)
	{
		free(record_cache_buf);
		record_cache_buf = NULL;
	}

	//关闭框架流
	if(avi_app)
	{
		app_frame_enable(avi_app,0);
		unregister_user_action(NULL,avi_app);
	}

	//关闭框架流
	if(pdm_app)
	{
		app_frame_enable(pdm_app,0);
		unregister_user_action(NULL,pdm_app);
	}
	if(signal)
	{
		msg_sema_del(signal);
	}

	
	re_vi->running = 0;
	//global_record_avi_thread_flag = 0;
	csi_kernel_task_del(csi_kernel_task_get_cur());

}
#endif

