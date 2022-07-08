#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openDML.h"

extern bool stdidx_write2disk(bool type,void *f,uint32 num,ODMLBUFF *buff,void *f_write);






//一些未移植的宏控制

#define FATFS_OPENDML	1
#define SYNC_EN	0

 

void stdindx_updata(void *f,ODMLBUFF *buff)
{
	#if FATFS_OPENDML
	//video stdindx
	stdidx_write2disk(0,f,buff->vidxcount,buff,osal_fwrite);
	//audo stdindx
	stdidx_write2disk(1,f,buff->aidxcount,buff,osal_fwrite);
	#else
	//video stdindx
	stdidx_write2disk(0,f,buff->vidxcount,buff,NULL);
	//audo stdindx
	stdidx_write2disk(1,f,buff->aidxcount,buff,NULL);
	#endif
			
}

int ODMLUpdateAVIInfo(void * file,ODMLBUFF *buff,bool Enpcm,void *f_write,ODMLAVIFILEHEADER * headerbuf)
{

	uint32 *ptemp;
 	int ret;
	ODMLAVIFILEHEADER *aviheader = headerbuf;
	
	/************AVIX updata**************/

	short i;
	//updata last AVIX
	if(buff->AVIXnum > -1){
	#if FATFS_OPENDML
	buff->movlist[buff->AVIXnum].riff.cb = osal_fsize(file) - buff->moviofest[buff->AVIXnum] - 8 ;//list riff size
	buff->movlist[buff->AVIXnum].movi.cb = osal_fsize(file) - buff->moviofest[buff->AVIXnum] - 20;//list movi size
	#endif
	}
	
	//write back AVIX size 
	printf("%s:%d\tavixnum:%d\n",__FUNCTION__,__LINE__,buff->AVIXnum);
	for(i = 0; i < 1+buff->AVIXnum ; i++ ){
		#if FATFS_OPENDML
		ret= osal_fseek(file,buff->moviofest[i] );
		ret= osal_fwrite(&buff->movlist[i],1,sizeof(AVIXMOVILIST),file);
		#endif
		if(ret != sizeof(AVIXMOVILIST)) {
		printf("rewrite_AVIX  err\n");
		}
	}	
	printf("rewrite_AVIX  end\n");
	/*****************************************/
	
	
	memset(aviheader,0,_ODML_AVI_HEAD_SIZE__);

//=========fill avifileheader info=========
#if FATFS_OPENDML
	ret = osal_fseek(file, 0);
	ret = osal_fread((void *)aviheader,1,_ODML_AVI_HEAD_SIZE__,file);
#endif
	
		 
	if(buff->AVIXnum > -1){
	aviheader->riff.cb = buff->moviofest[0] -8;//point to RIFF then cut list.fcc and list.cb
	aviheader->avih.dwTotalFrames =buff->avilisframecnt; //
	}
	else{
	#if FATFS_OPENDML
	aviheader->riff.cb = osal_fsize(file)-8;//
	#endif
	aviheader->avih.dwTotalFrames =buff->vframecnt; //
	}
	aviheader->strh_v.dwLength = buff->vframecnt;//
	
	if(Enpcm)
	aviheader->strh_a.dwLength = buff->aframecnt*(PCM_SHEET_SIZE-8)/2;
	//aviheader->strh_a.dwLength = buff->aframecnt*(PCM_SHEET_SIZE-8);
	else	
	aviheader->strh_a.dwLength = 0;
	
	/********************super indx********************/
	aviheader->vsupindxh.nEntriesInUse = buff->svidxcount;
	aviheader->asupindxh.nEntriesInUse = buff->saidxcount;
	memcpy(aviheader->aindx,buff->asuperindx,sizeof(superindx)*buff->saidxcount);
	memcpy(aviheader->vindx,buff->vsuperindx,sizeof(superindx)*buff->svidxcount);
	aviheader->strodml.dwTotalFrames = buff->vframecnt + aviheader->strh_a.dwLength;
	if(!Enpcm){
	//这里应该填什么?原来这里代码应该是有错的,暂时先去掉,为什么之前录卡没有出现问题?memset?
	//memcpy(aviheader->aindx,0,sizeof(superindx)*buff->saidxcount);
	aviheader->asupindxh.nEntriesInUse = 0;
	}
	

	/***********************"movi"*****************************/
	ptemp = (uint32*)((uint32)aviheader+_ODML_AVI_HEAD_SIZE__-12);			//data block head
	ptemp[0] = 0x5453494c;	
									//"LIST"
	if(buff->AVIXnum > -1)
	ptemp[1]=buff->moviofest[0] -_ODML_AVI_HEAD_SIZE__+4;
	else
	{
		#if FATFS_OPENDML
		ptemp[1] = osal_fsize(file)  -_ODML_AVI_HEAD_SIZE__+4; 
		#endif
	}
	
	ptemp[2] = 0x69766f6d;										//"movi"
//debgbuf(headerbuf, 512);

//=========write avifileheader info=======
	#if FATFS_OPENDML
	ret = osal_fseek(file, 0);
	
	ret = osal_fwrite((void *)aviheader,1,_ODML_AVI_HEAD_SIZE__,file);
	#else
	ret = _ODML_AVI_HEAD_SIZE__;
	#endif

	if(ret != _ODML_AVI_HEAD_SIZE__) {
		printf("avi update hd err\n");
		return false;
	}
	printf("osal_fwrite:ret =%d\n",ret);
	return	true;

}





