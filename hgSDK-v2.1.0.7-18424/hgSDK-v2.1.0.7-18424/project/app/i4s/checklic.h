#ifndef HEADER_CHECK_LIC_U_H
#define HEADER_CHECK_LIC_U_H
#include <stddef.h>


#ifdef  __cplusplus
extern "C" {
#endif



int checklic10(char*licbuf,char*modlename);

int getlic10(const char*sn,const char*lic,char*outlicbuf);
int getlic10_frommac(const char*sn,const unsigned char*mac,char*outlicbuf);






#ifdef  __cplusplus
}
#endif

#endif /* !HEADER_AES_H */
