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

#include <stdio.h>
#include "lwip\sockets.h"
#include "lwip\netif.h"
#include "lwip\dns.h"
#include "lwip\api.h"
#include "lwip\tcp.h"
#include <event.h>
#include <frame.h>
#include <stream.h>
#include <rtp.h>
#include "log.h"
#include <rtp_media.h>
#include "spook_config.h"
#include "encoder-audio.h"
#include "encoder-jpeg.h"
#include "rtp-audio.h"
#include "sys_config.h"

#ifdef send
#undef send
#endif 
extern void drop_conn( struct conn *c );
struct live_source;

typedef void (*live_creat)(struct live_source *source );
typedef void (*live_release)(struct live_source *source );


struct live_session {
	struct live_session *next;
	struct live_session *prev;
	struct session *sess;
	struct live_source *source;
	char path[64];
	int playing;
	int closed;
};

struct live_track {
	int index;
	struct live_source *source;
	struct stream_destination *stream;
	int ready;
	struct rtp_media *rtp;
};

struct live_source {
	struct live_session *sess_list;
	struct live_track track[MAX_TRACKS];
	void *thread_handle;
	void *signal;
	struct rtp_node live_node;
	//创建线程以及资源的回调函数,参数就是source
	live_creat creat;

	//释放线程以及资源的回调函数,参数就是source
	live_release release;
	const rtp_name *jpg_name;
	void *priv;
};

static int live_get_sdp( struct session *s, char *dest, int *len,
				char *path )
{
	struct live_session *ls = (struct live_session *)s->private;
	int i = 0, t;
	char *addr = "IP4 0.0.0.0";
	printf("%s:%d\tpath:%s\n",__FUNCTION__,__LINE__,path);
	#if 0
	//printf("rtp:%d ready:%d\n",ls->source->track[0].rtp,ls->source->track[0].ready);
	if( ! ls->source->track[0].rtp || ! ls->source->track[0].ready )
	{
		printf("rtp:%X\tready:%X\n",ls->source->track[0].rtp,ls->source->track[0].ready);
		return 0;
	}
	#endif

	if( s->ep[0] && s->ep[0]->trans_type == RTP_TRANS_UDP )
		addr = s->ep[0]->trans.udp.sdp_addr;

	i = snprintf( dest, *len,
		"v=0\r\no=- 1 1 IN IP4 127.0.0.1\r\ns=Test\r\na=type:broadcast\r\nt=0 0\r\nc=IN %s\r\n", addr );

		printf("%s:%d\n",__FUNCTION__,__LINE__);

	for( t = 0; t < MAX_TRACKS && ls->source->track[t].rtp; ++t )
	{
		int port;

		//if( ! ls->source->track[t].ready ) return 0;

		if( s->ep[t] && s->ep[t]->trans_type == RTP_TRANS_UDP )
			port = s->ep[t]->trans.udp.sdp_port;
		else
			port = 0;

		i += ls->source->track[t].rtp->get_sdp( dest + i, *len - i,
				96 + t, port,
				ls->source->track[t].rtp->private );
		if( port == 0 ) // XXX What's a better way to do this?
			i += sprintf( dest + i, "a=control:track%d\r\n", t );
	}
	
	*len = i;
	return t;
}

static int live_setup( struct session *s, int t )
{
	struct live_session *ls = (struct live_session *)s->private;
	int payload = 96 + t;

	//判断source是否已经创建了线程,如果没有创建线程,则在这里创建线程
	//线程创建成功后,实际内部不会进行太多操作,主要是读取视频和音频数据
	//直到play执行后,对应位被置位后,才会进行发送
	//如果teardown后,如果没有ls链表,则将线程删除,释放空间
	//删除线程,就要同时考虑将对应视频和音频接入的框架流disable掉,所以是不是应该有一个释放资源的回调函数呢
	//所以source是不是应该有一个创建线程和释放线程的回调函数,用的是自身的句柄
	//线程传入的参数可以通过source中的track查找,不用管t的值,而是看初始化的时候创建多少个track决定,配合创建线程




	//线程创建
	if(!ls->source->thread_handle)
	{
		if(ls->source->creat)
		{
			ls->source->creat(ls->source);
		}
	}
	else
	{
		printf("live thread already run:%X\t%X\n",(int)ls->source->thread_handle,(int)ls->source);
	}

	if( ! ls->source->track[t].rtp ) return -1;

	if( ls->source->track[t].rtp->get_payload )
		payload = ls->source->track[t].rtp->get_payload( payload,
					ls->source->track[t].rtp->private );
	s->ep[t] = new_rtp_endpoint( payload );
	s->ep[t]->session = s;
	
	return 0;
}
extern void clear_init_done(void *d);

