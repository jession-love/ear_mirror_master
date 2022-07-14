##
## Auto Generated makefile by CDK
## Do not modify this file, and any manual changes will be erased!!!   
##
## BuildSet
ProjectName            :=fpv_app_umac4
ConfigurationName      :=BuildSet
WorkspacePath          :=./
ProjectPath            :=./
IntermediateDirectory  :=Obj
OutDir                 :=$(IntermediateDirectory)
User                   :=i4season
Date                   :=14/07/2022
CDKPath                :=../../../../../C-Sky/CDK/
LinkerName             :=csky-elfabiv2-gcc
LinkerNameoption       :=
SIZE                   :=csky-elfabiv2-size
READELF                :=csky-elfabiv2-readelf
CHECKSUM               :=crc32
SharedObjectLinkerName :=
ObjectSuffix           :=.o
DependSuffix           :=.d
PreprocessSuffix       :=.i
DisassemSuffix         :=.asm
IHexSuffix             :=.ihex
BinSuffix              :=.bin
ExeSuffix              :=.elf
LibSuffix              :=.a
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
ElfInfoSwitch          :=-hlS
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
UnPreprocessorSwitch   :=-U
SourceSwitch           :=-c 
ObjdumpSwitch          :=-S
ObjcopySwitch          :=-O ihex
ObjcopyBinSwitch       :=-O binary
OutputFile             :=$(ProjectName)
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=fpv_app_umac4.txt
MakeDirCommand         :=mkdir
LinkOptions            := -mcpu=ck803  -nostartfiles -Wl,--gc-sections -T"$(ProjectPath)/utilities/gcc_csky.ld"
LinkOtherFlagsOption   :=-Wl,-zmax-page-size=1024
IncludePackagePath     :=
IncludeCPath           := $(IncludeSwitch). $(IncludeSwitch).. $(IncludeSwitch)../csky/configs $(IncludeSwitch)../csky/csi_core/include $(IncludeSwitch)../csky/csi_driver/include $(IncludeSwitch)../csky/csi_kernel/include $(IncludeSwitch)../csky/csi_kernel/rhino/arch/include $(IncludeSwitch)../csky/csi_kernel/rhino/common $(IncludeSwitch)../csky/csi_kernel/rhino/core/include $(IncludeSwitch)../csky/csi_kernel/rhino/driver $(IncludeSwitch)../csky/csi_kernel/rhino/pwrmgmt $(IncludeSwitch)../csky/libs/include $(IncludeSwitch)../sample $(IncludeSwitch)../sample/pdmSample $(IncludeSwitch)../sdk/driver/ $(IncludeSwitch)../sdk/include $(IncludeSwitch)../sdk/include/chip $(IncludeSwitch)../sdk/include/lib/net $(IncludeSwitch)../sdk/include/lib/net/lwip/ $(IncludeSwitch)../sdk/include/lib/net/lwip/include $(IncludeSwitch)../sdk/include/lib/net/lwip/include/posix $(IncludeSwitch)../sdk/include/lib/sdhost $(IncludeSwitch)../sdk/lib/ $(IncludeSwitch)../sdk/lib/audio $(IncludeSwitch)../sdk/lib/avformat/avi $(IncludeSwitch)../sdk/lib/bus/macbus $(IncludeSwitch)../sdk/lib/common $(IncludeSwitch)../sdk/lib/fs $(IncludeSwitch)../sdk/lib/fs/fatfs $(IncludeSwitch)../sdk/lib/lmac/lmac_bgn/rf $(IncludeSwitch)../sdk/lib/net/eloop $(IncludeSwitch)../sdk/lib/net/osal_socket $(IncludeSwitch)../sdk/lib/umac $(IncludeSwitch)../sdk/lib/umac/umac3 $(IncludeSwitch)../sdk/lib/umac/umac3/include $(IncludeSwitch)../sdk/lib/video/de_avi $(IncludeSwitch)../sdk/lib/video/en_avi $(IncludeSwitch)../sdk/lib/video/frame_memt $(IncludeSwitch)../sdk/lib/video/opendml $(IncludeSwitch)../test $(IncludeSwitch)app $(IncludeSwitch)app/algorithm/frame_memt $(IncludeSwitch)app/algorithm/pdmFilter $(IncludeSwitch)app/alk_net $(IncludeSwitch)app/audio $(IncludeSwitch)app/avi $(IncludeSwitch)app/boot_lib $(IncludeSwitch)app/cmd_input $(IncludeSwitch)app/dhcpd_eloop $(IncludeSwitch)app/gsensor $(IncludeSwitch)app/media $(IncludeSwitch)app/record $(IncludeSwitch)app/remoteControl $(IncludeSwitch)app/socketModule $(IncludeSwitch)app/speedTest $(IncludeSwitch)app/spook $(IncludeSwitch)app/update $(IncludeSwitch)svn/txw80x/hgSDK/trunk/project/demo/fpv/app/flash $(IncludeSwitch)svn/txw80x/hgSDK/trunk/project/demo/fpv/app/update $(IncludeSwitch)svn/txw80x/hgSDK/trunk/sdk/lib/app/net/alk_net $(IncludeSwitch)app/i4s  
IncludeAPath           := $(IncludeSwitch). $(IncludeSwitch)../csky/configs $(IncludeSwitch)../csky/csi_core/include $(IncludeSwitch)../csky/csi_driver/include $(IncludeSwitch)../csky/csi_kernel/include $(IncludeSwitch)../csky/csi_kernel/rhino/arch/include $(IncludeSwitch)../csky/csi_kernel/rhino/common $(IncludeSwitch)../csky/csi_kernel/rhino/core/include $(IncludeSwitch)../csky/csi_kernel/rhino/driver $(IncludeSwitch)../csky/csi_kernel/rhino/pwrmgmt $(IncludeSwitch)../csky/libs/include $(IncludeSwitch)../sample/pdmSample $(IncludeSwitch)../sdk/include $(IncludeSwitch)../sdk/include/chip $(IncludeSwitch)../sdk/include/lib/lwip/include $(IncludeSwitch)../sdk/lib/avformat/avi $(IncludeSwitch)../sdk/lib/bus/macbus $(IncludeSwitch)../sdk/lib/lmac/lmac_bgn/rf $(IncludeSwitch)../sdk/lib/net/eloop $(IncludeSwitch)../sdk/lib/video/de_avi $(IncludeSwitch)../sdk/lib/video/en_avi $(IncludeSwitch)../test $(IncludeSwitch)app $(IncludeSwitch)app/algorithm/frame_memt $(IncludeSwitch)app/algorithm/pdmFilter $(IncludeSwitch)app/audio $(IncludeSwitch)app/avi $(IncludeSwitch)app/boot_lib $(IncludeSwitch)app/cmd_input $(IncludeSwitch)app/dhcpd_eloop $(IncludeSwitch)app/media $(IncludeSwitch)app/record $(IncludeSwitch)app/remoteControl $(IncludeSwitch)app/socketModule $(IncludeSwitch)app/speedTest $(IncludeSwitch)app/spook $(IncludeSwitch)app/update $(IncludeSwitch)svn/txw80x/hgSDK/trunk/project/demo/fpv/app/flash $(IncludeSwitch)svn/txw80x/hgSDK/trunk/project/demo/fpv/app/update $(IncludeSwitch)svn/txw80x/hgSDK/trunk/sdk/lib/app/net/alk_net  
Libs                   := -Wl,--whole-archive  -Wl,--no-whole-archive $(LibrarySwitch)m $(LibrarySwitch)umac4 $(LibrarySwitch)txw80x $(LibrarySwitch)fpv_app  
ArLibs                 := "m" "umac4" "txw80x" "fpv_app" 
PackagesLibPath        :=
LibPath                :=$(LibraryPathSwitch)utilities $(LibraryPathSwitch)../libs  $(PackagesLibPath) 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       :=csky-elfabiv2-ar rcu
CXX      :=csky-elfabiv2-g++
CC       :=csky-elfabiv2-gcc
AS       :=csky-elfabiv2-gcc
OBJDUMP  :=csky-elfabiv2-objdump
OBJCOPY  :=csky-elfabiv2-objcopy
CXXFLAGS := -mcpu=ck803   $(PreprocessorSwitch)__NO_BOARD_INIT  $(PreprocessorSwitch)TXW80X  $(PreprocessorSwitch)SKB_POOL_ENABLE  $(PreprocessorSwitch)MPOOL_ALLOC  $(PreprocessorSwitch)CONFIG_UMAC4   -Os  -g  -Wall  -ffunction-sections -ffunction-sections -fdata-sections -Wno-comment -Wno-unused-function -Wno-unused-but-set-variable -I. 
CFLAGS   := -mcpu=ck803   $(PreprocessorSwitch)__NO_BOARD_INIT  $(PreprocessorSwitch)TXW80X  $(PreprocessorSwitch)SKB_POOL_ENABLE  $(PreprocessorSwitch)MPOOL_ALLOC  $(PreprocessorSwitch)CONFIG_UMAC4   -Os  -g  -Wall  -ffunction-sections -ffunction-sections -fdata-sections -Wno-comment -Wno-unused-function -Wno-unused-but-set-variable -I. 
ASFLAGS  := -mcpu=ck803   $(PreprocessorSwitch)__NO_BOARD_INIT  $(PreprocessorSwitch)TXW80X   -Wa,--gdwarf2  -I.  


Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/device$(ObjectSuffix) $(IntermediateDirectory)/syscfg$(ObjectSuffix) $(IntermediateDirectory)/hgic_usart$(ObjectSuffix) $(IntermediateDirectory)/hgic_malloc$(ObjectSuffix) $(IntermediateDirectory)/hgic_assert$(ObjectSuffix) $(IntermediateDirectory)/hal_dev$(ObjectSuffix) $(IntermediateDirectory)/hal_gpio$(ObjectSuffix) $(IntermediateDirectory)/hal_i2c$(ObjectSuffix) $(IntermediateDirectory)/hal_sdio_slave$(ObjectSuffix) \
	$(IntermediateDirectory)/hal_timer$(ObjectSuffix) $(IntermediateDirectory)/hal_watchdog$(ObjectSuffix) $(IntermediateDirectory)/hal_usb_device$(ObjectSuffix) $(IntermediateDirectory)/hal_spi$(ObjectSuffix) $(IntermediateDirectory)/hal_spi_nor$(ObjectSuffix) $(IntermediateDirectory)/hal_sysaes$(ObjectSuffix) $(IntermediateDirectory)/hal_uart$(ObjectSuffix) $(IntermediateDirectory)/hal_i2s$(ObjectSuffix) $(IntermediateDirectory)/hal_pdm$(ObjectSuffix) $(IntermediateDirectory)/hal_led$(ObjectSuffix) \
	$(IntermediateDirectory)/hal_pwm$(ObjectSuffix) $(IntermediateDirectory)/hal_capture$(ObjectSuffix) $(IntermediateDirectory)/hal_adc$(ObjectSuffix) $(IntermediateDirectory)/hal_netdev$(ObjectSuffix) $(IntermediateDirectory)/hal_dvp$(ObjectSuffix) $(IntermediateDirectory)/hal_dma$(ObjectSuffix) $(IntermediateDirectory)/hal_jpeg$(ObjectSuffix) $(IntermediateDirectory)/libc_malloc$(ObjectSuffix) $(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) $(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) \
	$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) $(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) $(IntermediateDirectory)/txw80x_isr$(ObjectSuffix) \
	$(IntermediateDirectory)/txw80x_pin_function$(ObjectSuffix) $(IntermediateDirectory)/txw80x_trap_c$(ObjectSuffix) $(IntermediateDirectory)/txw80x_vectors$(ObjectSuffix) $(IntermediateDirectory)/dma_dw_dmac$(ObjectSuffix) $(IntermediateDirectory)/dma_hg_m2m_dma$(ObjectSuffix) $(IntermediateDirectory)/gpio_hggpio_v3$(ObjectSuffix) $(IntermediateDirectory)/i2c_hgi2c$(ObjectSuffix) $(IntermediateDirectory)/i2s_hgi2s_v0$(ObjectSuffix) $(IntermediateDirectory)/spi_hgspi_v1$(ObjectSuffix) $(IntermediateDirectory)/timer_hgtimer_v6$(ObjectSuffix) \
	$(IntermediateDirectory)/uart_hguart_v2$(ObjectSuffix) $(IntermediateDirectory)/wdt_hg_wdt_v1$(ObjectSuffix) $(IntermediateDirectory)/crc_hg_crc$(ObjectSuffix) $(IntermediateDirectory)/sdhost_hgsdh_dev$(ObjectSuffix) $(IntermediateDirectory)/pdm_hgpdm_v0$(ObjectSuffix) $(IntermediateDirectory)/led_hgled_v0$(ObjectSuffix) $(IntermediateDirectory)/pwm_hgpwm_v0$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/capture_hgcapture_v0$(ObjectSuffix) $(IntermediateDirectory)/csi_hgdvp$(ObjectSuffix) $(IntermediateDirectory)/adc_hgadc_v0$(ObjectSuffix) \
	$(IntermediateDirectory)/sdhost_sdhost$(ObjectSuffix) $(IntermediateDirectory)/usb_app_usb_device_wifi$(ObjectSuffix) $(IntermediateDirectory)/usb_app_usb_host_uvc$(ObjectSuffix) $(IntermediateDirectory)/avi_avi_app$(ObjectSuffix) $(IntermediateDirectory)/cmd_input_cmd_input$(ObjectSuffix) $(IntermediateDirectory)/media_media$(ObjectSuffix) $(IntermediateDirectory)/record_avi_record$(ObjectSuffix) $(IntermediateDirectory)/remoteControl_remote_control$(ObjectSuffix) $(IntermediateDirectory)/socketModule_socket_module$(ObjectSuffix) $(IntermediateDirectory)/speedTest_speedTest$(ObjectSuffix) \
	$(IntermediateDirectory)/spook_access_log$(ObjectSuffix) $(IntermediateDirectory)/spook_encoder-audio$(ObjectSuffix) $(IntermediateDirectory)/spook_encoder-jpeg$(ObjectSuffix) $(IntermediateDirectory)/spook_ephoto$(ObjectSuffix) $(IntermediateDirectory)/spook_frame$(ObjectSuffix) $(IntermediateDirectory)/spook_http$(ObjectSuffix) $(IntermediateDirectory)/spook_live$(ObjectSuffix) $(IntermediateDirectory)/spook_log$(ObjectSuffix) $(IntermediateDirectory)/spook_pmsg$(ObjectSuffix) $(IntermediateDirectory)/spook_rtp-audio$(ObjectSuffix) \
	$(IntermediateDirectory)/spook_rtp-jpeg$(ObjectSuffix) $(IntermediateDirectory)/spook_rtp$(ObjectSuffix) $(IntermediateDirectory)/spook_rtsp$(ObjectSuffix) $(IntermediateDirectory)/spook_session$(ObjectSuffix) $(IntermediateDirectory)/spook_spook$(ObjectSuffix) $(IntermediateDirectory)/spook_spookctl$(ObjectSuffix) $(IntermediateDirectory)/spook_tcp$(ObjectSuffix) $(IntermediateDirectory)/spook_webfile$(ObjectSuffix) $(IntermediateDirectory)/update_ota$(ObjectSuffix) $(IntermediateDirectory)/gsensor_g_sensor$(ObjectSuffix) \
	$(IntermediateDirectory)/xmodem_tx_xmodem_tx$(ObjectSuffix) $(IntermediateDirectory)/audio_audio_8311_demo$(ObjectSuffix) $(IntermediateDirectory)/audio_audio_demo$(ObjectSuffix) $(IntermediateDirectory)/audio_hal_i2s_audio$(ObjectSuffix) $(IntermediateDirectory)/audio_pdm_audio$(ObjectSuffix) $(IntermediateDirectory)/sta_interface_sta_interface$(ObjectSuffix) $(IntermediateDirectory)/i4s_aes_core$(ObjectSuffix) $(IntermediateDirectory)/i4s_alk$(ObjectSuffix) $(IntermediateDirectory)/i4s_i4s$(ObjectSuffix) $(IntermediateDirectory)/i4s_i4s_port$(ObjectSuffix) \
	$(IntermediateDirectory)/i4s_i4sgsensor$(ObjectSuffix) $(IntermediateDirectory)/i4s_mcu_icb-4u27$(ObjectSuffix) $(IntermediateDirectory)/adapter_csi_rhino$(ObjectSuffix) $(IntermediateDirectory)/core_k_buf_queue$(ObjectSuffix) $(IntermediateDirectory)/core_k_dyn_mem_proc$(ObjectSuffix) $(IntermediateDirectory)/core_k_err$(ObjectSuffix) $(IntermediateDirectory)/core_k_event$(ObjectSuffix) $(IntermediateDirectory)/core_k_idle$(ObjectSuffix) $(IntermediateDirectory)/core_k_mm$(ObjectSuffix) $(IntermediateDirectory)/core_k_mm_blk$(ObjectSuffix) \
	$(IntermediateDirectory)/core_k_mm_debug$(ObjectSuffix) $(IntermediateDirectory)/core_k_mutex$(ObjectSuffix) $(IntermediateDirectory)/core_k_obj$(ObjectSuffix) $(IntermediateDirectory)/core_k_pend$(ObjectSuffix) $(IntermediateDirectory)/core_k_queue$(ObjectSuffix) $(IntermediateDirectory)/core_k_ringbuf$(ObjectSuffix) $(IntermediateDirectory)/core_k_sched$(ObjectSuffix) $(IntermediateDirectory)/core_k_sem$(ObjectSuffix) $(IntermediateDirectory)/core_k_stats$(ObjectSuffix) $(IntermediateDirectory)/core_k_sys$(ObjectSuffix) \
	$(IntermediateDirectory)/core_k_task$(ObjectSuffix) $(IntermediateDirectory)/core_k_task_sem$(ObjectSuffix) $(IntermediateDirectory)/core_k_tick$(ObjectSuffix) $(IntermediateDirectory)/core_k_time$(ObjectSuffix) 

