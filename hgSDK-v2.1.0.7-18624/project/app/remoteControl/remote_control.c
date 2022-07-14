#include "event.h"
#include "sys_config.h"
#include "remote_control.h"
#include "avi_record.h"
#include "socket_module.h"


#ifdef  OPENDML_EN 
struct remote_s
{
	int type;
	//char arg[24];
	int crc;

};

struct record_video r_v = {NULL,NULL,0};

int get_mjpeg_avi_control_status()
{
	return r_v.running;
}


extern uint8_t get_camera_dpi();//外部声明这个函数,获取分辨率使用
void set_mjpeg_avi_control_status(int value)
{
#if JPG_EN
	switch(get_camera_dpi())
#elif USB_EN
	switch(get_camera_uvc_dpi())
#else
	switch(get_camera_dpi())
#endif
	{
		case 0:
			printf("%s dpi is invail\n",__FUNCTION__);
			return;
			break;
		case 1:
			printf("%s dpi is VGA\n",__FUNCTION__);
			r_v.width = 640;
			r_v.height = 480;
			break;
		case 2:
			printf("%s dpi is 720P\n",__FUNCTION__);
			r_v.width = 1280;
			r_v.height = 720;

			break;
		default:
			printf("%s dpi is err\n",__FUNCTION__);
			return;
			break;
	}
	r_v.running = value;
}

void *get_mjpeg_avi_control_handle()
{
	return (void*)&r_v;
}


extern void mjpeg_app_avi_control_thread(void *d);
extern int get_mjpeg_avi_control_status();
extern void set_mjpeg_avi_control_status(int value);
void *get_mjpeg_avi_control_handle();










//暂时只有录像线程使用
int global_record_avi_thread_flag = 0;

#if 1

static void remote_control_event(struct event *ei, void *d)
{
	int fd = ei->ev.fd.fd;
	int res;
	struct remote_s *remote_control = (struct remote_s *)d;
	
	res = recv(fd,remote_control,sizeof(struct remote_s ),0);

	if(res <= 0)
	{
		printf("%s res:%d\n",__FUNCTION__,res);
		eloop_remove_event(ei);
		if(fd > 0)
		{
			close(fd);
		}
		if(remote_control)
		{
			free(remote_control);
		}
		return;
	}
	send(fd,remote_control,sizeof(struct remote_s),0);
	//判断remote_control执行不同事件,例如录像等
	printf("type:%d\t%d\n",remote_control->type,res);
	if(remote_control->type == 1 && !global_record_avi_thread_flag)
	{
		void *handle;
		if(!get_mjpeg_avi_control_status())
		{
			set_mjpeg_avi_control_status(1);
			csi_kernel_task_new((void*)mjpeg_app_avi_control_thread, "m_avi", get_mjpeg_avi_control_handle(), 10, 0, NULL, 2048,&handle);
		}
		else
		{
			printf("%s record thread is running\n",__FUNCTION__);
		}
	}
	else if(remote_control->type == 2)
	{
		if(get_mjpeg_avi_control_status())
		{
			set_mjpeg_avi_control_status(0);
		}
		else
		{
			printf("%s record thread isn't running\n",__FUNCTION__);
		}
	}  
	else
	{
		printf("%s:%d err\tglobal_record_avi_thread_flag:%d\n",__FUNCTION__,__LINE__,global_record_avi_thread_flag);
	}

	
}
 
static void remote_control_accept(void *e, void *d)
{
	int server_fd = (int)d;
	struct sockaddr_in addr;
	socklen_t namelen = sizeof(addr);
	int client_fd;
	client_fd = accept(server_fd, (struct sockaddr*)&addr, &namelen);
	if(client_fd < 0)
	{
		printf("%s accept() error\n",__FUNCTION__);  
	}
	struct remote_s *remote_control = (struct remote_s *)malloc(sizeof(struct remote_s *));
	printf("%s client_fd:%d\n",__FUNCTION__,client_fd);
	eloop_add_fd( client_fd , EVENT_READ, EVENT_F_ENABLED, (void*)remote_control_event, (void*)remote_control);

}
 
void remote_control_Server(int port)
{

	printf("%s : port: %d\n", __FUNCTION__,port);

	int fd = creat_tcp_socket(port);
	printf("%s fd:%d\n",__FUNCTION__,fd);
	if(listen(fd,1)== -1)
	{  
		perror("listen()error\n");  
		return ;  
	}  

	eloop_add_fd( fd, EVENT_READ, EVENT_F_ENABLED, remote_control_accept,(void*)fd );
}
#endif

#endif
