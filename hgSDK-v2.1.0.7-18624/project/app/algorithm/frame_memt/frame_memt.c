#include "frame_memt.h"


int32 memt_sema_init(struct os_semaphore *sem)
{
	return os_sema_init(sem,0);
}

int32 memt_sema_down(struct os_semaphore *sem,int32 tmo_ms)
{
	return os_sema_down(sem,tmo_ms);	
}

int32 memt_sema_up(struct os_semaphore *sem)
{
	return os_sema_up(sem);
}

int32 memt_sema_del(struct os_semaphore *sem)
{
	return os_sema_del(sem);
}








//将msg信号量挂在全局上,可以通过名字搜索,
struct global_msg_sema_struct
{
	struct list_head  list;	
	void *signal;
	const char *name;
};

static LIST_HEAD(global_msg_sema);


void* find_msg_sema_with_name(const char *name)
{
	if(!name)
	{
		return NULL;
	}
	struct global_msg_sema_struct* msg = (struct global_msg_sema_struct*)global_msg_sema.next;
	struct global_msg_sema_struct* global_msg = (struct global_msg_sema_struct*)&global_msg_sema;

	while(msg != global_msg)
	{
		if(strlen(msg->name)!= strlen(name) && !memcmp(msg->name,name,strlen(msg->name)))
		{
			return msg->signal;
		}
		msg = (struct global_msg_sema_struct*)msg->list.next;
	}

	return NULL;
	
}


void* find_msg_sema_with_signal(void *signal)
{
	if(!signal)
	{
		return NULL;
	}
	struct global_msg_sema_struct* msg = (struct global_msg_sema_struct*)global_msg_sema.next;
	struct global_msg_sema_struct* global_msg = (struct global_msg_sema_struct*)&global_msg_sema;

	while(msg != global_msg)
	{
		if(msg->signal == signal)
		{
			return msg;
		}
		msg = (struct global_msg_sema_struct*)msg->list.next;
	}

	return NULL;
	
}


void add_msg_sema(struct global_msg_sema_struct *add_msg)
{
	//未初始化
	if(!add_msg)
	{
		return;
	}
	uint32_t flags;
	flags = disable_irq();
	list_add_tail(&add_msg->list,&global_msg_sema);
	enable_irq(flags);
	
}

int del_msg_sema(void *signal)
{
	struct global_msg_sema_struct *del_msg;
	//未初始化
	if(!signal)
	{
		return 1;
	}
	del_msg = find_msg_sema_with_signal(signal);
	if(!del_msg)
	{
		return 2;
	}
	uint32_t flags;
	flags = disable_irq();
	list_del(&del_msg->list);
	enable_irq(flags);
	free(del_msg);
	return 0;
	
}




void* msg_sema_init(void *signal_null,int32_t msg_count, int32_t msg_size,const char *name)
{
	void *signal = NULL;
	if(!name)
	{
		goto msg_sema_init_end;
	}
	if(find_msg_sema_with_name(name))
	{
		goto msg_sema_init_end;
	}
	struct global_msg_sema_struct *msg = (struct global_msg_sema_struct *)malloc(sizeof(struct global_msg_sema_struct));
	if(!msg)
	{
		goto msg_sema_init_end;
	}
	
	signal = csi_kernel_msgq_new(msg_count,msg_size);
	if(!signal)
	{
		free(msg);
		goto msg_sema_init_end;
	}
	add_msg_sema(msg);
	msg->signal = signal;
	msg->name = name;
msg_sema_init_end:
	return signal;
}

void *msg_sema_down(void *signal,int32 tmo_ms)
{
	void *val = NULL;
	int res;
	res = csi_kernel_msgq_get(signal,&val,tmo_ms);	
	return val;
}

void msg_sema_up(void *signal,const void *msg_ptr, int32_t tmo_ms)
{
	csi_kernel_msgq_put(signal,&msg_ptr,0,tmo_ms);
}

int msg_sema_del(void *signal)
{  	
	if(!signal)
	{
		return 1;
	}
	if(del_msg_sema(signal))
	{
		return 2;
	}
	csi_kernel_msgq_del(signal);
	return 0;
}