void ODMLbuff_init(ODMLBUFF *buff)
{
	memset((void *)buff,0,sizeof(ODMLBUFF));
	buff->AVIXnum = -1;
	buff->qwOffset = _ODML_AVI_HEAD_SIZE__ - 4;//point to first movi
}


/****************************************************************************
 * name   	:supindxbuff_updata
 * funtion	:should record stdinx infomation  when the stdindx wirte to disk
 *           this funtion just write data to buff not to disk 
 * ************************************************************************/
bool supindxbuff_updata(bool typ,uint64 ofset,uint32 size,uint32 packnum,ODMLBUFF *buff)
{

	if(typ)
	{
		buff->asuperindx[buff->saidxcount].qwOffset = ofset;
		buff->asuperindx[buff->saidxcount].dwSize = size;
		//buff->asuperindx[buff->saidxcount].dwDuration = packnum;
		buff->asuperindx[buff->saidxcount].dwDuration = packnum*(PCM_SHEET_SIZE-8)/2;
		//printf("packnum:%d\tdwDuration:%d\r\n",packnum,buff->asuperindx[buff->saidxcount].dwDuration);
		#if SYNC_EN
		if(buff->Sync_buff.asupindx_count == 32)
		{
			buff->Sync_buff.asupindx_count = 0;
			memset(&buff->Sync_buff.asupidx,0,512);
			buff->Sync_buff.asupidx_sector ++;
		}
		buff->Sync_buff.asupidx.Aindx[buff->Sync_buff.asupindx_count].qwOffset= ofset;
		buff->Sync_buff.asupidx.Aindx[buff->Sync_buff.asupindx_count].dwSize= size;
		buff->Sync_buff.asupidx.Aindx[buff->Sync_buff.asupindx_count].dwDuration= packnum*(PCM_SHEET_SIZE-8)/2;
		buff->Sync_buff.asupindx_count ++;
		#endif
		buff->saidxcount++;
		if(buff->saidxcount == SupIDXBUFLEN-1)
		{
			printf("supindxbuff full\n");
			return false;
		}
		

	}
	else
	{
		buff->vsuperindx[buff->svidxcount].qwOffset = ofset;
		buff->vsuperindx[buff->svidxcount].dwSize =size;
		buff->vsuperindx[buff->svidxcount].dwDuration = packnum;
		
		#if SYNC_EN
		if(buff->Sync_buff.vsupindx_count == 32)
		{
			buff->Sync_buff.vsupindx_count = 0;
			memset(&buff->Sync_buff.vsupidx,0,512);
			buff->Sync_buff.vsupidx_sector++;
		}
		buff->Sync_buff.vsupidx.vindx[buff->Sync_buff.vsupindx_count].qwOffset= ofset;
		buff->Sync_buff.vsupidx.vindx[buff->Sync_buff.vsupindx_count].dwSize= size;
		buff->Sync_buff.vsupidx.vindx[buff->Sync_buff.vsupindx_count].dwDuration= packnum;
		buff->Sync_buff.vsupindx_count ++;
		#endif
		
		buff->svidxcount++;
		if(buff->svidxcount == SupIDXBUFLEN-1)
		{
			printf("supindxbuff full\n");
			return false;
		}
	}
	
	return true;
}
//uint32 junk[128];

#if 0

