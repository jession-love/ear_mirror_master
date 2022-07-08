#ifndef __NETSPEEDTEST_H
#define __NETSPEEDTEST_H
#include "eloop.h"
#ifndef INVALID_SOCKET
#define INVALID_SOCKET ~0
#endif

struct ret_struct
{
	unsigned short int PID;
	//只有创建任务才需要session值,其他不用
	unsigned short int session;
	unsigned int start_time;
	unsigned int total_time;
	unsigned long long recv_size;
	unsigned long long send_size;

};


struct speedTestPack
{
	unsigned int seq;
	unsigned int len;
	unsigned int crc;
	char *buf;//可以是无效buf,仅仅测试速度使用
};


//udp定时发送结构
struct UdpToIp
{
	struct sockaddr_in remote_addr;
	struct event *timer_e;
	struct event *udp_e;
	int timer;
	int fd;
};


//pc的状态获取成功
#define CLIENT_GET_TASK_STA_SUCCESS    100


#define RESPONSE_SUCCESS  200
#define CREAT_SOCKET_FAIL 201
#define SOCKET_VAILD      202



#define ARGLEN_TOOLONG    301
#define ARGLEN_TYPE_ERR   302
#define ARG_NO_FOUND_PID  303
#define ARG_ERR           304
#define ARG_CMD_ERR       305



#define TASK_STOP_SUCCESS   400
#define TASK_IS_RUNNING     401
#define TASK_NOT_RUNNING    402
#define TASK_NOT_EXIST      403
#define TASK_NO_STOP_ACTION 404

//服务器的状态获取成功
#define GET_TASK_STA_SUCCESS 500


#define CLIENT_CREAT_SUCCESS        600
#define CLIENT_CREAT_TASK_NO_CREAT  601



#define CLIENT_SET_IP        700
#define CLIENT_START_FAIL    701


#define MSG_TO_SERVER    800


#define BAD_ERR               901
#define NO_FOUND_SERVERIP     902
#define EVENT_ALREADY_EXIST   903
#define PORT_NO_MATCH         904
#define SOCKET_ALREADY_EXIST  905
#define TCPRX_TASK_ERR        906




void Udp_to_ip(char *ip,int port,int timer);






#endif
