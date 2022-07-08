#include "sys_config.h"
#include "tx_platform.h"
#include "list.h"
#include "dev.h"
#include "typesdef.h"
#include "lib/video/dvp/cmos_sensor/csi.h"
#include "devid.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "lib/video/dvp/jpeg/jpg.h"
#include "jpgdef.h"
#include "hal/jpeg.h"


#include "osal/semaphore.h"
#include "openDML.h"
//#include "media.h"
#include "lib/common/common.h"



#if JPG_EN

#if FRAME_MANAGEMENT
#include "frame_memt.h"
#endif

extern struct dvp_device *dvp_test;
extern uint8_t qc_mode;


#if FRAME_MANAGEMENT
#define JPG_FRAME_NUM 5
#define JPG_FRAME_LEN 150*1024
app_analyze g_app_analyze;
app_frame_management* g_afmemt;
app_frame_management* g_afmemt_test;
uint8 psram_frame_dvp_buf[JPG_FRAME_NUM * JPG_FRAME_LEN]__attribute__ ((section(".psram.src")));
void jpeg_app_analyze();
#endif

k_task_handle_t manalyze_handle;
struct list_head* get_frame();

#if FRAME_MANAGEMENT
uint8 psram_jpg_buf[JPG_NODE*(JPG_BUF_LEN+JPG_TAIL_RESERVER)]__attribute__ ((section(".psram.src")));
#else
uint8 psram_jpg_buf[JPG_NODE*(JPG_BUF_LEN+JPG_TAIL_RESERVER)];//__attribute__ ((section(".psram.src")));

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define JPEG_FRAME_NUM     2


volatile struct list_head free_tab;			//空闲列表，存放空间节点

volatile struct list_head *jpg_p;			//jpg模块的节点指针
volatile struct list_head *usr_p;			//应用的节点指针

volatile jpeg_frame jpg_frame[JPEG_FRAME_NUM];			//最大frame数量
volatile struct list_head* jpg_f_p;			//当前jpg所使用的frame

volatile mjpeg_node jpg_node_src[JPG_NODE]; 

volatile uint32 outbuff_isr = 0;
volatile uint8 default_qt = 0xf;

/**@brief 
 * 将传入的frame根节点下的buf池都送回空间池（free_tab）中
 */
bool free_get_node_list(volatile struct list_head *head,volatile struct list_head *free){
	if(list_empty((struct list_head *)head)){
		return 0;
	}	

	list_splice_init((struct list_head *)head,(struct list_head *)free);
	return 1;
}


/**@brief 
 * 从初始化的frame根节点中抽取其中一个空闲的根节点进行使用，并标记frame的使用状态为正在使用
 * @param 是否开启抢占模式，如果开启，则将上一帧已完成的帧节点删掉，并返回
 */


volatile struct list_head* get_new_frame_head(int grab){
	uint8 frame_num = 0;
	for(frame_num = 0;frame_num < JPEG_FRAME_NUM;frame_num++){
		if(jpg_frame[frame_num].usable == 0){
			jpg_frame[frame_num].usable = 1;
			return &jpg_frame[frame_num].list;
		}

	}

	if(grab)								//是否开启抢占模式,开启抢占后，肯定有frame返回，上一帧没使用的frame肯定usable为2
	{
		
		for(frame_num = 0;frame_num < JPEG_FRAME_NUM;frame_num++){
			if(jpg_frame[frame_num].usable == 2){
				jpg_frame[frame_num].usable = 1;
				free_get_node_list(&jpg_frame[frame_num].list,&free_tab);
				return &jpg_frame[frame_num].list;
			}
		}
	}
	
	return 0;
}



/**@brief 
 * 获取当前frame根节点中保存成功的frame，返回根节点以供链表查询
 */

extern struct list_head *get_node(volatile struct list_head *head,volatile struct list_head *del);
extern uint32 get_addr(volatile struct list_head *list);


void set_frame_ready(jpeg_frame *jf){
	jf->usable = 2;
}


/**@brief 
 * 将传入的frame根节点状态调整为空闲，以供下次重新获取
 */
