/*
 * Copyright (C) 2004 Nathan Lutchansky <lutchann@litech.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdlib.h>
#include <stdio.h>
//#include <test_util.h>
#include <csi_config.h>
 
#include <event.h>
#include <log.h>
#include <frame.h>
#include <list.h>
//#include <linux/linux_mutex.h>
#include "spook_config.h"
#include "rtp.h"

#include "lwip/api.h"
#include <csi_kernel.h>

#include "csi_core.h"
#include "list.h"

#include "jpgdef.h"


#include "osal/sleep.h"

#ifdef USB_EN
#include "dev/usb/uvc_host.h"
#endif

#include "osal/string.h"


struct frame *new_frame(void)
{
	struct frame *f;

	//新帧
	f = (struct frame*)malloc(sizeof(struct frame));

	return f;
}

struct frame *new_audio_frame(void)
{
	struct frame *f;

	//新帧
	f = (struct frame*)malloc(sizeof(struct frame));
	return f;
}




void unref_frame( struct frame *f )
{
//	int r;
	//del_frame
	if(!f)
	{
		return;
	}
	#if (JPG_EN == 1 || USB_EN == 1) && FRAME_MANAGEMENT != 1
		DEL_JPG_FRAME(f->get_f);
	#elif FRAME_MANAGEMENT == 1
	app_frame_management* rtp_app;
	rtp_user_action* rtp_act;
	rtp_app = f->app;
	rtp_act = (rtp_user_action*)rtp_app->app_action;
	rtp_act->del_frame(f->get_f,rtp_app);
	#endif

}



k_task_handle_t jpeg_send_handle;





#if FRAME_MANAGEMENT == 1
#define SCAN_BUF_LEN	1024
#if SCAN_BUF_LEN > SPOOK_CACHE_BUF_LEN
	#err "please check SCAN_BUF_LEN and SPOOK_CACHE_BUF_LEN"
#endif
extern unsigned int live_buf_cache[SPOOK_CACHE_BUF_LEN/4];
unsigned char *scan_buf = (unsigned char*)live_buf_cache;
static rtp_user_action rtp_action={
	.get_frame = get_app_node_from_deal_list,
	.del_frame = finish_app_node,
	.write     = NULL,
};
  
static rtp_user_action rtp_audio_action={
	.get_frame = get_app_node_from_deal_list,
	.del_frame = finish_app_node,
	.write     = NULL,
};
#endif

extern app_analyze pdm_app_analyze;
#if FRAME_MANAGEMENT == 1

//设置msg的数量,用于video与audio共用,通过这个消息知道是哪个流发送的,替换传统的wakeup,效率可能比传统wakeup低,应该影响不大
/*
	msg_count:消息的总长度,因为与video与audio共用,可能会被某个全部占用,导致其他的接入不了,所以最上层(消耗msg的线程一般优先级比较高或者处理时间比较短,
	           能尽快将msg获取到,这样才不会有可能导致阻塞)
	msg_size:传递的参数size,一般用指针就可以了
*/

void *spook_register_video(const char *name,const char *action_name,int node_num)
{
	app_analyze *rtp_app_analyze = NULL;
	app_frame_management* rtp_afmemt = NULL;
	rtp_app_analyze = find_app_analyze(name);
	if(rtp_app_analyze)
	{
		rtp_afmemt = register_user_action(rtp_app_analyze,(void*)&rtp_action,(char*)action_name);//g_app_analyze(mjpeg)\uvc_app_analyze(uvc)
		app_node_init(rtp_afmemt,node_num);
	}
	return rtp_afmemt;

}

