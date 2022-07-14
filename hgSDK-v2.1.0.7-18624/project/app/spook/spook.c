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
#include "spook_config.h"
#include "spook.h"
#include "sys_config.h"
#include "encoder-audio.h"
#include "encoder-jpeg.h"


unsigned long random_key;
static int init_random(void)
{
	random_key = 0x12345678;
	return 0;
}

uint32_t get_random(void)
{ 
	return random_key;
}

void random_bytes( unsigned char *dest, int len )
{
	int i;

	for( i = 0; i < len; ++i )
		dest[i] = (random_key++) & 0xff;
}

void random_id( unsigned char *dest, int len )
{
	int i;

	for( i = 0; i < len / 2; ++i )
		sprintf( (char *)(dest + i * 2), "%02X",
				(unsigned int)( (random_key++) & 0xff ) );
	dest[len] = 0;
}



void global_init(void)
{
	config_port(SPOOK_PORT);
}
extern void spook_send_thread(void *d);

static struct rtp_node spook;
k_task_handle_t spook_send_handle;
extern int web_init(void);
extern int live_init(const rtp_name *jpg_name);

const rtp_name live_dvp = {
	.sema_name       = "dvp-live-video-audio",
	.rtp_video_name	 = "dvp_video",
	.audio_name      = "pdm",
	.thread_name     = "live_dvp",
	.stream_name     = "jpeg_dvp",
	.path            = "/webcam",
};

const rtp_name live_uvc = {
	.sema_name       = "uvc-live-video-audio",
	.rtp_video_name	 = "uvc_video",
	.audio_name      = "pdm",
	.thread_name     = "live_uvc",
	.stream_name     = "jpeg_uvc",
	.path			 = "/webcam",
	
};


static void spook_thread(void *d)
{
	/* debug  level :
	* SL_DEBUG		0
	* SL_VERBOSE	1
	* SL_INFO		2
	* SL_WARN		3
	* SL_ERR		4
	*/
	void *video_ex = NULL;
	void *audio_ex = NULL;

	spook_log_init(SL_DEBUG);
	
	spook_log( SL_DEBUG, "> spook init");
	
	init_random();
	global_init();
  	//jpeg_init();
#if USB_EN
	video_ex = jpeg_init(&live_uvc);

#endif
#if JPG_EN
	video_ex = jpeg_init(&live_dvp);
#endif

 	//video_ex = jpeg_init_ret();
	//jpeg_init2_ret();


#if FRAME_MANAGEMENT == 1  && RTP_SOUND == 1
	audio_ex = rtp_audio_init_ret();
#endif

	
#if FRAME_MANAGEMENT == 1  && SDH_EN == 1
	//文件的音频流,实际就是换一个名称
	file_audio_init_ret();
	file_jpeg_init_ret();
	//在有框架的情况下,才有可能播放文件
	web_init();
#endif

#if JPG_EN
	live_init(&live_dvp);
#endif

#if USB_EN
	live_init(&live_uvc);
#endif


	//live_usb_init();
	//http_init();
	spook.video_ex = video_ex;
	spook.audio_ex = audio_ex;
#if FRAME_MANAGEMENT == 1
#else
	csi_kernel_task_new((k_task_entry_t)spook_send_thread, "spook_send", &spook, 9, 0, NULL, 1024, &spook_send_handle);
#endif


	//event_loop( 0 );
}

void spook_init(void)
{
	//thread_create(spook_thread, 0, TASK_SPOOK_PRIO, 0, 110*1024, "spook");
	spook_thread(0);
}