Objects2=$(IntermediateDirectory)/core_k_timer$(ObjectSuffix) $(IntermediateDirectory)/core_k_workqueue$(ObjectSuffix) $(IntermediateDirectory)/driver_hook_impl$(ObjectSuffix) $(IntermediateDirectory)/driver_systick$(ObjectSuffix) $(IntermediateDirectory)/driver_yoc_impl$(ObjectSuffix) $(IntermediateDirectory)/driver_hook_weak$(ObjectSuffix) \
	$(IntermediateDirectory)/fatfs_diskio$(ObjectSuffix) $(IntermediateDirectory)/fatfs_fatfs_test$(ObjectSuffix) $(IntermediateDirectory)/fatfs_ff$(ObjectSuffix) $(IntermediateDirectory)/fatfs_ffunicode$(ObjectSuffix) $(IntermediateDirectory)/fatfs_syscall$(ObjectSuffix) $(IntermediateDirectory)/fatfs_osal_file$(ObjectSuffix) $(IntermediateDirectory)/uvc_hg_usb_device$(ObjectSuffix) $(IntermediateDirectory)/uvc_uvc_host$(ObjectSuffix) $(IntermediateDirectory)/de_avi_avidemux$(ObjectSuffix) $(IntermediateDirectory)/en_avi_openDML$(ObjectSuffix) \
	$(IntermediateDirectory)/frame_memt_frame_memt$(ObjectSuffix) $(IntermediateDirectory)/pdmFilter_pdmFilter$(ObjectSuffix) $(IntermediateDirectory)/csky_cpu_impl$(ObjectSuffix) $(IntermediateDirectory)/csky_csky_sched$(ObjectSuffix) $(IntermediateDirectory)/csky_port_c$(ObjectSuffix) $(IntermediateDirectory)/csky_port_s$(ObjectSuffix) $(IntermediateDirectory)/arch_sys_arch$(ObjectSuffix) $(IntermediateDirectory)/spi_nor_spi_nor_bus$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_bf2013$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_bf3703$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_sensor_bf3a03$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_gc0308$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_gc0309$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_gc0328$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_ov2640$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_ov7670$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_ov7725$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_xc7016_h63$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_xcg532$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_xc7011_gc1054$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_sensor_xc7011_h63$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_gc0311$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_gc0329$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_gc0312$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_bf3720$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_sp0a19$(ObjectSuffix) $(IntermediateDirectory)/sensor_sensor_sp0718$(ObjectSuffix) $(IntermediateDirectory)/gsensor_da280$(ObjectSuffix) $(IntermediateDirectory)/cmos_sensor_csi$(ObjectSuffix) $(IntermediateDirectory)/jpeg_jpg$(ObjectSuffix) \
	$(IntermediateDirectory)/jpeg_jpg_table$(ObjectSuffix) 

Objects3=$(IntermediateDirectory)/api_api_lib$(ObjectSuffix) $(IntermediateDirectory)/api_api_msg$(ObjectSuffix) $(IntermediateDirectory)/api_err$(ObjectSuffix) $(IntermediateDirectory)/api_if_api$(ObjectSuffix) $(IntermediateDirectory)/api_netbuf$(ObjectSuffix) $(IntermediateDirectory)/api_netdb$(ObjectSuffix) $(IntermediateDirectory)/api_netifapi$(ObjectSuffix) $(IntermediateDirectory)/api_sockets$(ObjectSuffix) $(IntermediateDirectory)/api_tcpip$(ObjectSuffix) \
	$(IntermediateDirectory)/core_altcp$(ObjectSuffix) $(IntermediateDirectory)/core_altcp_alloc$(ObjectSuffix) $(IntermediateDirectory)/core_altcp_tcp$(ObjectSuffix) $(IntermediateDirectory)/core_def$(ObjectSuffix) $(IntermediateDirectory)/core_dns$(ObjectSuffix) $(IntermediateDirectory)/core_inet_chksum$(ObjectSuffix) $(IntermediateDirectory)/core_init$(ObjectSuffix) $(IntermediateDirectory)/core_ip$(ObjectSuffix) $(IntermediateDirectory)/core_mem$(ObjectSuffix) $(IntermediateDirectory)/core_memp$(ObjectSuffix) \
	$(IntermediateDirectory)/core_netif$(ObjectSuffix) $(IntermediateDirectory)/core_pbuf$(ObjectSuffix) $(IntermediateDirectory)/core_raw$(ObjectSuffix) $(IntermediateDirectory)/core_stats$(ObjectSuffix) $(IntermediateDirectory)/core_sys$(ObjectSuffix) $(IntermediateDirectory)/core_tcp$(ObjectSuffix) $(IntermediateDirectory)/core_tcp_in$(ObjectSuffix) $(IntermediateDirectory)/core_tcp_out$(ObjectSuffix) $(IntermediateDirectory)/core_timeouts$(ObjectSuffix) $(IntermediateDirectory)/core_udp$(ObjectSuffix) \
	$(IntermediateDirectory)/netif_bridgeif$(ObjectSuffix) $(IntermediateDirectory)/netif_bridgeif_fdb$(ObjectSuffix) $(IntermediateDirectory)/netif_ethernet$(ObjectSuffix) $(IntermediateDirectory)/netif_ethernetif$(ObjectSuffix) $(IntermediateDirectory)/netif_lowpan6$(ObjectSuffix) $(IntermediateDirectory)/netif_lowpan6_ble$(ObjectSuffix) $(IntermediateDirectory)/netif_lowpan6_common$(ObjectSuffix) $(IntermediateDirectory)/netif_slipif$(ObjectSuffix) $(IntermediateDirectory)/netif_zepif$(ObjectSuffix) $(IntermediateDirectory)/ipv4_autoip$(ObjectSuffix) \
	$(IntermediateDirectory)/ipv4_dhcp$(ObjectSuffix) $(IntermediateDirectory)/ipv4_etharp$(ObjectSuffix) $(IntermediateDirectory)/ipv4_icmp$(ObjectSuffix) $(IntermediateDirectory)/ipv4_igmp$(ObjectSuffix) $(IntermediateDirectory)/ipv4_ip4$(ObjectSuffix) $(IntermediateDirectory)/ipv4_ip4_addr$(ObjectSuffix) $(IntermediateDirectory)/ipv4_ip4_frag$(ObjectSuffix) $(IntermediateDirectory)/ipv6_dhcp6$(ObjectSuffix) $(IntermediateDirectory)/ipv6_ethip6$(ObjectSuffix) $(IntermediateDirectory)/ipv6_icmp6$(ObjectSuffix) \
	$(IntermediateDirectory)/ipv6_inet6$(ObjectSuffix) $(IntermediateDirectory)/ipv6_ip6$(ObjectSuffix) $(IntermediateDirectory)/ipv6_ip6_addr$(ObjectSuffix) $(IntermediateDirectory)/ipv6_ip6_frag$(ObjectSuffix) $(IntermediateDirectory)/ipv6_mld6$(ObjectSuffix) $(IntermediateDirectory)/ipv6_nd6$(ObjectSuffix) $(IntermediateDirectory)/ppp_auth$(ObjectSuffix) $(IntermediateDirectory)/ppp_ccp$(ObjectSuffix) $(IntermediateDirectory)/ppp_chap-md5$(ObjectSuffix) $(IntermediateDirectory)/ppp_chap-new$(ObjectSuffix) \
	$(IntermediateDirectory)/ppp_chap_ms$(ObjectSuffix) $(IntermediateDirectory)/ppp_demand$(ObjectSuffix) $(IntermediateDirectory)/ppp_eap$(ObjectSuffix) $(IntermediateDirectory)/ppp_ecp$(ObjectSuffix) $(IntermediateDirectory)/ppp_eui64$(ObjectSuffix) $(IntermediateDirectory)/ppp_fsm$(ObjectSuffix) $(IntermediateDirectory)/ppp_ipcp$(ObjectSuffix) $(IntermediateDirectory)/ppp_ipv6cp$(ObjectSuffix) $(IntermediateDirectory)/ppp_lcp$(ObjectSuffix) $(IntermediateDirectory)/ppp_magic$(ObjectSuffix) \
	$(IntermediateDirectory)/ppp_mppe$(ObjectSuffix) $(IntermediateDirectory)/ppp_multilink$(ObjectSuffix) $(IntermediateDirectory)/ppp_ppp$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppapi$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppcrypt$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppoe$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppol2tp$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppos$(ObjectSuffix) $(IntermediateDirectory)/ppp_upap$(ObjectSuffix) $(IntermediateDirectory)/ppp_utils$(ObjectSuffix) \
	$(IntermediateDirectory)/ppp_vj$(ObjectSuffix) $(IntermediateDirectory)/polarssl_arc4$(ObjectSuffix) $(IntermediateDirectory)/polarssl_des$(ObjectSuffix) $(IntermediateDirectory)/polarssl_md4$(ObjectSuffix) $(IntermediateDirectory)/polarssl_md5$(ObjectSuffix) $(IntermediateDirectory)/polarssl_sha1$(ObjectSuffix) $(IntermediateDirectory)/__rt_entry$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) $(Objects3) 

##
## Main Build Targets 
##
.PHONY: all
all: $(IntermediateDirectory)/$(OutputFile)

$(IntermediateDirectory)/$(OutputFile):  $(Objects) Always_Link 
	$(LinkerName) $(OutputSwitch) $(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) $(LinkerNameoption) $(LinkOtherFlagsOption)  -Wl,--ckmap=$(ProjectPath)/Lst/$(OutputFile).map  @$(ObjectsFileList)  $(LinkOptions) $(LibPath) $(Libs)
	@mv $(ProjectPath)/Lst/$(OutputFile).map $(ProjectPath)/Lst/$(OutputFile).temp && $(READELF) $(ElfInfoSwitch) $(ProjectPath)/Obj/$(OutputFile)$(ExeSuffix) > $(ProjectPath)/Lst/$(OutputFile).map && echo ====================================================================== >> $(ProjectPath)/Lst/$(OutputFile).map && cat $(ProjectPath)/Lst/$(OutputFile).temp >> $(ProjectPath)/Lst/$(OutputFile).map && rm -rf $(ProjectPath)/Lst/$(OutputFile).temp
	$(OBJCOPY) $(ObjcopySwitch) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix)  $(ProjectPath)/Obj/$(OutputFile)$(IHexSuffix) 
		$(OBJDUMP) $(ObjdumpSwitch) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix)  > $(ProjectPath)/Lst/$(OutputFile)$(DisassemSuffix) 
	@echo size of target:
	@$(SIZE) $(ProjectPath)$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@echo -n checksum value of target:  
	@$(CHECKSUM) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@$(ProjectName).modify.bat $(IntermediateDirectory) $(OutputFile)$(ExeSuffix) 

Always_Link:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.c  
	$(CC) $(SourceSwitch) main.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/main$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/main$(PreprocessSuffix) main.c

$(IntermediateDirectory)/device$(ObjectSuffix): device.c  
	$(CC) $(SourceSwitch) device.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/device$(ObjectSuffix) -MF$(IntermediateDirectory)/device$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/device$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/device$(PreprocessSuffix): device.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/device$(PreprocessSuffix) device.c

$(IntermediateDirectory)/syscfg$(ObjectSuffix): syscfg.c  
	$(CC) $(SourceSwitch) syscfg.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/syscfg$(ObjectSuffix) -MF$(IntermediateDirectory)/syscfg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/syscfg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/syscfg$(PreprocessSuffix): syscfg.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/syscfg$(PreprocessSuffix) syscfg.c

$(IntermediateDirectory)/hgic_usart$(ObjectSuffix): ../csky/hgic/usart.c  
	$(CC) $(SourceSwitch) ../csky/hgic/usart.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hgic_usart$(ObjectSuffix) -MF$(IntermediateDirectory)/hgic_usart$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hgic_usart$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hgic_usart$(PreprocessSuffix): ../csky/hgic/usart.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hgic_usart$(PreprocessSuffix) ../csky/hgic/usart.c

