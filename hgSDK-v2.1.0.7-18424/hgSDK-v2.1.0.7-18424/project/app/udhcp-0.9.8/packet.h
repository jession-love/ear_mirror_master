#ifndef _PACKET_H
#define _PACKET_H

#include "typesdef.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
//#include "board_config.h"

#if WIFI_SUPPORT_VERSION != BOARD_WIFI_SV6152
#define time_t uint32_t
#endif
#define udphdr udp_hdr

typedef uint16  __sum16;
typedef uint32_t  __wsum;

struct dhcpMessage {
	uint8 op;
	uint8 htype;
	uint8 hlen;
	uint8 hops;
	uint32 xid;
	uint16 secs;
	uint16 flags;
	uint32 ciaddr;
	uint32 yiaddr;
	uint32 siaddr;
	uint32 giaddr;
	uint8 chaddr[16];
	uint8 sname[64];
	uint8 file[128];
	uint32 cookie;
	uint8 options[308]; /* 312 - cookie */ 
};
#if 1
struct iphdr {
	uint8_t	ihl:4,
		version:4;
	uint8_t	tos;
	uint16_t	tot_len;
	uint16_t	id;
	uint16_t	frag_off;
	uint8_t	ttl;
	uint8_t	protocol;
	__sum16	check;
	uint32_t	saddr;
	uint32_t	daddr;
	/*The options start here. */
};
#endif

struct udp_dhcp_packet {
	struct iphdr ip;
	struct udphdr udp;
	struct dhcpMessage data;
};

void init_header(struct dhcpMessage *packet, char type);
int get_packet(struct dhcpMessage *packet, int fd);
//uint16 checksum(void *addr, int count);
uint16 checksum(uint16_t *addr, int nleft);
int raw_packet(struct dhcpMessage *payload, uint32 source_ip, int source_port,
		   uint32 dest_ip, int dest_port, unsigned char *dest_arp, int ifindex);
int kernel_packet(struct dhcpMessage *payload, uint32 source_ip, int source_port,
		   uint32 dest_ip, int dest_port);

long time(long* t);


#endif
