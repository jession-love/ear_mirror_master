/**@file avidemux.c
 * @brief avi解包支持
 * @details 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avitype.h"
#include "osal_file.h"
#include "avidemux.h"
//#include "common.h" 

#define FourCC(aa, bb, cc, dd) ( ((aa)&0xFF) | (((bb)<<8)&0xFF00) | (((cc)<<16)&0xFF0000) | (((dd)<<24)&0xFF000000) )
#define FourCCCC(i) (i)&0xFF,(i>>8)&0xFF,(i>>16)&0xFF,(i>>24)&0xFF

#ifndef BIT
#define BIT(b) (1<<(b))
#endif





// 在parse階段，哪些LIST是需要深入分析的
static int need_parse_list(uint32_t fourcc)
{
  return
    fourcc == FourCC('A','V','I',' ') ||
    fourcc == FourCC('h','d','r','l') ||
    fourcc == FourCC('s','t','r','l') ||
    0;
}

#define MAX_LIST_DEPTH 5



//avi数据头解析，不是全局变量
void *avidemux_parse(void *fp,void *aviinfo_point)
{
  AVISTREAMHEADER strh;
  struct _aviindex_chunk indx;
  struct avistrinfo *str = NULL;
  struct aviinfo *aviinfo = (struct aviinfo *)aviinfo_point;
  uint32_t list_end[MAX_LIST_DEPTH]; // RIFF/LIST棧，記錄其結束地址
  uint32_t lvl;
  uint32_t fcc_size[3];              // 緩存讀入的FourCC及其size
  uint32_t pos;
  uint32_t err;

  lvl = 0;
  list_end[lvl] = osal_fsize (fp);
  printf ("%s : file size %08X\n", __func__, list_end[lvl]);

  pos = 0;
  while (pos < list_end[lvl]) {
    if (osal_fread ((char*)fcc_size, 1, 8, fp) < 8) {
      printf ("%s : read fourcc & size error\n", __func__);
      break;
    }
//    printf ("%s : %c%c%c%c %08X", __func__, FourCCCC(fcc_size[0]), fcc_size[1]);
    pos += 8;
    // 檢查長度
    if (fcc_size[1] + pos > list_end[lvl]) {
      printf ("%s : fourcc length error\n", __func__);
      break;
    }

    err = 0;
    switch (fcc_size[0]) {
    case FourCC('R','I','F','F'):
    case FourCC('L','I','S','T'):
      // 檢查LIST的類型
      if (osal_fread((char*)&fcc_size[2], 1, 4, fp) < 4) {
        printf ("%s : read LIST name error\n", __func__);
        err = 1;
        break;
      }
      if (need_parse_list(fcc_size[2]) && lvl < MAX_LIST_DEPTH-1) {
        // 需要分析這個LIST，並且棧空間還夠用
        list_end[++lvl] = pos + fcc_size[1];
        pos += 4;
//        printf ("%s : pos  %08X %08X %08X", __func__, pos, list_end[lvl], fcc_size[1]);
      } else {
        pos += fcc_size[1];
        osal_fseek (fp, pos);
      }
      break;
    case FourCC('s','t','r','h'):
      if (osal_fread(((void*)&strh)+2*sizeof(DWORD), 1, sizeof(strh)-2*sizeof(DWORD), fp) < sizeof(strh)-2*sizeof(DWORD)) {
        printf ("%s : read strh type error\n", __func__);
        err = 1;
        break;
      }
      pos += fcc_size[1];
//      printf ("found strh %s %dx%d %d frames", &strh.fccType, strh.rcFrame.right, strh.rcFrame.bottom, strh.dwLength);

      if (strh.fccType == FourCC('v','i','d','s')) {
        str = &aviinfo->str[0];
        aviinfo->strmsk |= BIT(0);
      } else {
        str = &aviinfo->str[1];
        aviinfo->strmsk |= BIT(2);
      }
      str->dwScale       = strh.dwScale * 1000;
      str->dwRate        = strh.dwRate;
      str->dwTotalFrames = strh.dwLength;
      str->cur = 0;
      str->avi = aviinfo;
      break;
    case FourCC('i','n','d','x'):
      if (NULL == str) {
        printf ("%s : indx but no strh\n", __func__);
        err = 1;
        break;
      }
      if (osal_fread(((void*)&indx)+2*sizeof(DWORD), 1, sizeof(indx)-2*sizeof(DWORD), fp) < sizeof(indx)-2*sizeof(DWORD)) {
        printf ("%s : read indx type error\n", __func__);
        err = 1;
        break;
      }
      // 先緩存超級索引的前512B，以便獲取duration
      if (osal_fread((char*)str->indx, 1, 512, fp) < 512) {
        printf ("%s : read indx type error\n", __func__);
        err = 1;
        break;
      }
      str->dwIndexNum  = indx.nEntriesInUse;
      str->dwIndexBase = pos + sizeof(indx)-2*sizeof(DWORD);
      str->dwDuration  = str->indx[0].dwDuration;
      str->cached_indx_base = 0;
      str->cached_ixnn_base = 0;
      str->super_idx        = ~0;
//      printf ("found indx @%08X", str->dwIndexBase);
      pos += fcc_size[1];
      // 雖然緩存了512B的超級索引，但該長度不是最終的長度
      osal_fseek (fp, pos);
      break;
    default:
      pos += fcc_size[1];
      osal_fseek (fp, pos);
    } // switch (fcc_size[0])
    if (err) break;

    if (pos == list_end[lvl]) {
      // pop
      --lvl;
    }
    if (pos > list_end[lvl]) {
      printf ("%s : list overread\n", __func__);
      break;
    }
  }  // while (pos < list_end[lvl])
  printf ("str[0] : %d @%08X:%08X\n", (int)aviinfo->str[0].dwTotalFrames, (int)aviinfo->str[0].dwIndexBase, (int)aviinfo->str[0].dwIndexNum);
  aviinfo->fp = fp;
  if(err)
  {
  	return NULL;
  }
  return aviinfo;
}



  
#if 0
void print_aviinfo(struct aviinfo *info)
{
	struct avistrinfo *str;
	str = &info->str[0];
	printf("dwsale:%d\n",str->dwScale);
	printf("dwRate:%d\n",str->dwRate);
	printf("dwTotalFrames:%d\n",str->dwTotalFrames);
	printf("dwIndexNum:%d\n",str->dwIndexNum);
	printf("dwIndexBase:%d\n",str->dwIndexBase);
	printf("dwDuration:%d\n",str->dwDuration);
	printf("dwEntryNum:%d\n",str->dwEntryNum);
	printf("dwEntryBase:%d\n",str->dwEntryBase);
}
#endif


/**@brief 在sync之後，準備進入讀數據的操作
 * @param str [in] 流
 * @param base [out] 幀在文件中的偏移
 * @param size [oit] 幀的內容長度
 * @return 
 */