void del_frame(volatile struct list_head* frame_list)
{


	jpeg_frame* fl;
	if(list_empty((struct list_head *)frame_list) != TRUE){
		NVIC_DisableIRQ(JPG_IRQn);
		free_get_node_list(frame_list,&free_tab);
		fl = list_entry((struct list_head *)frame_list,jpeg_frame,list);
		fl->usable = 0;
		NVIC_EnableIRQ(JPG_IRQn);
		return;		

	}else{
		NVIC_DisableIRQ(JPG_IRQn);
		fl = list_entry((struct list_head *)frame_list,jpeg_frame,list);
		fl->usable = 0;
		NVIC_EnableIRQ(JPG_IRQn);
		return;
	}


}


/**@brief 
 * 初始化free_tab池，将mjpeg_node的节点都放到空闲池中，供frame后面提取使用
 * @param ftb 空闲池头指针
 * @param jpn mjpeg_node节点源
 * @param use_num  mjpeg_node的数量，即多少个mjpeg_node放到空闲池
 * @param addr   mjpeg_node的总buf起始地址
 * @param buf_len  每个mjpeg_node所关联到的数据量
 */
void free_table_init(volatile struct list_head *ftb, volatile mjpeg_node* jpn,int use_num,uint32 addr,uint32 buf_len){
	int itk;
	for(itk = 0;itk < use_num;itk++){
		jpn->buf_addr = (uint8*)(addr + itk*(buf_len+JPG_TAIL_RESERVER)+JPG_HEAD_RESERVER);
		list_add_tail((struct list_head *)&jpn->list,(struct list_head *)ftb); 
		jpn++;
	}
}


/**@brief 
 * 从空间池中提取一个节点，放到队列中，并将此节点作为返回值返回
 */
struct list_head *get_node(volatile struct list_head *head,volatile struct list_head *del){
	if(list_empty((struct list_head *)del)){
		return 0;
	}

	list_move((struct list_head *)del->next,(struct list_head *)head);

	return head->next;				//返回最新的位置
}


int get_node_count(volatile struct list_head *head)
{
	int count = 0;
	struct list_head *first = (struct list_head *)head;
	while(head->next != first)
	{
		head = head->next;
		count++;
	}

	return count;				//返回最新的位置
}



/**@brief 
 * 从当前使用的节点回放到消息池中
 */
bool put_node(volatile struct list_head *head,volatile struct list_head *del){
	//portINIT_CRITICAL();
	if(list_empty((struct list_head *)del)){
		return 0;
	}

	NVIC_DisableIRQ(JPG_IRQn);
	list_move(del->next,(struct list_head *)head);
	NVIC_EnableIRQ(JPG_IRQn);

	return 1;
}

/**@brief 
 * 获取当前jpeg节点buf地址进行返回
 */
uint32 get_addr(volatile struct list_head *list){
	mjpeg_node* mjn;
	mjn = list_entry((struct list_head *)list,mjpeg_node,list);
	return (uint32)mjn->buf_addr;
}


uint8 printf_free_node(){
	uint8 nnum = 0;
	struct list_head *list_p;

	list_p = (struct list_head *)&free_tab;
	list_p = list_p->next;
	while(list_p != &free_tab){
//		printf("ftb:%x \r\n",list_p);
		list_p = list_p->next;
		nnum++;
	}
	return nnum;
}

void printf_frame_node(struct list_head *list){
	struct list_head *list_p;

	list_p = list;
	list_p = list_p->next;
	while(list_p != list){
//		printf("fam:%x \r\n",list_p);
		list_p = list_p->next;
	}	

}

void jpg_DQT_updata(struct jpg_device *p_jpg,uint8 upOdown){
	uint32 *ptable;
	static uint8 pdqt_tab = DQT_DEF;
	if(upOdown == 1){
		pdqt_tab--;
		if(pdqt_tab == 0xff)
			pdqt_tab = 0;
	}else{
		pdqt_tab++;
		if(pdqt_tab == 6)
			pdqt_tab = 5;
	}
	
	ptable = (uint32*)quality_tab[pdqt_tab];
	jpg_updata_dqt(p_jpg,ptable);
}


extern volatile  uint32 hs_isr;
volatile uint8 rtp_speed = 0;     //1:降低    2:提升

