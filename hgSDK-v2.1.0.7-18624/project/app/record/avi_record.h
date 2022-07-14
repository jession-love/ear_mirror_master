#ifndef __AVI_RECORD_H
#define __AVI_RECORD_H
#include "typesdef.h"

struct record_video
{
	const char *video_memt_name;
	const char *audio_memt_name;
	int width;
	int height;
	int loop;
	int running;
	
};

void mjpeg_app_avi_control_thread(void *d);

#endif
