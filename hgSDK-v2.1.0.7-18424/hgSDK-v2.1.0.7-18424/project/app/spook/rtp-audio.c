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
#include <log.h>
#include <frame.h>
#include <stream.h>
#include <rtp.h>
#include <rtp_media.h>
#include <spook_config.h>
#include "list.h"
#include "jpgdef.h"
#include "rtp.h"
#include "session.h"
#include "osal/sleep.h"
 







struct rtp_audio {
	//为了获取到frame,因为使用了链表形式
	struct frame *f;
	unsigned int timestamp;	
};



static int audio_get_sdp( char *dest, int len, int payload, int port, void *d )
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return snprintf( dest, len, "m=audio %d RTP/AVP 97\r\na=rtpmap:97 L16/16000/1\r\n", port );
}

#define AUDIO_SAMPLE	16000

static int audio_process_frame( struct frame *f, void *d )
{
	printf("+");
	struct rtp_audio *out = (struct rtp_audio*)d;
	out->f = f;
	out->timestamp = f->timestamp*AUDIO_SAMPLE/(1000);
	//printf("%s timestamp:%X\n",__FUNCTION__,out->timestamp);
	//out->timestamp += (f->length/2);

	//printf("audio out:%X\n",out);
	//printf("A:%X\n",f);
	return 1;
}

static int audio_get_payload( int payload, void *d )
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return 97;
}

static uint8_t *audio_send_buf = (uint8_t*)live_buf_cache;
static int audio_send( struct rtp_endpoint *ep, void *d )
{
	//printf("%s:%d\n",__FUNCTION__,__LINE__);
	struct rtp_audio *out = (struct rtp_audio*)d;
	uint8_t *inter,*rtphdr,*realdata;
	int size = -1;
	int timeouts = 0;
	int fd = ep->trans.udp.rtp_fd;
	uint32_t i;

	struct sockaddr_in rtpaddr;
	unsigned int namelen = sizeof( rtpaddr );

	if( getsockname( fd, (struct sockaddr *)&rtpaddr, &namelen ) < 0 ) {
		spook_log( SL_ERR, "sendmsg getsockname error");
	}



	inter = audio_send_buf;
	rtphdr = inter;
	realdata = rtphdr + 12;
	struct frame *f = out->f;
	uint8_t *real_buf = get_app_buf_adr(f->get_f);

	//printf("out:%X\n",out);
	//printf("real_buf:%X\t%X\t%X\t%d\n",real_buf,real_buf[0],real_buf[1],f->length);
	//memcpy(realdata,real_buf,f->length);
	for(i=0;i<f->length;i+=2)
	{
		realdata[i] = real_buf[i+1];
		realdata[i+1] = real_buf[i];
	}
	//printf("length111:%d\n",f->length);
	//printf("real:%X\t%X\t%d\n",realdata[0],realdata[1],out->timestamp);
	ep->last_timestamp = ( ep->start_timestamp + out->timestamp )& 0xFFFFFFFF;


	inter[0] = 2 << 6; /* version */
	//帧结束
	inter[1] = ep->payload | 0x80;

	PUT_16(rtphdr + 2, ep->seqnum );
	PUT_32(rtphdr+4, ep->last_timestamp);
	PUT_32( rtphdr + 8, ep->ssrc );
	ep->seqnum = ( ep->seqnum + 1 ) & 0xFFFF;

	while(size < 0 )
	{
		//size = sendto(fd, sendtobuf, total_len, MSG_DONTWAIT, (struct sockaddr *)&rtpaddr, namelen);
		//-8是音频过来前8字节是空的
		size = sendto(fd, audio_send_buf, f->length+12, MSG_DONTWAIT, (struct sockaddr *)&rtpaddr, namelen);
		//printf("seq:%d\t%X\t%X\n",ep->seqnum,audio_send_buf[12],audio_send_buf[13] );
		timeouts++;

		if(timeouts>20)
		{
			break;
		}
		if(timeouts%2==0)
		{
			os_sleep_ms(1);
		}

		
	}

	//累计发送的数据量大小
	ep->octet_count += (f->length+12);

	//累计发送的数据包数量,由于现在音频只有1K左右,所以这里+1,实际要看发送多少包,然后增加多少包
	++ep->packet_count;

	

	
	//printf("%s:%d\ttime:%d\n",__FUNCTION__,__LINE__,krhino_sys_tick_get());
	return 0;
}



struct rtp_media *new_rtp_media_audio_stream( struct stream *stream )
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	struct rtp_audio *out;
	int fincr, fbase;
	struct rtp_media *m;

	stream->get_framerate( stream, &fincr, &fbase );
	out = (struct rtp_audio *)malloc( sizeof( struct rtp_audio ) );
	out->f = NULL;
	out->timestamp = 0;
	//return new_rtp_media( audio_get_sdp, audio_get_payload,audio_process_frame, audio_send, out );
	m = new_rtp_rtcp_media( audio_get_sdp, audio_get_payload,audio_process_frame, audio_send,new_rtcp_send, out );
	if(m)
	{
		m->per_ms_incr = AUDIO_SAMPLE/(1000);
	}
	return m;
}


