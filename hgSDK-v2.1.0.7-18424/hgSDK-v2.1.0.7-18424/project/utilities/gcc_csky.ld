/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     gcc_csky.ld
 * @brief    csky linker file
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
MEMORY
{
    I-SRAM : ORIGIN = 0x0        , LENGTH = 0x80000   /* I-SRAM  128KB */
    D-SRAM : ORIGIN = 0x20000000 , LENGTH = 0x80000   /* D-SRAM  512KB */
    O-SRAM : ORIGIN = 0x50000000 , LENGTH = 0x800000   /* off-chip SRAM 8MB */
    SRAM   : ORIGIN = 0x20000000 , LENGTH = 0x48000   /* on-chip SRAM 288KB */
    SRAM2   : ORIGIN = 0x20051000 , LENGTH = 0x1800
    FLASH  : ORIGIN = 0x18000000 , LENGTH = 0x100000 
    LMAC_BUF : ORIGIN = 0x20051000 , LENGTH = 18*1024   /* on-chip SRAM 18KB */    
}

__heap_size = 0x11000;

PROVIDE (__ram_end  = 0x20048000);
PROVIDE (__heap_end = 0x20048000);

REGION_ALIAS("REGION_TEXT",    FLASH);
REGION_ALIAS("REGION_TEXT2",    SRAM2);
REGION_ALIAS("REGION_DATA2",    SRAM2);
REGION_ALIAS("REGION_RODATA",  FLASH);
REGION_ALIAS("REGION_DATA",    SRAM);
REGION_ALIAS("REGION_BSS",     SRAM);
REGION_ALIAS("REGION_INIT",    SRAM);

