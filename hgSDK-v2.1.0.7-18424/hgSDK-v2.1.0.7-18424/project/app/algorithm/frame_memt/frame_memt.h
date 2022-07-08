#ifndef _FRAME_MEMT_H_
#define _FRAME_MEMT_H_
#include "tx_platform.h"
#include "list.h"
#include "typesdef.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "string.h"
#include "csi_kernel.h"


typedef void (*up_action)();
typedef struct 
{
	struct list_head  list;				        //此frame的节点信息
	struct list_head* list_real;				//与实际空间绑定的链表地址
	uint32_t frame_counter;						    //记录frame编号
	uint32_t frame_len;						        //帧长度
}app_frame_msg;

typedef struct
{
	struct list_head list;				        //此frame的节点信息
	struct list_head* app_analyze_node_head;    //app_analyze里最高
	uint32_t frame_len;
	int new_frame_num;						    //最新帧编号
	char   counter;							    //此frame的应用使用数量
	uint8_t* addr;									//绑定的地址
	uint32_t timestamp;
	uint32_t send_timestamp;							//分发到应用的时候的时间,主要用于调试,看看什么时候发出去,如果没有释放,可以大概知道什么时候的数据帧
	void (*free_frame)(struct list_head* list,struct list_head *anly_free_node);  //释放节点回去
}real_frame_msg;

typedef struct 
{
	struct list_head list;      			//管理应用部分
	struct list_head free_list;             //节点管理表
	real_frame_msg *fp;						//记录节点的数组(初始化的时候记录一下,如果重新初始化,释放这部分内存,然后重新申请一个fp记录,重新初始化需要考虑当前的节点没有被使用)
	struct os_mutex lock;
}app_analyze;


typedef void (*msg_up_callback)(void *signal,const void *msg_ptr, int32_t tmo_ms);
typedef void* (*msg_down_callback) (void*signal,int32 tmo_ms); 

typedef struct 
{
	struct list_head list;						//应用的节点链表	
	struct list_head free_list;				    //此应用的空闲链表头
	struct list_head deal_list;	                //此应用的处理链表头
	
	char name[20];
	void *app_action;
	uint32_t last_frame_utime,tmp;						//本应用上次处理帧所费时间，用于帧管理拓展
	int up_enable;
	int enable;									//当前注册的应用是否使用此帧图像
	void *fp;
	app_analyze* gaa;


	
	struct os_semaphore sem;
	void (*init)(struct os_semaphore* sem);
	void (*up)(struct os_semaphore* sem); 								//唤醒app调用函数
	void (*down)(struct os_semaphore* sem,int32 tmo_ms); 
	void (*del)(struct os_semaphore* sem); 


	void *signal_msg;							//接入msg信号量
	//void (*msg_up)(void *signal,const void *msg_ptr, int32_t tmo_ms); 								//唤醒app调用函数
	msg_up_callback msg_up;
	msg_down_callback msg_down;
	
	//void (*msg_down)(void *signal,int32 tmo_ms); 


	
}app_frame_management;


app_frame_management* register_user_action(app_analyze* gaa,void *mation,char* name);
void unregister_user_action(app_analyze* gaa,app_frame_management* app_memt);
app_frame_management* get_frame_memt_by_name(char* name,app_analyze *aaze);
void free_real_msg(struct list_head *real_node,struct list_head *anly_free_node);
void app_analyze_init(app_analyze* gaa,uint8_t free_node_num,uint8_t* buf,uint32_t buf_len);
void app_node_init(app_frame_management* afmemt,uint8_t app_node_num);
struct list_head * get_app_node(app_frame_management* afmemt);
void put_app_node(struct list_head * app_node,app_frame_management* afmemt);
uint8_t real_node_2_app_node(struct list_head *real_node,struct list_head * app_node);
void finish_app_node(struct list_head * app_node,app_frame_management* afmemt);
uint8_t* get_app_buf_adr(struct list_head * app_node);
uint32_t get_app_buf_len(struct list_head * app_node);
struct list_head *get_app_analyze_node(app_analyze* gaa);
void set_real_buf_timestamp(struct list_head * real_node,uint32_t time);
void set_app_buf_len(struct list_head * real_node,uint32_t len);
uint8_t* get_real_buf_adr(struct list_head * real_node);
uint8_t map_realnode_2_app_node_msg(app_analyze* aaze,struct list_head *real_node);
struct list_head *get_app_node_from_deal_list(app_frame_management* afmemt);
void app_frame_enable(app_frame_management* afmemt,uint8_t enable);


app_analyze* find_app_analyze(const char *name);
int app_analyze_init_with_name(app_analyze* gaa,const char *name,uint8_t free_node_num,uint8_t* buf,uint32_t buf_len);


void* msg_sema_init(void *signal_null,int32_t msg_count, int32_t msg_size,const char *name);

int msg_sema_del(void *signal);
void wake_up_analyze_list_app(app_analyze* aaze);
void user_action_access_msg(app_frame_management* app_memt,void *msg);
int app_analyze_reinit_with_name(app_analyze* gaa,const char *name,uint8_t free_node_num,uint8_t* buf,uint32_t buf_len);
uint32_t get_real_buf_timestamp(struct list_head * app_node);

app_frame_management* register_user_action_other_sem(app_analyze* gaa,void *mation,char* name,void *d);
void frame_memt_err(void *d);
uint8_t force_del_frame(struct list_head *real_node);
struct list_head *get_app_node_from_deal_list_with_del(app_frame_management* afmemt);











#endif

