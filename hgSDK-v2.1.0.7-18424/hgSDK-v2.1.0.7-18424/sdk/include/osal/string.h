#ifndef __OS_STRING_H__
#define __OS_STRING_H__
#ifdef __MBED__
#include "osal/mbed/string.h"
#endif

#ifdef __CSKY__
#include "osal/csky/string.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void *_os_malloc(int size);
void _os_free(void *ptr);
void *_os_zalloc(int size);
void *_os_realloc(void *ptr, int size);
void *_os_calloc(size_t nmemb, size_t size);

void *_os_malloc_t(int size, const char *func, int line);
void _os_free_t(void *ptr);
void *_os_zalloc_t(int size, const char *func, int line);
void *_os_realloc_t(void *ptr, int size, const char *func, int line);
void *_os_calloc_t(size_t nmemb, size_t size, const char *func, int line);

void hw_memcpy(void *dest, const void *src, uint32 size);
void hw_memset(void * dest, uint8 val, uint32 n);
void hw_memcpy0(void *dest, const void *src, uint32 size);

void *_os_memcpy(void *str1, const void *str2, int32 n);
char *_os_strcpy(char *dest, const char *src);
void *_os_memset(void *str, int c, int32 n);
void *_os_memmove(void *str1, const void *str2, size_t n);
char *_os_strncpy(char *dest, const char *src, int32 n);
int _os_sprintf(char *str, const char *format, ...);
int _os_vsnprintf(char *s, size_t n, const char *format, va_list arg);
int _os_snprintf(char *str, size_t size, const char *format, ...);

#ifdef MEM_TRACE //mem trace
#ifndef os_malloc
#define os_malloc(s) _os_malloc_t(s, __FUNCTION__, __LINE__)
#endif
#ifndef os_free
#define os_free(p)   do{ _os_free_t((void *)p); (p)=NULL;}while(0)
#endif
#ifndef os_zalloc
#define os_zalloc(s) _os_zalloc_t(s, __FUNCTION__, __LINE__)
#endif
#ifndef os_realloc
#define os_realloc(p,s) _os_realloc_t(p, s, __FUNCTION__, __LINE__)
#endif
#ifndef os_calloc
#define os_calloc(p,s) _os_calloc_t(p, s, __FUNCTION__, __LINE__)
#endif
#else
#ifndef os_malloc
#define os_malloc(s) _os_malloc(s)
#endif
#ifndef os_free
#define os_free(p)   do{ _os_free((void *)p); (p)=NULL;}while(0)
#endif
#ifndef os_zalloc
#define os_zalloc(s) _os_zalloc(s)
#endif
#ifndef os_realloc
#define os_realloc(p,s) _os_realloc(p,s)
#endif
#ifndef os_calloc
#define os_calloc(p,s) _os_calloc(p, s)
#endif
#endif

#ifndef MAC2STR
#define MAC2STR(a) (a)[0]&0xff, (a)[1]&0xff, (a)[2]&0xff, (a)[3]&0xff, (a)[4]&0xff, (a)[5]&0xff
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

#ifndef STR2MAC
#define STR2MAC(s, a) str2mac((char *)(s), (uint8 *)(a))
#endif

#ifndef IP2STR
#define IP2STR(ip) ((ip)>>24)&0xff,((ip)>>16)&0xff,((ip)>>8)&0xff,(ip&0xff)
#define IPSTR "%d.%d.%d.%d"
#endif

#define MAC_EQU(a1,a2)    (os_memcmp((a1), (a2), 6)==0)
#define IS_BCAST_ADDR(a)  (((a)[0]&(a)[1]&(a)[2]&(a)[3]&(a)[4]&(a)[5]) == 0xff)
#define IS_MCAST_ADDR(a)  ((a)[0]&0x01)
#define IS_ZERO_ADDR(a)   (!((a)[0] | (a)[1] | (a)[2] | (a)[3] | (a)[4] | (a)[5]))

#define os_printf(fmt, ...)  hgprintf("[%u]"fmt, os_jiffies(), ##__VA_ARGS__)
#define _os_printf(fmt, ...) hgprintf(fmt, ##__VA_ARGS__)


void disable_print(void);
void enable_print(void);
void hgprintf(const char *fmt, ...);

typedef void (*osprint_hook)(void *priv, char *msg);
void osprint_redirect(osprint_hook hook, void *priv);

int32 hex2int(char c);
int32 hex2char(char *hex);
int32 hex2bin(char *hex, uint8 *buf, uint32 len);
void str2mac(char *macstr, uint8 *mac);
void dump_hex(char *str, uint8 *data, uint32 len, int32 newline);
void dump_key(char *str, uint8 *key, uint32 len, uint32 sp);
void key_str(uint8 *key, uint32 key_len, uint8 *str_buf);
void *os_memdup(const void *ptr, uint32 len);
int32 os_random_bytes(uint8 *data, int32 len);

#ifdef __cplusplus
}
#endif
#endif