uint8 jpg_quality_tidy(uint32 len){
	uint8 updata_dqt = 0;
	if(qc_mode){
		if(len >= 15*1024){
			default_qt++;
			if(default_qt == 0x10){
				default_qt = 0x8;
				updata_dqt = 2;
			}
		}else if(len <= 5*1024){
			default_qt--;	
			if(default_qt == 0){
				default_qt = 8;	
				updata_dqt = 1;
			}	
		}		
		return updata_dqt;
	}	
#if 1
	if(len >= 35*1024){
		default_qt++;
		if(default_qt == 0x10){
			default_qt = 0x8;
			updata_dqt = 2;
		}
	}else if(len <= 30*1024){
		default_qt--;	
		if(default_qt == 0){
			default_qt = 8;	
			updata_dqt = 1;
		}	
	}
#else

	if(len >= 35*1024){
		default_qt++;
		if(default_qt == 0x10){
			default_qt = 0x8;
			updata_dqt = 2;
		}		
	}else{
		if(rtp_speed == 1){
			default_qt++;
			if(default_qt == 0x10){
				default_qt = 0x8;
				updata_dqt = 2;
			}
		}else if(rtp_speed == 2){
			default_qt--;	
			if(default_qt == 0){
				default_qt = 8; 
				updata_dqt = 1;
			}
		}
		rtp_speed = 0;
	}
#endif
	return updata_dqt;
}


//认证或者用户学习使用
void jpeg_user()				//应用层
{
	uint32 count = 0;
	uint32 flen;
	uint32 itk;
	struct list_head* get_f;
	mjpeg_node* mjn;
	jpeg_frame* jpf;
	uint8 printf_dat = 0;
	while(1){
		get_f = get_frame();
		if(get_f){
			printf("usr====>get_f:%x\r\n",(uint32)get_f);
			count++;
			usr_p = get_f;
#if 1			
			
			jpf = list_entry(get_f,jpeg_frame,list);
			flen = jpf->frame_len;

			
				
			if((flen >= 150*1024) || (count == 51))
			{	
				printf("flen:%d\r\n",flen);
				dvp_close(dvp_test);
				//printf("flen:%d\r\n",flen);
				printf_frame_node(get_f);
				printf_dat = 1;
			}	
#endif			
			while(list_empty((struct list_head *)usr_p) != TRUE)
			{
				mjn = list_entry((struct list_head *)usr_p->next,mjpeg_node,list);
				
				//printf("mjn:%x\r\n",mjn);
				/*
					此处使用当前mjn，获取节点对应的buf数据位置，对数据进行处理
				*/
#if 1				
//				if(count == 25)
				if(printf_dat)
				{
					if(flen > JPG_BUF_LEN){
						for(itk = 0;itk < JPG_BUF_LEN;itk++){
							printf("%02x ",mjn->buf_addr[itk-JPG_HEAD_RESERVER]);
						}
						flen -= (JPG_BUF_LEN - JPG_HEAD_RESERVER);
					}
					else{
						for(itk = 0;itk < flen+JPG_HEAD_RESERVER;itk++){
							printf("%02x ",mjn->buf_addr[itk-JPG_HEAD_RESERVER]);
						}
						flen = 0;
						printf_dat = 0;
					}
				
				}
#endif				
				put_node(&free_tab,usr_p);				//节点归还到free_tab,边处理边给下一帧释放空间
			}
			del_frame(usr_p);							//删除frame
		
		}
		
	}
}




void jpg_room_init(){
	//INIT_LIST_HEAD(&jpg_node);
	INIT_LIST_HEAD((struct list_head *)&jpg_frame[0].list);
	INIT_LIST_HEAD((struct list_head *)&jpg_frame[1].list);
	INIT_LIST_HEAD((struct list_head *)&free_tab);
	printf("psram_jpg_buf size:%d\n",sizeof(psram_jpg_buf));
	free_table_init(&free_tab,(mjpeg_node*)&jpg_node_src,JPG_NODE,(uint32)&psram_jpg_buf,JPG_BUF_LEN);
	jpg_frame[0].usable = 0;
	jpg_frame[1].usable = 0;
	jpg_f_p = get_new_frame_head(1);								//预先分配第一frame，待帧结束的时候再分配下一frame			
	jpg_p = jpg_f_p;
}