$(IntermediateDirectory)/hgic_malloc$(ObjectSuffix): ../csky/hgic/malloc.c  
	$(CC) $(SourceSwitch) ../csky/hgic/malloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hgic_malloc$(ObjectSuffix) -MF$(IntermediateDirectory)/hgic_malloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hgic_malloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hgic_malloc$(PreprocessSuffix): ../csky/hgic/malloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hgic_malloc$(PreprocessSuffix) ../csky/hgic/malloc.c

$(IntermediateDirectory)/hgic_assert$(ObjectSuffix): ../csky/hgic/assert.c  
	$(CC) $(SourceSwitch) ../csky/hgic/assert.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hgic_assert$(ObjectSuffix) -MF$(IntermediateDirectory)/hgic_assert$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hgic_assert$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hgic_assert$(PreprocessSuffix): ../csky/hgic/assert.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hgic_assert$(PreprocessSuffix) ../csky/hgic/assert.c

$(IntermediateDirectory)/hal_dev$(ObjectSuffix): ../sdk/hal/dev.c  
	$(CC) $(SourceSwitch) ../sdk/hal/dev.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_dev$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_dev$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_dev$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_dev$(PreprocessSuffix): ../sdk/hal/dev.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_dev$(PreprocessSuffix) ../sdk/hal/dev.c

$(IntermediateDirectory)/hal_gpio$(ObjectSuffix): ../sdk/hal/gpio.c  
	$(CC) $(SourceSwitch) ../sdk/hal/gpio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_gpio$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_gpio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_gpio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_gpio$(PreprocessSuffix): ../sdk/hal/gpio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_gpio$(PreprocessSuffix) ../sdk/hal/gpio.c

$(IntermediateDirectory)/hal_i2c$(ObjectSuffix): ../sdk/hal/i2c.c  
	$(CC) $(SourceSwitch) ../sdk/hal/i2c.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_i2c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_i2c$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_i2c$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_i2c$(PreprocessSuffix): ../sdk/hal/i2c.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_i2c$(PreprocessSuffix) ../sdk/hal/i2c.c

$(IntermediateDirectory)/hal_sdio_slave$(ObjectSuffix): ../sdk/hal/sdio_slave.c  
	$(CC) $(SourceSwitch) ../sdk/hal/sdio_slave.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_sdio_slave$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_sdio_slave$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_sdio_slave$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_sdio_slave$(PreprocessSuffix): ../sdk/hal/sdio_slave.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_sdio_slave$(PreprocessSuffix) ../sdk/hal/sdio_slave.c

$(IntermediateDirectory)/hal_timer$(ObjectSuffix): ../sdk/hal/timer.c  
	$(CC) $(SourceSwitch) ../sdk/hal/timer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_timer$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_timer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_timer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_timer$(PreprocessSuffix): ../sdk/hal/timer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_timer$(PreprocessSuffix) ../sdk/hal/timer.c

$(IntermediateDirectory)/hal_watchdog$(ObjectSuffix): ../sdk/hal/watchdog.c  
	$(CC) $(SourceSwitch) ../sdk/hal/watchdog.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_watchdog$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watchdog$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_watchdog$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_watchdog$(PreprocessSuffix): ../sdk/hal/watchdog.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_watchdog$(PreprocessSuffix) ../sdk/hal/watchdog.c

$(IntermediateDirectory)/hal_usb_device$(ObjectSuffix): ../sdk/hal/usb_device.c  
	$(CC) $(SourceSwitch) ../sdk/hal/usb_device.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_usb_device$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_usb_device$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_usb_device$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_usb_device$(PreprocessSuffix): ../sdk/hal/usb_device.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_usb_device$(PreprocessSuffix) ../sdk/hal/usb_device.c

$(IntermediateDirectory)/hal_spi$(ObjectSuffix): ../sdk/hal/spi.c  
	$(CC) $(SourceSwitch) ../sdk/hal/spi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_spi$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_spi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_spi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_spi$(PreprocessSuffix): ../sdk/hal/spi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_spi$(PreprocessSuffix) ../sdk/hal/spi.c

$(IntermediateDirectory)/hal_spi_nor$(ObjectSuffix): ../sdk/hal/spi_nor.c  
	$(CC) $(SourceSwitch) ../sdk/hal/spi_nor.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_spi_nor$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_spi_nor$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_spi_nor$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_spi_nor$(PreprocessSuffix): ../sdk/hal/spi_nor.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_spi_nor$(PreprocessSuffix) ../sdk/hal/spi_nor.c

$(IntermediateDirectory)/hal_sysaes$(ObjectSuffix): ../sdk/hal/sysaes.c  
	$(CC) $(SourceSwitch) ../sdk/hal/sysaes.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_sysaes$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_sysaes$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_sysaes$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_sysaes$(PreprocessSuffix): ../sdk/hal/sysaes.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_sysaes$(PreprocessSuffix) ../sdk/hal/sysaes.c

$(IntermediateDirectory)/hal_uart$(ObjectSuffix): ../sdk/hal/uart.c  
	$(CC) $(SourceSwitch) ../sdk/hal/uart.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_uart$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_uart$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_uart$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_uart$(PreprocessSuffix): ../sdk/hal/uart.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_uart$(PreprocessSuffix) ../sdk/hal/uart.c

$(IntermediateDirectory)/hal_i2s$(ObjectSuffix): ../sdk/hal/i2s.c  
	$(CC) $(SourceSwitch) ../sdk/hal/i2s.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_i2s$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_i2s$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_i2s$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_i2s$(PreprocessSuffix): ../sdk/hal/i2s.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_i2s$(PreprocessSuffix) ../sdk/hal/i2s.c

$(IntermediateDirectory)/hal_pdm$(ObjectSuffix): ../sdk/hal/pdm.c  
	$(CC) $(SourceSwitch) ../sdk/hal/pdm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_pdm$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_pdm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_pdm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_pdm$(PreprocessSuffix): ../sdk/hal/pdm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_pdm$(PreprocessSuffix) ../sdk/hal/pdm.c

$(IntermediateDirectory)/hal_led$(ObjectSuffix): ../sdk/hal/led.c  
	$(CC) $(SourceSwitch) ../sdk/hal/led.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_led$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_led$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_led$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_led$(PreprocessSuffix): ../sdk/hal/led.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_led$(PreprocessSuffix) ../sdk/hal/led.c

$(IntermediateDirectory)/hal_pwm$(ObjectSuffix): ../sdk/hal/pwm.c  
	$(CC) $(SourceSwitch) ../sdk/hal/pwm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_pwm$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_pwm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_pwm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_pwm$(PreprocessSuffix): ../sdk/hal/pwm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_pwm$(PreprocessSuffix) ../sdk/hal/pwm.c

$(IntermediateDirectory)/hal_capture$(ObjectSuffix): ../sdk/hal/capture.c  
	$(CC) $(SourceSwitch) ../sdk/hal/capture.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_capture$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_capture$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_capture$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_capture$(PreprocessSuffix): ../sdk/hal/capture.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_capture$(PreprocessSuffix) ../sdk/hal/capture.c

$(IntermediateDirectory)/hal_adc$(ObjectSuffix): ../sdk/hal/adc.c  
	$(CC) $(SourceSwitch) ../sdk/hal/adc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_adc$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_adc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_adc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_adc$(PreprocessSuffix): ../sdk/hal/adc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_adc$(PreprocessSuffix) ../sdk/hal/adc.c

$(IntermediateDirectory)/hal_netdev$(ObjectSuffix): ../sdk/hal/netdev.c  
	$(CC) $(SourceSwitch) ../sdk/hal/netdev.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_netdev$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_netdev$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_netdev$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_netdev$(PreprocessSuffix): ../sdk/hal/netdev.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_netdev$(PreprocessSuffix) ../sdk/hal/netdev.c

$(IntermediateDirectory)/hal_dvp$(ObjectSuffix): ../sdk/hal/dvp.c  
	$(CC) $(SourceSwitch) ../sdk/hal/dvp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_dvp$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_dvp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_dvp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_dvp$(PreprocessSuffix): ../sdk/hal/dvp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_dvp$(PreprocessSuffix) ../sdk/hal/dvp.c

$(IntermediateDirectory)/hal_dma$(ObjectSuffix): ../sdk/hal/dma.c  
	$(CC) $(SourceSwitch) ../sdk/hal/dma.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_dma$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_dma$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_dma$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_dma$(PreprocessSuffix): ../sdk/hal/dma.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_dma$(PreprocessSuffix) ../sdk/hal/dma.c

$(IntermediateDirectory)/hal_jpeg$(ObjectSuffix): ../sdk/hal/jpeg.c  
	$(CC) $(SourceSwitch) ../sdk/hal/jpeg.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hal_jpeg$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_jpeg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hal_jpeg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hal_jpeg$(PreprocessSuffix): ../sdk/hal/jpeg.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hal_jpeg$(PreprocessSuffix) ../sdk/hal/jpeg.c

$(IntermediateDirectory)/libc_malloc$(ObjectSuffix): ../csky/libs/libc/malloc.c  
	$(CC) $(SourceSwitch) ../csky/libs/libc/malloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_malloc$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_malloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_malloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_malloc$(PreprocessSuffix): ../csky/libs/libc/malloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_malloc$(PreprocessSuffix) ../csky/libs/libc/malloc.c

$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix): ../csky/libs/libc/minilibc_port.c  
	$(CC) $(SourceSwitch) ../csky/libs/libc/minilibc_port.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_minilibc_port$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_minilibc_port$(PreprocessSuffix): ../csky/libs/libc/minilibc_port.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_minilibc_port$(PreprocessSuffix) ../csky/libs/libc/minilibc_port.c

$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix): ../csky/libs/mm/dq_addlast.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/dq_addlast.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_dq_addlast$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_dq_addlast$(PreprocessSuffix): ../csky/libs/mm/dq_addlast.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_dq_addlast$(PreprocessSuffix) ../csky/libs/mm/dq_addlast.c

$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix): ../csky/libs/mm/dq_rem.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/dq_rem.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_dq_rem$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_dq_rem$(PreprocessSuffix): ../csky/libs/mm/dq_rem.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_dq_rem$(PreprocessSuffix) ../csky/libs/mm/dq_rem.c

$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix): ../csky/libs/mm/lib_mallinfo.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/lib_mallinfo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_lib_mallinfo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_lib_mallinfo$(PreprocessSuffix): ../csky/libs/mm/lib_mallinfo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_lib_mallinfo$(PreprocessSuffix) ../csky/libs/mm/lib_mallinfo.c

$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix): ../csky/libs/mm/mm_addfreechunk.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/mm_addfreechunk.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_addfreechunk$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_addfreechunk$(PreprocessSuffix): ../csky/libs/mm/mm_addfreechunk.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_addfreechunk$(PreprocessSuffix) ../csky/libs/mm/mm_addfreechunk.c

$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix): ../csky/libs/mm/mm_free.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/mm_free.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_free$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_free$(PreprocessSuffix): ../csky/libs/mm/mm_free.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_free$(PreprocessSuffix) ../csky/libs/mm/mm_free.c

$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix): ../csky/libs/mm/mm_initialize.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/mm_initialize.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_initialize$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_initialize$(PreprocessSuffix): ../csky/libs/mm/mm_initialize.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_initialize$(PreprocessSuffix) ../csky/libs/mm/mm_initialize.c

$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix): ../csky/libs/mm/mm_leak.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/mm_leak.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_leak$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_leak$(PreprocessSuffix): ../csky/libs/mm/mm_leak.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_leak$(PreprocessSuffix) ../csky/libs/mm/mm_leak.c

$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix): ../csky/libs/mm/mm_mallinfo.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/mm_mallinfo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_mallinfo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_mallinfo$(PreprocessSuffix): ../csky/libs/mm/mm_mallinfo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_mallinfo$(PreprocessSuffix) ../csky/libs/mm/mm_mallinfo.c

$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix): ../csky/libs/mm/mm_malloc.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/mm_malloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_malloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_malloc$(PreprocessSuffix): ../csky/libs/mm/mm_malloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_malloc$(PreprocessSuffix) ../csky/libs/mm/mm_malloc.c

$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix): ../csky/libs/mm/mm_size2ndx.c  
	$(CC) $(SourceSwitch) ../csky/libs/mm/mm_size2ndx.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_size2ndx$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_size2ndx$(PreprocessSuffix): ../csky/libs/mm/mm_size2ndx.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_size2ndx$(PreprocessSuffix) ../csky/libs/mm/mm_size2ndx.c

$(IntermediateDirectory)/txw80x_isr$(ObjectSuffix): ../sdk/chip/txw80x/isr.c  
	$(CC) $(SourceSwitch) ../sdk/chip/txw80x/isr.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/txw80x_isr$(ObjectSuffix) -MF$(IntermediateDirectory)/txw80x_isr$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/txw80x_isr$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/txw80x_isr$(PreprocessSuffix): ../sdk/chip/txw80x/isr.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/txw80x_isr$(PreprocessSuffix) ../sdk/chip/txw80x/isr.c

$(IntermediateDirectory)/txw80x_pin_function$(ObjectSuffix): ../sdk/chip/txw80x/pin_function.c  
	$(CC) $(SourceSwitch) ../sdk/chip/txw80x/pin_function.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/txw80x_pin_function$(ObjectSuffix) -MF$(IntermediateDirectory)/txw80x_pin_function$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/txw80x_pin_function$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/txw80x_pin_function$(PreprocessSuffix): ../sdk/chip/txw80x/pin_function.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/txw80x_pin_function$(PreprocessSuffix) ../sdk/chip/txw80x/pin_function.c

$(IntermediateDirectory)/txw80x_trap_c$(ObjectSuffix): ../sdk/chip/txw80x/trap_c.c  
	$(CC) $(SourceSwitch) ../sdk/chip/txw80x/trap_c.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/txw80x_trap_c$(ObjectSuffix) -MF$(IntermediateDirectory)/txw80x_trap_c$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/txw80x_trap_c$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/txw80x_trap_c$(PreprocessSuffix): ../sdk/chip/txw80x/trap_c.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/txw80x_trap_c$(PreprocessSuffix) ../sdk/chip/txw80x/trap_c.c

$(IntermediateDirectory)/txw80x_vectors$(ObjectSuffix): ../sdk/chip/txw80x/vectors.S  
	$(AS) $(SourceSwitch) ../sdk/chip/txw80x/vectors.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/txw80x_vectors$(ObjectSuffix) -MF$(IntermediateDirectory)/txw80x_vectors$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/txw80x_vectors$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/txw80x_vectors$(PreprocessSuffix): ../sdk/chip/txw80x/vectors.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/txw80x_vectors$(PreprocessSuffix) ../sdk/chip/txw80x/vectors.S

$(IntermediateDirectory)/dma_dw_dmac$(ObjectSuffix): ../sdk/driver/dma/dw_dmac.c  
	$(CC) $(SourceSwitch) ../sdk/driver/dma/dw_dmac.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/dma_dw_dmac$(ObjectSuffix) -MF$(IntermediateDirectory)/dma_dw_dmac$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/dma_dw_dmac$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/dma_dw_dmac$(PreprocessSuffix): ../sdk/driver/dma/dw_dmac.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/dma_dw_dmac$(PreprocessSuffix) ../sdk/driver/dma/dw_dmac.c

