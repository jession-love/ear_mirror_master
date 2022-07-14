#include "sys_config.h"
#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "devid.h"
#include "osal/string.h"
#include "osal/irq.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/task.h"

#include "hal/usb_device.h"
#include "hal/gpio.h"

#include "dev/usb/hgusb20_v1_dev_api.h"
#include "dev/usb/uvc_host.h"

#define HOST_EP0_BUF  p_dev->usb_ep0buf
#define LOAD_U16(p)  ((((uint8 *)p)[0]<<0) | (((uint8 *)p)[1]<<8)) 

uint8_t uvc_dat[1024];
uint8 uvc_set_addr = 2;
uint8 hub_set_addr = 11;
uint8 hub_insert = 0;



void delay_ms(uint32 n)
{
    uint32 sys_clk = sys_get_apbclk();//ll_cc_sys_clk_get();

    if (n) {
        delay_us(n*1000);
    }
}


//获取描述符
bool usb_host_get_descriptor(struct hgusb20_dev *p_dev, uint8 desc_type, uint8 desc_index, uint8 *p_buf, uint16 len)
{
    p_dev->usb_ctrl.cmd.rtype = 0x80;
    p_dev->usb_ctrl.cmd.request = 0x06;                 //Get Descriptor
    p_dev->usb_ctrl.cmd.value = ((uint16)desc_type << 8) | desc_index;
    p_dev->usb_ctrl.cmd.index = 0;
    p_dev->usb_ctrl.cmd.length = len;
    
    return usb_host_ep0_request(p_dev, p_buf);
}

//接收完剩余数据，发送空包
bool usb_host_get_descriptor_end_do(struct hgusb20_dev *p_dev)
{
    while (p_dev->ep0_ptr.rx_len >= p_dev->usb_host.ep0_pkt) {              //收完剩余数据
         hgusb20_host_ep0_rx(p_dev, p_dev->usb_ep0buf, 0);
     }
     if (!p_dev->usb_host.stall) {
         hgusb20_host_ep0_tx(p_dev, NULL, 0);                                           //数据包接收完毕，发送一个空包
     }
     if (p_dev->usb_ctrl.error) {
         return FALSE;
     }
     return TRUE;
}


//第一次尝试获取设备描述符
bool usb_host_get_device_descriptor_1_do(struct hgusb20_dev *p_dev)
{
    p_dev->usb_ctrl.error = 0;
    if (usb_host_get_descriptor(p_dev, DEVICE_DESCRIPTOR, 0, p_dev->usb_ep0buf, 64)) {
        p_dev->usb_host.ep0_pkt = p_dev->usb_ep0buf[7];                             //Max Pkt
        if (p_dev->usb_host.ep0_pkt != 8 && p_dev->usb_host.ep0_pkt != 16 &&
            p_dev->usb_host.ep0_pkt != 32 && p_dev->usb_host.ep0_pkt != 64) {
            p_dev->usb_ctrl.error = 1;
            return FALSE;
        }    
    } else {
        return FALSE;
    }

    return TRUE;
}

//跳过N个ep0的值
void usb_host_ep0buf_skip_do(struct hgusb20_dev *p_dev, uint8 offset)
{
    p_dev->ep0_ptr.offset += offset;
    while (p_dev->ep0_ptr.offset >= p_dev->ep0_ptr.rx_len && p_dev->ep0_ptr.rx_len >= p_dev->usb_host.ep0_pkt) {
        p_dev->ep0_ptr.offset -= p_dev->ep0_ptr.rx_len;
        hgusb20_host_ep0_rx(p_dev, p_dev->usb_ep0buf, 0);
        if (p_dev->ep0_ptr.rx_len == 0) {
            printf("ep0 buf skip err\n");
            //usb_sw->usb_ctrl.error = 1;
            return;
        }
    }

}

//从EP0_BUF中获取USB的相关信息
void usb_host_get_info(struct hgusb20_dev *p_dev)
{
    p_dev->usb_ctrl.info.vid = LOAD_U16(&HOST_EP0_BUF[p_dev->ep0_ptr.offset]);                //获取描述符中的相关信息
    p_dev->usb_ctrl.info.pid = LOAD_U16(&HOST_EP0_BUF[p_dev->ep0_ptr.offset + 2]);
    p_dev->usb_ctrl.info.ver = LOAD_U16(&HOST_EP0_BUF[p_dev->ep0_ptr.offset + 4]);
}


//第二次获取设备描述符中的VID PID等产品信息
bool usb_host_get_device_descriptor_2_do(struct hgusb20_dev *p_dev)
{
    if (usb_host_get_descriptor(p_dev, DEVICE_DESCRIPTOR, 0, p_dev->usb_ep0buf, 0x12)) {
        //获取USB的相关信息
        //usb_host_ep0buf_skip_do(p_dev, 8);
        p_dev->ep0_ptr.offset += 8;
        usb_host_get_info(p_dev);
        return TRUE;
    }
    return FALSE;
}

//第一次尝试获取配置描述符
bool usb_host_get_configuration_descriptor_1(struct hgusb20_dev *p_dev, uint8 cfg_index)
{
    return usb_host_get_descriptor(p_dev, CONFIGURATION_DESCRIPTOR, cfg_index, p_dev->usb_ep0buf, 9);       //第一次，尝试获取配置描述符
}

//第二次获取配置描述符的Bulk端点信息等
bool usb_host_get_configuration_descriptor_3(struct hgusb20_dev *p_dev, uint8 cfg_index, uint16 desc_len, uint8* buf)
{
    if (desc_len < 18) {
        printf("desc_len err\n");
        return FALSE;
    }
    return usb_host_get_descriptor(p_dev, CONFIGURATION_DESCRIPTOR, cfg_index, buf, desc_len);
}


