
/******************************************************************************
 * @file     isr.c
 * @brief    source file for the interrupt server route
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#include <drv_common.h>
#include <csi_config.h>
#include "sys_config.h"

#include "soc.h"
#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif
#include "typesdef.h"
#include "errno.h"
#include "osal/irq.h"
#include "txw80x/ticker.h"

extern void ck_usart_irqhandler(int32_t idx);
extern void dw_timer_irqhandler(int32_t idx);
extern void dw_gpio_irqhandler(int32_t idx);
extern void systick_handler(void);
extern void xPortSysTickHandler(void);
extern void OSTimeTick(void);
static struct sys_hwirq sys_irqs[IRQ_NUM];

#if defined(CONFIG_SUPPORT_TSPEND) || defined(CONFIG_KERNEL_NONE)
#define  ATTRIBUTE_ISR __attribute__((isr))
#else
#define  ATTRIBUTE_ISR
#endif

#define readl(addr) ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

#if defined(CONFIG_KERNEL_RHINO)
#define SYSTICK_HANDLER systick_handler
#elif defined(CONFIG_KERNEL_FREERTOS)
#define SYSTICK_HANDLER xPortSysTickHandler
#elif defined(CONFIG_KERNEL_UCOS)
#define SYSTICK_HANDLER OSTimeTick
#endif

#if !defined(CONFIG_KERNEL_FREERTOS) && !defined(CONFIG_KERNEL_NONE)
#define  CSI_INTRPT_ENTER() csi_kernel_intrpt_enter()
#define  CSI_INTRPT_EXIT()  csi_kernel_intrpt_exit()
#else
#define  CSI_INTRPT_ENTER()
#define  CSI_INTRPT_EXIT()
#endif


#ifdef SYS_IRQ_STAT
#define SYS_IRQ_STATE_ST(irqn)\
    uint32 t1, t2;\
    sys_irqs[irqn].trig_cnt++;\
    t1 = hw_timer_get_tick();

#define SYS_IRQ_STATE_END(irqn)\
    do {\
        t2 = hw_timer_get_tick() - t1;\
        sys_irqs[irqn].tot_time += t2;\
        if (t2 > sys_irqs[irqn].max_time) {\
            sys_irqs[irqn].max_time = t2;\
        }\
    } while(0);

#else
#define SYS_IRQ_STATE_ST(irqn)
#define SYS_IRQ_STATE_END(irqn)
#endif

#define SYSTEM_IRQ_HANDLE_FUNC(func_name,irqn)\
    ATTRIBUTE_ISR void func_name(void)\
    {\
        SYS_IRQ_STATE_ST(irqn);\
        CSI_INTRPT_ENTER();\
        if (sys_irqs[irqn].handle) {\
            sys_irqs[irqn].handle(sys_irqs[irqn].data);\
        }\
        CSI_INTRPT_EXIT();\
        SYS_IRQ_STATE_END(irqn);\
    }

int32_t request_irq(uint32_t irq_num, irq_handle handle, void *data)
{
    if (irq_num < IRQ_NUM) {
        sys_irqs[irq_num].data = data;
        sys_irqs[irq_num].handle = handle;
        return RET_OK;
    } else {
        return -EINVAL;
    }
}

int32_t release_irq(uint32_t irq_num)
{
    if (irq_num < IRQ_NUM) {
        irq_disable(irq_num);
        sys_irqs[irq_num].handle = NULL;
        sys_irqs[irq_num].data   = NULL;
        return RET_OK;
    } else {
        return -EINVAL;
    }
}

void irq_status(void)
{
#ifdef SYS_IRQ_STAT
    u32 i;
    printf("SYS IRQ:\r\n");
    for (i = 0; i < IRQ_NUM; i++) {
        if (i != TIM0_IRQn && sys_irqs[i].tot_time) {
            printf("  IRQ%-2d: trig:%-8d tot_time:%-8d max:%-8d [hdl:%x]\r\n", i,
                      sys_irqs[i].trig_cnt, sys_irqs[i].tot_time,
                      sys_irqs[i].max_time, (uint32_t)sys_irqs[i].handle);
            sys_irqs[i].trig_cnt = 0;
            sys_irqs[i].tot_time = 0;
            sys_irqs[i].max_time = 0;
        }
    }
#endif
}

ATTRIBUTE_ISR void CORET_IRQHandler(void)
{
    SYS_IRQ_STATE_ST(CORET_IRQn);
    CSI_INTRPT_ENTER();
    readl(0xE000E010);
    SYSTICK_HANDLER();
    CSI_INTRPT_EXIT();
    SYS_IRQ_STATE_END(CORET_IRQn);
}

SYSTEM_IRQ_HANDLE_FUNC(USB20DMA_USB11SOF_IRQHandler, USB20DMA_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(USB20MC_USB11CTL_IRQHandler, USB20MC_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(UART0_IRQHandler, UART0_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(UART1_IRQHandler, UART1_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(UART2_IRQHandler, UART2_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(QSPI_IRQHandler, QSPI_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SPI0_IRQHandler, SPI0_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SPI1_IRQHandler, SPI1_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SPI2_IRQHandler, SPI2_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SPI3_IRQHandler, SPI3_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(TIM0_IRQHandler, TIM0_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(TIM1_IRQHandler, TIM1_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(TIM2_IRQHandler, TIM2_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(TIM3_IRQHandler, TIM3_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(TIM4_IRQHandler, TIM4_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SUPTMR0_IRQHandler, SUPTMR0_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SUPTMR1_IRQHandler, SUPTMR1_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SUPTMR2_IRQHandler, SUPTMR2_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SDIO_IRQHandler, SDIO_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SDIO_RST_IRQHandler, SDIO_RST_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SDHOST_IRQHandler, SDHOST_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(LMAC_IRQHandler, LMAC_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(GMAC_IRQHandler, GMAC_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(M2M0_IRQHandler, M2M0_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(M2M1_IRQHandler, M2M1_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SYSAES_IRQHandler, SYSAES_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(CRC_IRQHandler, CRC_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(ADKEY_IRQHandler, ADKEY_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(PD_TMR_IRQHandler, PD_TMR_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(WKPND_IRQHandler, WKPND_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(PDWKPND_IRQHandler, PDWKPND_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(LVD_IRQHandler, LVD_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(WDT_IRQHandler, WDT_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(SYS_ERR_IRQHandler, SYS_ERR_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(IIS0_IRQHandler, IIS0_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(IIS1_IRQHandler, IIS1_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(GPIOA_IRQHandler, GPIOA_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(GPIOB_IRQHandler, GPIOB_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(DVP_IRQHandler, DVP_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(JPG_IRQHandler, JPG_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(STMR0_IRQHandler   , STMR0_IRQn   )
SYSTEM_IRQ_HANDLE_FUNC(STMR1_IRQHandler   , STMR1_IRQn   )
SYSTEM_IRQ_HANDLE_FUNC(STMR2_IRQHandler   , STMR2_IRQn   )
SYSTEM_IRQ_HANDLE_FUNC(STMR3_IRQHandler   , STMR3_IRQn   )
SYSTEM_IRQ_HANDLE_FUNC(PDM_IRQHandler     , PDM_IRQn     )
SYSTEM_IRQ_HANDLE_FUNC(GPIOC_IRQHandler   , GPIOC_IRQn   )
SYSTEM_IRQ_HANDLE_FUNC(UART4_IRQHandler   , UART4_IRQn   )
SYSTEM_IRQ_HANDLE_FUNC(GFSK_IRQHandler    , GFSK_IRQn    )
SYSTEM_IRQ_HANDLE_FUNC(CMP_IRQHandler     , CMP_IRQn     )
SYSTEM_IRQ_HANDLE_FUNC(UART5_IRQHandler   , UART5_IRQn   )
SYSTEM_IRQ_HANDLE_FUNC(SPI5_IRQHandler    , SPI5_IRQn    )
SYSTEM_IRQ_HANDLE_FUNC(SPI6_IRQHandler    , SPI6_IRQn    )
SYSTEM_IRQ_HANDLE_FUNC(ADKEY1_IRQHandler  , ADKEY1_IRQn  )
SYSTEM_IRQ_HANDLE_FUNC(LED_IRQHandler     , LED_IRQn     )
SYSTEM_IRQ_HANDLE_FUNC(SUPTMR3_IRQHandler , SUPTMR3_IRQn )
SYSTEM_IRQ_HANDLE_FUNC(SUPTMR4_IRQHandler , SUPTMR4_IRQn )
SYSTEM_IRQ_HANDLE_FUNC(SUPTMR5_IRQHandler , SUPTMR5_IRQn )
SYSTEM_IRQ_HANDLE_FUNC(USB20PHY_IRQHandler, USB20PHY_IRQn)
SYSTEM_IRQ_HANDLE_FUNC(TK_IRQHandler      , TK_IRQn      )
SYSTEM_IRQ_HANDLE_FUNC(LEDTMR0_IRQHandler , LEDTMR0_IRQn )
SYSTEM_IRQ_HANDLE_FUNC(LEDTMR1_IRQHandler , LEDTMR1_IRQn )
SYSTEM_IRQ_HANDLE_FUNC(LEDTMR2_IRQHandler , LEDTMR2_IRQn )
SYSTEM_IRQ_HANDLE_FUNC(LEDTMR3_IRQHandler , LEDTMR3_IRQn )