static void live_play( struct session *s, double *start )
{
	struct live_session *ls = (struct live_session *)s->private;
	int t;
	
	if( start ) *start = -1;
	ls->playing = 1;
	for( t = 0; t < MAX_TRACKS && ls->source->track[t].rtp; ++t )
	{
			//对应位置位,然后就线程会自动发送数据
			struct live_track *track = &ls->source->track[t];
			track->ready = 1;
			clear_init_done(track->rtp->private);
			if( s->ep[t] ) set_waiting( ls->source->track[t].stream, 1 );
	}
}

static void track_check_running( struct live_source *source, int t )
{
	struct live_session *ls;

	for( ls = source->sess_list; ls; ls = ls->next )
		if( ls->playing && ls->sess->ep[t] ) return;

	set_waiting( source->track[t].stream, 0 );
}

void live_release_event(void *ei, void *d)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	struct session *s = (struct session*)d;
	struct web_session *ls = (struct web_session *)s->private;
	struct conn *c  = s->conn;
	if(c)
	{
		drop_conn( c );
	}
	free( ls );
	del_session( s );
	
} 


static void live_teardown( struct session *s, struct rtp_endpoint *ep )
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	struct live_session *ls = (struct live_session *)s->private;
	struct live_source *source = ls->source;
	int i, remaining = 0;

	for( i = 0; i < MAX_TRACKS && ls->source->track[i].rtp; ++i )
	{
		if( ! s->ep[i] ) continue;
		if( ! ep || s->ep[i] == ep )
		{
			del_rtp_endpoint( s->ep[i] );
			s->ep[i] = NULL;
			track_check_running( ls->source, i );
		} else ++remaining;
	}

	if( remaining == 0 )
	{
		//解锁ls链表
		if( ls->next ) ls->next->prev = ls->prev;
		if( ls->prev ) ls->prev->next = ls->next;
		else ls->source->sess_list = ls->next;
		//启动event将事件移除
		//断开链表后,开启eloop的alarm事件,清除对应的资源
		eloop_add_alarm(os_jiffies(),EVENT_F_ENABLED,live_release_event,(void*)s);
	}

	//如果有teardown后,则需要查看source是否还有ls,没有,则删除source的资源
	ls = source->sess_list;
	if(!ls)
	{
	
		//因为所有的东西被释放,所以就要将线程和资源释放
		if(source->release)
		{
			source->release(source);
		}
	}

}


//置位,统一在后面去teardown

static void live_closed( struct session *s, struct rtp_endpoint *ep )
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	struct live_session *ls = (struct live_session *)s->private;
	ls->closed = 1;
}





static struct session *live_open( char *path, void *d )
{
	struct live_source *source = (struct live_source *)d;
	struct live_session *ls;
	printf("!!!!!!!!%s path:%s\n",__FUNCTION__,path);
	ls = (struct live_session *)malloc( sizeof( struct live_session ) );
	ls->next = source->sess_list;
	if( ls->next ) ls->next->prev = ls;
	source->sess_list = ls;
	ls->prev = NULL;
	ls->sess = new_session();
	ls->source = source;
	ls->playing = 0;
	ls->sess->get_sdp = live_get_sdp;
	ls->sess->setup = live_setup;
	ls->sess->play = live_play;
	ls->sess->teardown = live_teardown;
	ls->sess->closed = live_closed;
	ls->sess->private = ls;
	ls->closed = 0;
	if(strlen(path)<sizeof(ls->path))
	{
		memcpy(ls->path,path,strlen(path)+1);
	}
	else
	{
		printf("%s path too long:%d\n",__FUNCTION__,strlen(path));
	}


	return ls->sess;
}

