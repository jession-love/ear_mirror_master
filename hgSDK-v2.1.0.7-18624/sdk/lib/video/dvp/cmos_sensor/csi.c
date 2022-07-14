#include "sys_config.h"
#include "typesdef.h"
#include "lib/video/dvp/cmos_sensor/csi.h"
#include "devid.h"
#include "hal/gpio.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "dev/csi/hgdvp.h"


#if DVP_EN



#define RESET_HIGH()		{gpio_set_val(rsn,1);}

#define RESET_LOW()		    {gpio_set_val(rsn,0);}





#define IIC_CLK 120000UL

uint8 u8SensorwriteID,u8SensorreadID;
uint8 u8Addrbytnum,u8Databytnum;

static uint8 g_byDevWriteAddr;
static uint8 g_byDevReadAddr;

static _Sensor_Ident_ *devSensorInit=NULL;

Output_photo photo_msg;
SNSER snser; 
struct dvp_device *dvp_test;
struct i2c_device *iic_test;


static const _Sensor_Ident_ *devSensorInitTable[] = {

#if DEV_SENSOR_GC0308	
	&gc0308_init,
#endif

#if DEV_SENSOR_GC0309	
	&gc0309_init,
#endif

#if DEV_SENSOR_GC0311	
	&gc0311_init,
#endif

#if DEV_SENSOR_GC0312	
	&gc0312_init,
#endif

#if DEV_SENSOR_GC0328	
	&gc0328_init,
#endif

#if DEV_SENSOR_GC0329
	&gc0329_init,
#endif

#if DEV_SENSOR_BF3A03	
	&bf3a03_init,
#endif

#if DEV_SENSOR_BF3703	
	&bf3703_init,
#endif

#if DEV_SENSOR_OV2640	
	&ov2640_init,
#endif

#if DEV_SENSOR_OV7725	
	&ov7725_init,
#endif

#if DEV_SENSOR_OV7670	
	&ov7670_init,
#endif

#if DEV_SENSOR_BF2013
	&bf2013_init,
#endif

#if DEV_SENSOR_XC7016_H63	
	&xc7016_h63_init,
#endif

#if DEV_SENSOR_XC7011_H63	
	&xc7011_h63_init,
#endif

#if DEV_SENSOR_XC7011_GC1054	
	&xc7011_gc1054_init,
#endif


#if DEV_SENSOR_XCG532	
	&xcg532_init,
#endif

#if DEV_SENSOR_GC2145
	&gc2145_init,
#endif

#if DEV_SENSOR_SP0718
	&sp0718_init,
#endif
	
#if DEV_SENSOR_SP0A19
	&sp0a19_init,
#endif
	
#if DEV_SENSOR_BF3720
	&bf3720_init,
#endif

	NULL,
};

static const _Sensor_Adpt_ *devSensorOPTable[] = {

#if DEV_SENSOR_GC0308	
	&gc0308_cmd,
#endif

#if DEV_SENSOR_GC0309	
	&gc0309_cmd,
#endif

#if DEV_SENSOR_GC0311	
	&gc0311_cmd,
#endif

#if DEV_SENSOR_GC0312	
	&gc0312_cmd,
#endif

#if DEV_SENSOR_GC0328	
	&gc0328_cmd,
#endif

#if DEV_SENSOR_GC0329	
	&gc0329_cmd,
#endif

#if DEV_SENSOR_BF3A03	
	&bf3a03_cmd,
#endif

#if DEV_SENSOR_BF3703	
	&bf3703_cmd,
#endif

#if DEV_SENSOR_OV2640	
	&ov2640_cmd,
#endif

#if DEV_SENSOR_OV7725	
	&ov7725_cmd,
#endif

#if DEV_SENSOR_OV7670	
	&ov7670_cmd,
#endif

#if DEV_SENSOR_BF2013
	&bf2013_cmd,
#endif

#if DEV_SENSOR_XC7016_H63
	&xc7016_h63_cmd,
#endif

#if DEV_SENSOR_XC7011_H63
	&xc7011_h63_cmd,
#endif

#if DEV_SENSOR_XC7011_GC1054	
	&xc7011_gc1054_cmd,
#endif

#if DEV_SENSOR_XCG532	
	&xcg532_cmd,
#endif

#if DEV_SENSOR_GC2145
	&gc2145_cmd,
#endif

#if DEV_SENSOR_SP0718
	&sp0718_cmd,
#endif

#if DEV_SENSOR_SP0A19
	&sp0a19_cmd,
#endif

#if DEV_SENSOR_BF3720
	&bf3720_cmd,
#endif

};



