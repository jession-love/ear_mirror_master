#include "typesdef.h"
#include "sys_config.h"
#include "lib/video/dvp/cmos_sensor/csi.h"
#include "tx_platform.h"
#include "list.h"
#include "dev.h"
#include "hal/i2c.h"

#if DEV_SENSOR_BF3720




SENSOR_INIT_SECTION static const unsigned char BF3720InitTable[CMOS_INIT_LEN]= 
{	


	// FAE provided config
	0x12, 0x80,//84
//0x12 , 0x03,//00
	0x15, 0x03, // vsync high, hsync high
	0x3a, 0x00, // UYVY
//0x09 , 0x10,//01??11
	0x1b, 0x6e, //2F PLL
	0x13, 0x80,//70 COM8[1]??0x13??=1??b0о?????????????????
			  //????????????????????0x01??blue gain????
			  //0x02??red gain???????????????
	0x8c, 0x00, //
	0x8d, 0x04,
	0x87, 0x2a, //FF gloabl gain
	0x13, 0x87,//07 COM8[1]??0x13??=1??b0о?????????????????
			  //????????????????????0x01??blue gain???? 
			  //0x02??red gain???????????????
	0xbb, 0x20, // reserved
	0x2f, 0x10, //
	0x27, 0xc1, // reserved
	0x16, 0x80, // reserved
	0xe1, 0x98, // reserved
	0xe4, 0x0f, // reserved
	0xe3, 0x04, // reserved
	0xec, 0xa8, // reserved
	0x20, 0x40, // reserved
	0xeb, 0xd5, // reserved
	0xd2, 0x19, // gain diff ?
	0x33, 0x80, // lens shading
	0x34, 0x28, // lens shading
	0x35, 0x34, // lens shading
	0x65, 0x32, // lens shading
	0x66, 0x30, // lens shading
	0xf1, 0x00, // module by pass
	0x24, 0x69,//82 AE_TAR1 ??A??????????
	0x97, 0x69,//78 AE_TAR2 A????????????
			  //0x55??BRIGHT bit[7]?????λ????bit[7]?0????????????
			  //??bit[7]?1????????????
	0x25, 0x88, // ae loc ?
//	 0x81, 0xff, // 	
//	  0x94, 0x82, //22 glb??	 
	  
	0x82, 0x2a, //22 glb??
	0x83, 0x3a, //49 glb??
	0x84, 0x38, //32 glb??
	0x85, 0x41, //5E glb??
	0x86, 0x90, //A0 glb??
	0x8a, 0x9c, //C3 A6 50 hz banding filter
	// 0x8b, 0x9c, //BB 60 hz banding filter
	0x39, 0x0f, //CF ee
	0x98, 0x12, //02 windows select, weight select
	0x3f, 0x48,//90??28??????1????7????0????ü?????????1[6:0]????????
			  //1:???????????????
	0x90, 0x30,//30 20	INT_TIM_TH	???е???????????????
	0x91, 0x20,//20 1c	OFFSET_TH1 ??????int-tim<int-tim-th??
			  //?????1+?????th1*x/4?????????????????
	0x2d, 0x40,//40 60	Y_AVER_TH	Y_Aver??????????????е????????????
	0x2e, 0x38,//38 24	OFFSET_TH2 ??????Y_AVER<Y_AVER_TH,
			  //?????1-?????th2*x/4????????????????????

//gamma ????????
	0x40, 0x58, // gamma
	0x41, 0x54, // gamma
	0x42, 0x4e, // gamma
	0x43, 0x44, // gamma
	0x44, 0x3e, // gamma
	0x45, 0x39, // gamma
	0x46, 0x35, // gamma
	0x47, 0x31, // gamma
	0x48, 0x2e, // gamma
	0x49, 0x2b, // gamma
	0x4b, 0x29, // gamma
	0x4c, 0x27, // gamma
	0x4e, 0x23, // gamma
	0x4f, 0x20, // gamma
	0x50, 0x1f, // gamma
//	  0x3f, 0x80,//??? [7:6]??11?????g????gamma??	   
#if 0
//gamma default
0x40, 0x88,
0x41, 0x70,
0x42, 0x5f,
0x43, 0x50,
0x44, 0x46,
0x45, 0x40,
0x46, 0x3a,
0x47, 0x36,
0x48, 0x32,
0x49, 0x2f,
0x4b, 0x2c,
0x4c, 0x29,
0x4e, 0x25,
0x4f, 0x22,
0x50, 0x1f,

//gamma ?????????????
0x40, 0x98,
0x41, 0x88,
0x42, 0x6e,
0x43, 0x59,
0x44, 0x4d,
0x45, 0x45,
0x46, 0x3e,
0x47, 0x39,
0x48, 0x35,
0x49, 0x31,
0x4b, 0x2e,
0x4c, 0x2b,
0x4e, 0x26,
0x4f, 0x23,
0x50, 0x20,

//gamma low noise
0x40, 0x68,
0x41, 0x60,
0x42, 0x54,
0x43, 0x46,
0x44, 0x3e,
0x45, 0x39,
0x46, 0x35,
0x47, 0x31,
0x48, 0x2e,
0x49, 0x2b,
0x4b, 0x29,
0x4c, 0x27,
0x4e, 0x24,
0x4f, 0x22,
0x50, 0x20,
0x3f, 0x28,//??? [7:6]??11?????g????gamma??
			  //10???????????g????gamma???????????????
#endif

	0xd4, 0x0a, 	  
	0x51, 0x0a,
	0x52, 0x18,
	0x53, 0x0e,
	0x54, 0x17,
	0x57, 0x95,
	0x58, 0x7d,
	0x59, 0x62,
	0x5a, 0x83,
	0x5b, 0x21,
	0x5d, 0x95,
	0xd3, 0x00,

#if 0                                       
//CC??A?? CCM ????
0xd4, 0x0a, 	  
0x51, 0x0a,
0x52, 0x18,
0x53, 0x0e,
0x54, 0x17,
0x57, 0x95,
0x58, 0x7d,
0x59, 0x62,
0x5a, 0x83,
0x5b, 0x21,
0x5d, 0x95,
0xd3, 0x00,

//CC??A?? CCM ????
0xd4, 0x0a,
0x51, 0x19,
0x52, 0x24,
0x53, 0x0b,
0x54, 0x12,
0x57, 0x77,
0x58, 0x65,
0x59, 0x45,
0x5a, 0x5e,
0x5b, 0x19,
0x5d, 0x95,
0xd3, 0x00,


//CC??A?? CCM ??????
0xd4, 0x0a,
0x51, 0x05,
0x52, 0x16,
0x53, 0x11,
0x54, 0x15,
0x57, 0x8f,
0x58, 0x7a,
0x59, 0x48,
0x5a, 0x88,
0x5b, 0x3f,
0x5d, 0x95,
0xd3, 0x00,
//0x55, 0x50, // cc Brightness control:
0x56, 0x38, // cc Y Coefficient for Contrast
#endif

//a??CCM
	0xd4, 0x4a, // FRAM_NUM_THD ?
	0x51, 0x0f, // cc
	0x52, 0x35, // cc
	0x53, 0x26, // cc
	0x54, 0x17, // cc
//	  0x55, 0x19, // cc Brightness control:
//	  0x56, 0x3c, // cc Y Coefficient for Contrast
	0x57, 0x86, // cc
	0x58, 0x9d, // cc
	0x59, 0x5b, // cc
	0x5a, 0x8d, // cc
	0x5b, 0x32, // cc
	0x5d, 0x9d, // cc
	0xd3, 0x00, // reserved



//??A????? 
	0x3b, 0x00, // COEF_SELSAT_COEF_SEL
	0xb1, 0xd0, // Saturation
	0xb2, 0xb8, // Saturation
	0x3b, 0x01, // COEF_SELSAT_COEF_SEL
	0xb1, 0xd0, //c0 0xb1??????????????????????0x80????????????
			  //?????????0x80?????????????????????????
			  //?????0xff???????С??0x80?????С????????????
	0xb2, 0xb8, //a0?????????????????????0x80????????????
			  //?????????0x80????????????????
			  //?????0xff???????С??0x80?????С????????????
	0xb0, 0xd4, //d4 Saturation ??????????6:0 ?????????? df??ff
	0xb3, 0x86, //86 Saturation ??????????84,8f??ff??f6??f4
	0x70, 0x87, //81 ????λ????????????????????1????????????
			  //??????0?????????????
	0x72, 0xa2, //72  62 
	   //[7:6]???????????????????????????????????????????????
	   //[5:4]?????????????????????????????????????????????
	0x7a, 0x41,//33 45 ???????
			  //[7:4]??????????????????????С??????????????
			  //?С?????????????????????
			  //[3:0]???????????????С???????С??????????????
			  //??????????????????????
	0x78, 0x33, //33 0x78[0]????????????λ????????1?????????????????????0??????????????
	0x5c, 0x00, // Saturation
	0xa2, 0x0c, // B Gain λ[5:0]?????????????
	0xa3, 0x27, // B Gain λ[5:0]?????????????
	0xa4, 0x09, // R Gain λ[5:0]??????????????
	0xa5, 0x30, // R Gain λ[5:0]??????????????
	0xa7, 0x97, // Base B Gain	 [7]?????????? λ[4:0]??????B????
	0xa8, 0x19, //15 Base R Gain [7]??????????? λ[4:0]??????R????
	0xa9, 0x52, // 57 5a AWB?????CB
	0xaa, 0x52, // 54	   AWB?????CR
	0xab, 0x18, // 12	 AWB?????CBCR
	0xac, 0x30, // 3c	   AWB?????Y_??
	0xad, 0xf0, //		 AWB?????Y_??
	0xae, 0x33, //57 λ[7:4]??????f?????????? λ[3:0]??????f?????????
	0xd0, 0x00, // Reserved
	0xd1, 0x91, // 00??93
 //   0x6a, 0x02,	
	0x23, 0x36, //33 ???????[2:0]?? [2:0]???????????У???????gain[2:0]??
				  //[6:4]??????????У???????gain[2:0]??
	0xee, 0x30, // 4c??88
	0x91, 0x9c, // be used for auto offset adjust.
	0x78, 0x73, // IntCtr?
	0x7c, 0x48, // Reserved
	0xd9, 0xb6, // Gain

	0x89, 0x2d, // 720p fps //min 15fps
	0x8a, 0x9c, // 720p fps
	0x8b, 0x3e, // 720p fps

	0x4a, 0x70,//40
#if 1 //1280X720
	0x17, 0x00, //14 16  22 HSTART high 8bit
	0x18, 0x64, //8C B6 182 HSTOP high 8bit
	0x19, 0x00, //0B 1E 30 VSTART  high 8bit
	0x1a, 0x4C, //4E  78 26VSTOP high 8bit
#else//960X536
	0x17, 0x14,
	0x18, 0x8C,
	0x03, 0x22,
	0x19, 0x0B,
	0x1a, 0x4e,
#endif
//	  0x03, 0x00, // Bit[6:4]: VREF end low 3 bits Bit[2:0]: VREF start low 3 bits
//	  0x32, 0x00, // Bit[6:4]: HREF end low 3 bits Bit[2:0]: HREF start low 3 bits
//	  0x12, 0x00,
//	  0x4a, 0x30,
//	  0x11, 0x01,

	0x1f, 0x10,
	0x22, 0x10,
	0x26, 0x10,
	
	0x56, 0x38,
	
	0x09, 0x10, //10// output drive capability
	0x1e, 0x30, // Mirror/Vflip Enable


   
-1,-1};



SENSOR_OP_SECTION const _Sensor_Adpt_ bf3720_cmd= 
{	
	.typ = 1, //YUV
	.pixelw = 800,
	.pixelh= 608,
	.hsyn = 1,
	.vsyn = 1,
	.rduline = 0,//
	.rawwide = 1,//10bit
	.colrarray = 2,//0:_RGRG_ 1:_GRGR_,2:_BGBG_,3:_GBGB_
	.init = (uint8 *)BF3720InitTable,
	.rotate_adapt = {0},
	.hvb_adapt = {0x80,0x0a,0x80,0x0a},
	. mclk = 24000000,
	.p_fun_adapt = {NULL,NULL,NULL},
	.p_xc7016_adapt = NULL, 	
};

const _Sensor_Ident_ bf3720_init=
{
	0x20,0xDC,0xDD,0x01,0x01,0xFD
};


#endif