void add_AVIX( void *f,uint64 fileoffest,ODMLBUFF *buff,void *f_write)
{
	uint32 br;
	uint32 fillsize;
	uint32 junk[2];
	
	buff -> AVIXnum ++;//record how many AVIX it has 
	if(buff -> AVIXnum == AVIXMAXNUM ){
		buff -> AVIXnum =0;
		printf("Add AVIX error!!! \n");
	}
	buff -> movlist[buff -> AVIXnum].riff.fcc = 0x46464952;//RIFF
	buff -> movlist[buff -> AVIXnum].riff.cb = 0;//size
	buff -> movlist[buff -> AVIXnum].riff.fcctype = 0X58495641;//AVIX
	
	buff -> movlist[buff -> AVIXnum].movi.fcc = 0x5453494c;//LIST
	buff -> movlist[buff -> AVIXnum].movi.cb = 0;//size
	buff -> movlist[buff -> AVIXnum].movi.fcctype = 0x69766f6d;//movi
	#if FATFS_OPENDML
	osal_fwrite( &buff -> movlist[buff -> AVIXnum], 1,sizeof(AVIXMOVILIST),f);	
	#endif
	fillsize =512-(br%512);
	if(fillsize != 512 ){
			//printf("fill size:%d",fillsize);
			//memset((void *)junk,0,512);
			junk[0] = 0x4b4e554a;//junk
			junk[1]	= fillsize-8;
			#if FATFS_OPENDML
			osal_fwrite ((void*)&junk,1, 8,f);
			osal_fseek(f,osal_ftell(f)+fillsize-8);
			#endif
		}
	buff ->moviofest[buff -> AVIXnum] = fileoffest;

	buff ->qwOffset = fileoffest+20;//fileoffest point to RIFF  qwOffset should point to "movi"
	if(buff -> AVIXnum > 0){//more then one AVIX
		buff ->movlist[buff -> AVIXnum-1].riff.cb = fileoffest - buff ->moviofest[buff -> AVIXnum-1]-8 ;//list riff size
		buff ->movlist[buff -> AVIXnum-1].movi.cb = fileoffest - buff ->moviofest[buff -> AVIXnum-1]-20;//list movi size
	}

}
#endif

/**************************************************************
 * name   	:OMDLvideo_header_write
 * funtion	:write the OpenDML AVI file
 * ***********************************************************/