//设置配置
bool usb_host_set_configuration(struct hgusb20_dev *p_dev, uint8 cfg_value)
{
    p_dev->usb_ctrl.cmd.rtype = 0x00;
    p_dev->usb_ctrl.cmd.request = 0x09;                 //Set Configuration
    p_dev->usb_ctrl.cmd.value = cfg_value;
    p_dev->usb_ctrl.cmd.index = 0;
    p_dev->usb_ctrl.cmd.length = 0;
    return usb_host_ep0_request(p_dev, NULL);
}


//设置接口
bool usb_host_set_interface(struct hgusb20_dev *p_dev, uint8 index, uint8 value)
{
    p_dev->usb_ctrl.cmd.rtype = 0x01;
    p_dev->usb_ctrl.cmd.request = 0x0b;                    //Set Interface
    p_dev->usb_ctrl.cmd.value = value;
    p_dev->usb_ctrl.cmd.index = index;
    p_dev->usb_ctrl.cmd.length = 0;
    return usb_host_ep0_request(p_dev, NULL);
}

//获取描述符
bool usb_host_get_cur(struct hgusb20_dev *p_dev, uint16 uid, uint16 cs, uint32 len, uint8 *pinf)
{
    p_dev->usb_ctrl.cmd.rtype = 0xa1;
    p_dev->usb_ctrl.cmd.request = UVC_GET_CUR;                 //Get Descriptor
    p_dev->usb_ctrl.cmd.value = cs;
    p_dev->usb_ctrl.cmd.index = uid;
    p_dev->usb_ctrl.cmd.length = len;


    return usb_host_ep0_request(p_dev, pinf);
}

//获取描述符
bool usb_host_set_cur(struct hgusb20_dev *p_dev, uint16 uid, uint16 cs, uint32 len, uint8 *pinf)
{
    p_dev->usb_ctrl.cmd.rtype = 0x21;
    p_dev->usb_ctrl.cmd.request = UVC_SET_CUR;                 //Get Descriptor
    p_dev->usb_ctrl.cmd.value = cs;
    p_dev->usb_ctrl.cmd.index = uid;
    p_dev->usb_ctrl.cmd.length = len;


    return usb_host_ep0_request(p_dev, pinf);
}

void usb_host_set_address(struct hgusb20_dev *p_dev, uint8 addr)
{
    os_sleep_ms(10);
    p_dev->usb_ctrl.cmd.rtype = 0x00;
    p_dev->usb_ctrl.cmd.request = 0x05;                 //Set Address
    p_dev->usb_ctrl.cmd.value = addr;
    p_dev->usb_ctrl.cmd.index = 0;
    p_dev->usb_ctrl.cmd.length = 0;
    usb_host_ep0_request(p_dev, NULL);
}




VIDEO_COMMIT commit;
UVC_SLEST uvc_select;
UVC_DES uvc_des;
volatile UVCDEV uvc;
volatile uint8 uvc_cur_indx = 0;
uint8 uvc_cfg_des[2014] = {0};



void analysis_uvc_desc(uint8 *desc,uint32 desclen)
{

    uint8 *temp_desc;
    uint8 is_cv = 0;
    static uint8 cun = 0;
    cun ++;
    printf("_____analysis_uvc_desc cun _____%d\r\n",cun);
    uint32 desc_len = desclen;
    temp_desc = desc;
    memset((uint8 *)&uvc_des,0,sizeof(UVC_DES));
    
    uvc_des.control_endpoint = 0xff;
    while(desc_len)
    {
        uvc_des.DesLen = temp_desc[0];
        uvc_des.DescriptorType = temp_desc[1];  
        
        if(uvc_des.DescriptorType == STRAME_O_CONTORL){
            is_cv=temp_desc[6];
            
        }
        
        if(is_cv == 1){//cv
                if(uvc_des.DescriptorType == INTERFACE_Type){
                    uvc_des.Subtype     = temp_desc[2];
                    //printf("CV_subtype :%d\n",uvc_des.Subtype);
                }
                if(uvc_des.DescriptorType == ENDPOINT_DESC && temp_desc[3]==0x03){
                    uvc_des.control_endpoint= temp_desc[2]&0x7f;
                    printf("control_endpoint :%d \n",uvc_des.control_endpoint);
                }
        }
        if(is_cv == 2){//SV
            if(uvc_des.DescriptorType == INTERFACE_Type){
                    uvc_des.Subtype     = temp_desc[2];
                    if(uvc_des.Subtype ==VS_INPUT_HEADER ){
                        memcpy(&uvc_des.vs_head, temp_desc, sizeof(VS_HEAD));
                    }
                    else if(uvc_des.Subtype ==VS_FORMAT_UNCOMPRESSED ||uvc_des.Subtype ==VS_FORMAT_MJPEG  ){
                        memcpy(&uvc_des.vs_format[uvc_des.vs_format_num++], temp_desc, sizeof(VS_FORMAT));
                    }/*
                    else if(uvc_des.Subtype ==VS_FRAME_UNCOMPRESSED || uvc_des.Subtype ==VS_FRAME_MJPEG ){
                        my_memcpy(&uvc_des.vs_frame[uvc_des.vs_frame_num], temp_desc, sizeof(VS_FRAME));
                        uvc_des.vs_frame[uvc_des.vs_frame_num].wWidth = temp_desc[6]<<8|temp_desc[5];
                        uvc_des.vs_frame[uvc_des.vs_frame_num].wHeight= temp_desc[8]<<8|temp_desc[7];
                        uvc_des.vs_frame_num++;
                    }*/
                    else if( uvc_des.Subtype ==VS_FRAME_MJPEG ){
                        memcpy(&uvc_des.vs_frame[uvc_des.vs_frame_num], temp_desc, sizeof(VS_FRAME));
                        uvc_des.vs_frame[uvc_des.vs_frame_num].wWidth = temp_desc[6]<<8|temp_desc[5];
                        uvc_des.vs_frame[uvc_des.vs_frame_num].wHeight= temp_desc[8]<<8|temp_desc[7];
                        uvc_des.vs_frame_num++;
                    }
                }
            if(uvc_des.DescriptorType == STRAME_O_CONTORL && temp_desc[0]==sizeof(SINTF_DESC)){
                memcpy(&uvc_des.intfs[uvc_des.intfs_count++],temp_desc,temp_desc[0]);
                if(uvc_des.intfs_count>= __INTFS_STACK__)uvc_des.intfs_count =0;
            }
            if(uvc_des.DescriptorType == ENDPOINT_DESC && temp_desc[0]==(sizeof(SEP_DESC)-1)){
                memcpy(&uvc_des.edpt[uvc_des.edpt_count++],temp_desc,temp_desc[0]);
                if(uvc_des.edpt_count>= __INTFS_STACK__)uvc_des.edpt_count =0;
            }
        }
        
        temp_desc +=uvc_des.DesLen ;
        desc_len -= uvc_des.DesLen ;
    }
    /*
    uint32 ix,id;
    uint8 *debg_buff;
    for(ix=0;ix<uvc_des.intfs_count;ix++){
        debg_buff = (uint8 *)&uvc_des.intfs[ix];
        for(id=0;id<9;id++){
            printf(" %x",debg_buff[id]);
        }
        printf("\n***********************\n");
    }
    for(ix=0;ix<uvc_des.edpt_count;ix++){
        debg_buff = (uint8 *)&uvc_des.edpt[ix];
        for(id=0;id<7;id++){
            printf(" %x",debg_buff[id]);
        }
        printf("\n***********************\n");
    }
    
    printf("intfs num %d,edpt_num %d \n",uvc_des.intfs_count,uvc_des.edpt_count);
    */
}