const _Sensor_Ident_ null_init={0x00,0x00,0x00,0x00,0x00,0x00};


/*******************************************************************************
* Function Name  : Delay_Cnt
* Description    : delay cnt nop
* Input          : nop number
* Output         : None
* Return         : None
*******************************************************************************/
extern void Delay_nopCnt(uint32 cnt);

/*******************************************************************************
* Function Name  : sensor_reset
* Description    : for sensor reset before start
* Input          : nop number
* Output         : None
* Return         : None
*******************************************************************************/
__weak void sensor_reset(void){
	uint8_t rsn,pdn;

	rsn = PIN_DVP_RESET;//get_func_io("dvp", "RSN");
	if(rsn != 255){
		gpio_iomap_output(rsn,GPIO_IOMAP_OUTPUT);
	}
	else{
		return;
	}

	pdn = PIN_DVP_PDN;//get_func_io("dvp", "PDN");
	if(pdn != 255){
		gpio_iomap_output(pdn,GPIO_IOMAP_OUTPUT);
		gpio_set_val(pdn,1);
		os_sleep_ms(1);
		gpio_set_val(pdn,0);
	}

	
	RESET_HIGH();
	os_sleep_ms(50);
	RESET_LOW();
	os_sleep_ms(200);
	RESET_HIGH();
	os_sleep_ms(50);

}


void i2c_SetSetting(struct i2c_setting *p_i2c_setting)
{
	if(!p_i2c_setting)
		return;
	
    g_byDevWriteAddr = p_i2c_setting->u8DevWriteAddr;
    g_byDevReadAddr = p_i2c_setting->u8DevReadAddr;	

	return;
}



static int sensorCheckId(struct i2c_device *p_iic,const _Sensor_Ident_ *p_sensor_ident,const _Sensor_Adpt_ *p_sensor_adpt)
{
	int8 u8Buf[3];
	uint32 id= 0;
	uint32 i = 0;
	

	u8SensorwriteID = p_sensor_ident->w_cmd;
	u8SensorreadID =p_sensor_ident->r_cmd;
	u8Addrbytnum = p_sensor_ident->addr_num;
	u8Databytnum = p_sensor_ident->data_num;




	

	
	i2c_ioctl(p_iic,IIC_SET_DEVICE_ADDR,u8SensorwriteID>>1);
	//id = i2c_sensor_read(p_iic,u8Buf,p_sensor_ident->addr_num,p_sensor_ident->data_num,u8SensorwriteID,u8SensorreadID);

	if(p_sensor_adpt->preset){
		for(i=0;;i+=p_sensor_ident->data_num+p_sensor_ident->addr_num){
			if((p_sensor_adpt->preset[i]==0xFF)&&(p_sensor_adpt->preset[i+1]==0xFF)){
				os_printf("preset table num:%d\r\n",i);
				break;
			}			
			i2c_write(iic_test, (int8*)&p_sensor_adpt->preset[i],p_sensor_ident->addr_num, (int8*)&p_sensor_adpt->preset[i+p_sensor_ident->addr_num], p_sensor_ident->data_num);
		}		
	}


	u8Buf[0] = p_sensor_ident->id_reg;
	if(p_sensor_ident->addr_num == 2)
	{
		u8Buf[0] = p_sensor_ident->id_reg>>8;
		u8Buf[1] = p_sensor_ident->id_reg;
	}

	
	i2c_read(p_iic,u8Buf,p_sensor_ident->addr_num,(int8*)&id,p_sensor_ident->data_num);
	os_printf("SID: %x, %x, %x, %x,%x\r\n",id,p_sensor_ident->id,u8SensorwriteID,u8SensorreadID,p_sensor_ident->id_reg);
	if(id == p_sensor_ident->id)
		return 1;
	else
		return -1;
}


