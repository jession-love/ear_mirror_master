#include "sys_config.h"
#include "typesdef.h"
#include "osal/string.h"

#include "syscfg.h"
#include "i4s_port.h"

int sys_port_get_apinfo(struct cProWifi *wifi_info)
{
	struct sys_config _sys_cfg;

	syscfg_read(&_sys_cfg, sizeof(_sys_cfg));
	os_memcpy(wifi_info->SSID, _sys_cfg.ssid, 32);
	os_memcpy(wifi_info->PASSWD, _sys_cfg.key, 32);
	wifi_info->channel = 1;

	return 0;
}

int sys_port_get_mac(uint8_t *mac)
{
	sysctrl_efuse_mac_addr_calc(mac);
	return 0;
}

int sys_port_get_license(struct cProLic *lic)
{
	uint8_t mac[6] = {0};
	
	sys_port_get_mac(mac);
	getlic10_frommac("12345678", mac, (char *)lic->lic);
	os_strcpy(lic->sn, "12345678");

	return 0;
}

int sys_port_set_license(struct cProLic *lic)
{
	return 0;
}