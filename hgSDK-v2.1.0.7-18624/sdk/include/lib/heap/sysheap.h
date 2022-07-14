
#ifndef _SYS_HEAP_H_
#define _SYS_HEAP_H_

enum SYSHEAP_FLAGS {
    SYSHEAP_FLAGS_MEM_LEAK_TRACE     = (1u<<0),
    SYSHEAP_FLAGS_MEM_OVERFLOW_CHECK = (1u<<1),
};

int sysheap_init(void *heap_start, unsigned int heap_size, unsigned int flags);
void *sysheap_alloc(int size, const char *func, int line);
void sysheap_free(void *ptr);
void sysheap_collect_init(void);
int32 sysheap_of_check(void *ptr, uint32 size);
#ifdef MPOOL_ALLOC
void sysheap_status(void);
#else
#define sysheap_status()
#endif

#endif

