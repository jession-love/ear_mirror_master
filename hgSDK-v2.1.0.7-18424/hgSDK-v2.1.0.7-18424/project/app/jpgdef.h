#ifndef __JPGDEF_H
#define __JPGDEF_H
#if FRAME_MANAGEMENT  == 1
#include "frame_memt.h"
extern app_analyze uvc_app_analyze;
extern app_analyze g_app_analyze;;
#endif
#include "sys_config.h"




//框架的时候,
#if FRAME_MANAGEMENT  == 1
//psram的时候设置为1,无psram的时候,设置0
	#ifndef JPG_HEAD_RESERVER
	#define JPG_HEAD_RESERVER  24       //每段JPEG BUF前面预留用于填充其他数据的数据量，无需预留填0
	#endif
	#ifndef JPG_BUF_LEN
	#define JPG_BUF_LEN        4096+24//2880//4096+24    // 1*1024// 1*1024//       JPG buf长度+reserver长度		  1308
	#endif
	#ifndef JPG_NODE
	#define JPG_NODE		   30*2//18//30*2
	#endif
	#ifndef JPG_TAIL_RESERVER
	#define JPG_TAIL_RESERVER  0
	#endif

	
	#define UVC_PSRAM 1
	#define UVC_HEAD_RESERVER  24       //每段JPEG BUF前面预留用于填充其他数据的数据量，无需预留填0
	#define	GET_JPG_buf(f)		get_app_buf_adr(f)
	#define	GET_JPG_LEN(f)		get_app_buf_len(f)
	//节点长度,现在节点长度与实际buf内容长度是一致的(由框架决定)
	#define	GET_NODE_LEN(f)		get_app_buf_len(f)
	//框架没有单独节点释放接口
	#define FREE_JPG_NODE(f)	frame_memt_err(f)	



	
//无框架
#else


	#if JPG_EN == 1
		extern int get_jpg_node_len();
		#define	GET_NODE_LEN(f)		get_jpg_node_len(f)
		#define	GET_JPG_LEN(f)		get_jpeg_len(f)
		#define	GET_JPG_buf(f)		get_jpeg_first_buf(f)
		#define	GET_JPG_FRAME()		get_frame()
		#define FREE_JPG_NODE(f)	del_jpeg_first_node(f)
		#define DEL_JPG_FRAME(f)	del_jpeg_frame(f)

		#ifndef JPG_HEAD_RESERVER
		#define JPG_HEAD_RESERVER  24       //每段JPEG BUF前面预留用于填充其他数据的数据量，无需预留填0
		#endif
		#ifndef JPG_TAIL_RESERVER
		#define JPG_TAIL_RESERVER  8 
		#endif
		#ifndef JPG_BUF_LEN
		#define JPG_BUF_LEN        1308    // 1*1024// 1*1024//       JPG buf长度+reserver长度		  1308
		#endif
		#ifndef JPG_NODE
		#define JPG_NODE		   40
		#endif

	#elif USB_EN == 1
		extern int get_node_uvc_len();
		#define UVC_HEAD_RESERVER  24       //每段JPEG BUF前面预留用于填充其他数据的数据量，无需预留填0
		#define UVC_PSRAM 0
		#define	GET_NODE_LEN(f)		get_node_uvc_len(f)
		#define	GET_JPG_LEN(f)		get_uvc_frame_len(f)
		#define	GET_JPG_buf(f)		get_uvc_buf()
		#define	GET_JPG_FRAME()		get_uvc_message_gloal()
		#define FREE_JPG_NODE(f)	free_uvc_blank_nopsram(f)
		#define DEL_JPG_FRAME(f)	uvc_sema_up()

	#endif

#endif








#if FRAME_MANAGEMENT  == 1
typedef struct {
	struct list_head* (*get_frame)(app_frame_management* afmemt); 
	void (*del_frame)(struct list_head *d,app_frame_management* afmemt);
	int (*write)(void *fp,void *d,int flen);
}rtp_user_action;        //jpg的私有行为

#endif
void *get_jpeg_first_buf(void *d);
void del_jpeg_first_node(void *d);
void del_jpeg_frame(void *d);
struct list_head* get_frame();
uint32 get_jpeg_len(void *d);

#endif