app_frame_management* register_user_action(app_analyze* gaa,void *mation,char* name)
{
	uint32 flags;
	app_frame_management* app_memt;
	app_memt = malloc(sizeof(app_frame_management));
	app_memt->app_action = mation;
	app_memt->enable     = 0;
	app_memt->up         = memt_sema_up;
	app_memt->down         = memt_sema_down;
	app_memt->init         = memt_sema_init;
	app_memt->del         = memt_sema_del;
	app_memt->signal_msg  = NULL;
	app_memt->msg_up = msg_sema_up;
	app_memt->msg_down = msg_sema_down; 
	
	app_memt->up_enable	 = 1;
	app_memt->fp = NULL;
	app_memt->gaa = gaa;
	memset(app_memt->name,0,20);
	memcpy(app_memt->name,name,strlen(name));
	INIT_LIST_HEAD(&app_memt->free_list);								//初始化当前应用链表头
	INIT_LIST_HEAD(&app_memt->deal_list);
	INIT_LIST_HEAD(&app_memt->list);
	//app_memt->sem.hdl = NULL;
	memset(&app_memt->sem,0,sizeof(app_memt->sem));
	app_memt->init(&app_memt->sem);
	flags = disable_irq();
	list_add_tail(&app_memt->list,&gaa->list);					//将应用链表头放到管理链表中
	enable_irq(flags);
	return app_memt;
}



app_frame_management* register_user_action_other_sem(app_analyze* gaa,void *mation,char* name,void *d)
{
	if(!d)
	{
		printf("%s err,arg sem is err\n",__FUNCTION__);
		return NULL;
	}
	uint32 flags;
	app_frame_management* app_memt;
	app_memt = malloc(sizeof(app_frame_management));
	app_memt->app_action = mation;
	app_memt->enable     = 0;
	app_memt->up         = memt_sema_up;
	app_memt->down         = memt_sema_down;
	app_memt->init         = memt_sema_init;
	app_memt->del         = NULL;
 
	app_memt->signal_msg  = NULL;
	app_memt->msg_up = msg_sema_up;
	app_memt->msg_down = msg_sema_down;

	

	app_memt->up_enable	 = 1;
	app_memt->fp = NULL;
	app_memt->gaa = gaa;
	memset(app_memt->name,0,20);
	memcpy(app_memt->name,name,strlen(name));
	INIT_LIST_HEAD(&app_memt->free_list);								//初始化当前应用链表头
	INIT_LIST_HEAD(&app_memt->deal_list);
	INIT_LIST_HEAD(&app_memt->list);
	app_memt->sem.hdl = d;
	//app_memt->init(&app_memt->sem);
	flags = disable_irq();
	list_add_tail(&app_memt->list,&gaa->list);					//将应用链表头放到管理链表中
	enable_irq(flags);
	return app_memt;
}

//app_frame_management的msg信号量接入
void user_action_access_msg(app_frame_management* app_memt,void *msg)
{
	app_memt->signal_msg = msg;
}


void unregister_user_action(app_analyze* gaa,app_frame_management* app_memt){
	uint32 flags;
	if(!app_memt)
	{
		return;
	}
	app_frame_enable(app_memt,0);
	//将缓存帧清除
	void *get_f;
	while(1)
	{
		get_f = get_app_node_from_deal_list(app_memt);
		if(!get_f)
		{
			break;
		}
		finish_app_node(get_f,app_memt);
	}

	
	os_mutex_lock(&app_memt->gaa->lock,osWaitForever);
	flags = disable_irq();
	list_del(&app_memt->list);					//将应用链表头抽出管理链表
	enable_irq(flags);
	os_mutex_unlock(&app_memt->gaa->lock);

	
	if(app_memt->del)
	{
		app_memt->del(&app_memt->sem);
	}

	if(app_memt->fp)
	{
		free(app_memt->fp);
	}
	free(app_memt);
}

