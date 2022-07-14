#ifndef __CMD_INPUT_H
#define __CMD_INPUT_H

#define CMD_CHECK 1
#define CMD_CHECK_PASSWORD	0xabcd

typedef enum 

{
	CMD_INPUT_DEFAULT = 0,
	CMD_INPUT_SYSTEM,
	CMD_INPUT_XMODEM,
	CMD_INPUT_USER,
	CMD_INPUT_MAX,
	
}CMD_INPUT_ENUM;

typedef int (*cmd_uart_handle_func)(const char *buf,unsigned int len);


struct cmd_list_head
{
	struct list_head list; 
	int count;
	void *freebuf;
};

struct cmd_list_struct
{
	struct list_head list; 
	unsigned int len;
	unsigned char *buf;
	unsigned int rx_len;
	#if CMD_CHECK == 1
	unsigned int magic;
	#endif
};

int cmd_init(void *cmd_uart,unsigned int num,unsigned int len);
struct cmd_list_struct *get_cmd_list();

void free_cmd_list(struct cmd_list_struct *free_list);
int cmd_uart_handle_register(cmd_uart_handle_func func,int index);
int cmd_uart_handle_unregister(int index);






#endif
