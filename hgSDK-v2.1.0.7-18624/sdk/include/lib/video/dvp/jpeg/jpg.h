#ifndef _JPG_MODE_DEFINE_
#define _JPG_MODE_DEFINE_


#define DQT_DEF				1




typedef struct 
{
	struct list_head list;				//jpeg_frame的节点头
	uint32 frame_len;						//帧长度
	uint8 usable;							//判断是否可用       0:空闲
										//			   1:节点补充中或正在使用中
										//			   2:可用
	
}jpeg_frame;


typedef struct
{     
	struct list_head list;
	uint8* buf_addr;
}mjpeg_node;

void jpg_start();
void jpeg_user();
//void jpg_open(struct hgjpg_hw *p_jpg,uint8 mode);
//void hgjpg_attach(uint32 dev_id, struct hgjpg *jpg);

bool put_node(volatile struct list_head *head,volatile struct list_head *del);
void del_frame(volatile struct list_head* frame_list);
struct list_head* get_frame();

void mjpeg_sema_init();
void mjpeg_sema_down(int32 tmo_ms);
void mjpeg_sema_up();
void mjpeg_avi_thread(void *d);


extern const uint16  htable[384];
extern const uint8  dhtable[412];

extern char quality_tab[6][128];

#if SCEN_EN
#define IMAGE_W_J    IMAGE_W/2
#define IMAGE_H_J    IMAGE_H/2
#else
#define IMAGE_W_J    IMAGE_W
#define IMAGE_H_J    IMAGE_H
#endif

#endif

