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
#include "avidemux.h"
#include "avi_app.h"


#ifdef send
#undef send
#endif
struct web_source;
struct web_session;


typedef int (*web_creat)(struct web_session *ls,struct web_source *source );
typedef int (*web_release)(struct web_session *ls,struct web_source *source );
typedef int (*web_play)(struct web_session *ls,struct web_source *source );

struct web_session {
	struct web_session *next;
	struct web_session *prev;
	struct session *sess;
	struct web_source *source;
	char path[64];
	int playing;
	int closed;
	void *priv;
};


struct web_track {
	int index;
	struct web_source *source;
	struct stream_destination *stream;
	int ready;
	struct rtp_media *rtp;
};
  
struct web_source {
	struct web_session *sess_list;
	struct web_track track[MAX_TRACKS];
	void *thread_handle;
	void *signal;
	void *priv;
	struct rtp_node web_node;
	//创建线程以及资源的回调函数,参数就是source
	web_creat creat;

	//释放线程以及资源的回调函数,参数就是source
	web_release release;
	web_play play;
	
};

int web_file_ready(struct web_session *ls,struct web_source *source);




static int webfile_get_sdp( struct session *s, char *dest, int *len,
				char *path )
{
	struct web_session *ls = (struct web_session *)s->private;
	int i = 0, t;
	char *addr = "IP4 0.0.0.0";
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	#if 0
	//printf("rtp:%d ready:%d\n",ls->source->track[0].rtp,ls->source->track[0].ready);
	if( ! ls->source->track[0].rtp || ! ls->source->track[0].ready )
	{
		printf("rtp:%X\tready:%X\n",ls->source->track[0].rtp,ls->source->track[0].ready);
		return 0;
	}
	#endif
	#if 0
	ls->path = path;
	res = ls->source->ready(ls,ls->source);

	if(res)
	{
		printf("%s ready err\n",__FUNCTION__);
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

static int webfile_setup( struct session *s, int t )
{
	struct web_session *ls = (struct web_session *)s->private;
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
			ls->source->creat(ls,ls->source);
		}
	}
	else
	{
		printf("web thread already run:%X\t%x\n",(int)ls->source->thread_handle,(int)ls->source);
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

static void webfile_play( struct session *s, double *start )
{
	struct web_session *ls = (struct web_session *)s->private;
	int t;
	
	if( start ) *start = -1;
	ls->playing = 1;
	
	for( t = 0; t < MAX_TRACKS && ls->source->track[t].rtp; ++t )
	{
			//对应位置位,然后就线程会自动发送数据
			struct web_track *track = &ls->source->track[t];
			track->ready = 1;
			clear_init_done(track->rtp->private);
			if( s->ep[t] ) set_waiting( ls->source->track[t].stream, 1 );
	}
	ls->source->play(ls,ls->source);
}

static void track_check_running( struct web_source *source, int t )
{
	struct web_session *ls;
	

	for( ls = source->sess_list; ls; ls = ls->next )
		if( ls->playing && ls->sess->ep[t] ) return;

	set_waiting( source->track[t].stream, 0 );
}

extern void drop_conn( struct conn *c );

void webfile_release_event(void *ei, void *d)
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
  
static void webfile_teardown( struct session *s, struct rtp_endpoint *ep )
{
	printf("%s :%d\n",__FUNCTION__,__LINE__);
	struct web_session *ls = (struct web_session *)s->private;
	int i, remaining = 0;
	struct web_source *source = ls->source;
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
		if( ls->next ) ls->next->prev = ls->prev;
		if( ls->prev ) ls->prev->next = ls->next;
		else ls->source->sess_list = ls->next;
		//断开链表后,开启eloop的alarm事件,清除对应的资源
		eloop_add_alarm(os_jiffies(),EVENT_F_ENABLED,webfile_release_event,(void*)s);
	}

	//如果有teardown后,则需要判断ls是否为空,为空,清除source的一些资源
	ls = source->sess_list;
	if(!ls)
	{
	
		//因为所有的东西被释放,所以就要将线程和资源释放
		if(source->release)
		{
			source->release(NULL,source);
		}
	}

}


//置位,统一在后面去teardown

static void webfile_closed( struct session *s, struct rtp_endpoint *ep )
{
	struct web_session *ls = (struct web_session *)s->private;
	ls->closed = 1;
}





static struct session *webfile_open( char *path, void *d )
{
	struct web_source *source = (struct web_source *)d;
	struct web_session *ls;

	ls = (struct web_session *)malloc( sizeof( struct web_session ) );
	ls->next = source->sess_list;
	if( ls->next ) ls->next->prev = ls;
	source->sess_list = ls;
	ls->prev = NULL;
	ls->sess = new_session();
	ls->source = source;
	ls->playing = 0;
	ls->sess->get_sdp = webfile_get_sdp;
	ls->sess->setup = webfile_setup;
	ls->sess->play = webfile_play;
	ls->sess->teardown = webfile_teardown;
	ls->sess->closed = webfile_closed;
	ls->sess->private = ls;
	ls->closed = 0;
	if(strlen(path)<sizeof(ls->path))
	{
		memcpy(ls->path,path,strlen(path)+1);
	}
	else
	{
	}
	return ls->sess;
}

static void next_web_frame( struct frame *f, void *d )
{
	struct web_track *track = (struct web_track *)d;/*d: source->track[t]*/
	struct web_session *ls;
	if(!f)
	{
		return;
	}

	if(!track->ready)
	{
		return;
	}
	/*struct web_session *next;*/
	//这里添加获取jpeg的节点头，发送完一帧后，再循环到这里，看是否有下一帧数据，如果有，那就继续发送，省去多return一次的时间
	/*callback: jpeg_process_frame, track->rtp->private: rtp_jpeg *out*/



	if( ! track->rtp->frame( f, track->rtp->private ) )						//这个按理说不用一直运行的，只运行一次则可以
	{
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		unref_frame( f );
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
}

static void *loop_search_ep(void *in_head,void *in_track,void **ep)
{
	struct web_session *ls;
	struct web_session *head = (struct web_session *)in_head;
	struct web_track *track = (struct web_track *)in_track;
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

static void next_web_frame_more_connect( struct frame *f, void *d )
{
	struct web_track *track = (struct web_track *)d;/*d: source->track[t]*/
	struct web_session *ls;
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



//如果传入的f是NULL,则代表需要主动断开
static void webfile_frame_end( struct frame *f, void *d ){
	struct web_track *track = (struct web_track *)d;/*d: source->track[t]*/
	struct web_session *ls;
	struct web_source *source;

	for( ls = track->source->sess_list; ls; ls = ls->next )
	{
		//printf("%s:%d\n",__FUNCTION__,__LINE__);
		source = ls->source;
		if( !f || ls->closed)
		{
			/*callback: jpeg_send;ls->sess->ep[track->index], track->rtp->private: out*/
			//track->rtp->send( ls->sess->ep[track->index], track->rtp->private );
			ls->sess->teardown( ls->sess, ls->sess->ep[track->index] );
			//如果有teardown后,则需要重新去轮询,因为之前的ls被释放了,虽然效率低,但考虑到设备数不多,这样应该还好
			ls = track->source->sess_list;
			if(!ls)
			{
				break;
			}
			
		}
	}
}

/************************ CONFIGURATION DIRECTIVES ************************/

static void *start_block(void)
{
	struct web_source *source;
	int i;

   	spook_log (SL_DEBUG, "web start_block");

	source = (struct web_source *)malloc( sizeof( struct web_source ) );
	//初始化source
	memset(source,0,sizeof( struct web_source ));
	source->sess_list = NULL;
	source->thread_handle = NULL;
	source->priv = NULL;
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
	struct web_source *source = (struct web_source *)d;

    spook_log (SL_DEBUG, "web end_block");

	if( ! source->track[0].rtp )
	{
		spook_log( SL_ERR, "web: no media sources specified!" );
		return -1;
	}

	return 0;
}

//视频注册
static int set_track( char *name, void *d )
{
	struct web_source *source = (struct web_source *)d;
	int t;

    spook_log (SL_DEBUG, "web set_track %s", name);

	for( t = 0; t < MAX_TRACKS && source->track[t].rtp; ++t );

	if( t == MAX_TRACKS )
	{
		spook_log( SL_ERR, "web: exceeded maximum number of tracks" );
		return -1;
	}

	if( ! ( source->track[t].stream = connect_to_stream( name,
			next_web_frame_more_connect, &source->track[t] ) ) )
	{
		spook_log( SL_ERR,
				"web: unable to connect to stream \"%s\"", name );
		return -1;
	}

	
	source->web_node.video_ex = get_video_ex(source->track[t].stream->stream->private);
	

	disconnect_stream(source->track[t].stream,webfile_frame_end);
	
	
	source->track[t].rtp = new_rtp_media_jpeg_stream(
					source->track[t].stream->stream );

	if( ! source->track[t].rtp ) return -1;

	set_waiting( source->track[t].stream, 1 );

	return 0;
}


//音频注册
static int set_audio_track( char *name, void *d )
{
	struct web_source *source = (struct web_source *)d;
	int t;

    spook_log (SL_DEBUG, "web set_track %s", name);

	for( t = 0; t < MAX_TRACKS && source->track[t].rtp; ++t );

	if( t == MAX_TRACKS )
	{
		spook_log( SL_ERR, "web: exceeded maximum number of tracks" );
		return -1;
	}

	if( ! ( source->track[t].stream = connect_to_stream( name,
			next_web_frame, &source->track[t] ) ) )
	{
		spook_log( SL_ERR,
				"web: unable to connect to stream \"%s\"", name );
		return -1;
	}
			
	source->web_node.audio_ex = get_audio_ex(source->track[t].stream->stream->private);
	disconnect_stream(source->track[t].stream,webfile_frame_end);
	
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
  	spook_log (SL_DEBUG, "web set_path %s", path);

	/*if( num_tokens == 2 )*/
	{
		new_rtsp_location( path, NULL, NULL, NULL,
				webfile_open, d );
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

void register_web_fn(struct web_source *source,web_creat creat,web_release release,web_play ready)
{
	source->creat = creat;
	source->release = release;
	source->play = ready;
}



//创建实时预览的的线程
int web_creat_thread(struct web_session *ls,struct web_source *source)
{
	//首先生成生产资源的线程
	web_file_ready(ls,source);

	//然后创建对应调用资源的线程
	//先注册app_frame_management,并且绑定
	source->signal = msg_sema_init(NULL,10,sizeof(void*),"webfile-video-audio");
	source->web_node.rtp_video_app = spook_register_video("playback","rtpVideo",2);
	source->web_node.rtp_aduio_app = spook_register_audio("pdm","rtpAudio",8,source->web_node.rtp_video_app);

	user_action_access_msg(source->web_node.rtp_video_app,source->signal);
	user_action_access_msg(source->web_node.rtp_aduio_app,source->signal);

	//source->web_node.rtp_aduio_app = NULL;

	csi_kernel_task_new((k_task_entry_t)spook_send_thread2, "webfile", &source->web_node, 9, 0, NULL, 1024,&source->thread_handle);
	return 0;
}

//线程删除,实际还有内部资源需要释放,要考虑如何去寻找内部需要释放资源,例如:框架流如何清除?
int web_release_thread(struct web_session *ls,struct web_source *source)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	#if 0
	if(source->thread_handle)
	{
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		printf("%s source handle:%X\t%X\n",__FUNCTION__,source->thread_handle,source);
		printf("%s handle:%X\n",source->thread_handle);
		//对应的action也要删除
		unregister_user_action(NULL,source->web_node.rtp_video_app);
		unregister_user_action(NULL,source->web_node.rtp_aduio_app);
		void *tmp_thread = source->thread_handle;
		source->thread_handle = NULL;
		csi_kernel_task_del(tmp_thread);
		printf("%s del end\n",__FUNCTION__);
	}
	#endif
	//首先释放生产资源的线程,但是资源还是存在框架中,在重新启动的时候,会重新初始化对应的空间
	if(source->priv)
	{
		//释放文件解析的线程
		avi_file_free(source->priv);
		source->priv = NULL;
	}

	//然后关闭使用资源的线程
	if(source->thread_handle)
	{
		//对应的action也要删除
		unregister_user_action(NULL,source->web_node.rtp_video_app);
		unregister_user_action(NULL,source->web_node.rtp_aduio_app);
		msg_sema_del(source->signal);
		void *tmp_thread = source->thread_handle;
		source->thread_handle = NULL;
		csi_kernel_task_del(tmp_thread);

	}


	return 0;
}


//线程准备,在这里是打开文件,判断是否为正常的文件,需要分析path获取到对应文件的名字
//如果已经打开过文件,则会播放原来打开的文件,所以暂时不支持多个文件同时播放
int web_file_ready(struct web_session *ls,struct web_source *source)
{
	int res = 1;
	void *ret = NULL;
	printf("%s:%d\n",__FUNCTION__,__LINE__);

	if(source->priv)
	{
		printf("%s already open file\n",__FUNCTION__);
		res = 0;
		return res;
	}
	if(ls->path)
	{
		printf("%s path:%s\n",__FUNCTION__,ls->path);
		char *path = (char*)ls->path;
		char *base = path;
		char *filePath;
		char *c;
		int filePathLen = strlen(base);
		if(filePathLen > 1 && path[filePathLen - 1] == '/')
		{
		  --filePathLen;
		}

		c = strchr (base+1, '/');
		filePathLen -= c - base;

		filePath = (char*)malloc(filePathLen+16);
		
		filePath[0] = '0';
		filePath[1] = ':';
		strncpy (filePath+2, c, filePathLen);
		filePath[2+filePathLen] = '\0';

		//打开文件,判断是否为正常的avi文件
		ret = avi_file_parse(filePath);
		free(filePath);
		if(ret)
		{
			res = 0;
			source->priv = ret;
		}
	}
	if(!res)
	{
		struct avifp *fp = ret;
		//成功的话,创建对应的文件读取线程,teardown的时候,将线程关闭,句柄保存在source->priv的结构体中,web_avi_read_thread
		csi_kernel_task_new((k_task_entry_t)web_avi_read_thread, "web_rad", ret, 10, 0, NULL, 1024,&fp->thread_handle);
	}
	//失败
	return res;
}

int web_file_play(struct web_session *ls,struct web_source *source)
{
	int res = 0;
	set_web_avi_running(source->priv);
	return res;
}






int web_init(void)
{

#if FRAME_MANAGEMENT == 1

	struct web_source *source;

	source = start_block();
	set_path( RTSP_WEB_PATH, source );
	set_track( RTSP_WEBFILE_TRACK, source );
	#if RTP_SOUND
	set_audio_track( RTSP_LIVE_AUDIO_TRACK, source );
	#endif
	register_web_fn(source,web_creat_thread,web_release_thread,web_file_play);
	end_block(source);
#endif
	
	return 0;
}