$(IntermediateDirectory)/dma_hg_m2m_dma$(ObjectSuffix): ../sdk/driver/dma/hg_m2m_dma.c  
	$(CC) $(SourceSwitch) ../sdk/driver/dma/hg_m2m_dma.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/dma_hg_m2m_dma$(ObjectSuffix) -MF$(IntermediateDirectory)/dma_hg_m2m_dma$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/dma_hg_m2m_dma$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/dma_hg_m2m_dma$(PreprocessSuffix): ../sdk/driver/dma/hg_m2m_dma.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/dma_hg_m2m_dma$(PreprocessSuffix) ../sdk/driver/dma/hg_m2m_dma.c

$(IntermediateDirectory)/gpio_hggpio_v3$(ObjectSuffix): ../sdk/driver/gpio/hggpio_v3.c  
	$(CC) $(SourceSwitch) ../sdk/driver/gpio/hggpio_v3.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/gpio_hggpio_v3$(ObjectSuffix) -MF$(IntermediateDirectory)/gpio_hggpio_v3$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/gpio_hggpio_v3$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/gpio_hggpio_v3$(PreprocessSuffix): ../sdk/driver/gpio/hggpio_v3.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/gpio_hggpio_v3$(PreprocessSuffix) ../sdk/driver/gpio/hggpio_v3.c

$(IntermediateDirectory)/i2c_hgi2c$(ObjectSuffix): ../sdk/driver/i2c/hgi2c.c  
	$(CC) $(SourceSwitch) ../sdk/driver/i2c/hgi2c.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/i2c_hgi2c$(ObjectSuffix) -MF$(IntermediateDirectory)/i2c_hgi2c$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/i2c_hgi2c$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/i2c_hgi2c$(PreprocessSuffix): ../sdk/driver/i2c/hgi2c.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/i2c_hgi2c$(PreprocessSuffix) ../sdk/driver/i2c/hgi2c.c

$(IntermediateDirectory)/i2s_hgi2s_v0$(ObjectSuffix): ../sdk/driver/i2s/hgi2s_v0.c  
	$(CC) $(SourceSwitch) ../sdk/driver/i2s/hgi2s_v0.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/i2s_hgi2s_v0$(ObjectSuffix) -MF$(IntermediateDirectory)/i2s_hgi2s_v0$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/i2s_hgi2s_v0$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/i2s_hgi2s_v0$(PreprocessSuffix): ../sdk/driver/i2s/hgi2s_v0.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/i2s_hgi2s_v0$(PreprocessSuffix) ../sdk/driver/i2s/hgi2s_v0.c

$(IntermediateDirectory)/spi_hgspi_v1$(ObjectSuffix): ../sdk/driver/spi/hgspi_v1.c  
	$(CC) $(SourceSwitch) ../sdk/driver/spi/hgspi_v1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spi_hgspi_v1$(ObjectSuffix) -MF$(IntermediateDirectory)/spi_hgspi_v1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spi_hgspi_v1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spi_hgspi_v1$(PreprocessSuffix): ../sdk/driver/spi/hgspi_v1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spi_hgspi_v1$(PreprocessSuffix) ../sdk/driver/spi/hgspi_v1.c

$(IntermediateDirectory)/timer_hgtimer_v6$(ObjectSuffix): ../sdk/driver/timer/hgtimer_v6.c  
	$(CC) $(SourceSwitch) ../sdk/driver/timer/hgtimer_v6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/timer_hgtimer_v6$(ObjectSuffix) -MF$(IntermediateDirectory)/timer_hgtimer_v6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/timer_hgtimer_v6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/timer_hgtimer_v6$(PreprocessSuffix): ../sdk/driver/timer/hgtimer_v6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/timer_hgtimer_v6$(PreprocessSuffix) ../sdk/driver/timer/hgtimer_v6.c

$(IntermediateDirectory)/uart_hguart_v2$(ObjectSuffix): ../sdk/driver/uart/hguart_v2.c  
	$(CC) $(SourceSwitch) ../sdk/driver/uart/hguart_v2.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/uart_hguart_v2$(ObjectSuffix) -MF$(IntermediateDirectory)/uart_hguart_v2$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/uart_hguart_v2$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/uart_hguart_v2$(PreprocessSuffix): ../sdk/driver/uart/hguart_v2.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/uart_hguart_v2$(PreprocessSuffix) ../sdk/driver/uart/hguart_v2.c

$(IntermediateDirectory)/wdt_hg_wdt_v1$(ObjectSuffix): ../sdk/driver/wdt/hg_wdt_v1.c  
	$(CC) $(SourceSwitch) ../sdk/driver/wdt/hg_wdt_v1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/wdt_hg_wdt_v1$(ObjectSuffix) -MF$(IntermediateDirectory)/wdt_hg_wdt_v1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/wdt_hg_wdt_v1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/wdt_hg_wdt_v1$(PreprocessSuffix): ../sdk/driver/wdt/hg_wdt_v1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/wdt_hg_wdt_v1$(PreprocessSuffix) ../sdk/driver/wdt/hg_wdt_v1.c

$(IntermediateDirectory)/crc_hg_crc$(ObjectSuffix): ../sdk/driver/crc/hg_crc.c  
	$(CC) $(SourceSwitch) ../sdk/driver/crc/hg_crc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/crc_hg_crc$(ObjectSuffix) -MF$(IntermediateDirectory)/crc_hg_crc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/crc_hg_crc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/crc_hg_crc$(PreprocessSuffix): ../sdk/driver/crc/hg_crc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/crc_hg_crc$(PreprocessSuffix) ../sdk/driver/crc/hg_crc.c

$(IntermediateDirectory)/sdhost_hgsdh_dev$(ObjectSuffix): ../sdk/driver/sdhost/hgsdh_dev.c  
	$(CC) $(SourceSwitch) ../sdk/driver/sdhost/hgsdh_dev.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sdhost_hgsdh_dev$(ObjectSuffix) -MF$(IntermediateDirectory)/sdhost_hgsdh_dev$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sdhost_hgsdh_dev$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sdhost_hgsdh_dev$(PreprocessSuffix): ../sdk/driver/sdhost/hgsdh_dev.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sdhost_hgsdh_dev$(PreprocessSuffix) ../sdk/driver/sdhost/hgsdh_dev.c

$(IntermediateDirectory)/pdm_hgpdm_v0$(ObjectSuffix): ../sdk/driver/pdm/hgpdm_v0.c  
	$(CC) $(SourceSwitch) ../sdk/driver/pdm/hgpdm_v0.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/pdm_hgpdm_v0$(ObjectSuffix) -MF$(IntermediateDirectory)/pdm_hgpdm_v0$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/pdm_hgpdm_v0$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/pdm_hgpdm_v0$(PreprocessSuffix): ../sdk/driver/pdm/hgpdm_v0.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/pdm_hgpdm_v0$(PreprocessSuffix) ../sdk/driver/pdm/hgpdm_v0.c

$(IntermediateDirectory)/led_hgled_v0$(ObjectSuffix): ../sdk/driver/led/hgled_v0.c  
	$(CC) $(SourceSwitch) ../sdk/driver/led/hgled_v0.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/led_hgled_v0$(ObjectSuffix) -MF$(IntermediateDirectory)/led_hgled_v0$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/led_hgled_v0$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/led_hgled_v0$(PreprocessSuffix): ../sdk/driver/led/hgled_v0.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/led_hgled_v0$(PreprocessSuffix) ../sdk/driver/led/hgled_v0.c

$(IntermediateDirectory)/pwm_hgpwm_v0$(ObjectSuffix): ../sdk/driver/pwm/hgpwm_v0.c  
	$(CC) $(SourceSwitch) ../sdk/driver/pwm/hgpwm_v0.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/pwm_hgpwm_v0$(ObjectSuffix) -MF$(IntermediateDirectory)/pwm_hgpwm_v0$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/pwm_hgpwm_v0$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/pwm_hgpwm_v0$(PreprocessSuffix): ../sdk/driver/pwm/hgpwm_v0.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/pwm_hgpwm_v0$(PreprocessSuffix) ../sdk/driver/pwm/hgpwm_v0.c

$(IntermediateDirectory)/capture_hgcapture_v0$(ObjectSuffix): ../sdk/driver/capture/hgcapture_v0.c  
	$(CC) $(SourceSwitch) ../sdk/driver/capture/hgcapture_v0.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/capture_hgcapture_v0$(ObjectSuffix) -MF$(IntermediateDirectory)/capture_hgcapture_v0$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/capture_hgcapture_v0$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/capture_hgcapture_v0$(PreprocessSuffix): ../sdk/driver/capture/hgcapture_v0.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/capture_hgcapture_v0$(PreprocessSuffix) ../sdk/driver/capture/hgcapture_v0.c

$(IntermediateDirectory)/csi_hgdvp$(ObjectSuffix): ../sdk/driver/csi/hgdvp.c  
	$(CC) $(SourceSwitch) ../sdk/driver/csi/hgdvp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_hgdvp$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_hgdvp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_hgdvp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_hgdvp$(PreprocessSuffix): ../sdk/driver/csi/hgdvp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_hgdvp$(PreprocessSuffix) ../sdk/driver/csi/hgdvp.c

$(IntermediateDirectory)/adc_hgadc_v0$(ObjectSuffix): ../sdk/driver/adc/hgadc_v0.c  
	$(CC) $(SourceSwitch) ../sdk/driver/adc/hgadc_v0.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/adc_hgadc_v0$(ObjectSuffix) -MF$(IntermediateDirectory)/adc_hgadc_v0$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/adc_hgadc_v0$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/adc_hgadc_v0$(PreprocessSuffix): ../sdk/driver/adc/hgadc_v0.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/adc_hgadc_v0$(PreprocessSuffix) ../sdk/driver/adc/hgadc_v0.c

$(IntermediateDirectory)/sdhost_sdhost$(ObjectSuffix): ../sdk/lib/sdhost/sdhost.c  
	$(CC) $(SourceSwitch) ../sdk/lib/sdhost/sdhost.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sdhost_sdhost$(ObjectSuffix) -MF$(IntermediateDirectory)/sdhost_sdhost$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sdhost_sdhost$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sdhost_sdhost$(PreprocessSuffix): ../sdk/lib/sdhost/sdhost.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sdhost_sdhost$(PreprocessSuffix) ../sdk/lib/sdhost/sdhost.c

$(IntermediateDirectory)/usb_app_usb_device_wifi$(ObjectSuffix): ../sdk/lib/usb_app/usb_device_wifi.c  
	$(CC) $(SourceSwitch) ../sdk/lib/usb_app/usb_device_wifi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/usb_app_usb_device_wifi$(ObjectSuffix) -MF$(IntermediateDirectory)/usb_app_usb_device_wifi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/usb_app_usb_device_wifi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/usb_app_usb_device_wifi$(PreprocessSuffix): ../sdk/lib/usb_app/usb_device_wifi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/usb_app_usb_device_wifi$(PreprocessSuffix) ../sdk/lib/usb_app/usb_device_wifi.c

$(IntermediateDirectory)/usb_app_usb_host_uvc$(ObjectSuffix): ../sdk/lib/usb_app/usb_host_uvc.c  
	$(CC) $(SourceSwitch) ../sdk/lib/usb_app/usb_host_uvc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/usb_app_usb_host_uvc$(ObjectSuffix) -MF$(IntermediateDirectory)/usb_app_usb_host_uvc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/usb_app_usb_host_uvc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/usb_app_usb_host_uvc$(PreprocessSuffix): ../sdk/lib/usb_app/usb_host_uvc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/usb_app_usb_host_uvc$(PreprocessSuffix) ../sdk/lib/usb_app/usb_host_uvc.c

$(IntermediateDirectory)/avi_avi_app$(ObjectSuffix): app/avi/avi_app.c  
	$(CC) $(SourceSwitch) app/avi/avi_app.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/avi_avi_app$(ObjectSuffix) -MF$(IntermediateDirectory)/avi_avi_app$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/avi_avi_app$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/avi_avi_app$(PreprocessSuffix): app/avi/avi_app.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/avi_avi_app$(PreprocessSuffix) app/avi/avi_app.c

$(IntermediateDirectory)/cmd_input_cmd_input$(ObjectSuffix): app/cmd_input/cmd_input.c  
	$(CC) $(SourceSwitch) app/cmd_input/cmd_input.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/cmd_input_cmd_input$(ObjectSuffix) -MF$(IntermediateDirectory)/cmd_input_cmd_input$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/cmd_input_cmd_input$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/cmd_input_cmd_input$(PreprocessSuffix): app/cmd_input/cmd_input.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/cmd_input_cmd_input$(PreprocessSuffix) app/cmd_input/cmd_input.c

$(IntermediateDirectory)/media_media$(ObjectSuffix): app/media/media.c  
	$(CC) $(SourceSwitch) app/media/media.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/media_media$(ObjectSuffix) -MF$(IntermediateDirectory)/media_media$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/media_media$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/media_media$(PreprocessSuffix): app/media/media.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/media_media$(PreprocessSuffix) app/media/media.c

$(IntermediateDirectory)/record_avi_record$(ObjectSuffix): app/record/avi_record.c  
	$(CC) $(SourceSwitch) app/record/avi_record.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/record_avi_record$(ObjectSuffix) -MF$(IntermediateDirectory)/record_avi_record$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/record_avi_record$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/record_avi_record$(PreprocessSuffix): app/record/avi_record.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/record_avi_record$(PreprocessSuffix) app/record/avi_record.c

$(IntermediateDirectory)/remoteControl_remote_control$(ObjectSuffix): app/remoteControl/remote_control.c  
	$(CC) $(SourceSwitch) app/remoteControl/remote_control.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/remoteControl_remote_control$(ObjectSuffix) -MF$(IntermediateDirectory)/remoteControl_remote_control$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/remoteControl_remote_control$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/remoteControl_remote_control$(PreprocessSuffix): app/remoteControl/remote_control.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/remoteControl_remote_control$(PreprocessSuffix) app/remoteControl/remote_control.c

$(IntermediateDirectory)/socketModule_socket_module$(ObjectSuffix): app/socketModule/socket_module.c  
	$(CC) $(SourceSwitch) app/socketModule/socket_module.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/socketModule_socket_module$(ObjectSuffix) -MF$(IntermediateDirectory)/socketModule_socket_module$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/socketModule_socket_module$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/socketModule_socket_module$(PreprocessSuffix): app/socketModule/socket_module.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/socketModule_socket_module$(PreprocessSuffix) app/socketModule/socket_module.c