void printf_uvc_vsdesc(UVC_DES *ud)
{
    uint32 i;
    printf("=======VS_HEADER============\r\n");
    printf("bLength:                           %d\r\n",ud->vs_head.bLength );
    printf("bDescriptorType:                   %d\r\n",ud->vs_head.bDescriptorType );
    printf("bDescriptorSubtype:                %d\r\n",ud->vs_head.bDescriptorSubtype );
    printf(" bNumFormats:                      %d\r\n",ud->vs_head.bNumFormats );
    printf(" \r\n__________________________\r\n");
    printf("=======VS_FORMAT============\r\n");
    for(i = 0;i< ud->vs_format_num;i++){
            printf(" bLength:                          %d\r\n",ud->vs_format[i].bLength );
            printf(" bDescriptorType:                  %d\r\n",ud->vs_format[i].bDescriptorType );
            printf(" bDescriptorSubtype:               %d\r\n",ud->vs_format[i].bDescriptorSubtype );
            printf(" bFrameIndex:                      %d\r\n",ud->vs_format[i].bFrameIndex );
            printf(" bNumFrameDescriptors:             %d\r\n",ud->vs_format[i].bNumFrameDescriptors );
            printf(" \r\n__________________________\r\n");
    }
    printf("=======VS_FRAME============\r\n");
    for(i = 0;i< ud->vs_frame_num;i++){
            printf("    bLength:                         %d\r\n",ud->vs_frame[i].bLength );
            printf("    bDescriptorType:                 %d\r\n",ud->vs_frame[i].bDescriptorType );
            printf("    bDescriptorSubtype:              %d\r\n",ud->vs_frame[i].bDescriptorSubtype );
            printf("    bFrameIndex:                     %d\r\n",ud->vs_frame[i].bFrameIndex );
            printf("    bmCapabilities:                  %d\r\n",ud->vs_frame[i].bmCapabilities );
            printf("    wWidth:                          %d\r\n",ud->vs_frame[i].wWidth );
            printf("    wHeight:                         %d\r\n",ud->vs_frame[i].wHeight );
            printf(" \r\n__________________________\r\n");
    } 
}



void select_uvc_intfs(UVC_DES *ud,UVCDEV *uvc)
{   uint32 i;
    uint32 temp = 0;
    uint32 intfs_indx=0;
    uint32 edpt_indx=0;
    for(i =0;i<ud->intfs_count;i++)
    {
        if(ud->intfs[i].alt_tring > temp && ud->intfs[i].iclass == UVC_CLASS)
            intfs_indx = i;
        temp = ud->intfs[i].alt_tring;
    }
    temp =0;
    for(i =0;i<ud->edpt_count;i++)
    {
        //printf("temp%d plaload%d\n",temp,ud->edpt[i].pay_load);
        if(ud->edpt[i].pay_load > temp)
            edpt_indx = i;
        temp = ud->edpt[i].pay_load;
    }
//  ud->intfs_count
//  ud->edpt_count
    printf("intfs indx :%d edpt_indx %d \r\n",intfs_indx,edpt_indx);
    uvc->epstrm = ud->edpt[edpt_indx].ep_addr;
    uvc->strm_pload= ud->edpt[edpt_indx].pay_load;
    uvc->strm_interval = ud->edpt[edpt_indx].interval;

    uvc->ctyp = ud->intfs[intfs_indx].iclass;
    uvc->strm_intfs=ud->intfs[intfs_indx].num;
    uvc->strm_altset=ud->intfs[intfs_indx].alt_tring;
}


int uvc_resolution2indx(uint32 width,uint32 height)
{
   uint32 i = 0;
   for(i = 0;i < uvc_des.vs_frame_num;i++)
   {
       if(uvc_des.vs_frame[i].wWidth == width && uvc_des.vs_frame[i].wHeight == height)
           return uvc_des.vs_frame[i].bFrameIndex;
    }
    
    return -1;
}

