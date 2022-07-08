#ifndef _HGIC_SKBUFF_H_
#define _HGIC_SKBUFF_H_

#include <osal/atomic.h>
#include <osal/string.h>
#include "list.h"

#ifndef CONFIG_UMAC
struct sk_buff {
    struct sk_buff *next, *prev, *clone;
    uint8   cb[48];
    uint8  *tail;
    uint8  *data;
    uint8  *head;
    uint8  *end;
    /* clear below flags when skb free */
    uint16  len;
    uint8   acked: 1, cloned: 1, lmaced: 1, pkt_type: 3, tx: 1, src_in: 1;
    uint8   aid, priority;
    uint32  lifetime;
    void   *txinfo;
    void   *sta;
    /////////////////////////////////////
    atomic_t users;
    void (*free)(void *free_priv, struct sk_buff *skb);
    void *free_priv;
#ifdef SKB_TRACE
    char  *func;
    int32  line;
    uint32 magic;
#endif
};

static inline uint8 *skb_put(struct sk_buff *skb, uint32 len)
{
    uint8 *tmp = skb->tail;
#ifdef SKB_TRACE
    ASSERT((skb)->magic == 0x1a2b3c4d);
#endif
    ASSERT(skb->tail + len <= skb->end);
    skb->tail += len;
    skb->len  += len;
    return tmp;
}

static inline uint8 *skb_push(struct sk_buff *skb, uint32 len)
{
#ifdef SKB_TRACE
    ASSERT((skb)->magic == 0x1a2b3c4d);
#endif
    ASSERT(skb->data - len >= skb->head);
    skb->data -= len;
    skb->len  += len;
    return skb->data;
}

static inline uint8 *skb_pull(struct sk_buff *skb, uint32 len)
{
#ifdef SKB_TRACE
    ASSERT((skb)->magic == 0x1a2b3c4d);
#endif
    ASSERT(skb->len >= len && skb->data + len < skb->end);
    skb->len -= len;
    return skb->data += len;
}

static inline void skb_reserve(struct sk_buff *skb, int len)
{
#ifdef SKB_TRACE
    ASSERT((skb)->magic == 0x1a2b3c4d);
#endif
    ASSERT(skb->tail + len < skb->end);
    skb->data += len;
    skb->tail += len;
}

static inline int skb_tailroom(const struct sk_buff *skb)
{
    return (skb->end - skb->tail);
}

static inline unsigned int skb_headroom(const struct sk_buff *skb)
{
    return skb->data - skb->head;
}

static inline int skb_dataroom(const struct sk_buff *skb)
{
    return (skb->end - skb->data);
}

static inline struct sk_buff *skb_get(struct sk_buff *skb)
{
    atomic_inc(&skb->users);
    return skb;
}

static inline void *skb_put_zero(struct sk_buff *skb, unsigned int len)
{
    void *tmp = skb_put(skb, len);
    if (tmp) {
        os_memset(tmp, 0, len);
    }
#ifdef SKB_TRACE
    ASSERT((skb)->magic == 0x1a2b3c4d);
#endif
    return tmp;
}

static inline void *skb_put_data(struct sk_buff *skb, const void *data,
                                 unsigned int len)
{
    void *tmp = skb_put(skb, len);
    if (tmp) {
        os_memcpy(tmp, data, len);
    }
#ifdef SKB_TRACE
    ASSERT((skb)->magic == 0x1a2b3c4d);
#endif
    return tmp;
}

#endif
#endif