int OMDLvideo_header_write(void *f_write,void *f, AVI_INFO *msg,ODMLAVIFILEHEADER *headerbuf)
{
	uint32_t ret;
	
	ODMLAVIFILEHEADER *aviheader = headerbuf;
	memset((void *)aviheader,0,_ODML_AVI_HEAD_SIZE__);
	


	if(msg->pcm){
		//fill strl audqs
		aviheader->strl_a.fcc = 0x5453494c;			//"LIST"
		aviheader->strl_a.cb = 888-(SUPERIDXLEN*16)%512 +sizeof(aviheader->strl_a.fcctype)+2*sizeof(CHUNK)+sizeof(AVISTREAMHEADER)+sizeof(AVIWAVEFORMATEX) + sizeof(AVISUPERINDEXHEAD)+SUPERIDXLEN*sizeof(superindx);
		aviheader->strl_a.fcctype = 0x6c727473;		//"strl"
		//fill auds stream
		aviheader->strh_a.fcc = 0x68727473;						//"strh"
		aviheader->strh_a.cb = sizeof(AVISTREAMHEADER)-8;		
		aviheader->strh_a.fccType = 0x73647561;					//"auds" 
		aviheader->strh_a.dwScale = 2;//4
		aviheader->strh_a.dwRate = msg->audiofrq << 1;
		aviheader->strh_a.dwLength = 1024;//2048;
		aviheader->strh_a.dwSampleSize = 2;//4
		aviheader->strh_a.dwSuggestedBufferSize=0x00010000;  //
		aviheader->strh_a.dwQuality	=0xFFFFFFFF;	
		//fill auds stream info
		aviheader->strf_a.dwFourCC = 0x66727473;							//"strf"
		aviheader->strf_a.dwSize = sizeof(AVIWAVEFORMATEX);
		aviheader->wavinfo.wFormatTag = 1;			//1:pcm    2:adpcm
		aviheader->wavinfo.nChannels = 1;//2
		aviheader->wavinfo.nSamplesPerSec = msg->audiofrq;	//11025 , 22050, 44100
		aviheader->wavinfo.nAvgBytesPerSec = msg->audiofrq << 1;//64000
		aviheader->wavinfo.nBlockAlign = 2;	//4		// ch*bitspersample /8
		aviheader->wavinfo.wBitsPerSample = 16;
		

		
		//auds superindx
		aviheader->sidx_a.dwFourCC = 0x78646e69; //"indx"
		aviheader->sidx_a.dwSize  = sizeof(AVISUPERINDEXHEAD)+sizeof(superindx)*SUPERIDXLEN;

		aviheader->asupindxh.dwChunkId = 0x62773130;	//"01wb"
		aviheader->asupindxh.wLongsPerEntry=4;//4
		aviheader->asupindxh.bIndexSubType=0; // must be 0 or AVI_INDEX_2FIELD
		aviheader->asupindxh.bIndexType=AVI_INDEX_OF_INDEXES; // must be AVI_INDEX_OF_INDEXES
		aviheader->asupindxh.nEntriesInUse=0;//4
		aviheader->asupindxh.dwReserved[0]=0;//0
		aviheader->asupindxh.dwReserved[1]=0;//0
		aviheader->asupindxh.dwReserved[2]=0;//0

		//fill junk
		aviheader->junk.dwFourCC= 0x4b4e554a;			//"JUNK"
		aviheader->junk.dwSize= _ODML_AVI_HEAD_SIZE__ - (sizeof(ODMLAVIFILEHEADER) + 12);
	}
	else{
		aviheader->strl_a.fcc = 0x4b4e554a;			//"JUNK"
		aviheader->strl_a.cb = 888-(SUPERIDXLEN*16)%512 +sizeof(aviheader->strl_a.fcctype)+2*sizeof(CHUNK)+sizeof(AVISTREAMHEADER)+sizeof(AVIWAVEFORMATEX) + sizeof(AVISUPERINDEXHEAD)+SUPERIDXLEN*sizeof(superindx);
		//fill junk
        aviheader->junk.dwFourCC= 0x4b4e554a;			//"JUNK"
		aviheader->junk.dwSize= _ODML_AVI_HEAD_SIZE__ - (sizeof(ODMLAVIFILEHEADER) + 12);
	}




	

	//fill fcc and fcc type
	aviheader->riff.fcc = 0x46464952;		//"RIFF"
	aviheader->riff.fcctype = 0x20495641;	//"AVI "
	aviheader->hdrl.fcc = 0x5453494c;		//"LIST"
	aviheader->hdrl.cb =_ODML_AVI_HEAD_SIZE__ - 12 - 8-12;
	//aviheader->hdrl.cb =sizeof(AVIFILEHEADER) - 28; //point to junk in end of head 
	aviheader->hdrl.fcctype = 0x6c726468;	//"hdrl"

	//fill avimainheader
	aviheader->avih.fcc = 0x68697661;	//"avih"
	aviheader->avih.cb = sizeof(AVIMAINHEADER)-8;						
	aviheader->avih.dwMicroSecPerFrame = 0x8235;			// 30fps ,so (1s/30)*1000*1000 = 33333us
	aviheader->avih.dwFlags = AVIF_HASINDEX|AVIF_MUSTUSEINDEX;							//has index
	aviheader->strh_v.dwSuggestedBufferSize=0x00100000;


	aviheader->avih.dwStreams = 0x1 + msg->pcm;
	aviheader->avih.dwWidth = msg->win_w; //640
	aviheader->avih.dwHeight = msg->win_h;;//480

	//file move fcc and fcc type
	aviheader->strl_v.fcc = 0x5453494c;			//"LIST"
	
	aviheader->strl_v.cb = sizeof(aviheader->strl_v.fcctype) + 3*sizeof(CHUNK)+sizeof(AVISTREAMHEADER)+sizeof(AVIBITMAPINFOHEADER) + sizeof(VideoPropHeader) +sizeof(AVISUPERINDEXHEAD)+SUPERIDXLEN*sizeof(superindx) +1216-(SUPERIDXLEN*16)%512;
	
	aviheader->strl_v.fcctype = 0x6c727473;		//"strl"
	aviheader->strh_v.fcc = 0x68727473;						//"strh"
	aviheader->strh_v.cb = sizeof(AVISTREAMHEADER)-8;		
	aviheader->strh_v.fccType = 0x73646976;					//"vids"
	aviheader->strh_v.fccHandler = 0x47504a4d;				//"MJPG"
	aviheader->strh_v.dwScale = 1;
	aviheader->strh_v.dwRate = 30;							
	aviheader->strh_v.dwStart = 0;
	aviheader->strh_v.rcFrame.right =msg->win_w; 
	aviheader->strh_v.rcFrame.bottom = msg->win_h;//480

	//fill vids stream info
	aviheader->strf_v.dwFourCC = 0x66727473;							//"strf"
	aviheader->strf_v.dwSize = sizeof(AVIBITMAPINFOHEADER);				
	aviheader->bitmapinfo.biSize = sizeof(AVIBITMAPINFOHEADER);			
	aviheader->bitmapinfo.biWidth = msg->win_w; //640
	aviheader->bitmapinfo.biHeight = msg->win_h;//480
	aviheader->bitmapinfo.biPlanes = 1;							// always 1
	aviheader->bitmapinfo.biBitCount = 24;							// 1, 4, 8, 16, 24, 32
	aviheader->bitmapinfo.biCompression = 0x47504a4d;				//"MJPG"
    aviheader->strh_v.dwSuggestedBufferSize=0x00100000;  //
    aviheader->strh_v.dwQuality	=0xFFFFFFFF;	
	
	
	//video superindx
	aviheader->sidx_v.dwFourCC = 0x78646e69; //"indx"
	aviheader->sidx_v.dwSize  = sizeof(AVISUPERINDEXHEAD)+sizeof(superindx)*SUPERIDXLEN;
	
	printf("dw:%X\t%X\t%X\t%X\n",(int)aviheader->sidx_v.dwSize,sizeof(AVISUPERINDEXHEAD),sizeof(superindx),SUPERIDXLEN);
	aviheader->vsupindxh.dwChunkId = 0x63643030;	//"00dc"
	aviheader->vsupindxh.wLongsPerEntry=4;//4
	aviheader->vsupindxh.bIndexSubType=0; // must be 0 or AVI_INDEX_2FIELD
	aviheader->vsupindxh.bIndexType=AVI_INDEX_OF_INDEXES; // must be AVI_INDEX_OF_INDEXES
	aviheader->vsupindxh.nEntriesInUse=0;//4
	aviheader->vsupindxh.dwReserved[0]=0;//0
	aviheader->vsupindxh.dwReserved[1]=0;//0
	aviheader->vsupindxh.dwReserved[2]=0;//0
	//Video Properties Header	
	
	aviheader->c_vprp.dwFourCC=0x70727076; //"vprp"
	aviheader->c_vprp.dwSize = sizeof(VideoPropHeader);
	aviheader->vprp.dwFrameWidthInPixels=msg->win_w;
	aviheader->vprp.dwFrameHeightInLines=msg->win_h;

	aviheader->vprp.dwHTotalInT = msg->win_w;
	aviheader->vprp.dwVTotalInLines=msg->win_h;
	aviheader->vprp.dwVerticalRefreshRate=30;
	aviheader->vprp.dwFrameAspectRatio=0x00100009;
	aviheader->vprp.VideoFormatToken=0;
	aviheader->vprp.VideoStandard=0;
	aviheader->vprp.nbFieldPerFrame =1;
	aviheader->vprp.FieldInfo.CompressedBMHeight=msg->win_h;
	aviheader->vprp.FieldInfo.CompressedBMWidth=msg->win_w;
	aviheader->vprp.FieldInfo.ValidBMHeight=msg->win_h;
	aviheader->vprp.FieldInfo.ValidBMWidth=msg->win_w;
	aviheader->vprp.FieldInfo.ValidBMXOffset=0;
	aviheader->vprp.FieldInfo.ValidBMYOffset=0;
	aviheader->vprp.FieldInfo.VideoXOffsetInT=0;
	aviheader->vprp.FieldInfo.VideoYValidStartLine=0;
	
	//Extended AVI Header (dmlh)
	aviheader->obml.fcc=0x5453494c;			//"LIST"
	aviheader->obml.cb=sizeof(ODMLExtendedAVIHeader) + sizeof(aviheader->obml.fcctype);			
	aviheader->obml.fcctype =0x6c6d646f;			//"odml"
	aviheader->strodml.OpenDML_Header=0x686c6d64; //"dmlh"
	aviheader->strodml.size = sizeof(ODMLExtendedAVIHeader)-8;
	aviheader->strodml.dwTotalFrames=0;			//"TotalFrame"
	/****************secotor junk fill******************/
	aviheader->sector1_junk.dwFourCC= 0x4b4e554a;
	aviheader->sector1_junk.dwSize=260;
	
	aviheader->sector2_junk.dwFourCC= 0x4b4e554a;
	aviheader->sector2_junk.dwSize=512-(SUPERIDXLEN*16)%512 - 8;
	
	aviheader->sector3_junk.dwFourCC= 0x4b4e554a;
	aviheader->sector3_junk.dwSize=428;
	
	aviheader->sector4_junk.dwFourCC= 0x4b4e554a;
	aviheader->sector4_junk.dwSize=368;
	
	aviheader->sector5_junk.dwFourCC= 0x4b4e554a;
	aviheader->sector5_junk.dwSize=512-(SUPERIDXLEN*16)%512 - 8;
	
	//printf("j1:%X\tj2:%X\tj3:%X\tj4:%X\tj5:%X\n",aviheader->sector1_junk.dwSize,aviheader->sector2_junk.dwSize,aviheader->sector3_junk.dwSize,aviheader->sector4_junk.dwSize,aviheader->sector5_junk.dwSize);
	/*****************************************************/
	printf("VIDEO REC :file head size %d \n",_ODML_AVI_HEAD_SIZE__);

#if FATFS_OPENDML
	ret =  osal_fwrite(headerbuf,1, _ODML_AVI_HEAD_SIZE__,f);			// 4K AVI head
	if(ret != _ODML_AVI_HEAD_SIZE__) {
		printf("vai -w hd err\n");
		return -1;
	}
#endif
	//printf("header ok\n");
	return 1;	
}
//uint32 junk[128];

