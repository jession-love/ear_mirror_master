#ifndef __SYS_CONFIG_H__
#define __SYS_CONFIG_H__
#include <stdint.h>
#include "project_config.h"
/////////////// CACHE ////////////
#define SYS_CACHE_ENABLE                1


#define PROJECT_TYPE PRO_TYPE_FPV

#ifndef OS_TASK_COUNT
#define OS_TASK_COUNT 20
#endif

#ifndef SYSCFG_ENABLE
#define SYSCFG_ENABLE 1
#endif

/* 系统出厂参数区 : sys_factory_param[] */
#define SYS_FACTORY_PARAM_SIZE          2048


extern uint32_t userpool_start;
extern uint32_t userpool_end;
#define USER_POOL_START   (userpool_start)
#define USER_POOL_SIZE    (userpool_end - userpool_start)

#if DVP_EN == 1 
	#define DVP_MALLOC_SIZE	40*1024
#else
	#define DVP_MALLOC_SIZE	0*1024
#endif

#ifndef SYS_HEAP_SIZE
#define  SYS_HEAP_SIZE (68*1024+DVP_MALLOC_SIZE)
#endif

#ifndef WIFI_RX_BUFF_SIZE
#define  WIFI_RX_BUFF_SIZE (17*1024)
#endif

#define SYS_HEAP_START    (USER_POOL_START)
#define WIFI_RX_BUFF_ADDR (SYS_HEAP_START+SYS_HEAP_SIZE)
#define SKB_POOL_ADDR     (WIFI_RX_BUFF_ADDR+WIFI_RX_BUFF_SIZE)
#define SKB_POOL_SIZE     (USER_POOL_START+USER_POOL_SIZE-SKB_POOL_ADDR)


#ifndef DEFAULT_SYS_CLK
#define DEFAULT_SYS_CLK   180000000UL //options: <= 180Mhz
#endif

#ifndef ATCMD_UARTDEV
#define ATCMD_UARTDEV     HG_UART1_DEVID
#endif

////////////////////////////////////////////////////lwip cfg////////////////////////////////////////////////////////////

#define MBED_CONF_LWIP_IP_VER_PREF  4
#define NO_SYS                      0
#define LWIP_IPV4                   1
#define LWIP_IPV6                   0
// On dual stack configuration how long wait for preferred stack
// before selecting either IPv6 or IPv4
#if LWIP_IPV4 && LWIP_IPV6
#define ADDR_TIMEOUT                MBED_CONF_LWIP_ADDR_TIMEOUT
#else
#define ADDR_TIMEOUT                0
#endif

#define SYS_LIGHTWEIGHT_PROT        1

#define LWIP_RAW                    1
#define MEM_LIBC_MALLOC 0

#define MEMP_MEM_MALLOC 0 
#define MEM_ALIGNMENT                   4
#define MEMP_NUM_NETCONN                16
#define MEMP_NUM_TCP_PCB                16

#define TCPIP_THREAD_STACKSIZE          1024
#define TCP_LISTEN_BACKLOG              1
#define TCPIP_MBOX_SIZE                 16
#define DEFAULT_UDP_RECVMBOX_SIZE       10
#define DEFAULT_TCP_RECVMBOX_SIZE       10
#define DEFAULT_ACCEPTMBOX_SIZE         10
#define LWIP_TCPIP_TIMEOUT              1
#define LWIP_TCP_KEEPALIVE              1
#define SO_REUSE                        1
#define LWIP_TCPIP_CORE_LOCKING         0
#define LWIP_NETIF_LOOPBACK             1


#define TCPIP_THREAD_PRIO           OS_TASK_PRIORITY_BELOW_NORMAL+2//OS_TASK_PRIORITY_ABOVE_NORMAL

#ifdef LWIP_DEBUG
#define DEFAULT_THREAD_STACKSIZE    512*2
#else
#define DEFAULT_THREAD_STACKSIZE    512
#endif

#define MEMP_NUM_SYS_TIMEOUT        16

#define DNS_SERVER_ADDRESS(ipaddr) (ip4_addr_set_u32(ipaddr,ipaddr_addr("208.67.222.222")))
// 32-bit alignment
#define MEM_ALIGNMENT               4

/* Keepalive values, compliant with RFC 1122. Don't change this unless you know what you're doing */
#define  TCP_KEEPIDLE_DEFAULT     2000UL /* Default KEEPALIVE timer in milliseconds */

#define  TCP_KEEPINTVL_DEFAULT    1000UL   /* Default Time between KEEPALIVE probes in milliseconds */

#define  TCP_KEEPCNT_DEFAULT      10U        /* Default Counter for KEEPALIVE probes */



#define MEM_SIZE                        10*1024
#define TCP_MSS                         1460
#define MEMP_NUM_PBUF                   10
#define MEMP_NUM_UDP_PCB                8
#define MEMP_NUM_TCP_SEG                80
#define MEMP_NUM_NETBUF                 16
#define PBUF_POOL_SIZE                  6
#define ETH_PAD_SIZE                    0
#define LWIP_TRANSPORT_ETHERNET         1
#define LWIP_SO_SNDTIMEO                1
#define LWIP_SO_RCVTIMEO                1
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD 1
#define MEMP_NUM_REASSDATA              10
#define TCP_SND_BUF                     (8 * TCP_MSS)
#define TCP_WND                         (4 * TCP_MSS)
#define TCP_SND_QUEUELEN                ((4 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))

// One netconn is needed for each UDPSocket, TCPSocket or TCPServer.
// Each requires 236 bytes of RAM (total rounded to multiple of 512).
//#define MEMP_NUM_NETCONN            4

/**
 * LWIP_SO_RCVBUF==1: Enable SO_RCVBUF processing.
 */

#define TCP_QUEUE_OOSEQ             1

#define LWIP_DHCP                   LWIP_IPV4
#define LWIP_DNS                    1
#define LWIP_SOCKET                 1

#define SO_REUSE                    1

// Support Multicast
#include "stdlib.h"
#define LWIP_IGMP                   LWIP_IPV4
#define LWIP_COMPAT_SOCKETS         1
#define LWIP_POSIX_SOCKETS_IO_NAMES 1
#define LWIP_SO_RCVTIMEO            1
#define LWIP_TCP_KEEPALIVE          1
#define LWIP_TIMEVAL_PRIVATE        0

// Fragmentation on, as per IPv4 default
#define LWIP_IPV6_FRAG              LWIP_IPV6

// Queuing "disabled", as per IPv4 default (so actually queues 1)
#define LWIP_ND6_QUEUEING           0

#define LWIP_PLATFORM_BYTESWAP      1

#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS 1


#if LWIP_TRANSPORT_ETHERNET

// Broadcast
#define IP_SOF_BROADCAST            0
#define IP_SOF_BROADCAST_RECV       0

#define LWIP_BROADCAST_PING         1

#define LWIP_CHECKSUM_ON_COPY       0

#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

#elif LWIP_TRANSPORT_PPP

#define TCP_SND_BUF                     (3 * 536)
#define TCP_WND                         (2 * 536)

#define LWIP_ARP 0

#define PPP_SUPPORT 1
#define CHAP_SUPPORT                    1
#define PAP_SUPPORT                     1
#define PPP_THREAD_STACKSIZE            4*192
#define PPP_THREAD_PRIO 0

#define MAXNAMELEN                      64     /* max length of hostname or name for auth */
#define MAXSECRETLEN                    64

#else
#error A transport mechanism (Ethernet or PPP) must be defined
#endif

#endif