/*******************************************************************************
* Function Name  : Sensor_ReadRegister
* Description    : read sensor register
* Input          : *pbdata :sensor register addr
                   u8AddrLength:sensor register addr length
                   u8DataLength:sensor register data length
* Output         : None
* Return         : u32i2cReadResult:the result from sensor register
*******************************************************************************/
static _Sensor_Adpt_ * sensorAutoCheck(struct i2c_device *p_iic,uint8 *init_buf)
{
	uint8 i;
	_Sensor_Adpt_ * devSensor_Struct=NULL;
	for(i=0;devSensorInitTable[i] != NULL;i++)
	{		
		sensor_reset();
		if(sensorCheckId(p_iic,devSensorInitTable[i],devSensorOPTable[i])>=0)
		{
			os_printf("id =%x num:%d \n",devSensorInitTable[i]->id,i);
			devSensorInit = (_Sensor_Ident_ *) devSensorInitTable[i];
//#if CMOS_AUTO_LOAD
//			devSensor_Struct = sensor_adpt_load(devSensorInit->id,devSensorInit->w_cmd,devSensorInit->r_cmd,init_buf);
//#else
			devSensor_Struct = (_Sensor_Adpt_ *) devSensorOPTable[i];
//#endif
			break;
		}
	}
	if(devSensor_Struct == NULL)
	{
		os_printf("Er: unkown!");
		devSensorInit = (_Sensor_Ident_ *)&null_init;
		return NULL; // index 0 is test only
	}
	return devSensor_Struct;
}

volatile uint8 yuv_buf_head[16]__attribute__ ((section(".sbss")));;
//volatile uint8 yuvbuf[2][IMAGE_W*8*2+IMAGE_W*8+16];

uint8 *yuvbuf[2];


volatile uint8 psram_buf_head[16]__attribute__ ((section(".psram.src")));
#if ONLY_Y
uint8 psram_buf[IMAGE_H][IMAGE_W] __attribute__ ((aligned(4),section(".psram.src")));
#elif ((IMAGE_FORMAT == 1) || (IMAGE_FORMAT == 2))&&(RGB2YUV == 0)
uint8 psram_buf[IMAGE_H][IMAGE_W*2] __attribute__ ((aligned(4),section(".psram.src")));
#elif (IMAGE_FORMAT == 1)&&(RGB2YUV == 1)
//uint8 psram_buf[IMAGE_H][IMAGE_W*2] __attribute__ ((aligned(4),section(".psram.src")));
uint8 psram_buf[30][384*40] __attribute__ ((aligned(4),section(".psram.src")));

#else
uint8 psram_buf[30][384*40] __attribute__ ((aligned(4),section(".psram.src")));
#endif
uint8 psram_buf_tail[2032]__attribute__ ((section(".psram.src")));


//uint8 sram_jpeg[90*1024];

volatile uint32 line_buf = 0;

extern volatile uint32 done_dvp;

extern volatile uint32 vs_isr;

void dvp_hsie_isr(uint32 irq,uint32 image_h,uint32  param){

}

//帧开始信号
void dvp_vsie_isr(uint32 irq,uint32 dev,uint32 param){

}

void dvp_line_isr(uint32 irq,uint32 image_h,uint32 param){

}

void dvp_fhfie_isr(uint32 irq,uint32 dev,uint32 param){
	os_printf("fh\r\n");
}

void dvp_fovie_isr(uint32 irq,uint32 dev,uint32 param){
	os_printf("fv\r\n");
}

void dvp_sip_isr(uint32 irq,uint32 dev,uint32 param){
	os_printf("sip reset DVP\r\n");
}

void dvp_line_isr_test(uint32 irq,uint32 image_h,uint32 param){
	return;
}