/*注意:超级索引部分可能现在只能存32个，一个超级索引指向300条标准索引，因此最大帧数为32*300=9600帧，320秒的视频*/
uint32_t avidemux_read_begin(struct avistrinfo *str, uint32_t *base, uint32_t *size)
{
  uint32_t cont;
  uint32_t super_idx = str->cur / str->dwDuration; // 標準索引在超級索引上的序號
  //super_idx* sizeof(struct _avisuperindex_entry);		这里修改了，好像原来会导致超级索引表复写
  uint32_t super_sec = super_idx /32; // 先得到相對扇區基址
  uint32_t super_ind = super_idx % (512/sizeof(struct _avisuperindex_entry));

//读到最大帧数后返回size为0后退出 
  if(str->cur>=str->dwTotalFrames)
  {
  	printf("avi no frame!\r\n");
		str->cur = str->dwTotalFrames;
  	*size = 0;
		return 1;
  }
  super_sec = str->dwIndexBase + super_sec*512;
  // 讀入超級索引
  if (super_sec != str->cached_indx_base) {
	printf ("Vsuper_sec %08X\r\n", super_sec);
    str->cached_indx_base = super_sec;
    osal_fseek(str->avi->fp, super_sec);
    osal_fread ((char*)str->indx, 1, 512, str->avi->fp);
  }
  if (super_idx != str->super_idx) {
	printf ("Vsuper_idx %08X %08X\r\n", (int)super_idx, (int)str->indx[super_ind].dwOffsetl);
    struct _aviindex_chunk *stdidx = (struct _aviindex_chunk *) str->ixnn;
    str->super_idx = super_idx;
    osal_fseek (str->avi->fp, str->indx[super_ind].dwOffsetl);
    osal_fread ((char*)stdidx, 1, 512, str->avi->fp);
    str->dwEntryNum  = stdidx->nEntriesInUse;
    str->dwEntryBase = stdidx->dwBaseOffsetl;
    str->cached_ixnn_base = str->indx[super_ind].dwOffsetl;
  }
  uint32_t std_idx = str->cur - super_idx * str->dwDuration + 4;  // ixnn頭上32B
  uint32_t std_sec = (std_idx * sizeof(struct _avistdindex_entry)) & ~511;
  uint32_t std_ind = std_idx % (512/sizeof(struct _avistdindex_entry));
//printf ("std_idx %d, dwDuration %d std_sec %08X %08X\r\n", std_idx, str->dwDuration, std_sec, ~511);
  std_sec += str->indx[super_ind].dwOffsetl;  // 當前僅支持4G文件
  // 讀入標準索引
  if (std_sec != str->cached_ixnn_base) {
	printf ("Vstd_sec %08X\r\n", std_sec);
    str->cached_ixnn_base = std_sec;
    osal_fseek (str->avi->fp, std_sec);
    osal_fread ((char*)str->ixnn, 1, 512, str->avi->fp);
  }
  cont = str->ixnn[std_ind].dwOffset + str->dwEntryBase;
  //osal_fseek (str->avi->fp, cont - 8);//到00dc或者00wb的位置
  osal_fseek (str->avi->fp, cont);//实际图片或者音频的起始位置
  //printf("cont:%X\n",cont);
  if (base) *base = cont;
  if (size) *size = str->ixnn[std_ind].dwSize;
  return 0;
}
 