app_frame_management* get_frame_memt_by_name(char* name,app_analyze *aaze){
	struct list_head *pos;
	list_for_each(pos,&aaze->list){
		app_frame_management *af_memt = list_entry (pos, app_frame_management, list);

		if(strcmp((const char*)name,(const char *)af_memt->name) == 0){
			return af_memt;
		}
	}
	return NULL;
}

void free_real_msg(struct list_head *real_node,struct list_head *anly_free_node){
	uint32 flags;
	INIT_LIST_HEAD(real_node);
	flags = disable_irq();
	list_move(real_node,anly_free_node);
	enable_irq(flags);
	
}
real_frame_msg *rfmsg_check;


void app_analyze_init(app_analyze* gaa,uint8_t free_node_num,uint8_t* buf,uint32_t buf_len){
	uint8_t itk = 0;
	real_frame_msg *fp;
	gaa->fp = NULL;
	os_mutex_init(&gaa->lock);
	INIT_LIST_HEAD(&gaa->list);						    //初始化应用节点头
	INIT_LIST_HEAD(&gaa->free_list);				    //初始化空闲节点头
	fp = malloc(free_node_num*sizeof(real_frame_msg));
	for(itk = 0; itk < free_node_num;itk++){
		INIT_LIST_HEAD(&fp[itk].list);
		fp[itk].addr = buf+itk*buf_len;                 //此节点与实际空间一对一绑定
		list_add_tail(&fp[itk].list,&gaa->free_list);	//将刚申请的节点放到空闲节点表中
		fp[itk].counter = 0;
		fp[itk].free_frame = free_real_msg;
		fp[itk].app_analyze_node_head = &gaa->free_list;
		if(itk == 0){
			rfmsg_check = &fp[itk];
		}
	}
	gaa->fp = fp;
}


//将节点挂在全局上,可以通过名字搜索,但是同名不会创建成功,单链表
struct global_app_analyze_struct
{
	struct list_head  list;	
	app_analyze* app;
	const char *name;
};

static LIST_HEAD(global_app_analyze);
 

app_analyze* find_app_analyze(const char *name)
{
	if(!name)
	{
		return NULL;
	}
	uint32 flags;
	app_analyze* ret = NULL;
	flags = disable_irq();

	struct global_app_analyze_struct* app = (struct global_app_analyze_struct*)global_app_analyze.next;
	struct global_app_analyze_struct* global_app = (struct global_app_analyze_struct*)&global_app_analyze;

	while(app != global_app)
	{
		if((strlen(app->name) == strlen(name)) &&!memcmp(app->name,name,strlen(app->name)))
		{
			ret = app->app;
			goto find_app_analyze_end;
		}
		app = (struct global_app_analyze_struct*)app->list.next;
	}
find_app_analyze_end:

	enable_irq(flags);
	return ret;
	
}


struct global_app_analyze_struct * find_global_app_analyze_struct(const char *name)
{
	if(!name)
	{
		return NULL;
	}
	uint32 flags;
	struct global_app_analyze_struct * ret = NULL;
	flags = disable_irq();

	struct global_app_analyze_struct* app = (struct global_app_analyze_struct*)global_app_analyze.next;
	struct global_app_analyze_struct* global_app = (struct global_app_analyze_struct*)&global_app_analyze;

	while(app != global_app)
	{
		if((strlen(app->name) == strlen(name)) &&!memcmp(app->name,name,strlen(app->name)))
		{
			ret = app;
			goto find_app_analyze_end;
		}
		app = (struct global_app_analyze_struct*)app->list.next;
	}
find_app_analyze_end:

	enable_irq(flags);
	return ret;
	
}



uint8_t add_app_analyze(struct global_app_analyze_struct *add_app)
{
	//未初始化
	if(!add_app)
	{
		return 1;
	}
	uint32 flags;
	flags = disable_irq();
	list_add_tail(&add_app->list,&global_app_analyze);
	enable_irq(flags);
	return 0;
	
}