bool stdidx_write2disk(bool type,void *f,uint32 num,ODMLBUFF *buff,void *f_write)
{

	uint32 junk[2];
	
	uint64 stdofset=0;
	uint32 size=0;
	uint32 fillsize=0;
    uint64 *p64;
	AVISTDINDEXHEAD stdindxhead;
	int ret = 0;
	memset((void *)&stdindxhead,0,sizeof(AVISTDINDEXHEAD));
	
	size = sizeof(stdindx)*num + sizeof(AVISTDINDEXHEAD) ;
	if(num==0)return true;
	//size = sizeof(stdindx)*num;
	stdindxhead.wLongsPerEntry = 2;
	stdindxhead.bIndexSubType=0;
	//stdindxhead.dwReserved= 0;
	stdindxhead.bIndexType= AVI_INDEX_OF_CHUNKS;
	stdindxhead.cb = sizeof(AVISTDINDEXHEAD)-8+sizeof(stdindx)*num;
    p64 = (uint64 *)&stdindxhead.qwBaseOffset ;
	*p64 = buff->qwOffset;
	stdindxhead.nEntriesInUse = num;
	#if FATFS_OPENDML
		stdofset =  osal_ftell(f);
	#else
		stdofset = 0;
	#endif
	fillsize =512-((sizeof(AVISTDINDEXHEAD)+ num*sizeof(stdindx))%512);
	if(type){
		//	printf("aia");
			stdindxhead.fcc = 0x31307869; // "ix01"
			stdindxhead.dwChunkId = 0x62773130; //"01dw"
			#if FATFS_OPENDML
			ret = osal_fwrite((void*)&stdindxhead, 1,sizeof(AVISTDINDEXHEAD),f);
			ret |= osal_fwrite ((void*)&buff->astdindx,1, sizeof(stdindx)*num,f);	/* Write data to a file */
			#endif
			buff->aidxcount = 0;
			memset((void *)&buff->astdindx,0,sizeof(stdindx)*num);

	}
	else{
			//printf("via");
			stdindxhead.fcc = 0x30307869; // "ix00"
			stdindxhead.dwChunkId = 0x63643030;	//"00cd"
       
            #if FATFS_OPENDML
			ret |= osal_fwrite ((void*)&stdindxhead, 1,sizeof(AVISTDINDEXHEAD) ,f);	/* Write data to a file */
			ret |= osal_fwrite ((void*)&buff->vstdindx,1,sizeof(stdindx)*num,f);	/* Write data to a file */
			#endif
			buff->vidxcount = 0;
			memset((void *)&buff->vstdindx,0,sizeof(stdindx)*num);
	}
	if(fillsize != 512 ){
		//	printf("fill size:%d",fillsize);
			//memset((void *)junk,0,512);
			junk[0] = 0x4b4e554a;//junk
			junk[1] = fillsize-8;
			#if FATFS_OPENDML
			ret |= osal_fwrite ((void*)&junk,1, 8,f);//写入junk头,为什么没有判断是否足够8字节?这里有点小问题,暂时不管
			ret |= osal_fseek(f, osal_ftell(f)+fillsize-8);
			#endif
		}
	
	
	if(ret == 0)return false;
	return supindxbuff_updata(type,stdofset,size,num,buff);
}