static void next_live_frame( struct frame *f, void *d )
{
	struct live_track *track = (struct live_track *)d;/*d: source->track[t]*/
	struct live_session *ls;
	if(!f)
	{
		return;
	}



	if(!track->ready)
	{
		return;
	}



	/*struct live_session *next;*/
	//这里添加获取jpeg的节点头，发送完一帧后，再循环到这里，看是否有下一帧数据，如果有，那就继续发送，省去多return一次的时间
	/*callback: jpeg_process_frame, track->rtp->private: rtp_jpeg *out*/

	if( ! track->rtp->frame( f, track->rtp->private ) )						//这个按理说不用一直运行的，只运行一次则可以
	{
		//unref_frame( f );
		return;
	}

#if 0
	if( ! track->ready )
	{
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		set_waiting( track->stream, 0 );
		track->ready = 1;
	}
#endif
	for( ls = track->source->sess_list; ls; ls = ls->next )
	{
		if( ls->playing && track->ready && ls->sess->ep[track->index] )
		{
			/*callback: jpeg_send;ls->sess->ep[track->index], track->rtp->private: out*/
			track->rtp->send( ls->sess->ep[track->index], track->rtp->private );
			if(track->rtp && track->rtp->rtcp_send)
			{
				track->rtp->rtcp_send( ls->sess->ep[track->index], track->rtp );
			}
		}
	}



	

	//unref_frame( f );
}

static void *loop_search_ep(void *in_head,void *in_track,void **ep)
{
	struct live_session *ls;
	struct live_session *head = (struct live_session *)in_head;
	struct live_track *track = (struct live_track *)in_track;
	for( ls = head; ls; ls = ls->next )
	{
		
		if( ls->playing && track->ready && ls->sess->ep[track->index] )
		{
			*ep = ls->sess->ep[track->index];
			return ls->next;
		}
	}
	*ep = NULL;
	return NULL;
}


static void next_live_frame_more_connect( struct frame *f, void *d )
{
	struct live_track *track = (struct live_track *)d;/*d: source->track[t]*/
	struct live_session *ls;
	if(!f)
	{
		return;
	}



	if(!track->ready)
	{
		return;
	}

	/*struct live_session *next;*/
	//这里添加获取jpeg的节点头，发送完一帧后，再循环到这里，看是否有下一帧数据，如果有，那就继续发送，省去多return一次的时间
	/*callback: jpeg_process_frame, track->rtp->private: rtp_jpeg *out*/

	if( ! track->rtp->frame( f, track->rtp->private ) )						//这个按理说不用一直运行的，只运行一次则可以
	{
		return;
	}

	if(track->rtp->send_more)
	{
		ls = track->source->sess_list;
		track->rtp->send_more( loop_search_ep,ls, track,track->rtp->private);
	}

	for( ls = track->source->sess_list; ls; ls = ls->next )
	{
		if( ls->playing && track->ready && ls->sess->ep[track->index] )
		{
			/*callback: jpeg_send;ls->sess->ep[track->index], track->rtp->private: out*/
			//track->rtp->send( ls->sess->ep[track->index], track->rtp->private );
			if(track->rtp && track->rtp->rtcp_send)
			{
				track->rtp->rtcp_send( ls->sess->ep[track->index], track->rtp );
			}
		}
	}
}



static void next_live_audio_frame( struct frame *f, void *d )
{
	struct live_track *track = (struct live_track *)d;/*d: source->track[t]*/
	struct live_session *ls;
	if(!f)
	{
		return;
	}

	if(!track->ready)
	{
		return;
	}
	/*struct live_session *next;*/
	//这里添加获取jpeg的节点头，发送完一帧后，再循环到这里，看是否有下一帧数据，如果有，那就继续发送，省去多return一次的时间
	/*callback: jpeg_process_frame, track->rtp->private: rtp_jpeg *out*/

	if( ! track->rtp->frame( f, track->rtp->private ) )						//这个按理说不用一直运行的，只运行一次则可以
	{
		//unref_frame( f );
		return;
	}
	for( ls = track->source->sess_list; ls; ls = ls->next )
	{
		//printf("%s:%d\n",__FUNCTION__,__LINE__);
		if( ls->playing && track->ready && ls->sess->ep[track->index] )
		{
			/*callback: jpeg_send;ls->sess->ep[track->index], track->rtp->private: out*/
			track->rtp->send( ls->sess->ep[track->index], track->rtp->private );
		
			if(track->rtp && track->rtp->rtcp_send)
			{
				track->rtp->rtcp_send( ls->sess->ep[track->index], track->rtp );
			}
		}
	}
	//unref_frame( f );
}