uint8_t del_app_analyze(const char *name)
{
	uint8_t res = 1;
	if(!name)
	{
		return res;
	}
	uint32 flags;
	struct global_app_analyze_struct* del;
	flags = disable_irq();

	struct global_app_analyze_struct* app = (struct global_app_analyze_struct*)global_app_analyze.next;
	struct global_app_analyze_struct* global_app = (struct global_app_analyze_struct*)&global_app_analyze;

	while(app != global_app)
	{
		if((strlen(app->name) == strlen(name)) &&!memcmp(app->name,name,strlen(app->name)))
		{
			del = app;
			list_del(&del->list);
			res = 0;
			goto del_app_analyze_end;
		}
		app = (struct global_app_analyze_struct*)app->list.next;
	}
del_app_analyze_end:

	return res;
	
}


uint8_t app_analyze_uninstall_with_name(const char *name)
{
	uint32 flags;
	int ret = RET_ERR;
	flags = disable_irq();

	struct global_app_analyze_struct *uninstall = find_global_app_analyze_struct(name);
	
	if(uninstall)
	{
		app_analyze* app = uninstall->app;
		//查看是否有应用挂载,如果有,那么卸载应该失败,等待所有应用卸载完毕后,才能释放改app_analyze
		if(!list_empty(&app->list))
		{
			goto app_analyze_uninstall_with_name_end;
		}

		del_app_analyze(name);
		os_mutex_del(&app->lock);
		if(app->fp)
		{
			free(app->fp);
		}
		free(uninstall);
		ret = RET_OK;
		goto app_analyze_uninstall_with_name_end;
	}
app_analyze_uninstall_with_name_end:

	enable_irq(flags);

	return ret;
}



int app_analyze_init_with_name(app_analyze* gaa,const char *name,uint8_t free_node_num,uint8_t* buf,uint32_t buf_len){
	uint8_t itk = 0;
	real_frame_msg *fp = NULL;

	int res = 0;
	gaa->fp = NULL;
	//先判断是否已经有同名挂在全局链表,如果有直接报错
	struct global_app_analyze_struct *app = (struct global_app_analyze_struct *)find_app_analyze(name);
	if(app)
	{
		res = -1;
		printf("%s same name:%s\n",__FUNCTION__,name);
		goto app_analyze_err;
	}
	app = (struct global_app_analyze_struct *)malloc(sizeof(struct global_app_analyze_struct));

	if(!app)
	{
		res = -2;
		goto app_analyze_err;
	}

	os_mutex_init(&gaa->lock);
	
	INIT_LIST_HEAD(&gaa->list);						    //初始化应用节点头
	INIT_LIST_HEAD(&gaa->free_list);				    //初始化空闲节点头
	fp = malloc(free_node_num*sizeof(real_frame_msg));
	if(!fp)
	{
		
		res = -3;
		goto app_analyze_err;
	}
	gaa->fp = fp;
	for(itk = 0; itk < free_node_num;itk++){
		INIT_LIST_HEAD(&fp[itk].list);
		fp[itk].addr = buf+itk*buf_len;                 //此节点与实际空间一对一绑定
		list_add_tail(&fp[itk].list,&gaa->free_list);	//将刚申请的节点放到空闲节点表中
		fp[itk].counter = 0;
		fp[itk].free_frame = free_real_msg;
		fp[itk].app_analyze_node_head = &gaa->free_list;
		if(itk == 0){
			rfmsg_check = &fp[itk];
		}
	}
	gaa->fp = fp;
	app->app = gaa;
	app->name = name;
	//成功添加
	add_app_analyze(app);
	printf("%s add success:%s\n",__FUNCTION__,name);
	return 0;
app_analyze_err:
	if(fp)
	{
		free(fp);
	}
	if(app)
	{
		free(app);
	}
	return res;
}