uint32_t avidemux_read (struct avistrinfo *str, uint8_t *buf, uint32_t len)
{
  uint32_t l;
  uint32_t len_tmp = len;
  int first_read = 0;
  while(len_tmp)
  {

  	  //第一次读取,少读4字节,是数据头,然后后面就以512的倍数去读取
  	  if(!first_read && (len_tmp >= (sizeof(str->read_buf_tmp)-4)))
  	  {
		  l = osal_fread (str->read_buf_tmp, 1, sizeof(str->read_buf_tmp)-4, str->avi->fp);
		  if(l == 0)
		  {
			  printf("%s:%d err\n",__FUNCTION__,__LINE__);
			  return 0;
		  }
		  //printf("%s:%d\tl:%d\n",__FUNCTION__,__LINE__,l);
		  memcpy(buf,str->read_buf_tmp,sizeof(str->read_buf_tmp)-4);
		  buf += (sizeof(str->read_buf_tmp)-4);
		  len_tmp -= (sizeof(str->read_buf_tmp)-4);
		  first_read = 1;

  	  }
	  else if(len_tmp >= sizeof(str->read_buf_tmp))
	  {
	  	l = osal_fread (str->read_buf_tmp, 1, sizeof(str->read_buf_tmp), str->avi->fp);
		if(l == 0)
		{
			printf("%s:%d err\n",__FUNCTION__,__LINE__);
			return 0;
		}
		//printf("%s:%d\tl:%d\n",__FUNCTION__,__LINE__,l);
		memcpy(buf,str->read_buf_tmp,sizeof(str->read_buf_tmp));
		buf += sizeof(str->read_buf_tmp);
		len_tmp -= sizeof(str->read_buf_tmp);
	  }
	  else if(len_tmp > 0)
	  {
	  	l = osal_fread (str->read_buf_tmp, 1, len_tmp, str->avi->fp);
		if(l == 0)
		{
			printf("%s:%d err\n",__FUNCTION__,__LINE__);
			return 0;
		}
		//printf("%s:%d\tl:%d\n",__FUNCTION__,__LINE__,l);
		hw_memcpy0(buf,str->read_buf_tmp,len_tmp);
		buf += len_tmp;
		len_tmp  = 0;

	  }
  }
  return len;
}

void *avidemux_stream (struct aviinfo *avi, uint32_t idx)
{
  return &avi->str[idx];
}

uint32_t avidemux_set_cur (struct avistrinfo *str,uint32_t cur)
{
  str->cur = cur;
  return str->cur;
}

 
#if 1



#endif