static void live_frame_end( struct frame *f, void *d ){
	struct live_track *track = (struct live_track *)d;/*d: source->track[t]*/
	struct live_session *ls;
	struct live_source *source;
	//printf("%s:%d\n",__FUNCTION__,__LINE__);

	for( ls = track->source->sess_list; ls; ls = ls->next )
	{
		source = ls->source;
		if( !f || ls->closed)
		{
			/*callback: jpeg_send;ls->sess->ep[track->index], track->rtp->private: out*/
			//track->rtp->send( ls->sess->ep[track->index], track->rtp->private );
			ls->sess->teardown( ls->sess, ls->sess->ep[track->index] );

			#if 1
			//如果有teardown后,则需要重新去轮询,因为之前的ls被释放了,虽然效率低,但考虑到设备数不多,这样应该还好
			ls = track->source->sess_list;
			if(!ls)
			{
				break;
			}
			#endif
			
		}
	}
}

/************************ CONFIGURATION DIRECTIVES ************************/

static void *start_block(void)
{
	struct live_source *source;
	int i;

   	spook_log (SL_DEBUG, "live start_block");

	source = (struct live_source *)malloc( sizeof( struct live_source ) );
	//初始化source
	memset(source,0,sizeof( struct live_source ));
	source->sess_list = NULL;
	source->thread_handle = NULL;
	//配置默认值
	for( i = 0; i < MAX_TRACKS; ++i )
	{
		source->track[i].index = i;
		source->track[i].source = source;
		source->track[i].stream = NULL;
		source->track[i].ready = 0;
		source->track[i].rtp = NULL;
		
		
	}

	return source;
}

static int end_block( void *d )
{
	struct live_source *source = (struct live_source *)d;

    spook_log (SL_DEBUG, "live end_block");

	if( ! source->track[0].rtp )
	{
		spook_log( SL_ERR, "live: no media sources specified!" );
		return -1;
	}

	return 0;
}

//视频注册
static int set_track( char *name, void *d )
{
	struct live_source *source = (struct live_source *)d;
	int t;

    spook_log (SL_DEBUG, "live set_track %s", name);

	for( t = 0; t < MAX_TRACKS && source->track[t].rtp; ++t );

	if( t == MAX_TRACKS )
	{
		spook_log( SL_ERR, "live: exceeded maximum number of tracks" );
		return -1;
	}

	if( ! ( source->track[t].stream = connect_to_stream( name,
			next_live_frame_more_connect, &source->track[t] ) ) )
	{
		spook_log( SL_ERR,
				"live: unable to connect to stream \"%s\"", name );
		return -1;
	}

	
	source->live_node.video_ex = get_video_ex(source->track[t].stream->stream->private);
	

	disconnect_stream(source->track[t].stream,live_frame_end);
	
	
	source->track[t].rtp = new_rtp_media_jpeg_stream(
					source->track[t].stream->stream );

	if( ! source->track[t].rtp ) return -1;

	set_waiting( source->track[t].stream, 1 );

	return 0;
}


//音频注册
static int set_audio_track( char *name, void *d )
{
	struct live_source *source = (struct live_source *)d;
	int t;

    spook_log (SL_DEBUG, "live set_track %s", name);

	for( t = 0; t < MAX_TRACKS && source->track[t].rtp; ++t );

	if( t == MAX_TRACKS )
	{
		spook_log( SL_ERR, "live: exceeded maximum number of tracks" );
		return -1;
	}

	if( ! ( source->track[t].stream = connect_to_stream( name,
			 next_live_audio_frame, &source->track[t] ) ) )
	{
		spook_log( SL_ERR,
				"live: unable to connect to stream \"%s\"", name );
		return -1;
	}
			
	source->live_node.audio_ex = get_audio_ex(source->track[t].stream->stream->private);
	disconnect_stream(source->track[t].stream,live_frame_end);
	
	//要重新新建
	source->track[t].rtp = new_rtp_media_audio_stream(
					source->track[t].stream->stream );


	//默认打开
	source->track[t].ready = 0;

	if( ! source->track[t].rtp ) return -1;

	set_waiting( source->track[t].stream, 1 );

	return 0;
}