/*********************************
			获取分辨率
0:无效
1:vga(640x480)
2:720p(1280x720)
*********************************/
uint8 get_camera_dpi()
{
	return 1;
}


void jpg_outbuff_full_isr(uint32 irq_flag,uint32 irq_data,uint32 param1,uint32 param2){
	uint32 addr;
	struct jpg_device *p_jpg = (struct jpg_device *)irq_data;
	if(outbuff_isr != 0xff)
		outbuff_isr++;
	else
		return;

	//printf("obl:%d\r\n",p_jpg->DMA_DLEN);
	jpg_p = get_node(jpg_p,&free_tab);
	if(jpg_p == NULL){
		outbuff_isr = 0xff;
		return;
	}

	addr = get_addr(jpg_p);
	jpg_set_addr(p_jpg,addr,JPG_BUF_LEN-JPG_HEAD_RESERVER);

}

void jpg_buf_err(uint32 irq_flag,uint32 irq_data,uint32 param1,uint32 param2){
	uint32 addr;
	struct jpg_device *p_jpg = (struct jpg_device *)irq_data;
	jpg_close(p_jpg);
	dvp_close(dvp_test);	
	printf("?\r\n");
	del_frame(jpg_f_p);
	
	jpg_f_p = get_new_frame_head(1);
	jpg_p = jpg_f_p;
	outbuff_isr = 0;

	jpg_p = get_node(jpg_p,&free_tab);							//预分配的两个buf地址，帧结束的时候要去掉一个预分配节点
	if(jpg_p == NULL){
		printf("need more node for new frame start1\r\n");
		while(1);
	}
	//p_jpg->DMA_TADR0 = get_addr(jpg_p);
	addr = get_addr(jpg_p);
	//jpg_set_addr0(p_jpg,addr);
	jpg_set_addr(p_jpg,addr,JPG_BUF_LEN-JPG_HEAD_RESERVER);
	jpg_p = get_node(jpg_p,&free_tab);
	if(jpg_p == NULL){
		printf("need more node for new frame start2\r\n");
		while(1);		
	}	
	//p_jpg->DMA_TADR1 = get_addr(jpg_p);	
	addr = get_addr(jpg_p);
	//jpg_set_addr1(p_jpg,addr);
	jpg_set_addr(p_jpg,addr,JPG_BUF_LEN-JPG_HEAD_RESERVER);
	jpg_open(p_jpg);
	dvp_open(dvp_test);

	
}

void jpg_done_isr(uint32 irq_flag,uint32 irq_data,uint32 param1,uint32 param2){
	struct jpg_device *p_jpg = (struct jpg_device *)irq_data;

	uint32 addr;
	jpeg_frame* jf;
	uint8 out_buf_num_err = 0;
	uint8 updata_dqt = 0;
	uint8 default_qt_last;
	uint32 jpg_len; 
	jpg_len = param1;//jpg_get_len(p_jpg);

	//if(outbuff_isr !=  jpg_get_outbuf_num(p_jpg))
	if(param2||(outbuff_isr == 255)){
		printf("jpg done len err:%d\r\n",outbuff_isr);
		out_buf_num_err = 1;
	}
	default_qt_last = default_qt;
	updata_dqt = jpg_quality_tidy(jpg_len);
	if(default_qt_last != default_qt){
		jpg_set_qt(p_jpg,default_qt);
	}

	if(get_node_count(&free_tab)<2)
	{
		outbuff_isr = 0xff;
	}
/**/
	if(jpg_len < outbuff_isr * (JPG_BUF_LEN-JPG_HEAD_RESERVER)){
		//printf("BUF len err:%d\r\n",jpg_len);
		del_frame(jpg_f_p);
	}else if(outbuff_isr == 0xff){
		//printf("outbuff_isr err,no room\r\n");
		del_frame(jpg_f_p);
	}else if(out_buf_num_err){
		//printf("out_buf_num_err happen\r\n");
		del_frame(jpg_f_p);
	}
	else{
		//帧OK
			
		jf = list_entry((struct list_head *)jpg_f_p,jpeg_frame,list);
		jf->frame_len = jpg_len;
		
		//因为有预分配机制，所以要先去掉最后一个节点
		put_node(&free_tab,jpg_p->prev);
		set_frame_ready(jf);

#if FRAME_MANAGEMENT
		//信号量通知
		mjpeg_sema_up();
#endif		
	}

	jpg_f_p = get_new_frame_head(1);
	jpg_p = jpg_f_p;
	outbuff_isr = 0;
	//再设置DMA地址
	jpg_p = get_node(jpg_p,&free_tab);							//预分配的两个buf地址，帧结束的时候要去掉一个预分配节点
	if(jpg_p == NULL){
		printf("need more node for new frame start1\r\n");
		while(1);
	}
	//p_jpg->DMA_TADR0 = get_addr(jpg_p);
	addr = get_addr(jpg_p);
	jpg_set_addr(p_jpg,addr,JPG_BUF_LEN-JPG_HEAD_RESERVER);
	jpg_p = get_node(jpg_p,&free_tab);
	if(jpg_p == NULL){
		printf("need more node for new frame start2\r\n");
		while(1);		
	}	
	addr = get_addr(jpg_p);
	jpg_set_addr(p_jpg,addr,JPG_BUF_LEN-JPG_HEAD_RESERVER);
	if(updata_dqt)
		jpg_DQT_updata(p_jpg,updata_dqt);


}