//重新初始化,对节点重新分配,所以需要原来就已经有这个name
int app_analyze_reinit_with_name(app_analyze* gaa,const char *name,uint8_t free_node_num,uint8_t* buf,uint32_t buf_len){
	uint8_t itk = 0;
	real_frame_msg *fp = NULL;

	int res = 0;
	//先判断是否已经有同名挂在全局链表,如果有直接报错
	struct global_app_analyze_struct *app = (struct global_app_analyze_struct *)find_app_analyze(name);
	if(!app)
	{
		res = -1;
		goto app_analyze_reinit_err;
	}
	INIT_LIST_HEAD(&gaa->list);						    //初始化应用节点头
	INIT_LIST_HEAD(&gaa->free_list);				    //初始化空闲节点头

	//重新初始化,所以先将原来申请的空间删除
	if(gaa->fp)
	{
		free(gaa->fp);
		gaa->fp = NULL;
	}
	fp = malloc(free_node_num*sizeof(real_frame_msg));
	if(!fp)
	{
		
		res = -3;
		goto app_analyze_reinit_err;
	}
	gaa->fp = fp;
	for(itk = 0; itk < free_node_num;itk++){
		INIT_LIST_HEAD(&fp[itk].list);
		fp[itk].addr = buf+itk*buf_len;                 //此节点与实际空间一对一绑定
		list_add_tail(&fp[itk].list,&gaa->free_list);	//将刚申请的节点放到空闲节点表中
		fp[itk].counter = 0;
		fp[itk].free_frame = free_real_msg;
		fp[itk].app_analyze_node_head = &gaa->free_list;
		if(itk == 0){
			rfmsg_check = &fp[itk];
		}
	}
	printf("%s add success\n",__FUNCTION__);
	return 0;
app_analyze_reinit_err:
	if(fp)
	{
		free(fp);
	}
	if(app)
	{
		free(app);
	}
	return res;
}




struct list_head *get_app_analyze_node(app_analyze* gaa)
{
	struct list_head *pos = NULL;
	real_frame_msg *rfmsg;
	uint32 flags;
	flags = disable_irq();
	if(list_empty(&gaa->free_list)){
		goto get_app_analyze_node_end;
	}
	pos = gaa->free_list.next;
	rfmsg = list_entry(pos,real_frame_msg,list);
	list_del(gaa->free_list.next);	
	
	rfmsg->counter = 0;
	get_app_analyze_node_end:
	enable_irq(flags);
	return pos;

}

void wake_up_analyze_list_app(app_analyze* aaze)
{
	struct list_head *pos;
	os_mutex_lock(&aaze->lock,osWaitForever);
	list_for_each(pos,&aaze->list){
		app_frame_management *af_memt = list_entry (pos, app_frame_management, list);

		if(af_memt->enable == 1 && af_memt->up_enable == 1 ){
			if(af_memt->signal_msg)
			{
				af_memt->msg_up(af_memt->signal_msg,af_memt,-1);
			}
			else if(af_memt->up )
			{
				//printf("up:%X\tname:%s\t%X\n",af_memt->up,af_memt->name,af_memt->sem.hdl);
				af_memt->up(&af_memt->sem);	
			}
		}
		else
		{
			//默认打开
			af_memt->up_enable = 1;
		}

	}	
	os_mutex_unlock(&aaze->lock);
	return ;
}


void app_node_init(app_frame_management* afmemt,uint8_t app_node_num){
	uint8_t itk = 0;
	app_frame_msg *fp;
	fp = malloc(app_node_num*sizeof(app_frame_msg));
	afmemt->fp = fp;
	for(itk = 0; itk < app_node_num;itk++){
		INIT_LIST_HEAD(&fp[itk].list);
		fp[itk].frame_counter = 0;
		fp[itk].frame_len     = 0;
		list_add_tail((struct list_head*)&fp[itk],&afmemt->free_list);	//将刚申请的节点放到空闲节点表中
	}	
}

void app_frame_enable(app_frame_management* afmemt,uint8_t enable){
	afmemt->enable = enable;
}


//提取节点，应用有空闲节点的话将节点提取出来，用于绑定信息
struct list_head * get_app_node(app_frame_management* afmemt)
{
	uint32 flags;
	flags = disable_irq();

	if(afmemt == NULL)
	{
		goto get_app_node_end;
	}
	if(list_empty(&afmemt->free_list))
	{
		goto get_app_node_end;
	}
	
	list_move(afmemt->free_list.next,&afmemt->deal_list);
	enable_irq(flags);
	return afmemt->deal_list.next;
get_app_node_end:
	enable_irq(flags);
	printf("#");
	return NULL;
}
 