/**************************************************************
 * name 	:ODMLadd_indx
 * function :when the video or audio frame wtite to sd card record
 * 			 their offset and size to the stdindx buff
 * *************************************************************/

bool ODMLadd_indx(bool type,void *f, ODMLBUFF *buff)//0 is video
{
	//static uint32 count = 0;
	bool ret = true;
	//printf("%s:%d\t%X\n",__FUNCTION__,__LINE__,f);
	if(type)
	{
		buff->astdindx[buff->aidxcount].dwOffset = buff->fileofset+8 - buff->qwOffset;
		buff->astdindx[buff->aidxcount].dwSize = buff->size-8;	
		buff->aidxcount++;
		if(buff->aidxcount == StdIDXBUFLEN) {
		#if FATFS_OPENDML
		ret &= stdidx_write2disk(1,f,buff->aidxcount,buff,osal_fwrite);
		#else
		ret &= stdidx_write2disk(1,f,buff->aidxcount,buff,NULL);
		#endif
#if SYNC_EN
			if(buff->vidxcount != 0){
				ret&= stdidx_write2disk(0,f,buff->vidxcount,buff,osal_fwrite);
			}
			OdmlSyncAVIInfo(f,buff,odmlattr.pcm);
#endif	

		}
		
	}
	else
	{
		buff->vstdindx[buff->vidxcount].dwOffset = buff->fileofset+8 - buff->qwOffset;
		buff->vstdindx[buff->vidxcount].dwSize = buff->size-8;	
		buff->vidxcount++;
		if(buff->vidxcount == StdIDXBUFLEN) {
			#if FATFS_OPENDML
			ret&=stdidx_write2disk(0,f,buff->vidxcount,buff,osal_fwrite);
			#else
			ret &= stdidx_write2disk(1,f,buff->aidxcount,buff,NULL);
			#endif
#if SYNC_EN
			if(buff->aidxcount != 0){
			   ret&=stdidx_write2disk(1,f,buff->aidxcount,buff,osal_fwrite);
			}
			OdmlSyncAVIInfo(f,buff,odmlattr.pcm);
#endif	
		}

	}
	
	return ret;

}

struct avi_test_head
{
	uint32 head;
	uint32 size;
};