void jpg_config_addr(struct jpg_device *p_jpg){
	uint32 addr;
	jpg_p = get_node(jpg_p,&free_tab);							//预分配的两个buf地址，帧结束的时候要去掉一个预分配节点
	addr = get_addr(jpg_p);
	jpg_set_addr(p_jpg,addr,JPG_BUF_LEN-JPG_HEAD_RESERVER);
	jpg_p = get_node(jpg_p,&free_tab);
	addr = get_addr(jpg_p);
	jpg_set_addr(p_jpg,addr,JPG_BUF_LEN-JPG_HEAD_RESERVER);
}

void jpg_isr_init(struct jpg_device *p_jpg){
	jpg_request_irq(p_jpg,(jpg_irq_hdl )&jpg_outbuff_full_isr,JPG_IRQ_FLAG_JPG_BUF_FULL,p_jpg);
	jpg_request_irq(p_jpg,(jpg_irq_hdl )&jpg_buf_err,JPG_IRQ_FLAG_ERROR,p_jpg);		
	jpg_request_irq(p_jpg,(jpg_irq_hdl )&jpg_done_isr,JPG_IRQ_FLAG_JPG_DONE,p_jpg);
	NVIC_EnableIRQ(JPG_IRQn);
}

void jpg_start(){
	printf("JPG start\r\n");
	struct jpg_device *jpeg_dev;
#if FRAME_MANAGEMENT
	mjpeg_sema_init();
	app_analyze_init_with_name(&g_app_analyze,"dvp_video",JPG_FRAME_NUM,psram_frame_dvp_buf,JPG_FRAME_LEN);
	csi_kernel_task_new((k_task_entry_t)jpeg_app_analyze, "m_analyze", 0, 1, 0, NULL, 512,&manalyze_handle);
#endif

	jpeg_dev = (struct jpg_device *)dev_get(HG_JPG_DEVID);	
	
	jpg_room_init();

//jpg table init
	jpg_init(jpeg_dev,DQT_DEF,default_qt);
	
//jpg csr config
	jpg_set_size(jpeg_dev,IMAGE_H_J,IMAGE_W_J);
//jpg config
	jpg_config_addr(jpeg_dev);
//jpg int init
	jpg_isr_init(jpeg_dev);
	jpg_open(jpeg_dev);

}

//获取实际内容buf的长度
int get_jpg_node_len()
{
	return JPG_BUF_LEN-JPG_HEAD_RESERVER;
}


void del_jpeg_frame(void *d)
{

	struct list_head* get_f = (struct list_head*)d;
	del_frame(get_f);

}

struct list_head* get_frame()
{
	uint8 frame_num = 0;
	for(frame_num = 0;frame_num < JPEG_FRAME_NUM;frame_num++){
		if(jpg_frame[frame_num].usable == 2){
			jpg_frame[frame_num].usable = 1;
			return (struct list_head *)&jpg_frame[frame_num].list;
		}
	}