void put_app_node(struct list_head * app_node,app_frame_management* afmemt){
	uint32 flags;
	flags = disable_irq();
	list_move(app_node,&afmemt->free_list);
	enable_irq(flags);
}

struct list_head *get_app_node_from_deal_list(app_frame_management* afmemt){
	if(list_empty(&afmemt->deal_list)){
		return NULL;
	}

	afmemt->tmp = csi_kernel_get_ticks();
	return afmemt->deal_list.prev;
}


struct list_head *get_app_node_from_deal_list_with_del(app_frame_management* afmemt){
	uint32 flags;
	struct list_head *get = NULL;
	flags = disable_irq();

	if(list_empty(&afmemt->deal_list)){
		goto get_app_node_from_deal_list_with_del_end;
	}

	afmemt->tmp = csi_kernel_get_ticks();
	get = afmemt->deal_list.prev;
	if(get)
	{
		list_del_init(get);
	}
get_app_node_from_deal_list_with_del_end:
	enable_irq(flags);
	return get;
}



uint8_t real_node_2_app_node(struct list_head *real_node,struct list_head * app_node){
	app_frame_msg*  afmsg;
	real_frame_msg* rfmsg; 
	if((real_node == NULL)||(app_node == NULL))
		return 1;

	rfmsg = list_entry(real_node,real_frame_msg,list);
	afmsg = list_entry(app_node,app_frame_msg,list);
	afmsg->list_real     = real_node;
	afmsg->frame_len     = rfmsg->frame_len;
	afmsg->frame_counter = rfmsg->new_frame_num;
	rfmsg->counter++;
	return 0;
} 


//直接绑定节点,是real_node_2_app_node与get_app_node的合并版本
uint8_t real_node_app_node(app_frame_management* afmemt,struct list_head *real_node)
{
	uint32 flags;
	app_frame_msg*  afmsg;
	real_frame_msg* rfmsg; 
	uint8_t res = 1;
	struct list_head * app_node;
	flags = disable_irq();
	if(afmemt == NULL)
	{
		goto real_node_app_node_end;
	}

	if(!real_node)
	{
		goto real_node_app_node_end;
	}
	if(list_empty(&afmemt->free_list))
	{
		goto real_node_app_node_end;
	}
		
	list_move(afmemt->free_list.next,&afmemt->deal_list);
	app_node = afmemt->deal_list.next;

	if(!app_node)
	{
		goto real_node_app_node_end;
	}

	//只有运行到这里才是返回正常值
	rfmsg = list_entry(real_node,real_frame_msg,list);
	afmsg = list_entry(app_node,app_frame_msg,list);
	afmsg->list_real	 = real_node;
	afmsg->frame_len	 = rfmsg->frame_len;
	afmsg->frame_counter = rfmsg->new_frame_num;
	rfmsg->counter++;
	res = 0;



real_node_app_node_end:
	enable_irq(flags);
	return res;

}


//强行删除frame,一般在源头操作,如果已经经过map_realnode_2_app_node_msg是不能这样操作
uint8_t force_del_frame(struct list_head *real_node)
{
	real_frame_msg* rfmsg; 
	rfmsg = list_entry(real_node,real_frame_msg,list);
	if(rfmsg->counter)
	{
		printf("%s:%d err\n",__FUNCTION__,__LINE__);
	}
	rfmsg->free_frame(real_node,rfmsg->app_analyze_node_head);
}
uint8_t map_realnode_2_app_node_msg(app_analyze* aaze,struct list_head *real_node){
	struct list_head *apppos;
	struct list_head *pos;
	uint8_t res;
	int tmp_counter ;
	real_frame_msg* rfmsg; 
	uint32 flags;
	

	rfmsg = list_entry(real_node,real_frame_msg,list);
	os_mutex_lock(&aaze->lock,osWaitForever);
	flags = disable_irq();
	rfmsg->counter++;
	enable_irq(flags);
	rfmsg->send_timestamp = csi_kernel_get_ticks();
	list_for_each(pos,&aaze->list){
		app_frame_management *af_memt = list_entry (pos, app_frame_management, list);
		if(af_memt->enable == 1){
			//apppos = get_app_node(af_memt);
			//res = real_node_2_app_node(real_node,apppos);

			res = real_node_app_node(af_memt,real_node);
			if(res)
			{
				printf("%s:%d\tname:%s\n",__FUNCTION__,__LINE__,af_memt->name);
				af_memt->up_enable = 0;
			}
			
		}
	}	
	flags = disable_irq();
	tmp_counter = (--rfmsg->counter);
	enable_irq(flags);

	if(tmp_counter== 0)
	{
		rfmsg->free_frame(real_node,rfmsg->app_analyze_node_head);
		
	}
	//counter不存在是<0
	else if(tmp_counter < 0)
	{
		printf("%s:%d err\n",__FUNCTION__,__LINE__);
	}
	os_mutex_unlock(&aaze->lock);
	return 0;
}