//
void opendml_write_video(ODMLBUFF *odml_buff,void *fp,void *jpeg,int size)
{
		struct avi_test_head  avi_head;
		struct avi_test_head  junk_head;
		int tell;
		int flag;
		static int w_flag = 0;
		odml_buff->fileofset = osal_ftell(fp);
		
		//预先计算是否要写junk
		tell = osal_ftell(fp);

		//8是00dc size
		tell = 0x200 - ((tell+((size+3)&(~0x03))+8)&(0x1ff));
		if(tell>8)
		{
			flag = 1;

			//printf("tell:%d\n",tell);
			avi_head.size = ((size+3)&(~0x03));
			avi_head.head = 0x63643030;//00dc
			
			junk_head.size = tell-8;
			junk_head.head = 0x4b4e554a;//junk

		}
		else
		{
			flag = 0;
			avi_head.size = ((size+3)&(~0x03)) + tell;
			avi_head.head = 0x63643030;

		}
		osal_fwrite(&avi_head,1,8,fp);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		//这里替换要实现写图片的操作
		osal_fwrite(jpeg,1,size,fp);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		if(avi_head.size-size)
		{
			osal_fseek(fp,osal_ftell(fp)+avi_head.size-size);
		}
		
		odml_buff->size = avi_head.size+8;
		
		if(!w_flag)
		{
			w_flag = 1;
			printf("size:%X\n",odml_buff->size );
		}

		
		//图片4byte对齐
		if(odml_buff->size%4)
		{
			odml_buff->size += 4-(odml_buff->size%4);
			osal_fseek(fp,osal_ftell(fp)+(4-(odml_buff->size%4)));
		}
		
		
		//junk填写,512对齐,不足8byte,则当作是图片数据,大于等于8byte,则填写junk
		if(flag)
		{
			osal_fwrite(&junk_head,1,8,fp);
			osal_fseek(fp,osal_ftell(fp)+junk_head.size);
			
		}

		odml_buff->inserofset= odml_buff->fileofset; //save jfile ofset for insert jpg
		odml_buff->insersize= odml_buff->size;
		odml_buff->vframecnt++;
		ODMLadd_indx(0,fp,odml_buff);
}



void opendml_write_video2(ODMLBUFF *odml_buff,void *fp,opendml_write_priv write_priv,int size,void *d)
{

		if(!fp)
		{
			return;
		}
		struct avi_test_head  avi_head;
		struct avi_test_head  junk_head;
		int tell;
		int flag;
		static int w_flag = 0;
		odml_buff->fileofset = osal_ftell(fp);
		
		//预先计算是否要写junk
		tell = osal_ftell(fp);

		//8是00dc size
		tell = 0x200 - ((tell+((size+3)&(~0x03))+8)&(0x1ff));
		if(tell>8)
		{
			flag = 1;

			//printf("tell:%d\n",tell);
			avi_head.size = ((size+3)&(~0x03));
			avi_head.head = 0x63643030;//00dc
			
			junk_head.size = tell-8;
			junk_head.head = 0x4b4e554a;//junk

		}
		else
		{
			flag = 0;
			avi_head.size = ((size+3)&(~0x03)) + tell;
			avi_head.head = 0x63643030;

		}
		osal_fwrite(&avi_head,1,8,fp);
		
		
		//这里替换要实现写图片的操作
		//osal_fwrite(jpeg,1,size,fp);
		write_priv(fp,d,size);
		

		if(avi_head.size-size)
		{
			osal_fseek(fp,osal_ftell(fp)+avi_head.size-size);
		}
		
		odml_buff->size = avi_head.size+8;
		
		if(!w_flag)
		{
			w_flag = 1;
			printf("size:%X\n",odml_buff->size );
		}

		
		//图片4byte对齐
		if(odml_buff->size%4)
		{
			odml_buff->size += 4-(odml_buff->size%4);
			osal_fseek(fp,osal_ftell(fp)+(4-(odml_buff->size%4)));
		}
		
		
		//junk填写,512对齐,不足8byte,则当作是图片数据,大于等于8byte,则填写junk
		if(flag)
		{
			osal_fwrite(&junk_head,1,8,fp);
			osal_fseek(fp,osal_ftell(fp)+junk_head.size);
			
		}

		odml_buff->inserofset= odml_buff->fileofset; //save jfile ofset for insert jpg
		odml_buff->insersize= odml_buff->size;
		odml_buff->vframecnt++;
		ODMLadd_indx(0,fp,odml_buff);
}