	return NULL;	
}

uint32 get_jpeg_len(void *d)
{
	uint32 flen;
	struct list_head* get_f = (struct list_head*)d;
	jpeg_frame* jpf;
	jpf = list_entry(get_f,jpeg_frame,list);
	flen = jpf->frame_len;
	//printf("%s:%d\tflen:%d\n",__FUNCTION__,__LINE__,flen);

	return flen;
}



void *get_jpeg_first_buf(void *d)
{
	struct list_head* get_f = (struct list_head*)d;
	mjpeg_node* mjn;
	mjn = list_entry(get_f->next,mjpeg_node,list);
	if(mjn)
	{
		return mjn->buf_addr;
	}
	return NULL;
}

void del_jpeg_first_node(void *d)
{
	struct list_head* get_f = (struct list_head*)d;
	put_node(&free_tab,get_f);	
}



//mjpeg信号量创建
static struct os_semaphore mjpeg_sem = {0,NULL};
void mjpeg_sema_init()
{
	os_sema_init(&mjpeg_sem,0);
}

void mjpeg_sema_down(int32 tmo_ms)
{
	os_sema_down(&mjpeg_sem,tmo_ms);
	//printf("$\n");
}

void mjpeg_sema_up()
{
	os_sema_up(&mjpeg_sem);
	//printf("@\n");
}






#if FRAME_MANAGEMENT

void jpeg_app_analyze()				//应用层
{
	uint32 count = 0;
	uint32 flen;
	uint8* addr;
	uint32 mask_len = 0;
	//uint32 itk;
	struct list_head* get_f;
	struct list_head* get_analy_node;
	mjpeg_node* mjn;
	jpeg_frame* jpf;
	//uint8 printf_dat = 0;
	//uint8 *check;
	while(1){
		mjpeg_sema_down(-1);
		NVIC_DisableIRQ(JPG_IRQn);
		get_f = get_frame();
		NVIC_EnableIRQ(JPG_IRQn);
		if(get_f){
			count++;
			usr_p = get_f;
			jpf = list_entry(get_f,jpeg_frame,list);
			flen = jpf->frame_len;
			
			get_analy_node = get_app_analyze_node(&g_app_analyze);
			
			if(get_analy_node){
				
				set_app_buf_len(get_analy_node,flen);
				addr = get_real_buf_adr(get_analy_node);
				//printf("psram:%x  %d\r\n",addr,flen);
			    mask_len = 0;
				while(list_empty((struct list_head *)usr_p) != TRUE)
				{
					mjn = list_entry(usr_p->next,mjpeg_node,list);
					/*
						此处使用当前mjn，获取节点对应的buf数据位置，对数据进行处理
					*/
					//if(printf_dat)
					{
						if(flen > JPG_BUF_LEN - JPG_HEAD_RESERVER){
							//for(itk = 0;itk < JPG_BUF_LEN;itk++){
							//	printf("%02x ",mjn->buf_addr[itk-JPG_HEAD_RESERVER]);
							//}
							flen -= (JPG_BUF_LEN - JPG_HEAD_RESERVER);
							hw_memcpy0(addr+mask_len,mjn->buf_addr,JPG_BUF_LEN - JPG_HEAD_RESERVER);
							mask_len += (JPG_BUF_LEN - JPG_HEAD_RESERVER); 
						}
						else{
							//for(itk = 0;itk < flen+JPG_HEAD_RESERVER;itk++){
							//	printf("%02x ",mjn->buf_addr[itk-JPG_HEAD_RESERVER]);
							//}
							hw_memcpy0(addr+mask_len,mjn->buf_addr,flen);
							flen = 0;
							//check = mjn->buf_addr+flen-
							//if(addr+mask_len)
						}
					
					}
					put_node(&free_tab,usr_p);				//节点归还到free_tab,边处理边给下一帧释放空间
				}


				map_realnode_2_app_node_msg(&g_app_analyze,get_analy_node);
				printf("W");
				wake_up_analyze_list_app(&g_app_analyze);		
				//printf("w");
			}

			printf("E");
			del_frame(usr_p);							//删除frame
		
		}
		
	}
}


#endif

#endif