static int set_path( char *path, void *d )
{
  	spook_log (SL_DEBUG, "live set_path %s", path);

	/*if( num_tokens == 2 )*/
	{
		new_rtsp_location( path, NULL, NULL, NULL,
				live_open, d );
		return 0;
	}
	/*
	if( num_tokens == 5 )
	{
		new_rtsp_location( tokens[1].v.str, tokens[2].v.str,
				tokens[3].v.str, tokens[4].v.str,
				live_open, d );
		return 0;
	}
	*/
	spook_log( SL_ERR, "rtsp-handler: syntax: Path <path> [<realm> <username> <password>]" );
	return -1;
}

void register_live_fn(struct live_source *source,live_creat creat,live_release release)
{
	source->creat = creat;
	source->release = release;
}




//创建实时预览的的线程
void live_create_thread(struct live_source *source)
{
	printf("%s:%d  %s\n",__FUNCTION__,__LINE__,source->jpg_name->sema_name);	
	//source->signal = msg_sema_init(NULL,10,sizeof(void*),"dvp-live-video-audio");
	source->signal = msg_sema_init(NULL,10,sizeof(void*),source->jpg_name->sema_name);
	//先注册app_frame_management,并且绑定
	//source->live_node.rtp_video_app = spook_register_video("dvp_video","rtpVideo",2);
	source->live_node.rtp_video_app = spook_register_video(source->jpg_name->rtp_video_name,"rtpVideo",2);
	
	//source->live_node.rtp_aduio_app = spook_register_audio("pdm","rtpAudio",8,source->live_node.rtp_video_app);
	source->live_node.rtp_aduio_app = spook_register_audio(source->jpg_name->audio_name,"rtpAudio",8,source->live_node.rtp_video_app);
	if(source->live_node.rtp_video_app)
	{
		user_action_access_msg(source->live_node.rtp_video_app,source->signal);
	}
	if(source->live_node.rtp_aduio_app)
	{
		user_action_access_msg(source->live_node.rtp_aduio_app,source->signal);
	}

	
	//csi_kernel_task_new((k_task_entry_t)spook_send_thread2, "live_dvp", &source->live_node, 9, 0, NULL, 1024,&source->thread_handle);
	csi_kernel_task_new((k_task_entry_t)spook_send_thread2, source->jpg_name->thread_name, &source->live_node, 9, 0, NULL, 1024,&source->thread_handle);
}

//创建实时预览的的线程
void live2_create_thread(struct live_source *source)
{
	printf("%s:%d  %s\n",__FUNCTION__,__LINE__,source->jpg_name->sema_name);	
	source->signal = msg_sema_init(NULL,10,sizeof(void*),"uvc-live-video-audio");
	//先注册app_frame_management,并且绑定
	source->live_node.rtp_video_app = spook_register_video("uvc_video","rtpVideo",2);
	
	source->live_node.rtp_aduio_app = spook_register_audio("pdm","rtpAudio",8,source->live_node.rtp_video_app);
	if(source->live_node.rtp_video_app)
	{
		user_action_access_msg(source->live_node.rtp_video_app,source->signal);
	}
	if(source->live_node.rtp_aduio_app)
	{
		user_action_access_msg(source->live_node.rtp_aduio_app,source->signal);
	}

	
	csi_kernel_task_new((k_task_entry_t)spook_send_thread2, "live_uvc", &source->live_node, 9, 0, NULL, 1024,&source->thread_handle);
}



 
//线程删除,实际还有内部资源需要释放,要考虑如何去寻找内部需要释放资源,例如:框架流如何清除?
void live_release_thread(struct live_source *source)
{
	if(source->thread_handle)
	{
		//对应的action也要删除
		unregister_user_action(NULL,source->live_node.rtp_video_app);
		unregister_user_action(NULL,source->live_node.rtp_aduio_app);
		msg_sema_del(source->signal);
		void *tmp_thread = source->thread_handle;
		source->thread_handle = NULL;
		csi_kernel_task_del(tmp_thread);
		printf("%s del end\n",__FUNCTION__);
	}
}




unsigned int live_buf_cache[SPOOK_CACHE_BUF_LEN/4];

int live_init(const rtp_name *jpg_name)
{
	struct live_source *source;

	source = start_block();
	set_path( jpg_name->path, source );
	set_track( jpg_name->stream_name, source );
	source->jpg_name = jpg_name;


	
	#if RTP_SOUND
	set_audio_track( RTSP_LIVE_AUDIO_TRACK, source );
	#endif
#if FRAME_MANAGEMENT == 1
	register_live_fn(source,live_create_thread,live_release_thread);
#endif
	end_block(source);
	return 0;
}