//当前APP使用节点结束，观察是否需要回收节点
void finish_app_node(struct list_head * app_node,app_frame_management* afmemt){
	app_frame_msg*  afmsg;
	real_frame_msg* rfmsg;
	uint32 flags;
	int now_counter;//counter保存
	afmsg = list_entry(app_node,app_frame_msg,list);
	rfmsg = list_entry(afmsg->list_real,real_frame_msg,list);
	//进来就将对应节点释放
	afmemt->last_frame_utime = csi_kernel_get_ticks() - afmemt->tmp;
											//将应用节点放回应用节点的free_list处
	flags = disable_irq();
	now_counter = (--rfmsg->counter);
	enable_irq(flags);
	
	if(now_counter == 0){
		rfmsg->free_frame(afmsg->list_real,rfmsg->app_analyze_node_head);	//所有应用使用完成，即将此节点放回原始free_list
	}
	put_app_node(app_node,afmemt);	
	

}

void set_app_buf_len(struct list_head * real_node,uint32_t len){
	real_frame_msg* rfmsg;
	rfmsg = list_entry(real_node,real_frame_msg,list);
	rfmsg->frame_len = len;
}

uint8_t* get_real_buf_adr(struct list_head * real_node){
	real_frame_msg* rfmsg;
	rfmsg = list_entry(real_node,real_frame_msg,list);
	return rfmsg->addr;
}


uint32_t get_real_node_buf_len(struct list_head * real_node)
{
	real_frame_msg* rfmsg;
	rfmsg = list_entry(real_node,real_frame_msg,list);
	return rfmsg->frame_len;

}

void set_real_buf_timestamp(struct list_head * real_node,uint32_t time){
	real_frame_msg* rfmsg;
	rfmsg = list_entry(real_node,real_frame_msg,list);
	rfmsg->timestamp = time;
	return ;
}

uint32_t get_real_buf_timestamp(struct list_head * app_node){
	app_frame_msg*  afmsg;
	real_frame_msg* rfmsg; 
	afmsg = list_entry(app_node,app_frame_msg,list);
	rfmsg = list_entry(afmsg->list_real,real_frame_msg,list);
	return rfmsg->timestamp;


}




uint8_t* get_app_buf_adr(struct list_head * app_node){
	app_frame_msg*  afmsg;
	real_frame_msg* rfmsg; 
	afmsg = list_entry(app_node,app_frame_msg,list);
	rfmsg = list_entry(afmsg->list_real,real_frame_msg,list);

	return rfmsg->addr;

}

uint32_t get_app_buf_len(struct list_head * app_node){
	app_frame_msg*  afmsg;
	real_frame_msg* rfmsg; 
	afmsg = list_entry(app_node,app_frame_msg,list);
	rfmsg = list_entry(afmsg->list_real,real_frame_msg,list);
	//printf("rfmsg:%X\n",rfmsg);
	return rfmsg->frame_len;

}

uint32_t get_app_node_time(app_frame_management* afmemt){
	return afmemt->last_frame_utime;
}
void frame_memt_err(void *d)
{
	printf("%s:%d\targ:%X\n",__FUNCTION__,__LINE__,d);
}




