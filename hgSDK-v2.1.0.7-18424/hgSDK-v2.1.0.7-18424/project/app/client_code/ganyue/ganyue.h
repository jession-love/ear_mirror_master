#ifndef _ALK_H_
#define _ALK_H_

typedef struct
{     
	uint8_t frame_num;
	uint8_t is_eof;
	uint8_t pkt_cnt;
	uint8_t size_H;
	uint8_t size_L;
	uint8_t angle_H;
	uint8_t angle_L;
	uint8_t power;
}alk_net_msg;





#endif