void opendml_write_audio(ODMLBUFF *odml_buff,void *fp,opendml_write_priv write_priv,int size,void *d)
{

		if(!fp)
		{
			return;
		}
		struct avi_test_head  avi_head;
		struct avi_test_head  junk_head;
		int tell;
		int flag;
		static int w_flag = 0;
		odml_buff->fileofset = osal_ftell(fp);
		
		//预先计算是否要写junk
		tell = osal_ftell(fp);

		//8是00dc size
		tell = 0x200 - ((tell+((size+3)&(~0x03))+8)&(0x1ff));
		if(tell>8)
		{
			flag = 1;

			//printf("tell:%d\n",tell);
			avi_head.size = ((size+3)&(~0x03));
			avi_head.head = 0x62773130;//00dc
			
			junk_head.size = tell-8;
			junk_head.head = 0x4b4e554a;//junk

		}
		else
		{
			flag = 0;
			avi_head.size = ((size+3)&(~0x03)) + tell;
			avi_head.head = 0x63643030;

		}
		osal_fwrite(&avi_head,1,8,fp);
		
		
		//这里替换要实现写图片的操作
		//osal_fwrite(jpeg,1,size,fp);
		write_priv(fp,d,size);
		

		if(avi_head.size-size)
		{
			osal_fseek(fp,osal_ftell(fp)+avi_head.size-size);
		}
		
		odml_buff->size = avi_head.size+8;
		
		if(!w_flag)
		{
			w_flag = 1;
			printf("size:%X\n",odml_buff->size );
		}

		
		//图片4byte对齐
		if(odml_buff->size%4)
		{
			odml_buff->size += 4-(odml_buff->size%4);
			osal_fseek(fp,osal_ftell(fp)+(4-(odml_buff->size%4)));
		}
		
		
		//junk填写,512对齐,不足8byte,则当作是图片数据,大于等于8byte,则填写junk
		if(flag)
		{
			osal_fwrite(&junk_head,1,8,fp);
			osal_fseek(fp,osal_ftell(fp)+junk_head.size);
			
		}

		odml_buff->inserofset= odml_buff->fileofset; //save jfile ofset for insert jpg
		odml_buff->insersize= odml_buff->size;
		odml_buff->aframecnt++;
		ODMLadd_indx(1,fp,odml_buff);
}




int insert_frame(ODMLBUFF *buff,void *pavi,int *tim_diff)
{
    /*??????*/
   // static int time_diff = 0;
    int add_num =0;
		int i;
		int insert_num = 1;
    if(buff->cur_timestamp==0 || 0 == buff->last_timestamp) return 0;
		//增加时间差,后面还需要减去一帧的时间差
    *tim_diff += buff->cur_timestamp - buff->last_timestamp ;//-  buff->ef_time;


		//放大1000倍,来提高精度
		//add_num = time_diff * buff->ef_fps/1000;这样才是准确的补帧数量,但通过放大1000倍,去除小数,提高精度
		add_num = (*tim_diff) * buff->ef_fps;				//计算补帧数量,通过时间差来计算,使用时间差*帧率/1000来准确计算补帧数量
		//减去一帧,已经保存的一帧
		add_num -= 1000;
	
		/*
    if(time_diff > buff->ef_time){
        printf("tmdif %d \n",time_diff);
        //add_num = time_diff / buff->ef_time;
    }
    */
    
    if(add_num > 1000){
			 
			 	add_num-=1000;//减去1000,代表减去1帧
        *tim_diff = add_num/buff->ef_fps;//计算剩下的补帧时间
        //通过音频校准视频,方法是插帧,但音频帧-1是由于可能存在有些视频帧存在缓存中,因此-1
        if(buff->aframecnt!=0 && (buff->aframecnt-1)*(PCM_SHEET_SIZE-8)*3>buff->vframecnt*100*32+(*tim_diff)*96)
        {
        	//printf("aframecnt:%d\tvframecnt:%d\r\n",buff->aframecnt,buff->vframecnt);
        	*tim_diff = (buff->aframecnt-1)*(PCM_SHEET_SIZE-8)/32-buff->vframecnt*100/3;//音频比视频多的时间就是需要补帧的数量,原公式为(buff->aframecnt-1)*(PCM_SHEET_SIZE-8)/32000*1000-buff.vframecnt/30*1000;
        	add_num = (*tim_diff) * buff->ef_fps;	//重新计算补帧的数量,倍数为1000
        }

				
        //printf("tmdif %d\t%d\t%d\n",time_diff,buff->vframecnt,buff->cur_timestamp-test_time_start2);//odmlbuff.aframecnt
				//计算一次补多少帧,100ms大约3帧,则一次补1帧
				if(*tim_diff<150)
				{
					insert_num = 1;
				}
				//250ms大约9帧,则一次补2帧
				else if(*tim_diff<250)
				{
					insert_num = 2;
				}
				//500ms大约16-17帧,则一次补2帧
				else if(*tim_diff<500)
				{
					insert_num = 3;
				}
				//其他的话,就补4帧
				else
				{
					insert_num = 4;
				}

        buff->vframecnt+=insert_num;
				*tim_diff = ((add_num-(insert_num-1)*1000)/buff->ef_fps);
				for(i=0;i<insert_num;i++)
				{
					printf("@");//补帧的标志
	        if(!ODMLadd_indx(0,pavi,buff)){
	            printf("add indx error \n");
	        }
				}
       // add_num --;
    }
		//计算准确的时间差
		else
		{
			*tim_diff = add_num/buff->ef_fps;//计算剩下的补帧时间
			insert_num = 0;
		}

		return insert_num;
}