$(IntermediateDirectory)/speedTest_speedTest$(ObjectSuffix): app/speedTest/speedTest.c  
	$(CC) $(SourceSwitch) app/speedTest/speedTest.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/speedTest_speedTest$(ObjectSuffix) -MF$(IntermediateDirectory)/speedTest_speedTest$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/speedTest_speedTest$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/speedTest_speedTest$(PreprocessSuffix): app/speedTest/speedTest.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/speedTest_speedTest$(PreprocessSuffix) app/speedTest/speedTest.c

$(IntermediateDirectory)/spook_access_log$(ObjectSuffix): app/spook/access_log.c  
	$(CC) $(SourceSwitch) app/spook/access_log.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_access_log$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_access_log$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_access_log$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_access_log$(PreprocessSuffix): app/spook/access_log.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_access_log$(PreprocessSuffix) app/spook/access_log.c

$(IntermediateDirectory)/spook_encoder-audio$(ObjectSuffix): app/spook/encoder-audio.c  
	$(CC) $(SourceSwitch) app/spook/encoder-audio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_encoder-audio$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_encoder-audio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_encoder-audio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_encoder-audio$(PreprocessSuffix): app/spook/encoder-audio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_encoder-audio$(PreprocessSuffix) app/spook/encoder-audio.c

$(IntermediateDirectory)/spook_encoder-jpeg$(ObjectSuffix): app/spook/encoder-jpeg.c  
	$(CC) $(SourceSwitch) app/spook/encoder-jpeg.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_encoder-jpeg$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_encoder-jpeg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_encoder-jpeg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_encoder-jpeg$(PreprocessSuffix): app/spook/encoder-jpeg.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_encoder-jpeg$(PreprocessSuffix) app/spook/encoder-jpeg.c

$(IntermediateDirectory)/spook_ephoto$(ObjectSuffix): app/spook/ephoto.c  
	$(CC) $(SourceSwitch) app/spook/ephoto.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_ephoto$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_ephoto$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_ephoto$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_ephoto$(PreprocessSuffix): app/spook/ephoto.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_ephoto$(PreprocessSuffix) app/spook/ephoto.c

$(IntermediateDirectory)/spook_frame$(ObjectSuffix): app/spook/frame.c  
	$(CC) $(SourceSwitch) app/spook/frame.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_frame$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_frame$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_frame$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_frame$(PreprocessSuffix): app/spook/frame.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_frame$(PreprocessSuffix) app/spook/frame.c

$(IntermediateDirectory)/spook_http$(ObjectSuffix): app/spook/http.c  
	$(CC) $(SourceSwitch) app/spook/http.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_http$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_http$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_http$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_http$(PreprocessSuffix): app/spook/http.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_http$(PreprocessSuffix) app/spook/http.c

$(IntermediateDirectory)/spook_live$(ObjectSuffix): app/spook/live.c  
	$(CC) $(SourceSwitch) app/spook/live.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_live$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_live$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_live$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_live$(PreprocessSuffix): app/spook/live.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_live$(PreprocessSuffix) app/spook/live.c

$(IntermediateDirectory)/spook_log$(ObjectSuffix): app/spook/log.c  
	$(CC) $(SourceSwitch) app/spook/log.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_log$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_log$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_log$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_log$(PreprocessSuffix): app/spook/log.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_log$(PreprocessSuffix) app/spook/log.c

$(IntermediateDirectory)/spook_pmsg$(ObjectSuffix): app/spook/pmsg.c  
	$(CC) $(SourceSwitch) app/spook/pmsg.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_pmsg$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_pmsg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_pmsg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_pmsg$(PreprocessSuffix): app/spook/pmsg.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_pmsg$(PreprocessSuffix) app/spook/pmsg.c

$(IntermediateDirectory)/spook_rtp-audio$(ObjectSuffix): app/spook/rtp-audio.c  
	$(CC) $(SourceSwitch) app/spook/rtp-audio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_rtp-audio$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_rtp-audio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_rtp-audio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_rtp-audio$(PreprocessSuffix): app/spook/rtp-audio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_rtp-audio$(PreprocessSuffix) app/spook/rtp-audio.c

$(IntermediateDirectory)/spook_rtp-jpeg$(ObjectSuffix): app/spook/rtp-jpeg.c  
	$(CC) $(SourceSwitch) app/spook/rtp-jpeg.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_rtp-jpeg$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_rtp-jpeg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_rtp-jpeg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_rtp-jpeg$(PreprocessSuffix): app/spook/rtp-jpeg.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_rtp-jpeg$(PreprocessSuffix) app/spook/rtp-jpeg.c

$(IntermediateDirectory)/spook_rtp$(ObjectSuffix): app/spook/rtp.c  
	$(CC) $(SourceSwitch) app/spook/rtp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_rtp$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_rtp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_rtp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_rtp$(PreprocessSuffix): app/spook/rtp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_rtp$(PreprocessSuffix) app/spook/rtp.c

$(IntermediateDirectory)/spook_rtsp$(ObjectSuffix): app/spook/rtsp.c  
	$(CC) $(SourceSwitch) app/spook/rtsp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_rtsp$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_rtsp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_rtsp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_rtsp$(PreprocessSuffix): app/spook/rtsp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_rtsp$(PreprocessSuffix) app/spook/rtsp.c

$(IntermediateDirectory)/spook_session$(ObjectSuffix): app/spook/session.c  
	$(CC) $(SourceSwitch) app/spook/session.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_session$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_session$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_session$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_session$(PreprocessSuffix): app/spook/session.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_session$(PreprocessSuffix) app/spook/session.c

$(IntermediateDirectory)/spook_spook$(ObjectSuffix): app/spook/spook.c  
	$(CC) $(SourceSwitch) app/spook/spook.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_spook$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_spook$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_spook$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_spook$(PreprocessSuffix): app/spook/spook.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_spook$(PreprocessSuffix) app/spook/spook.c

$(IntermediateDirectory)/spook_spookctl$(ObjectSuffix): app/spook/spookctl.c  
	$(CC) $(SourceSwitch) app/spook/spookctl.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_spookctl$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_spookctl$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_spookctl$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_spookctl$(PreprocessSuffix): app/spook/spookctl.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_spookctl$(PreprocessSuffix) app/spook/spookctl.c

$(IntermediateDirectory)/spook_tcp$(ObjectSuffix): app/spook/tcp.c  
	$(CC) $(SourceSwitch) app/spook/tcp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_tcp$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_tcp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_tcp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_tcp$(PreprocessSuffix): app/spook/tcp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_tcp$(PreprocessSuffix) app/spook/tcp.c

$(IntermediateDirectory)/spook_webfile$(ObjectSuffix): app/spook/webfile.c  
	$(CC) $(SourceSwitch) app/spook/webfile.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spook_webfile$(ObjectSuffix) -MF$(IntermediateDirectory)/spook_webfile$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spook_webfile$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spook_webfile$(PreprocessSuffix): app/spook/webfile.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spook_webfile$(PreprocessSuffix) app/spook/webfile.c

$(IntermediateDirectory)/update_ota$(ObjectSuffix): app/update/ota.c  
	$(CC) $(SourceSwitch) app/update/ota.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/update_ota$(ObjectSuffix) -MF$(IntermediateDirectory)/update_ota$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/update_ota$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/update_ota$(PreprocessSuffix): app/update/ota.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/update_ota$(PreprocessSuffix) app/update/ota.c

$(IntermediateDirectory)/gsensor_g_sensor$(ObjectSuffix): app/gsensor/g_sensor.c  
	$(CC) $(SourceSwitch) app/gsensor/g_sensor.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/gsensor_g_sensor$(ObjectSuffix) -MF$(IntermediateDirectory)/gsensor_g_sensor$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/gsensor_g_sensor$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/gsensor_g_sensor$(PreprocessSuffix): app/gsensor/g_sensor.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/gsensor_g_sensor$(PreprocessSuffix) app/gsensor/g_sensor.c

$(IntermediateDirectory)/xmodem_tx_xmodem_tx$(ObjectSuffix): app/xmodem_tx/xmodem_tx.c  
	$(CC) $(SourceSwitch) app/xmodem_tx/xmodem_tx.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/xmodem_tx_xmodem_tx$(ObjectSuffix) -MF$(IntermediateDirectory)/xmodem_tx_xmodem_tx$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/xmodem_tx_xmodem_tx$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/xmodem_tx_xmodem_tx$(PreprocessSuffix): app/xmodem_tx/xmodem_tx.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/xmodem_tx_xmodem_tx$(PreprocessSuffix) app/xmodem_tx/xmodem_tx.c

$(IntermediateDirectory)/audio_audio_8311_demo$(ObjectSuffix): app/audio/audio_8311_demo.c  
	$(CC) $(SourceSwitch) app/audio/audio_8311_demo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/audio_audio_8311_demo$(ObjectSuffix) -MF$(IntermediateDirectory)/audio_audio_8311_demo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/audio_audio_8311_demo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/audio_audio_8311_demo$(PreprocessSuffix): app/audio/audio_8311_demo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/audio_audio_8311_demo$(PreprocessSuffix) app/audio/audio_8311_demo.c

$(IntermediateDirectory)/audio_audio_demo$(ObjectSuffix): app/audio/audio_demo.c  
	$(CC) $(SourceSwitch) app/audio/audio_demo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/audio_audio_demo$(ObjectSuffix) -MF$(IntermediateDirectory)/audio_audio_demo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/audio_audio_demo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/audio_audio_demo$(PreprocessSuffix): app/audio/audio_demo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/audio_audio_demo$(PreprocessSuffix) app/audio/audio_demo.c

$(IntermediateDirectory)/audio_hal_i2s_audio$(ObjectSuffix): app/audio/hal_i2s_audio.c  
	$(CC) $(SourceSwitch) app/audio/hal_i2s_audio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/audio_hal_i2s_audio$(ObjectSuffix) -MF$(IntermediateDirectory)/audio_hal_i2s_audio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/audio_hal_i2s_audio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/audio_hal_i2s_audio$(PreprocessSuffix): app/audio/hal_i2s_audio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/audio_hal_i2s_audio$(PreprocessSuffix) app/audio/hal_i2s_audio.c

$(IntermediateDirectory)/audio_pdm_audio$(ObjectSuffix): app/audio/pdm_audio.c  
	$(CC) $(SourceSwitch) app/audio/pdm_audio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/audio_pdm_audio$(ObjectSuffix) -MF$(IntermediateDirectory)/audio_pdm_audio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/audio_pdm_audio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/audio_pdm_audio$(PreprocessSuffix): app/audio/pdm_audio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/audio_pdm_audio$(PreprocessSuffix) app/audio/pdm_audio.c

$(IntermediateDirectory)/sta_interface_sta_interface$(ObjectSuffix): app/sta_interface/sta_interface.c  
	$(CC) $(SourceSwitch) app/sta_interface/sta_interface.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sta_interface_sta_interface$(ObjectSuffix) -MF$(IntermediateDirectory)/sta_interface_sta_interface$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sta_interface_sta_interface$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sta_interface_sta_interface$(PreprocessSuffix): app/sta_interface/sta_interface.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sta_interface_sta_interface$(PreprocessSuffix) app/sta_interface/sta_interface.c

$(IntermediateDirectory)/i4s_aes_core$(ObjectSuffix): app/i4s/aes_core.c  
	$(CC) $(SourceSwitch) app/i4s/aes_core.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/i4s_aes_core$(ObjectSuffix) -MF$(IntermediateDirectory)/i4s_aes_core$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/i4s_aes_core$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/i4s_aes_core$(PreprocessSuffix): app/i4s/aes_core.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/i4s_aes_core$(PreprocessSuffix) app/i4s/aes_core.c

$(IntermediateDirectory)/i4s_alk$(ObjectSuffix): app/i4s/alk.c  
	$(CC) $(SourceSwitch) app/i4s/alk.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/i4s_alk$(ObjectSuffix) -MF$(IntermediateDirectory)/i4s_alk$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/i4s_alk$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/i4s_alk$(PreprocessSuffix): app/i4s/alk.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/i4s_alk$(PreprocessSuffix) app/i4s/alk.c

$(IntermediateDirectory)/i4s_i4s$(ObjectSuffix): app/i4s/i4s.c  
	$(CC) $(SourceSwitch) app/i4s/i4s.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/i4s_i4s$(ObjectSuffix) -MF$(IntermediateDirectory)/i4s_i4s$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/i4s_i4s$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/i4s_i4s$(PreprocessSuffix): app/i4s/i4s.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/i4s_i4s$(PreprocessSuffix) app/i4s/i4s.c

$(IntermediateDirectory)/i4s_i4s_port$(ObjectSuffix): app/i4s/i4s_port.c  
	$(CC) $(SourceSwitch) app/i4s/i4s_port.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/i4s_i4s_port$(ObjectSuffix) -MF$(IntermediateDirectory)/i4s_i4s_port$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/i4s_i4s_port$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/i4s_i4s_port$(PreprocessSuffix): app/i4s/i4s_port.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/i4s_i4s_port$(PreprocessSuffix) app/i4s/i4s_port.c

$(IntermediateDirectory)/i4s_i4sgsensor$(ObjectSuffix): app/i4s/i4sgsensor.c  
	$(CC) $(SourceSwitch) app/i4s/i4sgsensor.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/i4s_i4sgsensor$(ObjectSuffix) -MF$(IntermediateDirectory)/i4s_i4sgsensor$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/i4s_i4sgsensor$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/i4s_i4sgsensor$(PreprocessSuffix): app/i4s/i4sgsensor.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/i4s_i4sgsensor$(PreprocessSuffix) app/i4s/i4sgsensor.c

$(IntermediateDirectory)/i4s_mcu_icb-4u27$(ObjectSuffix): app/i4s/i4s_mcu/icb-4u27.c  
	$(CC) $(SourceSwitch) app/i4s/i4s_mcu/icb-4u27.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/i4s_mcu_icb-4u27$(ObjectSuffix) -MF$(IntermediateDirectory)/i4s_mcu_icb-4u27$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/i4s_mcu_icb-4u27$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/i4s_mcu_icb-4u27$(PreprocessSuffix): app/i4s/i4s_mcu/icb-4u27.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/i4s_mcu_icb-4u27$(PreprocessSuffix) app/i4s/i4s_mcu/icb-4u27.c

$(IntermediateDirectory)/adapter_csi_rhino$(ObjectSuffix): ../csky/csi_kernel/rhino/adapter/csi_rhino.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/adapter/csi_rhino.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/adapter_csi_rhino$(ObjectSuffix) -MF$(IntermediateDirectory)/adapter_csi_rhino$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/adapter_csi_rhino$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/adapter_csi_rhino$(PreprocessSuffix): ../csky/csi_kernel/rhino/adapter/csi_rhino.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/adapter_csi_rhino$(PreprocessSuffix) ../csky/csi_kernel/rhino/adapter/csi_rhino.c

$(IntermediateDirectory)/core_k_buf_queue$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_buf_queue.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_buf_queue.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_buf_queue$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_buf_queue$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_buf_queue$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_buf_queue$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_buf_queue.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_buf_queue$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_buf_queue.c

$(IntermediateDirectory)/core_k_dyn_mem_proc$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_dyn_mem_proc.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_dyn_mem_proc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_dyn_mem_proc$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_dyn_mem_proc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_dyn_mem_proc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_dyn_mem_proc$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_dyn_mem_proc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_dyn_mem_proc$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_dyn_mem_proc.c