static uint8 select_uvc_format(UVC_DES *ud,UVC_SLEST *us,uint8 index)
{
    uint32 i;
    uint8 format_type;
    //slect vs format
        memset((uint8 *)us,0,sizeof(UVC_SLEST));
        if(IS_UVC_JPEG){
            format_type = VS_FORMAT_MJPEG;  
        }
        else{
            format_type = VS_FORMAT_UNCOMPRESSED;
        }
        for(i=0;i<ud->vs_format_num;i++){
            if(ud->vs_format[i].bDescriptorSubtype ==  format_type){
                us->vs_format =     ud->vs_format[i];
                break;
            }
        }
        if(us->vs_format.bFrameIndex == 0){
            printf("can't suppost UVCFORMAT\r\n");
            return FALSE;
        }

     for(i=0;i<ud->vs_frame_num;i++){
        if(ud->vs_frame[i].bFrameIndex == index)
            us->vs_frame =  ud->vs_frame[i];
     }
    printf("FORMAT INDX %d\r\n",us->vs_format.bFrameIndex);
    printf("FRAME  INDX %d\r\n",us->vs_frame.bFrameIndex);
    return 1;
}

void select_resolution(uint8 *p,UVC_SLEST *su)
{
    p[2] = su->vs_format.bFrameIndex;
    p[3] = su->vs_frame.bFrameIndex;
    commit.bmhint = (p[1] << 8) + p[0];
    commit.bFormatIndex = p[2];
    commit.bFrameIndex = p[3];
    commit.dwFrameInterval = (p[7] << 24) +(p[6] << 16) +(p[5] << 8) +(p[4] << 0) ;
    commit.wKeyFrameRate = (p[9] << 8) + p[8];
    commit.wPFrameRate = (p[11] << 8) + p[10];
    commit.wCompQuality = (p[13] << 8) + p[12];
    commit.wCompWindowSize = (p[15] << 8) + p[14];
    commit.wDelay = (p[17] << 8) + p[16];
    commit.dwMaxVideoFrameSize=(p[21] << 24) +(p[20] << 16) +(p[19] << 8) +(p[18] << 0);
    commit.dwMaxPayloadTSize = (p[25] << 24) +(p[24] << 16) +(p[23] << 8) +(p[22] << 0);
    
    printf(".bmhint:%x\r\n",        commit.bmhint);
    printf(".bFormatIndex:%x\r\n",  commit.bFormatIndex);
    printf(".bFrameIndex:%x\r\n",   commit.bFrameIndex);
    printf(".dwFrameInterval:%x\r\n",commit.dwFrameInterval);
    printf(".wKeyFrameRate:%x\r\n", commit.wKeyFrameRate);
    printf(".wPFrameRate:%x\r\n",   commit.wPFrameRate);
    printf(".wCompQuality:%x\r\n",  commit.wCompQuality);
    printf(".wCompWindowSize:%x\r\n",commit.wCompWindowSize);
    printf(".wDelay:%x\r\n",        commit.wDelay);
    printf(".dwMaxVideoFrameSize:%x\r\n",commit.dwMaxVideoFrameSize);
    printf(".dwMaxPayloadTSize:%x\r\n",commit.dwMaxPayloadTSize);   
}

void cnvr(uint8 *p)
{
    commit.bmhint = (p[1] << 8) + p[0];
    commit.bFormatIndex = p[2];
    commit.bFrameIndex = p[3];
    commit.dwFrameInterval = (p[7] << 24) +(p[6] << 16) +(p[5] << 8) +(p[4] << 0) ;
    commit.wKeyFrameRate = (p[9] << 8) + p[8];
    commit.wPFrameRate = (p[11] << 8) + p[10];
    commit.wCompQuality = (p[13] << 8) + p[12];
    commit.wCompWindowSize = (p[15] << 8) + p[14];
    commit.wDelay = (p[17] << 8) + p[16];
    commit.dwMaxVideoFrameSize=(p[21] << 24) +(p[20] << 16) +(p[19] << 8) +(p[18] << 0) ;
    commit.dwMaxPayloadTSize = (p[25] << 24) +(p[24] << 16) +(p[23] << 8) +(p[22] << 0) ;
}




uint8 select_cur(struct hgusb20_dev *p_dev,uint8 index)
{
    int itk = 0;
    uint8 pcommit[26];
    os_sleep_ms(50);

    if(!select_uvc_format(&uvc_des,&uvc_select,index))
    {
        return 1;//error
    }
    
    uvc_cur_indx = index;  
    usb_host_set_interface(p_dev,uvc.strm_intfs ,0);
        
    
    if (!usb_host_get_cur(p_dev,1,0x0100,0x1a,pcommit)) {   //uvc_probe_control
        return FALSE;
    }
        
    select_resolution(pcommit,&uvc_select);

    if (!usb_host_set_cur(p_dev,1,0x0100,0x1a,pcommit)) {   //uvc_probe_control
        return FALSE;
    }

    if (!usb_host_get_cur(p_dev,1,0x0100,0x1a,pcommit)) {   //uvc_probe_control
        return FALSE;
    }
        
    cnvr(pcommit);
    printf("select cur finish\r\n");
    for(itk = 0;itk < 26;itk++){
        printf("%x ",pcommit[itk]);
    }
    printf("\r\n");

    if (!usb_host_set_cur(p_dev,1,0x0100,0x1a,pcommit)) {   //uvc_probe_control
        return FALSE;
    }
    if (!usb_host_get_cur(p_dev,1,0x0100,0x1a,pcommit)) {   //uvc_probe_control
        return FALSE;
    }
    cnvr(pcommit);
    printf("select cur finish2\r\n");
    for(itk = 0;itk < 26;itk++){
        printf("%x ",pcommit[itk]);
    }
    printf("\r\n");

    if (!usb_host_set_cur(p_dev,1,0x0200,0x1a,pcommit)) {
        return FALSE;
    }

    //uvc on
    //delay_5ms(50);
    os_sleep_ms(250);
    usb_host_set_interface(p_dev,uvc.strm_intfs ,uvc.strm_altset);

    return FALSE;
}

void uvc_stream_open(struct hgusb20_dev *p_dev,uint8 enable){
    if(enable){
        usb_host_set_interface(p_dev,uvc.strm_intfs ,uvc.strm_altset);
    }else{
        usb_host_set_interface(p_dev,uvc.strm_intfs ,0);
    }
}

