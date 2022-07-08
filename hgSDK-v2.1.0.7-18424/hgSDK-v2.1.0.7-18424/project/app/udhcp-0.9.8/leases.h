/* leases.h */
#ifndef _LEASES_H
#define _LEASES_H


struct dhcpOfferedAddr {
	uint8 chaddr[16];
	uint32 yiaddr;	/* network order */
	uint32 expires;	/* host order */
};

extern const unsigned char blank_chaddr[];

void clear_lease(uint8 *chaddr, uint32 yiaddr);
struct dhcpOfferedAddr *add_lease(uint8 *chaddr, uint32 yiaddr, unsigned long lease);
int lease_expired(struct dhcpOfferedAddr *lease);
struct dhcpOfferedAddr *oldest_expired_lease(void);
struct dhcpOfferedAddr *find_lease_by_chaddr(uint8 *chaddr);
struct dhcpOfferedAddr *find_lease_by_yiaddr(uint32 yiaddr);
uint32 find_address(int check_expired);
int check_ip(uint32 addr);


#endif