$(IntermediateDirectory)/core_k_err$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_err.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_err.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_err$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_err$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_err$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_err$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_err.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_err$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_err.c

$(IntermediateDirectory)/core_k_event$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_event.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_event.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_event$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_event$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_event$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_event$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_event.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_event$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_event.c

$(IntermediateDirectory)/core_k_idle$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_idle.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_idle.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_idle$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_idle$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_idle$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_idle$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_idle.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_idle$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_idle.c

$(IntermediateDirectory)/core_k_mm$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_mm.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_mm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_mm$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_mm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_mm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_mm$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_mm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_mm$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_mm.c

$(IntermediateDirectory)/core_k_mm_blk$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_mm_blk.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_mm_blk.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_mm_blk$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_mm_blk$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_mm_blk$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_mm_blk$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_mm_blk.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_mm_blk$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_mm_blk.c

$(IntermediateDirectory)/core_k_mm_debug$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_mm_debug.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_mm_debug.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_mm_debug$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_mm_debug$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_mm_debug$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_mm_debug$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_mm_debug.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_mm_debug$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_mm_debug.c

$(IntermediateDirectory)/core_k_mutex$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_mutex.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_mutex.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_mutex$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_mutex$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_mutex$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_mutex$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_mutex.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_mutex$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_mutex.c

$(IntermediateDirectory)/core_k_obj$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_obj.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_obj.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_obj$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_obj$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_obj$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_obj$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_obj.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_obj$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_obj.c

$(IntermediateDirectory)/core_k_pend$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_pend.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_pend.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_pend$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_pend$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_pend$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_pend$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_pend.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_pend$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_pend.c

$(IntermediateDirectory)/core_k_queue$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_queue.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_queue.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_queue$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_queue$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_queue$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_queue$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_queue.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_queue$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_queue.c

$(IntermediateDirectory)/core_k_ringbuf$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_ringbuf.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_ringbuf.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_ringbuf$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_ringbuf$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_ringbuf$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_ringbuf$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_ringbuf.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_ringbuf$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_ringbuf.c

$(IntermediateDirectory)/core_k_sched$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_sched.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_sched.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_sched$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_sched$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_sched$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_sched$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_sched.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_sched$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_sched.c

$(IntermediateDirectory)/core_k_sem$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_sem.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_sem.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_sem$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_sem$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_sem$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_sem$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_sem.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_sem$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_sem.c

$(IntermediateDirectory)/core_k_stats$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_stats.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_stats.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_stats$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_stats$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_stats$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_stats$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_stats.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_stats$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_stats.c

$(IntermediateDirectory)/core_k_sys$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_sys.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_sys.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_sys$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_sys$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_sys$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_sys$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_sys.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_sys$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_sys.c

$(IntermediateDirectory)/core_k_task$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_task.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_task.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_task$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_task$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_task$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_task$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_task.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_task$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_task.c

$(IntermediateDirectory)/core_k_task_sem$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_task_sem.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_task_sem.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_task_sem$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_task_sem$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_task_sem$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_task_sem$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_task_sem.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_task_sem$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_task_sem.c

$(IntermediateDirectory)/core_k_tick$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_tick.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_tick.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_tick$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_tick$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_tick$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_tick$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_tick.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_tick$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_tick.c

$(IntermediateDirectory)/core_k_time$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_time.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_time.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_time$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_time$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_time$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_time$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_time.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_time$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_time.c

$(IntermediateDirectory)/core_k_timer$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_timer.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_timer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_timer$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_timer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_timer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_timer$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_timer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_timer$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_timer.c

$(IntermediateDirectory)/core_k_workqueue$(ObjectSuffix): ../csky/csi_kernel/rhino/core/k_workqueue.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/core/k_workqueue.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_k_workqueue$(ObjectSuffix) -MF$(IntermediateDirectory)/core_k_workqueue$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_k_workqueue$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_k_workqueue$(PreprocessSuffix): ../csky/csi_kernel/rhino/core/k_workqueue.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_k_workqueue$(PreprocessSuffix) ../csky/csi_kernel/rhino/core/k_workqueue.c

$(IntermediateDirectory)/driver_hook_impl$(ObjectSuffix): ../csky/csi_kernel/rhino/driver/hook_impl.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/driver/hook_impl.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/driver_hook_impl$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_hook_impl$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/driver_hook_impl$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/driver_hook_impl$(PreprocessSuffix): ../csky/csi_kernel/rhino/driver/hook_impl.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/driver_hook_impl$(PreprocessSuffix) ../csky/csi_kernel/rhino/driver/hook_impl.c

$(IntermediateDirectory)/driver_systick$(ObjectSuffix): ../csky/csi_kernel/rhino/driver/systick.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/driver/systick.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/driver_systick$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_systick$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/driver_systick$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/driver_systick$(PreprocessSuffix): ../csky/csi_kernel/rhino/driver/systick.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/driver_systick$(PreprocessSuffix) ../csky/csi_kernel/rhino/driver/systick.c

$(IntermediateDirectory)/driver_yoc_impl$(ObjectSuffix): ../csky/csi_kernel/rhino/driver/yoc_impl.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/driver/yoc_impl.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/driver_yoc_impl$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_yoc_impl$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/driver_yoc_impl$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/driver_yoc_impl$(PreprocessSuffix): ../csky/csi_kernel/rhino/driver/yoc_impl.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/driver_yoc_impl$(PreprocessSuffix) ../csky/csi_kernel/rhino/driver/yoc_impl.c

$(IntermediateDirectory)/driver_hook_weak$(ObjectSuffix): ../csky/csi_kernel/rhino/driver/hook_weak.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/driver/hook_weak.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/driver_hook_weak$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_hook_weak$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/driver_hook_weak$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/driver_hook_weak$(PreprocessSuffix): ../csky/csi_kernel/rhino/driver/hook_weak.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/driver_hook_weak$(PreprocessSuffix) ../csky/csi_kernel/rhino/driver/hook_weak.c

$(IntermediateDirectory)/fatfs_diskio$(ObjectSuffix): ../sdk/lib/fs/fatfs/diskio.c  
	$(CC) $(SourceSwitch) ../sdk/lib/fs/fatfs/diskio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/fatfs_diskio$(ObjectSuffix) -MF$(IntermediateDirectory)/fatfs_diskio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_diskio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/fatfs_diskio$(PreprocessSuffix): ../sdk/lib/fs/fatfs/diskio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/fatfs_diskio$(PreprocessSuffix) ../sdk/lib/fs/fatfs/diskio.c

$(IntermediateDirectory)/fatfs_fatfs_test$(ObjectSuffix): ../sdk/lib/fs/fatfs/fatfs_test.c  
	$(CC) $(SourceSwitch) ../sdk/lib/fs/fatfs/fatfs_test.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/fatfs_fatfs_test$(ObjectSuffix) -MF$(IntermediateDirectory)/fatfs_fatfs_test$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_fatfs_test$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/fatfs_fatfs_test$(PreprocessSuffix): ../sdk/lib/fs/fatfs/fatfs_test.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/fatfs_fatfs_test$(PreprocessSuffix) ../sdk/lib/fs/fatfs/fatfs_test.c

$(IntermediateDirectory)/fatfs_ff$(ObjectSuffix): ../sdk/lib/fs/fatfs/ff.c  
	$(CC) $(SourceSwitch) ../sdk/lib/fs/fatfs/ff.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/fatfs_ff$(ObjectSuffix) -MF$(IntermediateDirectory)/fatfs_ff$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_ff$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/fatfs_ff$(PreprocessSuffix): ../sdk/lib/fs/fatfs/ff.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/fatfs_ff$(PreprocessSuffix) ../sdk/lib/fs/fatfs/ff.c

$(IntermediateDirectory)/fatfs_ffunicode$(ObjectSuffix): ../sdk/lib/fs/fatfs/ffunicode.c  
	$(CC) $(SourceSwitch) ../sdk/lib/fs/fatfs/ffunicode.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/fatfs_ffunicode$(ObjectSuffix) -MF$(IntermediateDirectory)/fatfs_ffunicode$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_ffunicode$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/fatfs_ffunicode$(PreprocessSuffix): ../sdk/lib/fs/fatfs/ffunicode.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/fatfs_ffunicode$(PreprocessSuffix) ../sdk/lib/fs/fatfs/ffunicode.c

$(IntermediateDirectory)/fatfs_syscall$(ObjectSuffix): ../sdk/lib/fs/fatfs/syscall.c  
	$(CC) $(SourceSwitch) ../sdk/lib/fs/fatfs/syscall.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/fatfs_syscall$(ObjectSuffix) -MF$(IntermediateDirectory)/fatfs_syscall$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_syscall$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/fatfs_syscall$(PreprocessSuffix): ../sdk/lib/fs/fatfs/syscall.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/fatfs_syscall$(PreprocessSuffix) ../sdk/lib/fs/fatfs/syscall.c

$(IntermediateDirectory)/fatfs_osal_file$(ObjectSuffix): ../sdk/lib/fs/fatfs/osal_file.c  
	$(CC) $(SourceSwitch) ../sdk/lib/fs/fatfs/osal_file.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/fatfs_osal_file$(ObjectSuffix) -MF$(IntermediateDirectory)/fatfs_osal_file$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_osal_file$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/fatfs_osal_file$(PreprocessSuffix): ../sdk/lib/fs/fatfs/osal_file.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/fatfs_osal_file$(PreprocessSuffix) ../sdk/lib/fs/fatfs/osal_file.c

$(IntermediateDirectory)/uvc_hg_usb_device$(ObjectSuffix): ../sdk/lib/video/uvc/hg_usb_device.c  
	$(CC) $(SourceSwitch) ../sdk/lib/video/uvc/hg_usb_device.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/uvc_hg_usb_device$(ObjectSuffix) -MF$(IntermediateDirectory)/uvc_hg_usb_device$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/uvc_hg_usb_device$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/uvc_hg_usb_device$(PreprocessSuffix): ../sdk/lib/video/uvc/hg_usb_device.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/uvc_hg_usb_device$(PreprocessSuffix) ../sdk/lib/video/uvc/hg_usb_device.c

$(IntermediateDirectory)/uvc_uvc_host$(ObjectSuffix): ../sdk/lib/video/uvc/uvc_host.c  
	$(CC) $(SourceSwitch) ../sdk/lib/video/uvc/uvc_host.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/uvc_uvc_host$(ObjectSuffix) -MF$(IntermediateDirectory)/uvc_uvc_host$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/uvc_uvc_host$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/uvc_uvc_host$(PreprocessSuffix): ../sdk/lib/video/uvc/uvc_host.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/uvc_uvc_host$(PreprocessSuffix) ../sdk/lib/video/uvc/uvc_host.c

$(IntermediateDirectory)/de_avi_avidemux$(ObjectSuffix): ../sdk/lib/video/de_avi/avidemux.c  
	$(CC) $(SourceSwitch) ../sdk/lib/video/de_avi/avidemux.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/de_avi_avidemux$(ObjectSuffix) -MF$(IntermediateDirectory)/de_avi_avidemux$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/de_avi_avidemux$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/de_avi_avidemux$(PreprocessSuffix): ../sdk/lib/video/de_avi/avidemux.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/de_avi_avidemux$(PreprocessSuffix) ../sdk/lib/video/de_avi/avidemux.c

$(IntermediateDirectory)/en_avi_openDML$(ObjectSuffix): ../sdk/lib/video/en_avi/openDML.c  
	$(CC) $(SourceSwitch) ../sdk/lib/video/en_avi/openDML.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/en_avi_openDML$(ObjectSuffix) -MF$(IntermediateDirectory)/en_avi_openDML$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/en_avi_openDML$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/en_avi_openDML$(PreprocessSuffix): ../sdk/lib/video/en_avi/openDML.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/en_avi_openDML$(PreprocessSuffix) ../sdk/lib/video/en_avi/openDML.c

$(IntermediateDirectory)/frame_memt_frame_memt$(ObjectSuffix): app/algorithm/frame_memt/frame_memt.c  
	$(CC) $(SourceSwitch) app/algorithm/frame_memt/frame_memt.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/frame_memt_frame_memt$(ObjectSuffix) -MF$(IntermediateDirectory)/frame_memt_frame_memt$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/frame_memt_frame_memt$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/frame_memt_frame_memt$(PreprocessSuffix): app/algorithm/frame_memt/frame_memt.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/frame_memt_frame_memt$(PreprocessSuffix) app/algorithm/frame_memt/frame_memt.c

$(IntermediateDirectory)/pdmFilter_pdmFilter$(ObjectSuffix): app/algorithm/pdmFilter/pdmFilter.c  
	$(CC) $(SourceSwitch) app/algorithm/pdmFilter/pdmFilter.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/pdmFilter_pdmFilter$(ObjectSuffix) -MF$(IntermediateDirectory)/pdmFilter_pdmFilter$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/pdmFilter_pdmFilter$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/pdmFilter_pdmFilter$(PreprocessSuffix): app/algorithm/pdmFilter/pdmFilter.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/pdmFilter_pdmFilter$(PreprocessSuffix) app/algorithm/pdmFilter/pdmFilter.c

$(IntermediateDirectory)/csky_cpu_impl$(ObjectSuffix): ../csky/csi_kernel/rhino/arch/csky/cpu_impl.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/arch/csky/cpu_impl.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csky_cpu_impl$(ObjectSuffix) -MF$(IntermediateDirectory)/csky_cpu_impl$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csky_cpu_impl$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csky_cpu_impl$(PreprocessSuffix): ../csky/csi_kernel/rhino/arch/csky/cpu_impl.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csky_cpu_impl$(PreprocessSuffix) ../csky/csi_kernel/rhino/arch/csky/cpu_impl.c

$(IntermediateDirectory)/csky_csky_sched$(ObjectSuffix): ../csky/csi_kernel/rhino/arch/csky/csky_sched.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/arch/csky/csky_sched.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csky_csky_sched$(ObjectSuffix) -MF$(IntermediateDirectory)/csky_csky_sched$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csky_csky_sched$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csky_csky_sched$(PreprocessSuffix): ../csky/csi_kernel/rhino/arch/csky/csky_sched.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csky_csky_sched$(PreprocessSuffix) ../csky/csi_kernel/rhino/arch/csky/csky_sched.c

$(IntermediateDirectory)/csky_port_c$(ObjectSuffix): ../csky/csi_kernel/rhino/arch/csky/port_c.c  
	$(CC) $(SourceSwitch) ../csky/csi_kernel/rhino/arch/csky/port_c.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csky_port_c$(ObjectSuffix) -MF$(IntermediateDirectory)/csky_port_c$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csky_port_c$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csky_port_c$(PreprocessSuffix): ../csky/csi_kernel/rhino/arch/csky/port_c.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csky_port_c$(PreprocessSuffix) ../csky/csi_kernel/rhino/arch/csky/port_c.c