uint8 uvc_dpi = 0;
bool enum_set_resolution(struct hgusb20_dev *p_dev,uint8 resolution)
{
    int indx = 0;
    if(resolution == 1)//VGA
    {
        indx=uvc_resolution2indx(640,480);
        uvc_dpi = 1;
    }
    
    if(resolution == 2){//720P
        indx=uvc_resolution2indx(1280,720);
        uvc_dpi = 2;
        if(indx == -1){
            indx=uvc_resolution2indx(640,480);
            uvc_dpi = 1;
            printf("uvc can not set resolution :720p\r\n");
        }
    }
    return select_cur(p_dev,indx);
}


void uvc_ep_init(struct hgusb20_dev *p_dev)
{
    hgusb20_host_rx_ep_init(p_dev, ISO_EP, uvc.epstrm, USB_ENDPOINT_XFER_ISOC, 1024);
    //hgusb20_set_address(p_dev, uvc_set_addr);
    hgusb20_ep_rx_kick(p_dev, ISO_EP, (uint32)uvc_dat, 0x400);
}

//usb host枚举成功后会执行的一个函数,默认为空,使用弱引用,返回是一个分辨率的值
__attribute__((weak)) void usb_host_enum_finish_init()
{
}

__attribute__((weak)) int uvc_default_dpi()
{
    return UVC_720P;
}


uint8 usb_host_ep0_enum_uvc(struct hgusb20_dev *p_dev)
{
    uint8 ret;
    uint8 addr = 0;
    uint16 desc_len;
    int dpi = UVC_720P;
    addr = uvc_set_addr;
    irq_disable(USB20MC_IRQn);
    usb_host_set_address(p_dev, addr);                              //设置设备地址
    hgusb20_set_address(p_dev, addr);
    os_sleep_ms(10);
    
    if (!usb_host_get_device_descriptor_2_do(p_dev)) {          //第二次获取失败，一般是设地址错误
        printf("SetAddr Err\r\n");
        goto __enum_err;
    }

    if (!usb_host_get_configuration_descriptor_1(p_dev, 0)) {
        goto __enum_err;
    }
    desc_len = (HOST_EP0_BUF[3] << 8)+ HOST_EP0_BUF[2];
    if(desc_len == 0){
        printf("< configuration dec len er>\n");
        goto __enum_err;
    }   
    if (!usb_host_get_configuration_descriptor_3(p_dev, 0, desc_len, uvc_cfg_des)) {
        printf("GetCfg Err\r\n");
        goto __enum_err;
    }
    printf("desc_len:%d\r\n",desc_len);
    analysis_uvc_desc(uvc_cfg_des,desc_len);
    printf_uvc_vsdesc(&uvc_des);
    
    select_uvc_intfs(&uvc_des,(UVCDEV *)&uvc);
    usb_host_set_configuration(p_dev, 1);       //设置配置
    
    os_sleep_ms(10);
    if (uvc.ctyp == UVC_CLASS) {
        printf("strm_intfs:%x, epstrm:%x, strm_pload: %x, strm_altset:%x \r\n",uvc.strm_intfs ,uvc.epstrm,uvc.strm_pload, uvc.strm_altset);
        dpi = uvc_default_dpi();
        ret = enum_set_resolution(p_dev,dpi);
        if(ret != 0){
            goto __enum_err;
        } 
        usb_host_enum_finish_init();
        uvc_room_init();
        printf(" uvc.epstrm %x,uvc.strm_pload %x,uvc.strm_interval %x\r\n",uvc.epstrm&0x7f,uvc.strm_pload,uvc.strm_interval);

        uvc_ep_init(p_dev);
        
        //if(uvc_host_init(uvc_addr_get(), uvc.epstrm,-1,uvc.strm_pload,uvc.strm_interval))
        {
    //#if EN_UVC_TAKE_PHOTO
    //      uvc_actech_as_control();
    //#endif
    //      uvc_actech_as_usensor();
        }
    } 
    
    if (p_dev->usb_ctrl.error) {
        printf("Enum ERR\r\n");
        goto __enum_err;
    }
    irq_enable(USB20MC_IRQn);
    printf("Enum OK\r\n");
    return TRUE;

__enum_err:
    irq_enable(USB20MC_IRQn);
    return FALSE;
}





uint8 hub_des[2014] = {0};
HUB_manage hub_manage;


uint8 get_int_enpoint(uint8 *desc)
{
    uint8 *temp = desc;
    uint32 temp_len =desc[0];
    while(temp_len){
        temp_len =temp[0];
        if(temp[0] == 0x7 && temp[1] == 0x5&&temp[3] == 0x3)
            return (temp[2]&0x7f);
        else{ 
            temp+=temp_len;
        }
    }
    return FALSE;
}

uint8 get_hub_port_num(uint8 *desc)
{
    
    uint8 *temp=desc;
    //uint32 len = desc[0];
    if(temp[0] == 0x9 && temp[1] == 0x29)
        return temp[2];
    else
        return FALSE;   
}


bool usb_hub_get_status(struct hgusb20_dev *p_dev,uint8 *pinf)
{
	bool ret;
    p_dev->usb_ctrl.cmd.rtype = 0xa0;
    p_dev->usb_ctrl.cmd.request = 0;                 //Get Descriptor
    p_dev->usb_ctrl.cmd.value = 0;
    p_dev->usb_ctrl.cmd.index = 0;
    p_dev->usb_ctrl.cmd.length = 4;
    ret = usb_host_ep0_request(p_dev, pinf);
    return ret;
}

bool usb_hub_set_port_feature(struct hgusb20_dev *p_dev,uint8 port,uint8 feature)
{
	bool ret;
    p_dev->usb_ctrl.cmd.rtype = 0x23;
    p_dev->usb_ctrl.cmd.request = 0x03;                 //Get Descriptor
    p_dev->usb_ctrl.cmd.value = feature;
    p_dev->usb_ctrl.cmd.index = port;
    p_dev->usb_ctrl.cmd.length = 0;
    p_dev->usb_ctrl.error = 0;
    ret = usb_host_ep0_request(p_dev, NULL);
    return ret;
}