void *spook_register_audio(const char *name,const char *action_name,int node_num,void *afmemt)
{
	app_analyze *rtp_app_pdm_analyze = NULL;
	app_frame_management* audio_afmemt = NULL;
	app_frame_management* rtp_afmemt = afmemt;
	rtp_app_pdm_analyze = find_app_analyze(name);
	if(rtp_app_pdm_analyze)
	{
		//rtp_afmemt = register_user_action(rtp_app_analyze,(void*)&rtp_audio_action,action_name);//g_app_analyze(mjpeg)\uvc_app_analyze(uvc)

		audio_afmemt = register_user_action_other_sem(rtp_app_pdm_analyze,(void*)&rtp_audio_action,"pdm",rtp_afmemt->sem.hdl);
		app_node_init(audio_afmemt,node_num);
	}
	return audio_afmemt;
}
#endif

 
void spook_send_thread(void *d)
{
	struct rtp_node *node = d;	
	struct frame_exchanger *ex = node->video_ex;
	uint32_t time_for_count;
	uint32_t cnt_num = 0;
	static struct frame *jpeg;
	void * get_f = NULL;
	uint32_t flen;
	int count_times = 0;
	spook_log( SL_VERBOSE, "spook_send_thread");
	unsigned int time = 0;
	int get_times_out = 0;


	time_for_count = os_jiffies();
	while(1) {
		get_f = (void*)GET_JPG_FRAME();
		if(get_f)
		{
			ex->ready = 0;
			count_times++;
			if(count_times>25)
			{
				printf("time:%lld\n",csi_kernel_get_ticks()-time);
				count_times = 0;
				time = csi_kernel_get_ticks();
			}
			
		}
		else
		{
			os_sleep_ms(1);
			get_times_out++;
			if(get_times_out>1000)
			{
				ex->f( NULL, ex->d );
				get_times_out = 0;
			}
			
			continue;
		}

		get_times_out = 0;
		if (get_f)
		{

				flen = GET_JPG_LEN(get_f);
				jpeg = ex->jf;//ex->slave_cur->f;
			    //指针赋值
			    jpeg->get_f = get_f;
				jpeg->node_len = GET_NODE_LEN(get_f);
				jpeg->d = GET_JPG_buf(get_f);

				cnt_num++;
				//printf("flen:%d   buf:%x %x\n",flen,jpeg->d[0],jpeg->d[1]);
				if((os_jiffies() - time_for_count) > 1000){
						time_for_count = os_jiffies();
						printf("cnt_num:%d\r\n",cnt_num);
						cnt_num = 0;
				}
				jpeg->length = flen;
				jpeg->first_length = GET_NODE_LEN(get_f);//JPG_BUF_LEN;
				jpeg->format = FORMAT_JPEG;
				/*
				* callback: get_back_frame( struct frame *f, void *d )
				* d: (jpeg_encoder *)en
				*/
				printf("#");
				ex->f( jpeg, ex->d );
		}
	}
}
 

#if FRAME_MANAGEMENT == 1