$(IntermediateDirectory)/csky_port_s$(ObjectSuffix): ../csky/csi_kernel/rhino/arch/csky/port_s.S  
	$(AS) $(SourceSwitch) ../csky/csi_kernel/rhino/arch/csky/port_s.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csky_port_s$(ObjectSuffix) -MF$(IntermediateDirectory)/csky_port_s$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csky_port_s$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/csky_port_s$(PreprocessSuffix): ../csky/csi_kernel/rhino/arch/csky/port_s.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csky_port_s$(PreprocessSuffix) ../csky/csi_kernel/rhino/arch/csky/port_s.S

$(IntermediateDirectory)/arch_sys_arch$(ObjectSuffix): ../sdk/lib/net/lwip/arch/sys_arch.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/arch/sys_arch.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/arch_sys_arch$(ObjectSuffix) -MF$(IntermediateDirectory)/arch_sys_arch$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/arch_sys_arch$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/arch_sys_arch$(PreprocessSuffix): ../sdk/lib/net/lwip/arch/sys_arch.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/arch_sys_arch$(PreprocessSuffix) ../sdk/lib/net/lwip/arch/sys_arch.c

$(IntermediateDirectory)/spi_nor_spi_nor_bus$(ObjectSuffix): ../sdk/lib/bus/spi/spi_nor/spi_nor_bus.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/spi/spi_nor/spi_nor_bus.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/spi_nor_spi_nor_bus$(ObjectSuffix) -MF$(IntermediateDirectory)/spi_nor_spi_nor_bus$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/spi_nor_spi_nor_bus$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/spi_nor_spi_nor_bus$(PreprocessSuffix): ../sdk/lib/bus/spi/spi_nor/spi_nor_bus.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/spi_nor_spi_nor_bus$(PreprocessSuffix) ../sdk/lib/bus/spi/spi_nor/spi_nor_bus.c

$(IntermediateDirectory)/sensor_sensor_bf2013$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_bf2013.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_bf2013.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_bf2013$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_bf2013$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_bf2013$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_bf2013$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_bf2013.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_bf2013$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_bf2013.c

$(IntermediateDirectory)/sensor_sensor_bf3703$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_bf3703.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_bf3703.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_bf3703$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_bf3703$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_bf3703$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_bf3703$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_bf3703.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_bf3703$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_bf3703.c

$(IntermediateDirectory)/sensor_sensor_bf3a03$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_bf3a03.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_bf3a03.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_bf3a03$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_bf3a03$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_bf3a03$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_bf3a03$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_bf3a03.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_bf3a03$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_bf3a03.c

$(IntermediateDirectory)/sensor_sensor_gc0308$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0308.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_gc0308.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_gc0308$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_gc0308$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_gc0308$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_gc0308$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0308.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_gc0308$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_gc0308.c

$(IntermediateDirectory)/sensor_sensor_gc0309$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0309.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_gc0309.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_gc0309$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_gc0309$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_gc0309$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_gc0309$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0309.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_gc0309$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_gc0309.c

$(IntermediateDirectory)/sensor_sensor_gc0328$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0328.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_gc0328.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_gc0328$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_gc0328$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_gc0328$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_gc0328$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0328.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_gc0328$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_gc0328.c

$(IntermediateDirectory)/sensor_sensor_ov2640$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_ov2640.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_ov2640.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_ov2640$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_ov2640$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_ov2640$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_ov2640$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_ov2640.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_ov2640$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_ov2640.c

$(IntermediateDirectory)/sensor_sensor_ov7670$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_ov7670.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_ov7670.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_ov7670$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_ov7670$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_ov7670$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_ov7670$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_ov7670.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_ov7670$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_ov7670.c

$(IntermediateDirectory)/sensor_sensor_ov7725$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_ov7725.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_ov7725.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_ov7725$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_ov7725$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_ov7725$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_ov7725$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_ov7725.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_ov7725$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_ov7725.c

$(IntermediateDirectory)/sensor_sensor_xc7016_h63$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_xc7016_h63.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_xc7016_h63.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_xc7016_h63$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_xc7016_h63$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_xc7016_h63$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_xc7016_h63$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_xc7016_h63.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_xc7016_h63$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_xc7016_h63.c

$(IntermediateDirectory)/sensor_sensor_xcg532$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_xcg532.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_xcg532.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_xcg532$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_xcg532$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_xcg532$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_xcg532$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_xcg532.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_xcg532$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_xcg532.c

$(IntermediateDirectory)/sensor_sensor_xc7011_gc1054$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_xc7011_gc1054.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_xc7011_gc1054.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_xc7011_gc1054$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_xc7011_gc1054$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_xc7011_gc1054$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_xc7011_gc1054$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_xc7011_gc1054.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_xc7011_gc1054$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_xc7011_gc1054.c

$(IntermediateDirectory)/sensor_sensor_xc7011_h63$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_xc7011_h63.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_xc7011_h63.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_xc7011_h63$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_xc7011_h63$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_xc7011_h63$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_xc7011_h63$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_xc7011_h63.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_xc7011_h63$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_xc7011_h63.c

$(IntermediateDirectory)/sensor_sensor_gc0311$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0311.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_gc0311.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_gc0311$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_gc0311$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_gc0311$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_gc0311$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0311.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_gc0311$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_gc0311.c

$(IntermediateDirectory)/sensor_sensor_gc0329$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0329.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_gc0329.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_gc0329$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_gc0329$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_gc0329$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_gc0329$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0329.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_gc0329$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_gc0329.c

$(IntermediateDirectory)/sensor_sensor_gc0312$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0312.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_gc0312.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_gc0312$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_gc0312$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_gc0312$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_gc0312$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_gc0312.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_gc0312$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_gc0312.c

$(IntermediateDirectory)/sensor_sensor_bf3720$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_bf3720.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_bf3720.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_bf3720$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_bf3720$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_bf3720$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_bf3720$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_bf3720.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_bf3720$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_bf3720.c

$(IntermediateDirectory)/sensor_sensor_sp0a19$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_sp0a19.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_sp0a19.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_sp0a19$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_sp0a19$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_sp0a19$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_sp0a19$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_sp0a19.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_sp0a19$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_sp0a19.c

$(IntermediateDirectory)/sensor_sensor_sp0718$(ObjectSuffix): ../sdk/lib/bus/iic/sensor/sensor_sp0718.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/sensor/sensor_sp0718.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/sensor_sensor_sp0718$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_sensor_sp0718$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/sensor_sensor_sp0718$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/sensor_sensor_sp0718$(PreprocessSuffix): ../sdk/lib/bus/iic/sensor/sensor_sp0718.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/sensor_sensor_sp0718$(PreprocessSuffix) ../sdk/lib/bus/iic/sensor/sensor_sp0718.c

$(IntermediateDirectory)/gsensor_da280$(ObjectSuffix): ../sdk/lib/bus/iic/gsensor/da280.c  
	$(CC) $(SourceSwitch) ../sdk/lib/bus/iic/gsensor/da280.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/gsensor_da280$(ObjectSuffix) -MF$(IntermediateDirectory)/gsensor_da280$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/gsensor_da280$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/gsensor_da280$(PreprocessSuffix): ../sdk/lib/bus/iic/gsensor/da280.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/gsensor_da280$(PreprocessSuffix) ../sdk/lib/bus/iic/gsensor/da280.c

$(IntermediateDirectory)/cmos_sensor_csi$(ObjectSuffix): ../sdk/lib/video/dvp/cmos_sensor/csi.c  
	$(CC) $(SourceSwitch) ../sdk/lib/video/dvp/cmos_sensor/csi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/cmos_sensor_csi$(ObjectSuffix) -MF$(IntermediateDirectory)/cmos_sensor_csi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/cmos_sensor_csi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/cmos_sensor_csi$(PreprocessSuffix): ../sdk/lib/video/dvp/cmos_sensor/csi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/cmos_sensor_csi$(PreprocessSuffix) ../sdk/lib/video/dvp/cmos_sensor/csi.c

$(IntermediateDirectory)/jpeg_jpg$(ObjectSuffix): ../sdk/lib/video/dvp/jpeg/jpg.c  
	$(CC) $(SourceSwitch) ../sdk/lib/video/dvp/jpeg/jpg.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/jpeg_jpg$(ObjectSuffix) -MF$(IntermediateDirectory)/jpeg_jpg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/jpeg_jpg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/jpeg_jpg$(PreprocessSuffix): ../sdk/lib/video/dvp/jpeg/jpg.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/jpeg_jpg$(PreprocessSuffix) ../sdk/lib/video/dvp/jpeg/jpg.c

$(IntermediateDirectory)/jpeg_jpg_table$(ObjectSuffix): ../sdk/lib/video/dvp/jpeg/jpg_table.c  
	$(CC) $(SourceSwitch) ../sdk/lib/video/dvp/jpeg/jpg_table.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/jpeg_jpg_table$(ObjectSuffix) -MF$(IntermediateDirectory)/jpeg_jpg_table$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/jpeg_jpg_table$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/jpeg_jpg_table$(PreprocessSuffix): ../sdk/lib/video/dvp/jpeg/jpg_table.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/jpeg_jpg_table$(PreprocessSuffix) ../sdk/lib/video/dvp/jpeg/jpg_table.c

$(IntermediateDirectory)/api_api_lib$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/api_lib.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/api_lib.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_api_lib$(ObjectSuffix) -MF$(IntermediateDirectory)/api_api_lib$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_api_lib$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_api_lib$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/api_lib.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_api_lib$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/api_lib.c

$(IntermediateDirectory)/api_api_msg$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/api_msg.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/api_msg.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_api_msg$(ObjectSuffix) -MF$(IntermediateDirectory)/api_api_msg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_api_msg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_api_msg$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/api_msg.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_api_msg$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/api_msg.c

$(IntermediateDirectory)/api_err$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/err.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/err.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_err$(ObjectSuffix) -MF$(IntermediateDirectory)/api_err$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_err$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_err$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/err.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_err$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/err.c

$(IntermediateDirectory)/api_if_api$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/if_api.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/if_api.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_if_api$(ObjectSuffix) -MF$(IntermediateDirectory)/api_if_api$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_if_api$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_if_api$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/if_api.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_if_api$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/if_api.c

$(IntermediateDirectory)/api_netbuf$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/netbuf.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/netbuf.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_netbuf$(ObjectSuffix) -MF$(IntermediateDirectory)/api_netbuf$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_netbuf$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_netbuf$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/netbuf.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_netbuf$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/netbuf.c

$(IntermediateDirectory)/api_netdb$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/netdb.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/netdb.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_netdb$(ObjectSuffix) -MF$(IntermediateDirectory)/api_netdb$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_netdb$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_netdb$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/netdb.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_netdb$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/netdb.c

$(IntermediateDirectory)/api_netifapi$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/netifapi.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/netifapi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_netifapi$(ObjectSuffix) -MF$(IntermediateDirectory)/api_netifapi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_netifapi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_netifapi$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/netifapi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_netifapi$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/netifapi.c

$(IntermediateDirectory)/api_sockets$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/sockets.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/sockets.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_sockets$(ObjectSuffix) -MF$(IntermediateDirectory)/api_sockets$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_sockets$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_sockets$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/sockets.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_sockets$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/sockets.c

$(IntermediateDirectory)/api_tcpip$(ObjectSuffix): ../sdk/lib/net/lwip/src/api/tcpip.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/api/tcpip.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/api_tcpip$(ObjectSuffix) -MF$(IntermediateDirectory)/api_tcpip$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/api_tcpip$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/api_tcpip$(PreprocessSuffix): ../sdk/lib/net/lwip/src/api/tcpip.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/api_tcpip$(PreprocessSuffix) ../sdk/lib/net/lwip/src/api/tcpip.c

$(IntermediateDirectory)/core_altcp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/altcp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/altcp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_altcp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_altcp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_altcp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_altcp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/altcp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_altcp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/altcp.c

$(IntermediateDirectory)/core_altcp_alloc$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/altcp_alloc.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/altcp_alloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_altcp_alloc$(ObjectSuffix) -MF$(IntermediateDirectory)/core_altcp_alloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_altcp_alloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_altcp_alloc$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/altcp_alloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_altcp_alloc$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/altcp_alloc.c

$(IntermediateDirectory)/core_altcp_tcp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/altcp_tcp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/altcp_tcp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_altcp_tcp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_altcp_tcp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_altcp_tcp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_altcp_tcp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/altcp_tcp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_altcp_tcp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/altcp_tcp.c

$(IntermediateDirectory)/core_def$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/def.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/def.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_def$(ObjectSuffix) -MF$(IntermediateDirectory)/core_def$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_def$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_def$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/def.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_def$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/def.c

$(IntermediateDirectory)/core_dns$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/dns.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/dns.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_dns$(ObjectSuffix) -MF$(IntermediateDirectory)/core_dns$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_dns$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_dns$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/dns.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_dns$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/dns.c

$(IntermediateDirectory)/core_inet_chksum$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/inet_chksum.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/inet_chksum.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_inet_chksum$(ObjectSuffix) -MF$(IntermediateDirectory)/core_inet_chksum$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_inet_chksum$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_inet_chksum$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/inet_chksum.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_inet_chksum$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/inet_chksum.c

$(IntermediateDirectory)/core_init$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/init.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/init.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_init$(ObjectSuffix) -MF$(IntermediateDirectory)/core_init$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_init$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_init$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/init.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_init$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/init.c

$(IntermediateDirectory)/core_ip$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ip.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ip.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_ip$(ObjectSuffix) -MF$(IntermediateDirectory)/core_ip$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_ip$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_ip$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ip.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_ip$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ip.c

$(IntermediateDirectory)/core_mem$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/mem.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/mem.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_mem$(ObjectSuffix) -MF$(IntermediateDirectory)/core_mem$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_mem$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_mem$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/mem.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_mem$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/mem.c

$(IntermediateDirectory)/core_memp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/memp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/memp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_memp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_memp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_memp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_memp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/memp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_memp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/memp.c

$(IntermediateDirectory)/core_netif$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/netif.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/netif.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_netif$(ObjectSuffix) -MF$(IntermediateDirectory)/core_netif$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_netif$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_netif$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/netif.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_netif$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/netif.c

$(IntermediateDirectory)/core_pbuf$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/pbuf.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/pbuf.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_pbuf$(ObjectSuffix) -MF$(IntermediateDirectory)/core_pbuf$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_pbuf$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_pbuf$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/pbuf.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_pbuf$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/pbuf.c

$(IntermediateDirectory)/core_raw$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/raw.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/raw.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_raw$(ObjectSuffix) -MF$(IntermediateDirectory)/core_raw$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_raw$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_raw$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/raw.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_raw$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/raw.c

$(IntermediateDirectory)/core_stats$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/stats.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/stats.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_stats$(ObjectSuffix) -MF$(IntermediateDirectory)/core_stats$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_stats$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_stats$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/stats.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_stats$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/stats.c

$(IntermediateDirectory)/core_sys$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/sys.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/sys.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_sys$(ObjectSuffix) -MF$(IntermediateDirectory)/core_sys$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_sys$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_sys$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/sys.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_sys$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/sys.c