bool usb_host_hub_get_descriptor(struct hgusb20_dev *p_dev, uint16 len)
{
	bool ret;
    p_dev->usb_ctrl.cmd.rtype = 0xa0;
    p_dev->usb_ctrl.cmd.request = 0x06;                 //Get Descriptor
    p_dev->usb_ctrl.cmd.value = 0;
    p_dev->usb_ctrl.cmd.index = 0;
    p_dev->usb_ctrl.cmd.length = len;


    ret = usb_host_ep0_request(p_dev, p_dev->usb_ep0buf);
	return ret;

}

bool usb_host_get_hub_descriptor(struct hgusb20_dev *p_dev,uint8* buf)
{
	bool ret;
    ret = usb_host_hub_get_descriptor(p_dev, 64);         //第一次，尝试获取设备描述符
    //p_dev->usb_host.ep0_pkt = p_dev->usb_ep0buf[7];                             //Max Pkt
    memcpy(buf,p_dev->usb_ep0buf,64);   
    return ret;
}


void usb_host_hub_init_do(struct hgusb20_dev *p_dev)
{    
    hgusb20_host_rx_ep_init(p_dev, HUB_EP, hub_manage.int_endpoint, USB_ENDPOINT_XFER_BULK, 4);
    hgusb20_set_address(p_dev, hub_set_addr);
    hgusb20_ep_rx_kick(p_dev, HUB_EP, (uint32)uvc_dat, 0x4);
}

bool usb_hub_get_port_status(struct hgusb20_dev *p_dev,uint8 *desc,uint8 port)
{
	bool ret;
    p_dev->usb_ctrl.cmd.rtype = 0xa3;
    p_dev->usb_ctrl.cmd.request = 0;                 //Get Descriptor
    p_dev->usb_ctrl.cmd.value = 0;
    p_dev->usb_ctrl.cmd.index = port;
    p_dev->usb_ctrl.cmd.length = 4;
    p_dev->usb_ctrl.error = 0;
	p_dev->usb_host.ep0_pkt = 255;
    ret = usb_host_ep0_request(p_dev, desc);
    return ret;
}


void hub_ep_init(struct hgusb20_dev *p_dev)
{
    usb_host_hub_init_do(p_dev);            //ep init
    //hub_isr_init(p_dev);
    irq_enable(USB20MC_IRQn);
    irq_enable(USB20DMA_IRQn);
}



uint8 usb_host_ep0_enum_hub(struct hgusb20_dev *p_dev)
{
    uint8 ret;
    int i;
    uint8 addr = 0;
    uint16 desc_len;

    uint8 rst = hgusb20_host_reset(p_dev);
    if (rst) {
        return rst;
    }
    rst = hgusb20_host_reset(p_dev);
    if (rst) { 
        return rst;
    }

    if (!usb_host_get_device_descriptor_2_do(p_dev)) {
        printf("GetDesc Err\r\n");
        return FALSE;
    }

    if (hgusb20_host_reset(p_dev)) {
        return FALSE;
    }
    if (p_dev->usb_ctrl.bus_high_speed) {
        printf("high_speed device plug in\r\n");
    } else {
        printf("full_speed device plug in\r\n");
    }    
    addr = hub_set_addr;
    //hub_set_addr++;
    //uvc_set_addr = 4;
    //addr = uvc_set_addr;
    usb_host_set_address(p_dev, hub_set_addr);                              //设置设备地址
    hgusb20_set_address(p_dev, addr);
    os_sleep_ms(10);

    if (!usb_host_get_device_descriptor_2_do(p_dev)) {          //第二次获取失败，一般是设地址错误
        printf("SetAddr Err\r\n");
        return FALSE;
    }
    //usb_host_get_device_end(usb_sw);                     //获取设备描述符后留接口
    if (!usb_host_get_configuration_descriptor_1(p_dev, 0)) {
        return FALSE;
    }
    desc_len = (HOST_EP0_BUF[3] << 8)+ HOST_EP0_BUF[2];
    if(desc_len == 0){
        printf("< configuration dec len er>\n");
        return FALSE;   
    }   
    if (!usb_host_get_configuration_descriptor_3(p_dev, 0, desc_len, hub_des)) {
        printf("GetCfg Err\r\n");
        return FALSE;
    }
    printf("desc_len:%d\r\n",desc_len);
    hub_manage.int_endpoint = get_int_enpoint(hub_des);
    usb_host_set_configuration(p_dev, 1);        //设置配置
    
    os_sleep_ms(10);
    ret = usb_host_get_hub_descriptor(p_dev,hub_des);
    if(ret != 1){
        printf("hub_descriptor err\r\n");
        return ret;
    }   
    os_sleep_ms(100);
    hub_manage.port_num = get_hub_port_num(hub_des);
    p_dev->usb_ctrl.error = 0;
    ret = usb_hub_get_status(p_dev,hub_des);
    if(ret != 1){
        printf("get status err\r\n");
        return ret;
    }
    printf("hub_des:%d port_num:%d  int_endpoint:%d\r\n",hub_des[0],hub_manage.port_num,hub_manage.int_endpoint);
    if(hub_des[0]==0)
    {
        for(i=1;i<hub_manage.port_num+1;i++){
            ret=usb_hub_set_port_feature(p_dev,i,0x08);
            if(ret != 1) return ret;
        //  printf("set port feature :%d \n",i);
        }
    }   

    for(i=1;i<hub_manage.port_num+1;i++){
        ret = usb_hub_get_port_status(p_dev,hub_des,i);
		if(ret == FALSE){
			return FALSE;
		}
			
        //printf("%x %x %x %x\n",desc[0],desc[1],desc[2],desc[3]);
        //printf("\n");
    }   
    printf("hub_enum_finish\r\n");
    
    printf("uvc_dat[0]:%x\r\n",uvc_dat[0]);
    hub_ep_init(p_dev);
    return TRUE;
}