struct hgdvp_hw  *dvp_watch;
bool csi_yuv_mode(){
	static _Sensor_Adpt_ *p_sensor_cmd = NULL;
	struct i2c_setting i2c_setting;
	int8 idbuf[3];
	uint32 i=0;
	uint8 adr_num,dat_num;
	uint8 sen_w_cmd,sen_r_cmd,id_c = 0;	
	uint16 image_w,image_h;

	dvp_test = (struct dvp_device *)dev_get(HG_DVP_DEVID);

	iic_test = (struct i2c_device *)dev_get(HG_I2C2_DEVID);

 
	dvp_init(dvp_test);

//1:init iic
	os_printf("csi_test start,iic init\r\n");

	//iic_init_sensor(IIC_CLK,SENSOR_IIC);
	i2c_set_baudrate(iic_test,IIC_CLK);
	i2c_open(iic_test, IIC_MODE_MASTER, IIC_ADDR_7BIT, 0);
	i2c_ioctl(iic_test,IIC_SDA_OUTPUT_DELAY,20);	
	i2c_ioctl(iic_test,IIC_FILTERING,20);
	i2c_ioctl(iic_test,IIC_STRONG_OUTPUT,1);
	//i2c_send_stop(iic_test);
	os_printf("iic init finish,sensor reset & set sensor clk into 6M\r\n");
//2:init sensor
	dvp_set_baudrate(dvp_test,6000000); 
	os_sleep_ms(3);
	os_printf("set sensor finish ,Auto Check sensor id\r\n");
	p_sensor_cmd = snser.p_sensor_cmd = sensorAutoCheck(iic_test,NULL);
	if(p_sensor_cmd == NULL){
		return FALSE;
	}
	os_printf("Auto Check sensor id finish\r\n");
	i2c_setting.u8DevWriteAddr = p_sensor_cmd->w_cmd;
	i2c_setting.u8DevReadAddr = p_sensor_cmd->r_cmd;
	i2c_SetSetting(&i2c_setting);
	
	os_printf("mclk:%dMHz\r\n",p_sensor_cmd->mclk);
	
	
	//sensor_ClockInit(((struct hgdvp*)dvp_test)->hw,p_sensor_cmd->mclk);
	dvp_set_baudrate(dvp_test,p_sensor_cmd->mclk);

	os_printf("init:%x u8Addrbytnum:%d,u8Databytnum:%d\r\n",(uint32)p_sensor_cmd->init,u8Addrbytnum,u8Databytnum);
    if(p_sensor_cmd->init!=NULL)
	{
			
			os_printf("SENSER....init\r\n");
			for(i=0;;i+=u8Addrbytnum+u8Databytnum)
			{
				if((p_sensor_cmd->init[i]==0xFF)&&(p_sensor_cmd->init[i+1]==0xFF)){
					os_printf("init table num:%d\r\n",i);
					break;
				}
	
				if((p_sensor_cmd->init[i]==0xFE)&&(p_sensor_cmd->init[i+1]==0xFE)){
					if(p_sensor_cmd->init[i+2]==0x01){
						os_sleep_ms(100);
					}
				}
				else{
//					os_printf("w:%x   u8Addrbytnum:%x u8Databytnum:%x u8SensorwriteID:%x u8SensorreadID:%x\r\n",p_sensor_cmd->init[i],u8Addrbytnum,u8Databytnum,u8SensorwriteID,u8SensorreadID);
					//i2c_sensor_write(iic_test,&p_sensor_cmd->init[i],u8Addrbytnum,u8Databytnum,u8SensorwriteID,u8SensorreadID);
					i2c_write(iic_test, (int8*)&p_sensor_cmd->init[i], u8Addrbytnum, (int8*)&p_sensor_cmd->init[i+u8Addrbytnum], u8Databytnum);
					//Sensor_WriteRegister(SENSOR_IIC,&p_sensor_cmd->init[i],u8Addrbytnum,u8Databytnum);
					if(i==0)
					{
						os_sleep_ms(1);
					}
				}
			}		
	
			if(p_sensor_cmd->p_xc7016_adapt.sensor_init_table != NULL)
			{
				adr_num = p_sensor_cmd->p_xc7016_adapt.addr_num;
				dat_num = p_sensor_cmd->p_xc7016_adapt.data_num;
				sen_w_cmd = u8SensorwriteID;
				sen_r_cmd = u8SensorreadID;
				u8SensorwriteID = p_sensor_cmd->p_xc7016_adapt.w_cmd;
				u8SensorreadID	= p_sensor_cmd->p_xc7016_adapt.r_cmd;
				idbuf[0] = p_sensor_cmd->p_xc7016_adapt.id_reg;		
				//id_c = i2c_sensor_read(iic_test,idbuf,adr_num,dat_num,u8SensorwriteID,u8SensorreadID);
				i2c_ioctl(iic_test,IIC_SET_DEVICE_ADDR,u8SensorwriteID>>1);
				i2c_read(iic_test,idbuf,adr_num,(int8*)&id_c,dat_num);
				os_printf("\r\nchildren SID: %x %x %x\r\n",id_c,u8SensorwriteID,u8SensorreadID);			
				for(i=0;;i+=adr_num+dat_num)
				{
					if((p_sensor_cmd->p_xc7016_adapt.sensor_init_table[i]==0xFF)&&(p_sensor_cmd->p_xc7016_adapt.sensor_init_table[i+1]==0xFF)){
						os_printf("sensor table num:%d\r\n",i);
						break;
					}

					i2c_write(iic_test, (int8*)&p_sensor_cmd->p_xc7016_adapt.sensor_init_table[i], adr_num, (int8*)&p_sensor_cmd->p_xc7016_adapt.sensor_init_table[i+adr_num], dat_num);
					//i2c_sensor_write(iic_test,&p_sensor_cmd->p_xc7016_adapt.sensor_init_table[i],adr_num,dat_num,u8SensorwriteID,u8SensorreadID);
					if(i==0)
					{
						os_sleep_ms(1);
					}
				}	
				u8SensorwriteID = sen_w_cmd;
				u8SensorreadID	= sen_r_cmd;
				i2c_ioctl(iic_test,IIC_SET_DEVICE_ADDR,u8SensorwriteID>>1);
				for(i=0;;i+=u8Addrbytnum+u8Databytnum)
				{
					if((p_sensor_cmd->p_xc7016_adapt.bypass_off[i]==0xFF)&&(p_sensor_cmd->p_xc7016_adapt.bypass_off[i+1]==0xFF)){
						os_printf("bypass table num:%d\r\n",i);
						break;
					}
					i2c_write(iic_test, (int8*)&p_sensor_cmd->p_xc7016_adapt.bypass_off[i], u8Addrbytnum, (int8*)&p_sensor_cmd->p_xc7016_adapt.bypass_off[i+u8Addrbytnum], u8Databytnum);
					//i2c_sensor_write(iic_test,&p_sensor_cmd->p_xc7016_adapt.bypass_off[i],u8Addrbytnum,u8Databytnum,u8SensorwriteID,u8SensorreadID);		
					if(i==0)
					{
						os_sleep_ms(1);
					}
				}			
				
			}
			
	}

	
	os_printf("SENSR ident ok:%d*%d\r\n",p_sensor_cmd->pixelw,p_sensor_cmd->pixelh);

#if SCEN_EN	
	image_h = p_sensor_cmd->pixelh/2;
	image_w = p_sensor_cmd->pixelw/2;
#else
	image_h = p_sensor_cmd->pixelh;
	image_w = p_sensor_cmd->pixelw;
#endif
	photo_msg.out_h = image_h;
	photo_msg.out_w = image_w;

//3:init csi

	os_printf("csi init start  --\r\n");
	os_printf("csi set size ====>%d*%d\r\n",image_w,image_h);

	yuvbuf[0] = malloc(p_sensor_cmd->pixelw*8*2+p_sensor_cmd->pixelw*8);
	if(yuvbuf[0] == NULL){
		printf("no room yuvbuf0\r\n");
		return FALSE;
	}
	yuvbuf[1] = malloc(p_sensor_cmd->pixelw*8*2+p_sensor_cmd->pixelw*8);
	if(yuvbuf[1] == NULL){
		printf("no room yuvbuf1\r\n");
		return FALSE;
	}	
#if SCEN_EN	
	dvp_set_size(dvp_test,0,0,image_w*2-1,image_h*2-1);
#else
	os_printf("csi dvp_size_set\r\n");
	dvp_set_size(dvp_test,0,0,image_w-1,image_h-1);
#endif
	
#if     (ONLY_Y == 1)
	dvp_set_addr1(dvp_test,(uint32)psram_buf[0]);
	dvp_set_addr2(dvp_test,(uint32)psram_buf[0]);
#else
	dvp_set_addr1(dvp_test,(uint32)yuvbuf[0]);
	dvp_set_addr2(dvp_test,(uint32)yuvbuf[1]);
#endif

#if SCEN_EN	
	dvp_set_half_size(dvp_test,1);
#endif 


	dvp_set_hsync_polarity(dvp_test,p_sensor_cmd->hsyn);
	dvp_set_vsync_polarity(dvp_test,p_sensor_cmd->vsyn);
	
	dvp_set_ycbcr(dvp_test,YUV_MODE);
 

#if     (ONLY_Y == 1)
	dvp_unload_uv(dvp_test,1);
#endif

#if		(ONCE_SAMPLE == 1) 
	dvp_set_once_sampling(dvp_test,1);
	os_printf("delay ,wait sensor image ok\r\n");
	for(i = 0;i < 100;i++){
		Delay_nopCnt(500*1000);
	}

#endif

	//dvp_debounce_enable(((struct hgdvp*)dvp_test)->hw,1,3);

	dvp_frame_load_precent(dvp_test,FRAME_RATE);

	dvp_low_high_threshold(dvp_test,1);
#ifdef DEMON_BOARD
	dvp_set_exchange_d5_d6(dvp_test,0);
#else
	dvp_set_exchange_d5_d6(dvp_test,1);
#endif
	os_printf("csi IRQ init\r\n");
#if     (ONLY_Y == 0)	

	dvp_request_irq(dvp_test,HSIE_ISR, (dvp_irq_hdl )&dvp_hsie_isr,(uint32)image_h);
	dvp_request_irq(dvp_test,VSIE_ISR, (dvp_irq_hdl )&dvp_vsie_isr,0);
	dvp_request_irq(dvp_test,SSCIE_EIGHT_ISR,(dvp_irq_hdl )&dvp_line_isr,(uint32)image_h);

	dvp_request_irq(dvp_test,FHFIE_ISR,(dvp_irq_hdl )&dvp_fhfie_isr,0);
	dvp_request_irq(dvp_test,FOVIE_ISR,(dvp_irq_hdl )&dvp_fovie_isr,0);
	dvp_request_irq(dvp_test,SIP_ISR,(dvp_irq_hdl )&dvp_sip_isr,0);

#else
	dvp_request_irq(dvp_test,HSIE_ISR, (dvp_irq_hdl )&dvp_hsie_isr,(uint32)image_h);
	dvp_request_irq(dvp_test,VSIE_ISR, (dvp_irq_hdl )&dvp_vsie_isr,0);
	dvp_request_irq(dvp_test,SSCIE_EIGHT_ISR,(dvp_irq_hdl )&dvp_line_isr_test,(uint32)image_h);
	//dvpirq_register(p_dvp,SSCIE_EIGHT_ISR,dvp_sscie_isr,image_h);
	
	dvp_request_irq(dvp_test,FHFIE_ISR,(dvp_irq_hdl )&dvp_fhfie_isr,0);
	dvp_request_irq(dvp_test,FOVIE_ISR,(dvp_irq_hdl )&dvp_fovie_isr,0);
#endif

	//dvp_open(dvp_test);

	os_printf("csi IRQ init finish,start get data\r\n");
	return TRUE;
}

bool csi_cfg(){
	bool ret;
	ret = csi_yuv_mode();
	return ret;
}

void csi_room_del(){
	if(yuvbuf[0]){
		free(yuvbuf[0]);
	}
	if(yuvbuf[1]){
		free(yuvbuf[1]);
	}
}

bool csi_open(){
	dvp_open(dvp_test);
	return 1;
}


#endif