$(IntermediateDirectory)/core_tcp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/tcp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/tcp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_tcp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_tcp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_tcp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_tcp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/tcp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_tcp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/tcp.c

$(IntermediateDirectory)/core_tcp_in$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/tcp_in.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/tcp_in.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_tcp_in$(ObjectSuffix) -MF$(IntermediateDirectory)/core_tcp_in$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_tcp_in$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_tcp_in$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/tcp_in.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_tcp_in$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/tcp_in.c

$(IntermediateDirectory)/core_tcp_out$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/tcp_out.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/tcp_out.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_tcp_out$(ObjectSuffix) -MF$(IntermediateDirectory)/core_tcp_out$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_tcp_out$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_tcp_out$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/tcp_out.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_tcp_out$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/tcp_out.c

$(IntermediateDirectory)/core_timeouts$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/timeouts.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/timeouts.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_timeouts$(ObjectSuffix) -MF$(IntermediateDirectory)/core_timeouts$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_timeouts$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_timeouts$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/timeouts.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_timeouts$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/timeouts.c

$(IntermediateDirectory)/core_udp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/udp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/udp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_udp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_udp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_udp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_udp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/udp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_udp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/udp.c

$(IntermediateDirectory)/netif_bridgeif$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/bridgeif.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/bridgeif.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_bridgeif$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_bridgeif$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_bridgeif$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_bridgeif$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/bridgeif.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_bridgeif$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/bridgeif.c

$(IntermediateDirectory)/netif_bridgeif_fdb$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/bridgeif_fdb.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/bridgeif_fdb.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_bridgeif_fdb$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_bridgeif_fdb$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_bridgeif_fdb$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_bridgeif_fdb$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/bridgeif_fdb.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_bridgeif_fdb$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/bridgeif_fdb.c

$(IntermediateDirectory)/netif_ethernet$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ethernet.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ethernet.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_ethernet$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_ethernet$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_ethernet$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_ethernet$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ethernet.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_ethernet$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ethernet.c

$(IntermediateDirectory)/netif_ethernetif$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ethernetif.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ethernetif.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_ethernetif$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_ethernetif$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_ethernetif$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_ethernetif$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ethernetif.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_ethernetif$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ethernetif.c

$(IntermediateDirectory)/netif_lowpan6$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/lowpan6.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/lowpan6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_lowpan6$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_lowpan6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_lowpan6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_lowpan6$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/lowpan6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_lowpan6$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/lowpan6.c

$(IntermediateDirectory)/netif_lowpan6_ble$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/lowpan6_ble.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/lowpan6_ble.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_lowpan6_ble$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_lowpan6_ble$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_lowpan6_ble$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_lowpan6_ble$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/lowpan6_ble.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_lowpan6_ble$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/lowpan6_ble.c

$(IntermediateDirectory)/netif_lowpan6_common$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/lowpan6_common.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/lowpan6_common.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_lowpan6_common$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_lowpan6_common$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_lowpan6_common$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_lowpan6_common$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/lowpan6_common.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_lowpan6_common$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/lowpan6_common.c

$(IntermediateDirectory)/netif_slipif$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/slipif.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/slipif.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_slipif$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_slipif$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_slipif$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_slipif$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/slipif.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_slipif$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/slipif.c

$(IntermediateDirectory)/netif_zepif$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/zepif.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/zepif.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/netif_zepif$(ObjectSuffix) -MF$(IntermediateDirectory)/netif_zepif$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/netif_zepif$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/netif_zepif$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/zepif.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/netif_zepif$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/zepif.c

$(IntermediateDirectory)/ipv4_autoip$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv4/autoip.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv4/autoip.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv4_autoip$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv4_autoip$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_autoip$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv4_autoip$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv4/autoip.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv4_autoip$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv4/autoip.c

$(IntermediateDirectory)/ipv4_dhcp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv4/dhcp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv4/dhcp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv4_dhcp$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv4_dhcp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_dhcp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv4_dhcp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv4/dhcp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv4_dhcp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv4/dhcp.c

$(IntermediateDirectory)/ipv4_etharp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv4/etharp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv4/etharp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv4_etharp$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv4_etharp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_etharp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv4_etharp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv4/etharp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv4_etharp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv4/etharp.c

$(IntermediateDirectory)/ipv4_icmp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv4/icmp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv4/icmp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv4_icmp$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv4_icmp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_icmp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv4_icmp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv4/icmp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv4_icmp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv4/icmp.c

$(IntermediateDirectory)/ipv4_igmp$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv4/igmp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv4/igmp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv4_igmp$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv4_igmp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_igmp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv4_igmp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv4/igmp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv4_igmp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv4/igmp.c

$(IntermediateDirectory)/ipv4_ip4$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv4/ip4.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv4/ip4.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv4_ip4$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv4_ip4$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_ip4$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv4_ip4$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv4/ip4.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv4_ip4$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv4/ip4.c

$(IntermediateDirectory)/ipv4_ip4_addr$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv4/ip4_addr.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv4/ip4_addr.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv4_ip4_addr$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv4_ip4_addr$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_ip4_addr$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv4_ip4_addr$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv4/ip4_addr.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv4_ip4_addr$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv4/ip4_addr.c

$(IntermediateDirectory)/ipv4_ip4_frag$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv4/ip4_frag.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv4/ip4_frag.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv4_ip4_frag$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv4_ip4_frag$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_ip4_frag$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv4_ip4_frag$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv4/ip4_frag.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv4_ip4_frag$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv4/ip4_frag.c

$(IntermediateDirectory)/ipv6_dhcp6$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/dhcp6.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/dhcp6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_dhcp6$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_dhcp6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_dhcp6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_dhcp6$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/dhcp6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_dhcp6$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/dhcp6.c

$(IntermediateDirectory)/ipv6_ethip6$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/ethip6.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/ethip6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_ethip6$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_ethip6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_ethip6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_ethip6$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/ethip6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_ethip6$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/ethip6.c

$(IntermediateDirectory)/ipv6_icmp6$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/icmp6.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/icmp6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_icmp6$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_icmp6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_icmp6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_icmp6$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/icmp6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_icmp6$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/icmp6.c

$(IntermediateDirectory)/ipv6_inet6$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/inet6.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/inet6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_inet6$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_inet6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_inet6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_inet6$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/inet6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_inet6$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/inet6.c

$(IntermediateDirectory)/ipv6_ip6$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/ip6.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/ip6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_ip6$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_ip6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_ip6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_ip6$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/ip6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_ip6$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/ip6.c

$(IntermediateDirectory)/ipv6_ip6_addr$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/ip6_addr.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/ip6_addr.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_ip6_addr$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_ip6_addr$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_ip6_addr$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_ip6_addr$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/ip6_addr.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_ip6_addr$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/ip6_addr.c

$(IntermediateDirectory)/ipv6_ip6_frag$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/ip6_frag.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/ip6_frag.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_ip6_frag$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_ip6_frag$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_ip6_frag$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_ip6_frag$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/ip6_frag.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_ip6_frag$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/ip6_frag.c

$(IntermediateDirectory)/ipv6_mld6$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/mld6.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/mld6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_mld6$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_mld6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_mld6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_mld6$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/mld6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_mld6$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/mld6.c

$(IntermediateDirectory)/ipv6_nd6$(ObjectSuffix): ../sdk/lib/net/lwip/src/core/ipv6/nd6.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/core/ipv6/nd6.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ipv6_nd6$(ObjectSuffix) -MF$(IntermediateDirectory)/ipv6_nd6$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_nd6$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ipv6_nd6$(PreprocessSuffix): ../sdk/lib/net/lwip/src/core/ipv6/nd6.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ipv6_nd6$(PreprocessSuffix) ../sdk/lib/net/lwip/src/core/ipv6/nd6.c

$(IntermediateDirectory)/ppp_auth$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/auth.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/auth.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_auth$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_auth$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_auth$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_auth$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/auth.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_auth$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/auth.c

$(IntermediateDirectory)/ppp_ccp$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ccp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/ccp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_ccp$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_ccp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ccp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_ccp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ccp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_ccp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/ccp.c

$(IntermediateDirectory)/ppp_chap-md5$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/chap-md5.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/chap-md5.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_chap-md5$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_chap-md5$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_chap-md5$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_chap-md5$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/chap-md5.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_chap-md5$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/chap-md5.c

$(IntermediateDirectory)/ppp_chap-new$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/chap-new.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/chap-new.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_chap-new$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_chap-new$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_chap-new$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_chap-new$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/chap-new.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_chap-new$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/chap-new.c

$(IntermediateDirectory)/ppp_chap_ms$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/chap_ms.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/chap_ms.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_chap_ms$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_chap_ms$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_chap_ms$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_chap_ms$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/chap_ms.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_chap_ms$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/chap_ms.c

$(IntermediateDirectory)/ppp_demand$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/demand.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/demand.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_demand$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_demand$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_demand$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_demand$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/demand.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_demand$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/demand.c

$(IntermediateDirectory)/ppp_eap$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/eap.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/eap.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_eap$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_eap$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_eap$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_eap$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/eap.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_eap$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/eap.c

$(IntermediateDirectory)/ppp_ecp$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ecp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/ecp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_ecp$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_ecp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ecp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_ecp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ecp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_ecp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/ecp.c

$(IntermediateDirectory)/ppp_eui64$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/eui64.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/eui64.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_eui64$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_eui64$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_eui64$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_eui64$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/eui64.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_eui64$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/eui64.c

$(IntermediateDirectory)/ppp_fsm$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/fsm.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/fsm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_fsm$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_fsm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_fsm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_fsm$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/fsm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_fsm$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/fsm.c

$(IntermediateDirectory)/ppp_ipcp$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ipcp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/ipcp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_ipcp$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_ipcp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ipcp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_ipcp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ipcp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_ipcp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/ipcp.c

$(IntermediateDirectory)/ppp_ipv6cp$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ipv6cp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/ipv6cp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_ipv6cp$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_ipv6cp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ipv6cp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_ipv6cp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ipv6cp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_ipv6cp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/ipv6cp.c

$(IntermediateDirectory)/ppp_lcp$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/lcp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/lcp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_lcp$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_lcp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_lcp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_lcp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/lcp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_lcp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/lcp.c

$(IntermediateDirectory)/ppp_magic$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/magic.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/magic.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_magic$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_magic$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_magic$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_magic$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/magic.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_magic$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/magic.c

$(IntermediateDirectory)/ppp_mppe$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/mppe.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/mppe.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_mppe$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_mppe$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_mppe$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_mppe$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/mppe.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_mppe$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/mppe.c

$(IntermediateDirectory)/ppp_multilink$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/multilink.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/multilink.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_multilink$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_multilink$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_multilink$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_multilink$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/multilink.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_multilink$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/multilink.c

$(IntermediateDirectory)/ppp_ppp$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ppp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/ppp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_ppp$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_ppp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ppp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_ppp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/ppp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_ppp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/ppp.c

$(IntermediateDirectory)/ppp_pppapi$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppapi.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/pppapi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_pppapi$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_pppapi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppapi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_pppapi$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppapi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_pppapi$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/pppapi.c

$(IntermediateDirectory)/ppp_pppcrypt$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppcrypt.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/pppcrypt.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_pppcrypt$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_pppcrypt$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppcrypt$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_pppcrypt$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppcrypt.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_pppcrypt$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/pppcrypt.c

$(IntermediateDirectory)/ppp_pppoe$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppoe.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/pppoe.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_pppoe$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_pppoe$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppoe$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_pppoe$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppoe.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_pppoe$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/pppoe.c

$(IntermediateDirectory)/ppp_pppol2tp$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppol2tp.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/pppol2tp.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_pppol2tp$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_pppol2tp$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppol2tp$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_pppol2tp$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppol2tp.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_pppol2tp$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/pppol2tp.c

$(IntermediateDirectory)/ppp_pppos$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppos.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/pppos.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_pppos$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_pppos$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppos$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_pppos$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/pppos.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_pppos$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/pppos.c

$(IntermediateDirectory)/ppp_upap$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/upap.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/upap.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_upap$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_upap$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_upap$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_upap$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/upap.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_upap$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/upap.c

$(IntermediateDirectory)/ppp_utils$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/utils.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/utils.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_utils$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_utils$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_utils$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_utils$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/utils.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_utils$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/utils.c

$(IntermediateDirectory)/ppp_vj$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/vj.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/vj.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ppp_vj$(ObjectSuffix) -MF$(IntermediateDirectory)/ppp_vj$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ppp_vj$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ppp_vj$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/vj.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ppp_vj$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/vj.c

$(IntermediateDirectory)/polarssl_arc4$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/arc4.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/arc4.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/polarssl_arc4$(ObjectSuffix) -MF$(IntermediateDirectory)/polarssl_arc4$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_arc4$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/polarssl_arc4$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/arc4.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/polarssl_arc4$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/arc4.c

$(IntermediateDirectory)/polarssl_des$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/des.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/des.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/polarssl_des$(ObjectSuffix) -MF$(IntermediateDirectory)/polarssl_des$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_des$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/polarssl_des$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/des.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/polarssl_des$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/des.c

$(IntermediateDirectory)/polarssl_md4$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/md4.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/md4.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/polarssl_md4$(ObjectSuffix) -MF$(IntermediateDirectory)/polarssl_md4$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_md4$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/polarssl_md4$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/md4.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/polarssl_md4$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/md4.c

$(IntermediateDirectory)/polarssl_md5$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/md5.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/md5.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/polarssl_md5$(ObjectSuffix) -MF$(IntermediateDirectory)/polarssl_md5$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_md5$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/polarssl_md5$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/md5.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/polarssl_md5$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/md5.c

$(IntermediateDirectory)/polarssl_sha1$(ObjectSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/sha1.c  
	$(CC) $(SourceSwitch) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/sha1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/polarssl_sha1$(ObjectSuffix) -MF$(IntermediateDirectory)/polarssl_sha1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_sha1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/polarssl_sha1$(PreprocessSuffix): ../sdk/lib/net/lwip/src/netif/ppp/polarssl/sha1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/polarssl_sha1$(PreprocessSuffix) ../sdk/lib/net/lwip/src/netif/ppp/polarssl/sha1.c


$(IntermediateDirectory)/__rt_entry$(ObjectSuffix): $(IntermediateDirectory)/__rt_entry$(DependSuffix)
	@$(AS) $(SourceSwitch) $(ProjectPath)/$(IntermediateDirectory)/__rt_entry.S $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/__rt_entry$(ObjectSuffix) $(IncludeAPath)
$(IntermediateDirectory)/__rt_entry$(DependSuffix):
	@$(CC) $(CFLAGS) $(IncludeAPath) -MG -MP -MT$(IntermediateDirectory)/__rt_entry$(ObjectSuffix) -MF$(IntermediateDirectory)/__rt_entry$(DependSuffix) -MM $(ProjectPath)/$(IntermediateDirectory)/__rt_entry.S

-include $(IntermediateDirectory)/*$(DependSuffix)