bool usb_host_enum1(struct hgusb20_dev *p_dev)
{
    uint8 try_cnt = 3;
    uint8 res;
    do {
		
        if (!hgusb20_is_device_online(p_dev)) {
            printf("host not online Err\r\n");
            break;
        }

        res = hgusb20_host_reset(p_dev);
        if (res) {
            return res;
        }
        res = hgusb20_host_reset(p_dev);
        if (res) { 
            return res;
        }
        if (!usb_host_get_device_descriptor_1_do(p_dev)) {
            printf("GetDesc Err\r\n");
            return FALSE;
        }
        
        if (hgusb20_host_reset(p_dev)) {
            return FALSE;
        }
        if (p_dev->usb_ctrl.bus_high_speed) {
            printf("high_speed device plug in\r\n");
        } else {
            printf("full_speed device plug in\r\n");
        }    

        //usb_host_init(usb_sw);
        res = usb_host_ep0_enum_uvc(p_dev);
        if (res == 0) {
            //hgusb20_reset_phy(p_dev);
            continue;
        } else if (res == 2) {
            break;
        }
        return TRUE;
    } while (--try_cnt);

    return FALSE;
}


uint8 hub_ep0_clear_port_feature(struct hgusb20_dev *p_dev, uint8 port,uint8 feature)
{
    p_dev->usb_ctrl.cmd.rtype = 0x23;
    p_dev->usb_ctrl.cmd.request = 0x01;                 //Set Configuration
    p_dev->usb_ctrl.cmd.value = feature;
    p_dev->usb_ctrl.cmd.index = port;
    p_dev->usb_ctrl.cmd.length = 0;	
	p_dev->usb_host.ep0_pkt = 255;
    usb_host_ep0_request(p_dev, NULL);
    os_sleep_ms(10);
    return FALSE;
}

uint8 hub_ep0_set_port_feature(struct hgusb20_dev *p_dev, uint8 port,uint8 feature)
{
    p_dev->usb_ctrl.cmd.rtype = 0x23;
    p_dev->usb_ctrl.cmd.request = 0x03;              //Set Configuration
    p_dev->usb_ctrl.cmd.value = feature;
    p_dev->usb_ctrl.cmd.index = port;
    p_dev->usb_ctrl.cmd.length = 0;
	p_dev->usb_host.ep0_pkt = 255;
    usb_host_ep0_request(p_dev, NULL);
    return FALSE;
}

uint8 hub_wait_port_connect(struct hgusb20_dev *p_dev,uint8 port)
{
    uint8 buff[10];
    uint8 ret;
    do{
        ret = usb_hub_get_port_status(p_dev,buff,port);
		if(ret == FALSE){
			return FALSE;
		}		
        os_sleep_ms(1);    
    }while(!(buff[0] & HUB_STATU_CONNECT) );
	return TRUE;
}

uint8 hub_wait_port_restend(struct hgusb20_dev *p_dev,uint8 port)
{
    uint8 buff[10];
	uint8 ret;
    ret = usb_hub_get_port_status(p_dev,buff,port);
	if(ret == FALSE){
		return FALSE;
	}	
    do{
        ret = usb_hub_get_port_status(p_dev,buff,port);
		if(ret == FALSE){
			return FALSE;
		}			
        os_sleep_ms(1);
    }while((buff[3]&HUB_STATU_RESTEND) != 0);
	return TRUE;
}

uint8 clean_hub_port(struct hgusb20_dev *p_dev,uint8 port)
{
    
    uint8 ret = 0;

//  u_addr_set(5);
//  if(ret != 0)return ret;
    ret =hub_ep0_clear_port_feature(p_dev,port,PORT_CONNECT);

    if(ret != 0)return ret;

    ret = hub_ep0_set_port_feature(p_dev,port,PORT_REST);
    if(ret != 0)return ret;
    
    ret = hub_wait_port_connect(p_dev,port);  
	if(ret == FALSE) return FALSE;

    ret = hub_ep0_clear_port_feature(p_dev,port,CLEAR_PORT_REST);
    if(ret != 0)return ret;

    ret = hub_wait_port_restend(p_dev,port);
    return ret;

}



int usb_hub_isr_handl(struct hgusb20_dev *p_dev)
{
    uint8 hub_port = 0 ;
    uint8 ret;
    
    uint32 rx_len;
    rx_len = hgusb20_ep_get_sie_rx_len(p_dev, HUB_EP);
    printf("uvc_dat[0]:%x  %d\r\n",uvc_dat[0],rx_len);
    if( rx_len > 0 ){
        for(hub_port = 1;hub_port<8;hub_port++ )
        {   
            if(uvc_dat[0]&BIT(hub_port))
                break;
        }
        
        printf("hub port %d connect  \n",hub_port);
        hub_manage.connect_port =hub_port;
        ret = clean_hub_port(p_dev,hub_manage.connect_port);
		printf("hub ret %d \n",ret);
		if(ret == FALSE)
    		return 1;
		else 
			return 0;
    }
    else{
        return 1;
    }
    if(ret!=0){     
        return 1;
    }
    return 0;
}


extern volatile uint8 hub_interrupt;



