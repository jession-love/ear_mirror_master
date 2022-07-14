#ifndef _HUCHONG_H_
#define _HUCHONG_H_

typedef struct AXIS_INFO
{
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t reserve;
}MY_AXIS_INFO_HDR_t;

typedef struct tvideo_my_hdr_st
{
    char magic[2]; // [0]:0x55 [1]:0xaa
    uint8_t id; //frame id
    uint8_t is_eof; //最后一帧标识
    
    uint8_t pkt_cnt; //一帧分片序号
    uint8_t reserve[3]; //保留，默念置0
    
    uint32_t size; //UDP包长, 即：sizeof(MY_HDR_ST) + 分片的数据长
    MY_AXIS_INFO_HDR_t axis_info;
}MY_HDR_ST, *MY_HDR_PTR;


//私有协议命令
typedef enum
{
    DEV_INFO_REQ = 0,
    DEV_INFO_RSP,
}MY_CMD_e;


typedef struct DEV_INFO
{
    uint32_t ver; //版本号
    uint8_t vbat; //电量
    uint8_t reserve[3]; //保留
    char ssid[32]; //设备热点名
}MY_DEV_INFO_t;
#endif
