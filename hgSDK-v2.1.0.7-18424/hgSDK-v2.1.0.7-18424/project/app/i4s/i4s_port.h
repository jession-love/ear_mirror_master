#ifndef _I4S_PORT_H
#define _I4S_PORT_H
#include "camera_pro.h"


int sys_port_get_apinfo(struct cProWifi *wifi_info);
int sys_port_get_mac(uint8_t *mac);
int sys_port_get_license(struct cProLic *lic);
int sys_port_set_license(struct cProLic *lic);




#endif