uint8 hub_enum_error = 0;
void usb_hub_device_insert(struct hgusb20_dev *p_dev)
{
	uint8 ret;
	while(1){
		
		os_sema_down(&p_dev->hub_sema, osWaitForever);
		if (usb_hub_isr_handl(p_dev) == 0) { 
			hgusb20_set_address(p_dev, 0);
			if (!usb_host_get_device_descriptor_1_do(p_dev)) {
				printf("GetDesc hub Err\r\n");
				hub_enum_error = 1;
				os_sema_up(&p_dev->hub_finish_sema);
				continue;
			}
			printf("enum uvc\r\n");
			ret = usb_host_ep0_enum_uvc(p_dev);
		}else{
			hub_enum_error = 1;
			os_sema_up(&p_dev->hub_finish_sema);
			continue;			
		}
		//hgusb20_ep_rx_kick(p_dev, HUB_EP, (uint32)uvc_dat, 0x4);
		//printf("up finish sema \n");
		hub_enum_error = 0;
		os_sema_up(&p_dev->hub_finish_sema);
	}
} 




uint8 usb_host_ep0_enum_hub_dev(struct hgusb20_dev *p_dev)
{
	uint8 ret = TRUE;
#if 0	
    while(hub_interrupt == 0)
        os_sleep_ms(1); 
    hub_interrupt = 0;
    if (usb_hub_isr_handl(p_dev) == 0) { 
        hgusb20_set_address(p_dev, 0);
        if (!usb_host_get_device_descriptor_1_do(p_dev)) {
            printf("GetDesc hub Err\r\n");
            return FALSE;
        }       
        ret = usb_host_ep0_enum_uvc(p_dev);
    }
#endif	
	//printf("wait hub device insert\r\n");
	os_sema_down(&p_dev->hub_finish_sema, osWaitForever);
	//printf("finish down \n");
	if(hub_enum_error){
		ret = FALSE;
	}
    return ret;
}

bool usb_host_enum2(struct hgusb20_dev *p_dev)
{
    uint8 try_cnt = 3;
    uint8 res;

    do {
        if (!hgusb20_is_device_online(p_dev)) {
            break;
        }

        //usb_host_init(usb_sw);
//        res = usb_host_ep0_enum_uvc(usb_sw);
        res = usb_host_ep0_enum_hub(p_dev);
        if (res == 0) {
            continue;
        } else if (res == 2) {
            break;
        }else if(res == 1){
            res = usb_host_ep0_enum_hub_dev(p_dev);
        }
		
        if (res == 0) {
            continue;
        } else if (res == 2) {
            break;
        }

        return TRUE;
    } while (--try_cnt);

    //usb_host_set_fail();
    return FALSE;
}


bool usb_uvc_host_enum(struct hgusb20_dev *p_dev)
{
	SDEV_DESC dev_desc;
    bool res = 0;
    res = hgusb20_host_reset(p_dev);
    if (res) {
        return res;
    }	
	hgusb20_set_address(p_dev, 0);
    if (!usb_host_get_device_descriptor_2_do(p_dev)) {          
        printf("SetAddr Err\r\n");
        return 0;
    }

	memcpy(&dev_desc,p_dev->usb_ep0buf,sizeof(SDEV_DESC));

	switch(dev_desc.device_class){
		case USB_DEV_HUB_CLASS:
			res = usb_host_enum2(p_dev);
		break;
		case USB_DEV_UVC_CLASS:
			res = usb_host_enum1(p_dev);
		break;
		default:
			printf("--USB can't suppost this device-- \n");
		break;	
	}

	if(res == 0){
		hgusb20_host_reset_phy(p_dev);
	}
    
    return res;
}


int32 usb_host_uvc_ioctl(struct usb_device *p_usb_d, uint32 cmd, uint32 param1, uint32 param2)
{
    uint16 *msg_buf;
    struct hgusb20_dev *p_dev = (struct hgusb20_dev *)p_usb_d;
    enum usb_host_io_cmd io_cmd = (enum usb_dev_io_cmd)cmd;
    int32 ret = RET_OK;

    switch (io_cmd) {
        case USB_HOST_IO_CMD_RESET:
            hgusb20_host_init(p_dev);
            break;

        case USB_HOST_IO_CMD_SET_CUR:
            msg_buf = (uint16*)param1;
            ret = usb_host_set_cur(p_dev,msg_buf[1], msg_buf[0], msg_buf[2], (uint8 *)&msg_buf[3]);   
            break;

        case USB_HOST_IO_CMD_GET_CUR:
            msg_buf = (uint16*)param1;
            ret = usb_host_get_cur(p_dev,msg_buf[1], msg_buf[0], msg_buf[2], (uint8 *)&msg_buf[3]);
            break;
            
        case USB_HOST_IO_CMD_SET_IDX:
            enum_set_resolution(p_dev, param1);
            break;
        
        default:
            ret = -ENOTSUPP;
            break;
    }
    return ret;
}


uint32 usb_host_bus_irq(uint32 irq, uint32 param1, uint32 param2, uint32 param3)
{
    struct hgusb20_dev *p_dev = (struct hgusb20_dev *)param1;
    uint32 ret_val = 1;
    uint32 ep = param2;
    uint32 len = len;
    switch (irq) {
        case USB_CONNECT:
            printf("usb connect: \r\n");
            os_sema_up(&p_dev->ep0_sema);
            break;
        case USB_DISCONNECT:
            printf("usb disconnect................\r\n");
            break;
        case USB_DEV_CTL_IRQ:
            ret_val = 0;
            break;
        case USB_BABBLE:
            printf("usb babble................\r\n");
            break;
        case USB_DEV_SUSPEND_IRQ:
            break;
        case USB_DEV_RESUME_IRQ:
            break;
//        case USB_DEV_SOF_IRQ:
//            break;
        case USB_EP_RX_IRQ:

//            printf("usb rx: %d\r\n", ep);
            if (ISO_EP == ep) {
                usb_dma_irq(p_dev);
            }
            if (HUB_EP == ep) {
				os_sema_up(&p_dev->hub_sema);
				printf("hub_ep...isr...HUB:%d ISO:%d  %x\r\n",HUB_EP,ISO_EP,param3);
            }            
            break;
        case USB_EP_TX_IRQ:
//            os_mutex_unlock(&p_dev->tx_lock);
            break;
        default:
            break;
    }
    return ret_val;
}



volatile uint8 hub_interrupt = 0;

