#ifndef _FTP_FS_H_
#define _FTP_FS_H_
#include "fatfs/ff.h"
#include "osal_file.h"

void ftp_getcwd( char *buf, unsigned int len );

//int  ftp_chdir( char *buf );

//void *ftp_opendir( char *path );
//void  ftp_closedir( void *HDIR );
//void *ftp_readdir( void *HDIR );
//unsigned int ftp_formatdir( void *HFIL, char *buf, unsigned int len );

//void *ftp_openfile_read( char *path );
//unsigned int ftp_readfile( void *HFIL, char *buf, unsigned int len );
//void ftp_lseek(void *HFIL, unsigned int pos);
//int ftp_closefile( void *HFIL );

//void *ftp_getstat( char *path );
//void ftp_freestat( void *HFIL );
unsigned int ftp_formatdir( void *HFIL, char *buf, unsigned int len );
//unsigned int ftp_filesize( void *HFIL );



FRESULT ftp2_getcwd(TCHAR* buff,UINT len);
FRESULT ftp_mount(FATFS* fs,const TCHAR* path,BYTE opt);
FRESULT ftp_chdir(const TCHAR* path);
DIR* ftp_opendir(const TCHAR* path);
FRESULT ftp_closedir(DIR *dp);
FILINFO* ftp_readdir(DIR* dir);
FRESULT ftp_stat(const TCHAR* path,FILINFO* fno);
int ftp_setcwd2( char *path );



void *virdisk(char *path);



















#endif /* _FTP_FS_H_ */
