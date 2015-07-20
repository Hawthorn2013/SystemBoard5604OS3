/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                 Freescale MPC55xx Board Support Package
*
*                                 (c) Copyright 2007; Micrium; Weston, FL
*                                           All Rights Reserved
*
* File    : BSP.C
* By      : Fabiano Kovalski
*********************************************************************************************************
*/

#include "includes.h"


        void    BSP_TmrTickISR(void);


/*$PAGE*/
/*
*********************************************************************************************************
*                                       TICK ISR
*
* Description: This function is the periodical time source of the uC/OS-II.
*
* Arguments  : none
*
* Note(s)    : BSP_TmrTickISR() MUST call OSTimeTick().
*********************************************************************************************************
*/

void  BSP_TmrTickISR (void)
{
    OSTimeTick();
}


/*
 * Init pin to GPIO
 * Output high
 * Success return 0
 */
int Init_GPIO(uint16_t pcrNo)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    SIU.PCR[pcrNo].R = 0x0203;
    SIU.GPDO[pcrNo].R = 1;
    return 0;
}


/*
 * Set GPIO
 * Success return 0
 */
int Set_GPIO(uint16_t pcrNo, int value)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    if (value)
    {
        SIU.GPDO[pcrNo].R = 1;
    }
    else
    {
        SIU.GPDO[pcrNo].R = 0;
    }
    return 0;
}


/*
 * Get GPIO
 * Success return 0
 */
int Get_GPIO(uint16_t pcrNo, int *value)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    *value = SIU.GPDO[pcrNo].R;
    return 0;
}


/*
 * Init BSP LED
 * LED will be turned on
 */
int Init_LED(void)
{
    Init_GPIO(PCR_BSP_LED1);
    Set_GPIO(PCR_BSP_LED1, PIN_VASLUE_LED_TURN_ON);
    Init_GPIO(PCR_BSP_LED2);
    Set_GPIO(PCR_BSP_LED2, PIN_VASLUE_LED_TURN_ON);
    Init_GPIO(PCR_BSP_LED3);
    Set_GPIO(PCR_BSP_LED3, PIN_VASLUE_LED_TURN_ON);
    Init_GPIO(PCR_BSP_LED4);
    Set_GPIO(PCR_BSP_LED4, PIN_VASLUE_LED_TURN_ON);
    return 0;
}


/*
 * Turn on LED
 * Success return 0
 */
int Turn_on_LED(uint16_t pcrNo)
{
    return Set_GPIO(pcrNo, PIN_VASLUE_LED_TURN_ON);
}


/*
 * Turn off LED
 * Success return 0
 */
int Turn_off_LED(uint16_t pcrNo)
{
    return Set_GPIO(pcrNo, PIN_VASLUE_LED_TURN_OFF);
}



/*
 * Toggle LED
 * Success return 0
 */
int Toggle_LED(uint16_t pcrNo)
{
    int tmpVal;
    if (Get_GPIO(pcrNo, &tmpVal))
    {
        return 1;
    }
    if (Set_GPIO(pcrNo, !tmpVal))
    {
        return 2;
    }
    return 0;
}


void Init_EMIOS_0(void)
{
    /* eMIOS0初始化80MHz分为10MHz */
    EMIOS_0.MCR.B.GPRE= 7;  /* GPRE+1=分频系数；/* Divide 80 MHz sysclk by 7+1 = 8 for 10MHz eMIOS clk */
    EMIOS_0.MCR.B.GPREN = 1;    /* Enable eMIOS clock */
    EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
    EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
}


int Init_Key(uint16_t pcr, uint16_t uc, uint16_t irq, INTCInterruptFn handler)
{
    EMIOS_0.CH[uc].CCR.B.MODE = EMIOS_MODE_SAIC;
    EMIOS_0.CH[uc].CCR.B.BSL = EMIOS_BSL_INTERNAL_BUS;
    EMIOS_0.CH[uc].CCR.B.EDSEL = EMIOS_EDSEL_BOTH_TRIGGERING;
    SIU.PCR[pcr].R = 0x0100;
    INTC_InstallINTCInterruptHandler(handler, irq, INTC_PRIORITY_BSP_S1_S4);
    EMIOS_0.CH[uc].CCR.B.FEN = 1;
    return 0;
}


