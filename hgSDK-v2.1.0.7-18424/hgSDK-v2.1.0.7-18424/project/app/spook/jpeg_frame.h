#ifndef __JPEG_FRAME__
#define __JPEG_FRAME__

#include <sys_config.h>

#include "api.h"
#include "linux_types.h
#if (SYS_CHIP_MODULE == APPO_TIGA)
#define JPEG_FIFO_SIZE			1
#elif (SYS_CHIP_MODULE == APPO_VISION)
#define JPEG_FIFO_SIZE			1
#endif

#define JPEG_FREE_MEM_WAKE		400

#define SCAN_DATA_OFFSET		0X265



struct list_slot
{
	struct list_slot *prev;
	struct list_slot *next;
	unsigned char *d;
};

struct jpeg_fifo
{
	unsigned char		*buf;
	int 				len;
	unsigned char 		*scan_data;
	int 				scan_data_len;
	unsigned short int	dri_len[100];

	int 				dri_num;
	unsigned int 		items;
	struct list_head 	list;
	struct list_slot 	*ls;
	boolean             *pIsUsing;
};
#define MARK_UNUSE(j) *(j->pIsUsing) = FALSE

struct jpeg_frame {
	unsigned char *buf;
	unsigned long size;
	unsigned long seq;
};


int prescan_DRI(struct jpeg_fifo *j);
void spook_scan_thread(void *d);

extern struct jpeg_fifo *jpeg_fifo_head;
extern int jpeg_scan_cond;
extern wait_event_t jpeg_scan_event;
extern int jpeg_send_cond;
extern wait_event_t jpeg_send_event;


#endif