ENTRY(Reset_Handler)
SECTIONS
{
    .text : AT(ADDR(.text)) {
        . = ALIGN(0x4) ;
        KEEP(*startup.o(.vectors))
        KEEP(*(SYS_PARAM))
        *(.dsleep_data)
        __stext = . ;
        *(.text)
        *(.text*)
        *(.text.*)
	*(INIT.TXT)
	*(INIT.TXT.*)
        *(.gnu.warning)
        *(.stub)
        *(.gnu.linkonce.t*)
        *(.glue_7t)
        *(.glue_7)
        *(.jcr)
        KEEP (*(.init*))
        KEEP (*(.fini))
        . = ALIGN (4) ;
        PROVIDE(__ctbp = .);
        *(.call_table_data)
        *(.call_table_text)
        . = ALIGN(0x10) ;
        __etext = . ;
    } > REGION_TEXT

    .rodata : {
        . = ALIGN(0x4) ;
        __srodata = .;
        *(.rdata)
        *(.rdata*)
        *(.rdata1)
        *(.rdata.*)
        *(.rodata)
        *(.rodata1)
        *(.rodata*)
        *(.rodata.*)
        *(.rodata.str1.4)
	*(INIT.RO)		
	*(INIT.RO.*)		
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        . = ALIGN(0x4) ;
        __erodata = .;
    } > REGION_RODATA
    
    .data : AT(LOADADDR(.rodata) + SIZEOF(.rodata)){
        . = ALIGN(0x4) ;
        __sdata = . ;
        __data_start__ = . ;
        data_start = . ;
	*(INIT.DAT)
	*(INIT.DAT.*)
        *(.got.plt)
        *(.got)
        *(.gnu.linkonce.r*)
        *(.data)
        *(.data*)
        *(.data1)
        *(.data.*)
        *(.gnu.linkonce.d*)
        *(.data1)
        *(.gcc_except_table)
        *(.gcc_except_table*)
        __start_init_call = .;
        *(.initcall.init)
        __stop_init_call = .;
        __start_cmd = .;
        *(.bootloaddata.cmd)
        . = ALIGN(4) ;
        __stop_cmd = .;
        *(.sdata)
        *(.sdata.*)
        *(.gnu.linkonce.s.*)
        *(__libc_atexit)
        *(__libc_subinit)
        *(__libc_subfreeres)
        *(.note.ABI-tag)
        . = ALIGN(0x4) ;
        __edata = .;
        __data_end__ = .;
    } > REGION_DATA
    
    __dsleep_ro_start__ = LOADADDR(.rodata) + SIZEOF(.rodata) + SIZEOF(.data);
    .dsleep : AT(__dsleep_ro_start__) {
        __dsleep_code_start__ = .;
        KEEP(*(.dsleep_vector))
        *(.dsleep_txt)
        *(.lmac_dsleep_txt)
        *(.wphy_dsleep_txt)
        *(.rf_digicali_dsleep_txt)
        __dsleep_code_end__ = .;
    } > LMAC_BUF

    .dsleep_buf (NOLOAD) : {
        *(.dsleep_stack)
        *(.lmac_dsleep_data)
        *(.wphy_dsleep_data)
        *(.rf_digicali_dsleep_data)
    } > LMAC_BUF
    
    /* 当.dsleep的代码是放到sram，则在bss之前需要预留.dsleep加载代码的空间 */
    .fill_dsleep_code (NOLOAD) : {
        . = . + (((__dsleep_ro_start__ & 0x20000000) == 0x20000000) ? SIZEOF(.dsleep) : 0);
    } > REGION_DATA
 

	
 .no_init (NOLOAD) : {
    *(.stack);
    *(.no_init);
 } > REGION_BSS
	
	_ota_start = LOADADDR(.data) + SIZEOF(.data)+0x10;
	.ota : AT(LOADADDR(.data) + SIZEOF(.data)+0x10) {
        . = ALIGN(0x4) ;
        KEEP (*(.otacode))
    } > REGION_TEXT2
	_ota_end = LOADADDR(.data) + SIZEOF(.data)+SIZEOF(.ota)+0x10;
	
	_iis_start = _ota_end;
	.iis : AT(LOADADDR(.ota) + SIZEOF(.ota)) {
        . = ALIGN(0x4) ;
        KEEP (*(.iiscode))
    } > REGION_TEXT2
	_iis_end = LOADADDR(.iis) + SIZEOF(.iis);
	
 .no_init2 (NOLOAD) : {
	. = ALIGN(0x4) ;
    *(.uart_cmd);
 } > REGION_DATA2
 
 
 .bss : {
  . = ALIGN(0x4) ;
  __sbss = ALIGN(0x4) ;
  __bss_start__ = . ;
  *(.dynsbss)
  *(.sbss.ota)
  *(.sbss)
  *(.sbss.*)
  *(.scommon)
  *(.dynbss)
  *(.bss)
  *(.bss.*)
  *(COMMON)
  . = ALIGN(0x4) ;
  __ebss = . ;
  __end = . ;
  end = . ;
  __bss_end__ = .;
 } > REGION_BSS
 ._user_heap : {
  . = ALIGN(0x4) ;
  __heap_start = .;
 } > REGION_BSS
  
  ._pdata_w 0x18900000 (NOLOAD): {
 	   *(.psram.src)
 	} 

 .eh_frame : ONLY_IF_RO { KEEP (*(.eh_frame)) } > REGION_BSS
 .gcc_except_table : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) } > REGION_BSS
 .eh_frame : ONLY_IF_RW { KEEP (*(.eh_frame)) }
 .gcc_except_table : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) }
 .eh_frame_hdr : { *(.eh_frame_hdr) }
 .preinit_array :
 {
  PROVIDE_HIDDEN (__preinit_array_start = .);
  KEEP (*(.preinit_array))
  PROVIDE_HIDDEN (__preinit_array_end = .);
 }
 .init_array :
 {
  PROVIDE_HIDDEN (__init_array_start = .);
  KEEP (*(SORT(.init_array.*)))
  KEEP (*(.init_array))
  PROVIDE_HIDDEN (__init_array_end = .);
 }
 .fini_array :
 {
  PROVIDE_HIDDEN (__fini_array_start = .);
  KEEP (*(.fini_array))
  KEEP (*(SORT(.fini_array.*)))
  PROVIDE_HIDDEN (__fini_array_end = .);
 }
 .junk 0 : { *(.rel*) *(.rela*) }
 .stab 0 : { *(.stab) }
 .stabstr 0 : { *(.stabstr) }
 .stab.excl 0 : { *(.stab.excl) }
 .stab.exclstr 0 : { *(.stab.exclstr) }
 .stab.index 0 : { *(.stab.index) }
 .stab.indexstr 0 : { *(.stab.indexstr) }
 .comment 0 : { *(.comment) }
 .debug 0 : { *(.debug) }
 .line 0 : { *(.line) }
 .debug_srcinfo 0 : { *(.debug_srcinfo) }
 .debug_sfnames 0 : { *(.debug_sfnames) }
 .debug_aranges 0 : { *(.debug_aranges) }
 .debug_pubnames 0 : { *(.debug_pubnames) }
 .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
 .debug_abbrev 0 : { *(.debug_abbrev) }
 .debug_line 0 : { *(.debug_line) }
 .debug_frame 0 : { *(.debug_frame) }
 .debug_str 0 : { *(.debug_str) }
 .debug_loc 0 : { *(.debug_loc) }
 .debug_macinfo 0 : { *(.debug_macinfo) }
 .debug_weaknames 0 : { *(.debug_weaknames) }
 .debug_funcnames 0 : { *(.debug_funcnames) }
 .debug_typenames 0 : { *(.debug_typenames) }
 .debug_varnames 0 : { *(.debug_varnames) }
 .debug_pubtypes 0 : { *(.debug_pubtypes) }
 .debug_ranges 0 : { *(.debug_ranges) }
 .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
 /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}


/* ROM Functions */
/* ASSERT(DEFINED(ROM_FUNC_ENABLE), "please disable ROM Functions in ld file") */
/* ASSERT(ADDR(.text) < 0x20000068, "ROM Functions data area conflict!!!")*/

/* INPUT(romcode.ld) */