void INTC_Handler_BSP_S1_S2(void)
{
    if (EMIOS_0.CH[EMIOS_0_UC_BSP_S1].CSR.B.FLAG)
    {
        EMIOS_0.CH[EMIOS_0_UC_BSP_S1].CSR.B.FLAG = 1;
        if (PIN_VALUE_KEY_UP == EMIOS_0.CH[EMIOS_0_UC_BSP_S1].CSR.B.UCIN)
        {
            LED1 = PIN_VASLUE_LED_TURN_ON;
        }
        else
        {
            LED1 = PIN_VASLUE_LED_TURN_OFF;
        }
    }
    if (EMIOS_0.CH[EMIOS_0_UC_BSP_S2].CSR.B.FLAG)
    {
        EMIOS_0.CH[EMIOS_0_UC_BSP_S2].CSR.B.FLAG = 1;
        if (PIN_VALUE_KEY_UP == EMIOS_0.CH[EMIOS_0_UC_BSP_S2].CSR.B.UCIN)
        {
            LED2 = PIN_VASLUE_LED_TURN_ON;
        }
        else
        {
            LED2 = PIN_VASLUE_LED_TURN_OFF;
        }
    }
}


void INTC_Handler_BSP_S3_S4(void)
{
    if (EMIOS_0.CH[EMIOS_0_UC_BSP_S3].CSR.B.FLAG)
    {
        EMIOS_0.CH[EMIOS_0_UC_BSP_S3].CSR.B.FLAG = 1;
        if (PIN_VALUE_KEY_UP == EMIOS_0.CH[EMIOS_0_UC_BSP_S3].CSR.B.UCIN)
        {
            LED3 = PIN_VASLUE_LED_TURN_ON;
        }
        else
        {
            LED3 = PIN_VASLUE_LED_TURN_OFF;
        }
    }
    if (EMIOS_0.CH[EMIOS_0_UC_BSP_S4].CSR.B.FLAG)
    {
        EMIOS_0.CH[EMIOS_0_UC_BSP_S4].CSR.B.FLAG = 1;
        if (PIN_VALUE_KEY_UP == EMIOS_0.CH[EMIOS_0_UC_BSP_S4].CSR.B.UCIN)
        {
            LED4= PIN_VASLUE_LED_TURN_ON;
        }
        else
        {
            LED4 = PIN_VASLUE_LED_TURN_OFF;
        }
    }
}


int Init_UART_0(uint32_t div_m, uint32_t div_f, INTCInterruptFn handler_rxi, INTCInterruptFn handler_txi, INTCInterruptFn handler_err)
{
    LINFLEX_0.LINCR1.B.INIT = 1;
    LINFLEX_0.LINCR1.R = 0x00000015;
    LINFLEX_0.LINIER.B.DRIE = 1;
    LINFLEX_0.LINIBRR.B.DIV_M = div_m;
    LINFLEX_0.LINFBRR.B.DIV_F = div_f;
    LINFLEX_0.UARTCR.B.UART = 1;
    LINFLEX_0.UARTCR.R = 0x00000033;
    LINFLEX_0.LINCR1.B.INIT = 0;
    SIU.PCR[PCR_BSP_RS232_TX].R = 0x0400;
    SIU.PCR[PCR_BSP_RS232_RX].R = 0x0103;
    INTC_InstallINTCInterruptHandler((INTCInterruptFn)handler_rxi, IRQ_BSP_RS232_RXI, INTC_PRIORITY_BSP_RS232_RXI);
    INTC_InstallINTCInterruptHandler((INTCInterruptFn)handler_txi, IRQ_BSP_RS232_TXI, INTC_PRIORITY_BSP_RS232_TXI);
    INTC_InstallINTCInterruptHandler((INTCInterruptFn)handler_err, IRQ_BSP_RS232_ERR, INTC_PRIORITY_BSP_RS232_ERR);
}