void spook_send_thread2(void *d)
{
	void *which = NULL;
	struct rtp_node *node = d;	
	struct frame_exchanger *ex = node->video_ex;
	uint32_t time_for_count;
	struct frame_exchanger *audio_ex = node->audio_ex;
	uint32_t cnt_num = 0;
	static struct frame *jpeg;
	static struct frame *audio;
	void * get_audio_f = NULL;
	void * get_f = NULL;
	uint32_t flen;
	int count_times = 0;
	char *first_addr = NULL;
	//OS_CPU_SR cpu_sr;
	app_frame_management* rtp_app = NULL;
	app_frame_management* rtp_aduio_app = NULL;
	rtp_user_action* rtp_act = NULL;
	rtp_user_action* rtp_aduio_act = NULL;
	spook_log( SL_VERBOSE, "spook_send_thread2");
	unsigned int time = 0;


	app_frame_management* rtp_afmemt = node->rtp_video_app;
	app_frame_management* audio_afmemt = node->rtp_aduio_app;

	printf("rtp_video:%X\taudio:%X\n",(int)rtp_afmemt,(int)audio_afmemt);

	rtp_app = rtp_afmemt;	
	if(rtp_app)
	{
		rtp_act = (rtp_user_action*)rtp_app->app_action;
		app_frame_enable(rtp_app,1);
	}


	rtp_aduio_app = audio_afmemt;	
	if(rtp_aduio_app)
	{
		rtp_aduio_act = (rtp_user_action*)rtp_aduio_app->app_action;
		app_frame_enable(rtp_aduio_app,1);
	}


	//走框架,但是却没有注册对应的video,那么就没有必要继续跑下去,但线程继续执行,需要等待自动释放
	while(!rtp_app)
	{
		printf("%s no found rtp_app ex:%x\taudio_ex:%X\n",__FUNCTION__,(int)ex,(int)audio_ex);
		if(ex)
		{
			printf("%s no found rtp_app ex1:%x\taudio_ex:%X\n",__FUNCTION__,(int)ex,(int)audio_ex);
			ex->f( NULL, ex->d );
		}
		if(audio_ex)
		{
			printf("%s no found rtp_app ex2:%x\taudio_ex:%X\n",__FUNCTION__,(int)ex,(int)audio_ex);
			audio_ex->f(NULL,audio_ex->d);
		}


		os_sleep(1);


	}


	time_for_count = os_jiffies();
	while(1) {
		which = rtp_app->msg_down(rtp_app->signal_msg,-1);
		printf("Q");
		get_f = NULL;
		get_audio_f = NULL;

		//这里不再用宏去隔离,如果没有video或者pdm,这里的值为NULL,不可能进去判断
		if((unsigned int)which == (unsigned int)rtp_app)
		{
			get_f = rtp_act->get_frame(rtp_app);
		}

		if((unsigned int)which == (unsigned int)rtp_aduio_app)
		{
			get_audio_f = rtp_aduio_act->get_frame(rtp_aduio_app);
		}


		if(get_f)
		{
			jpeg = ex->jf;
			ex->ready = 0;
			jpeg->app = rtp_app;
			count_times++;
			if(count_times>25)
			{
				printf("time:%lld\n",csi_kernel_get_ticks()-time);
				count_times = 0;
				time = csi_kernel_get_ticks();
			}
			
		}
		else if(get_audio_f)
		{
		}

		else
		{
			//走框架的时候,会跑到这里,有可能是主动断开连接
			ex->f( NULL, ex->d );
			audio_ex->f(NULL,audio_ex->d);
			os_sleep_ms(1);
			
			continue;
		}


		if (get_f)
		{
			flen = GET_JPG_LEN(get_f);
			jpeg = ex->jf;
		    //指针赋值
		    jpeg->get_f = get_f;
			first_addr = (char*)GET_JPG_buf(get_f);
			if(first_addr)
			{
				hw_memcpy0(scan_buf,first_addr,SCAN_BUF_LEN);
			}
			else
			{
				printf("%s err:%d\t%X\n",__FUNCTION__,__LINE__,(int)first_addr);
				continue;
			}
			jpeg->d = scan_buf;

			cnt_num++;
			//printf("flen:%d   buf:%x %x\taddr:%X\tbuf_addr:%X\n",flen,jpeg->d[0],jpeg->d[1],jpeg->d,first_addr);
			if((os_jiffies() - time_for_count) > 1000){
					time_for_count = os_jiffies();
					printf("cnt_num:%d\r\n",cnt_num);
					cnt_num = 0;
			}
			
			jpeg->node_len = GET_NODE_LEN(get_f);
			jpeg->length = flen;
			jpeg->first_length = SCAN_BUF_LEN;//JPG_BUF_LEN;
			/*
			* callback: get_back_frame( struct frame *f, void *d )
			* d: (jpeg_encoder *)en
			*/
			printf("#");
			ex->f( jpeg, ex->d );
		}


	if(get_audio_f)
	{
		//因为获取了音频,所以要将信号量减1,正常应该是立刻获取到
		if(get_f)
		{
			//rtp_app->down(&rtp_app->sem,0);
		}
		printf("A");
		audio = audio_ex->jf;//ex->slave_cur->f;
		audio->app = rtp_aduio_app;
		 audio->get_f = get_audio_f;
		 audio->length = GET_JPG_LEN(get_audio_f);
		 audio->timestamp = get_real_buf_timestamp(get_audio_f);
		 //printf("t:%d\n",audio->timestamp);
		//printf("length:%d\n",audio->length);
		 //get_app_buf_adr(get_audio_f);
		//rtp_aduio_act->del_frame(get_audio_f,rtp_aduio_app);
		audio_ex->f(audio,audio_ex->d);
	}


	}
}


#endif

struct frame_exchanger *new_exchanger_audio( int slots,
					frame_deliver_func func, void *d )
{
	struct frame_exchanger *ex;
	ex = (struct frame_exchanger *)malloc( sizeof( struct frame_exchanger ) );

	ex->f = func;
	ex->d = d;
	return ex;
}


struct frame_exchanger *new_exchanger( int slots,
					frame_deliver_func func, void *d )
{
	struct frame_exchanger *ex;

	ex = (struct frame_exchanger *)
			malloc( sizeof( struct frame_exchanger ) );
	ex->f = func;
	ex->d = d;



	/* read_mutex is used to avoid  closing the tcp when frame is sending through sendmsg */




	
	return ex;
}

int exchange_frame( struct frame_exchanger *ex, struct frame *frame )
{
	ex->jf= frame;
	return 0;
}