void INTC_Handler_BSP_UART_0_RXI(void)
{
    uint8_t rev_ch;

    while(!LINFLEX_0.UARTSR.B.DRF){}
    rev_ch = (uint8_t)LINFLEX_0.BDRM.B.DATA4;
    LINFLEX_0.UARTSR.B.DRF = 1;
    LINFLEX_0.BDRL.B.DATA0 = rev_ch;
    while(!LINFLEX_0.UARTSR.B.DTF){}
    LINFLEX_0.UARTSR.B.DTF = 1;
}


void INTC_Handler_BSP_UART_0_TXI(void)
{
    
}


void INTC_Handler_BSP_UART_0_ERR(void)
{
    
}


void Delay_us(uint32_t us)
{
    volatile int i, j;

    for (i = 0; i < us; i++)
    {
        for (j = 0; j < 9; j++) {}
    }
}


void Delay_ms(uint32_t ms)
{
    int i;

    for (i = 0; i < ms; i++)
    {
        Delay_us(1000);
    }
}


void Init_Pit(void)
{
    /* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
    PIT.PITMCR.R = 0x00000001;  /* Enable PIT and configure timers to stop in debug modem */
    PIT.CH[1].LDVAL.R = 1600000;    /* 800000==10ms */
    PIT.CH[1].TCTRL.R = 0x00000003; /* Enable PIT1 interrupt and make PIT active to count */
    INTC_InstallINTCInterruptHandler(OSTickISR_Out, 60, 1); /* PIT 1 interrupt vector with priority 1 */
}


void OSTickISR_Out(void)
{
    PIT.CH[1].TFLG.B.TIF = 1;   // MPC56xxB/P/S: Clear PIT 1 flag by writing 1
    //BD4 = ~BD4;
    OSTickISR();
    PIT.PITMCR.R=0x00000000;
}


void Disable_Watchdog(void)
{
    SWT.SR.R = 0x0000c520;  /* rite keys to clear soft lock bit */
    SWT.SR.R = 0x0000d928;
    SWT.CR.R = 0x8000010A;  /* Clear watchdog enable (WEN) */
}


void Init_ModesAndClock(void)
{
    ME.MER.R = 0x0000001D;  /* Enable DRUN, RUN0, SAFE, RESET modes */
    /* 设置sysclk */
    //CGM.FMPLL_CR.R = 0x02400100;  /* 8 MHz xtal: Set PLL0 to 64 MHz */
    CGM.FMPLL_CR.R = 0x01280000;    /* 8 MHz xtal: Set PLL0 to 80 MHz */
    //CGM.FMPLL_CR.R = 0x013C0000;  /* 8 MHz xtal: Set PLL0 to 120 MHz */
    ME.RUN[0].R = 0x001F0064;   /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclk选择锁相环时钟 */
    ME.RUNPC[0].R = 0x00000010; /* Peri. Cfg. 1 settings: only run in RUN0 mode      选择RUN0模式 */

    /* PCTL[?] 选择需要时钟模块(默认即可，不用设置) */
    //ME.PCTL[32].R = 0x00; /* MPC56xxB/P/S ADC 0: select ME.RUNPC[0] */
    ME.PCTL[32].B.DBG_F = 0;
    /* Mode Transition to enter RUN0 mode */
    ME.MCTL.R = 0x40005AF0; /* Enter RUN0 Mode & Key */
    ME.MCTL.R = 0x4000A50F; /* Enter RUN0 Mode & Inverted Key */

    while (ME.GS.B.S_MTRANS) {} /* Wait for mode transition to complete 等待模式转换完成 */
    while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode 等待选择RUN0模式 */

    /* 开peri0、1、2 */
    /* 外设时钟总线 可用于分频 */
    CGM.SC_DC[0].R = 0x84;  /* LIN */
    CGM.SC_DC[1].R = 0x80;  /* FLEXCAN,DSPI */
    CGM.SC_DC[2].R = 0x80;  /* eMIOS,CTU,ADC */
}


void Enable_IRQ(void)
{
    INTC.CPR.B.PRI = 0; /* Single Core: Lower INTC's current priority */
    asm(" wrteei 1");   /* Enable external interrupts */
}
